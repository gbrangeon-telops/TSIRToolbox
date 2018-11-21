// *****************************************************************************
//
//     Copyright (c) 2008, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

//
// To receives images using a PvPipeline, full recovery management
//

#include <PvSampleUtils.h>
#include <PvDeviceFinderWnd.h>
#include <PvDevice.h>
#include <PvPipeline.h>
#include <PvBuffer.h>
#include <PvStream.h>

#include <list>

#define MSG_RESTOREACQ ( 1000 )
#define MSG_STOPACQ ( 1003 )
#define MSG_QUIT ( 1004 )

PV_INIT_SIGNAL_HANDLER();

//
// Application class
//

class RecoverySampleApp : protected PvDeviceEventSink
{
public:

	RecoverySampleApp();
	~RecoverySampleApp();

	bool Run();

protected:

	bool ConnectDevice();
	bool OpenStream();
	void CloseStream();
	bool StartAcquisition();
	bool StopAcquisition();
	void AcquisitionLoop();
	void WrapUp();
	void HandleMsgQueue();

	// Inherited from PvDeviceEventSink
    void OnLinkDisconnected( PvDevice *aDevice );
    void OnLinkReconnected( PvDevice *aDevice );

private:

	PvString mDeviceIPAddress;
	PvDevice mDevice;
	PvStream mStream;
	PvPipeline mPipeline;

	// Communication link GenICam parameters
    PvGenBoolean *mLinkRecoveryEnabled;

	// Device GenICam parameters
	PvGenInteger *mTLLocked;
	PvGenInteger *mPayloadSize;
	PvGenCommand *mStart;
	PvGenCommand *mStop;

	// Stream GenICam parameters
	PvGenInteger *mCount;
	PvGenFloat *mFrameRate;
	PvGenFloat *mBandwidth;
    PvGenCommand *mResetTimestamp;

	// Message queue used by callbacks to send requests to main thread
	HANDLE mMsgQueueMutex;
	std::list<int> mMsgQueue;

    bool mQuit;
};


//
// Constructor
//

RecoverySampleApp::RecoverySampleApp()
	: mPipeline( &mStream )
	, mTLLocked( NULL )
    , mPayloadSize( NULL )
	, mStart( NULL )
	, mStop( NULL )
	, mCount( NULL )
	, mFrameRate( NULL )
	, mBandwidth( NULL )
    , mResetTimestamp( NULL )
    , mQuit( false )
{
	mMsgQueueMutex = ::CreateMutex( NULL, FALSE, NULL );
}


//
// Destructor
//

RecoverySampleApp::~RecoverySampleApp()
{
	WrapUp();

	::CloseHandle( mMsgQueueMutex );
	mMsgQueueMutex = 0;
}


//
// Main, only public function. Runs the applicaiton
//

bool RecoverySampleApp::Run()
{
	// Select, connect device
	ConnectDevice();

	// Open stream, pipeline, setup buffers
	OpenStream();

	// Start acquisition
	StartAcquisition();

	// Acquisition loop. Will break when user hits a key
	AcquisitionLoop();

	// Stops acquisition
	StopAcquisition();
	
	// Closes, disconnects, etc.
	WrapUp();

	return true;
}


//
// Selects, connects a device
//

bool RecoverySampleApp::ConnectDevice()
{
	cout << "RecoverySampleApp::ConnectDevice()" << endl;

	// Create, show a GEV Device finder dialog
	PvDeviceFinderWnd lDeviceFinderWnd;
	lDeviceFinderWnd.ShowModal();

	// Get the connectivity information for the selected GEV Device
	PvDeviceInfo* lDeviceInfo = lDeviceFinderWnd.GetSelected();
	if( lDeviceInfo == NULL )
	{
		cout << "No device selected." << endl;
		return false;
	}

	// Connect to the GEV Device
	cout << "Connecting to " <<  lDeviceInfo->GetMACAddress().GetAscii() << endl;
	if ( !mDevice.Connect( lDeviceInfo ).IsOK() )
	{
		cout << "Unable to connect to " << lDeviceInfo->GetMACAddress().GetAscii() << endl;
		return false;
	}
	cout << "Successfully connected to " << lDeviceInfo->GetMACAddress().GetAscii() << endl;
	cout << "" << endl;

	// Get communication link paramaters we need
	PvGenParameterArray *lCommParams = mDevice.GetGenLink();
	mLinkRecoveryEnabled = lCommParams->GetBoolean( "LinkRecoveryEnabled" );

	// Get device parameters needed to control streaming
	PvGenParameterArray *lDeviceParams = mDevice.GetGenParameters();
	mTLLocked = lDeviceParams->GetInteger( "TLParamsLocked" );
    mPayloadSize = lDeviceParams->GetInteger( "PayloadSize" );
	mStart = lDeviceParams->GetCommand( "AcquisitionStart" );
	mStop = lDeviceParams->GetCommand( "AcquisitionStop" );
    mResetTimestamp = lDeviceParams->GetCommand( "GevTimestampControlReset" );

	// Register this class as an event sink for PvDevice callbacks
	mDevice.RegisterEventSink( this );

	// Save IP address (used when opening the stream object)
	mDeviceIPAddress = lDeviceInfo->GetIPAddress();

    //
    // This sample is about recovery. Let's enable link recovery,
    // which is disabled by default.
    //
    // Link recovery does just that - making sure recovery will happen after 
    // losing the connection to a device.
    //
    mLinkRecoveryEnabled->SetValue( true );
    
	return true;
}


//
// Opens stream, pipeline, allocs buffers
//

bool RecoverySampleApp::OpenStream()
{
	cout << "RecoverySampleApp::OpenStream()" << endl;

	// Create, open the PvStream object
	cout << "Opening stream to device" << endl;
	PvResult lResult = mStream.Open( mDeviceIPAddress );
	if ( !lResult.IsOK() )
	{
		return false;
	}
    
    // Reading payload size from device
    PvInt64 lSize = 0;
    mPayloadSize->GetValue( lSize );

    // Create, init the PvPipeline object
    mPipeline.SetBufferSize( static_cast<PvUInt32>( lSize ) );
	mPipeline.SetBufferCount( 16 );

	// The pipeline needs to be "armed", or started before  we instruct the device to send us images
	cout << "Starting pipeline" << endl;
	lResult = mPipeline.Start();
	if ( !lResult.IsOK() )
	{
		cout << "Unable to start pipeline" << endl;
		return false;
	}

	// Get stream parameters/stats
	PvGenParameterArray *lStreamParams = mStream.GetParameters();
	mCount = lStreamParams->GetInteger( "ImagesCount" );
	mFrameRate = lStreamParams->GetFloat( "AcquisitionRateAverage" );
	mBandwidth = lStreamParams->GetFloat( "BandwidthAverage" );

	PvGenBoolean *lIgnoreResends = lStreamParams->GetBoolean( "IgnoreMissingPackets" );
	lIgnoreResends->SetValue( true );

	return true;
}


//
// Closes the stream, pipeline
//

void RecoverySampleApp::CloseStream()
{
	mCount = NULL;
	mFrameRate = NULL;
	mBandwidth = NULL;

	if ( mPipeline.IsStarted() )
	{
		cout << "Unable to stop the pipeline" << endl;
		mPipeline.Stop();
	}

	if ( mStream.IsOpen() )
	{
		cout << "Closing stream" << endl;
		mStream.Close();
	}
}


//
// Starts image acquisition
//

bool RecoverySampleApp::StartAcquisition()
{
	cout << "RecoverySampleApp::StartAcquisition()" << endl;

	// Have to set the Device IP destination to the Stream
	PvResult lResult = mDevice.SetStreamDestination( mStream.GetLocalIPAddress(), mStream.GetLocalPort() );
	if ( !lResult.IsOK() )
	{
		cout << "Setting stream destination failed" << endl;
		return false;
	}

	// TLParamsLocked is optional but when present, it MUST be set to 1
	// before sending the AcquisitionStart command
	if ( mTLLocked != NULL )
	{
		cout << "Setting TLParamsLocked to 1" << endl;
		mTLLocked->SetValue( 1 );
	}

    cout << "Resetting timestamp counter..." << endl;
    mResetTimestamp->Execute();

	// The pipeline is already "armed", we just have to tell the device
	// to start sending us images
	cout << "Sending StartAcquisition command to device" << endl;
	lResult = mStart->Execute();
	if ( !lResult.IsOK() )
	{
		cout << "Unable to start acquisition" << endl;
		return false;
	}

	return true;
}


//
// Stops acquisition
//

bool RecoverySampleApp::StopAcquisition()
{
	cout << "RecoverySampleApp::StopAcquisition()" << endl;

	// Tell the device to stop sending images
	PvResult lResult = mStop->Execute();
	if ( !lResult.IsOK() )
	{
		cout << "Unable to stop acquisition" << endl;
		// Ignore failure, may be called when device is lost
	}

	// If present reset TLParamsLocked to 0. Must be done AFTER the 
	// streaming has been stopped
	if ( mTLLocked != NULL )
	{
		cout << "Resetting TLParamsLocked to 0" << endl;
		mTLLocked->SetValue( 0 );
		// Ignore failure, may be called when device is lost
	}

	// Reset streaming destination (optional...)
	lResult = mDevice.ResetStreamDestination();
	if ( !lResult.IsOK() )
	{
		cout << "Unable to reset stream destination" << endl;
		// Ignore failure, may be called when device is lost
	}

	// Important: abort all currently queued buffer, otherwise timeouts
	// are going to be reported before we can successfully resume streaming
	// if this method is called in the context of a recovery attempt
	mStream.AbortQueuedBuffers();

	mStream.FlushPacketQueue(); 

	return true;
}


//
// Acquisition loop
//

void RecoverySampleApp::AcquisitionLoop()
{
	cout << "RecoverySampleApp::AcquisitionLoop()" << endl;

	char  lDoodle[] = "|\\-|-/";
	int lDoodleIndex = 0;
	bool lFirstTimeout = true;

	PvInt64 lImageCountVal = 0;
	double lFrameRateVal = 0.0;
	double lBandwidthVal = 0.0;

	// Acquire images until the user instructs us to stop
	cout << "<press a key to stop streaming>" << endl;
	while ( !PvKbHit() )
	{
		// Do all recovery tasks requested through message queue
		HandleMsgQueue();
        if ( mQuit )
        {
            cout << "Quit flag set, terminating application" << endl;
            break;
        }

		if ( mStream.IsOpen() && mPipeline.IsStarted() )
		{
			// Retrieve next buffer		
			PvBuffer *lBuffer = NULL;
            PvResult  lOperationResult;
			PvResult lResult = mPipeline.RetrieveNextBuffer( &lBuffer, 1000, &lOperationResult );
			
			if ( lResult.IsOK() )
			{
                if (lOperationResult.IsOK())
                {
                    //
                    // We now have a valid buffer. This is where you would typically process the buffer.
                    // -----------------------------------------------------------------------------------------
                    // ...

    				mCount->GetValue( lImageCountVal );
	    			mFrameRate->GetValue( lFrameRateVal );
		    		mBandwidth->GetValue( lBandwidthVal );
				
					// If the buffer contains an image, display width and height
					PvUInt32 lWidth = 0, lHeight = 0;
					if ( lBuffer->GetPayloadType() == PvPayloadTypeImage )
					{
						// Get image specific buffer interface
						PvImage *lImage = lBuffer->GetImage();

						// Read width, height
						lWidth = lBuffer->GetImage()->GetWidth();
						lHeight = lBuffer->GetImage()->GetHeight();
					}
					
					cout << fixed << setprecision( 1 );
					cout << lDoodle[ lDoodleIndex ];
					cout << " BlockID: " << uppercase << hex << setfill('0') << setw(16) << lBuffer->GetBlockID() << " W: " << dec << lWidth << " H: " 
						 << lHeight << " " << lFrameRateVal << " FPS " << ( lBandwidthVal / 1000000.0 ) << " Mb/s  \r";

	    			lFirstTimeout = true;
                }
				// We have an image - do some processing (...) and VERY IMPORTANT,
				// release the buffer back to the pipeline
				mPipeline.ReleaseBuffer( lBuffer );
            }
			else
			{
				// Timeout
				
				if ( lFirstTimeout )
				{
					cout << "" << endl;
					lFirstTimeout = false;
				}

				cout << "Image timeout " << lDoodle[ lDoodleIndex ] << endl;
			}

			++lDoodleIndex %= 6;
		}
		else
		{
			// No stream/pipeline, must be in recovery. Wait a bit...
			::Sleep( 100 );
		}
	}

	PvGetChar(); // Flush key buffer for next stop
	cout << "" << endl;
}


//
// Wrap-up: closes, disconnects, etc.
//

void RecoverySampleApp::WrapUp()
{
	cout << "RecoverySampleApp::WrapUp()" << endl;

	CloseStream();

	mTLLocked = NULL;
	mStart = NULL;
	mStop = NULL;
    mResetTimestamp = NULL;

	if ( mDevice.IsConnected() )
	{
		// Unregister event sink (callbacks)
		mDevice.UnregisterEventSink( this );

		cout << "Disconnecting device" << endl;
		mDevice.Disconnect();
	}
}


//
// Processes the message queue.
//
// A message queue is used to have the PvDevice recovery callbacks
// perform operations in the context of the main applicaiton thread.
//

void RecoverySampleApp::HandleMsgQueue()
{
	::WaitForSingleObject( mMsgQueueMutex, INFINITE );

		while ( mMsgQueue.size() > 0 )
		{
			int lMsg = mMsgQueue.front();
			mMsgQueue.pop_front();

			switch ( lMsg )
			{
			case MSG_RESTOREACQ:

				cout << "** Main thread processing MSG_RESTARTACQ" << endl;

				OpenStream();
				StartAcquisition();

				break;

			case MSG_STOPACQ:

				cout << "** Main thread processing MSG_STOPACQ" << endl;

				StopAcquisition();
				CloseStream();

				break;

            case MSG_QUIT:

				cout << "** Main thread processing MSG_QUIT" << endl;

                mQuit = true;
                        
                break;
			}
		}

	::ReleaseMutex( mMsgQueueMutex );
}


//
// PvDeviceEventSink callback
//
// Notification that the device just got disconnected.
//

void RecoverySampleApp::OnLinkDisconnected( PvDevice *aDevice )
{
	cout << "=====> Recovery callback: OnLinkDisconnected" << endl;

    bool lEnabled = false;
    mLinkRecoveryEnabled->GetValue( lEnabled );
    if ( lEnabled )
    {
        // We lost the device, stop acquisition
        ::WaitForSingleObject( mMsgQueueMutex, INFINITE );
	        mMsgQueue.push_back( MSG_STOPACQ );
        ::ReleaseMutex( mMsgQueueMutex );
    }
    else
    {
        // If recovery is not enabled, just signal to terminate the application
        ::WaitForSingleObject( mMsgQueueMutex, INFINITE );
	        mMsgQueue.push_back( MSG_QUIT );
        ::ReleaseMutex( mMsgQueueMutex );
    }
}


//
// PvDeviceEventSink callback
//
// Notification that the device just got reconnected. Restart acquisition...
//

void RecoverySampleApp::OnLinkReconnected( PvDevice *aDevice )
{
	cout << "=====> Recovery callback: OnLinkReconnected" << endl;

    ::WaitForSingleObject( mMsgQueueMutex, INFINITE );
		mMsgQueue.push_back( MSG_RESTOREACQ );
    ::ReleaseMutex( mMsgQueueMutex );
}


//
// Main function
//

int main()
{
	// Receives images using a PvPipeline, full recovery management
	cout << "***************************************************" << endl;
	cout << "PvRecoverySample - image acquisition from a device" << endl;
	cout << "*-------------------------------------------------*" << endl;
	cout << "* It is recommended to use a persistent, fixed IP *" << endl;
	cout << "* address on a device when relying on automatic   *" << endl;
	cout << "* recovery.                                       *" << endl;
	cout << "***************************************************" << endl;
	RecoverySampleApp lApp;	
	bool lRetVal = lApp.Run();

	cout << endl;
	cout << "<press a key to exit>" << endl;
    PvWaitForKeyPress();

	return ( lRetVal ) ? 0 : -1;
}

