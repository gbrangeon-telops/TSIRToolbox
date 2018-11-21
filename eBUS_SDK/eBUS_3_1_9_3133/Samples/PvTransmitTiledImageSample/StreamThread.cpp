// *****************************************************************************
//
//     Copyright (c) 2012, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#include "stdafx.h"

#include "ConnectionThread.h"
#include "StreamThread.h"
#include "Constants.h"
#include "SetupDlg.h"
#include "StreamDeviceFinderWnd.h"

StreamThread::StreamThread()
    : mMainWnd( NULL ) 
    , mRow( -1 )
    , mColumn( -1 )
    , mCurrentBuffersTable( NULL )
    , mStart( NULL )
    , mStop( NULL )
    , mTLLocked( NULL )
    , mConnected( false )
    , mLastPayloadSize( 0 )
{
    memset( mBuffers, 0, sizeof( SmartBuffer* ) * RX_POOL_SIZE );
}

bool StreamThread::Initialize( CWnd* aMainWnd, PvInt32 aRow, PvInt32 aColumn, CurrentBuffersTable* aCurrentBuffersTable )
{
    mMainWnd = aMainWnd;
    mRow = aRow;
    mColumn = aColumn;
    mCurrentBuffersTable = aCurrentBuffersTable;

    return mReturnBufferQueues.Initialize( RX_POOL_SIZE + 1 );
}

StreamThread::~StreamThread()
{
    Disconnect();
}

PvResult StreamThread::Connect( const CString &aIfAddress )
{
    SetupDlg lSetupDlg( &mSetup );
    PvResult lResult;
   
    StreamDeviceFinderWnd lFinder( aIfAddress );
    lFinder.SetTitle( _T( "GigE Vision Device Selection" ) );
    lResult = lFinder.ShowModal();
    if ( !lResult.IsOK() || ( lFinder.GetSelected() == NULL ) )
    {
        return PvResult::Code::CANCEL;
    }
    if ( lSetupDlg.DoModal() != IDOK )
    {
        return PvResult::Code::CANCEL;
    }

    CWaitCursor lCursor;

    // Just in case we came here still connected...
    Disconnect();

    // Device connection, packet size negociation and stream opening is performed in a separate 
    // thread while we display a progress dialog to the user
    ConnectionThread lConnectionThread( &mSetup, lFinder.GetSelected(), &mDevice, &mStream, NULL );
    lResult = lConnectionThread.Connect();
    if ( !lResult.IsOK() )
    {
        Disconnect();
        return lResult;
    }

    mIPAddress = lFinder.GetSelected()->GetIPAddress().GetUnicode();
    mMACAddress = lFinder.GetSelected()->GetMACAddress().GetUnicode();
    mModel = lFinder.GetSelected()->GetModel().GetUnicode();
    mUserDefinedName = lFinder.GetSelected()->GetUserDefinedName().GetUnicode();

    if ( mDevice.IsConnected() )
    {
        // Register PvDevice callbacks
        mDevice.RegisterEventSink( this );
    }

    mConnected = true;
    return PvResult::Code::OK;
}

void StreamThread::Disconnect()
{
    mGenBrowserTabbedWnd.Close();

    mDevice.UnregisterEventSink( this );

    Stop();

    if ( mDevice.IsConnected() )
    {
        mDevice.Disconnect();
    }

    if ( mStream.IsOpen() )
    {
        mStream.Close();
    }

    // Clear the used buffers memory
    if( mLastPayloadSize )
    {
        for( int i = 0; i < RX_POOL_SIZE; i++ )
        {
            delete mBuffers[ i ];
            mBuffers[ i ] = NULL;
        }
        mLastPayloadSize = 0;
    }

    mConnected = false;
}

bool StreamThread::Configure( unsigned int& aWidth, unsigned int& aHeight )
{
    PvUInt32 lPayloadSize;
    PvResult lResult;
    PvBuffer lBuffer;
    PvGenParameterArray* lDeviceParams;
    PvGenCommand* lResetTimestamp;
    PvGenCommand* lResetStats;

    // Get access to the parameters
    lDeviceParams = mDevice.GetGenParameters();
    mStart = dynamic_cast<PvGenCommand *>( lDeviceParams->Get( "AcquisitionStart" ) );
    mStop = dynamic_cast<PvGenCommand *>( lDeviceParams->Get( "AcquisitionStop" ) );
    mMode = dynamic_cast<PvGenEnum *>( mDevice.GetGenParameters()->Get( "AcquisitionMode" ) );
    mTLLocked = dynamic_cast<PvGenInteger *>( lDeviceParams->Get( "TLParamsLocked" ) );
    lResetTimestamp = dynamic_cast<PvGenCommand *>( lDeviceParams->Get( "GevTimestampControlReset" ) );
    lResetStats = dynamic_cast<PvGenCommand *>( mStream.GetParameters()->Get( "Reset" ) );

    aHeight = 0;
    aWidth = 0;

    // Reset the statistic
    lResetStats->Execute();

    // Reset timestamp counter 
    if( lResetTimestamp )
    {
        lResetTimestamp->Execute();
    }

    if( mSetup.GetRole() == ROLE_DATA )
    {
        if( mLastPayloadSize == 0 )
        {
            // In data receiver only mode, we do not know what will be the image size 
            // For this reason, we will re-allocate the image size on the fly. This is
            // bad but the only way we can make it work
            lResult = CreateNewBuffers( DEFAULT_RX_BUFFER_ALLOCATION_SIZE );
            if( !lResult )
            {
                return false;
            }

            // We need to cache the payload size for future starts...
            mLastPayloadSize = DEFAULT_RX_BUFFER_ALLOCATION_SIZE;
        }
    }
    else
    {
        // Get the current configured payload size
        lPayloadSize = GetPayloadSize( aWidth, aHeight );
        if( lPayloadSize == 0 )
        {
            // We cannot get the size, the buffer cannot be pre-allocated
            return false;
        }

        // Manage the size of the buffer allocated to receive the images
        if( mLastPayloadSize == 0 )
        {
            // This is a case where no buffers were allocated
            lResult = CreateNewBuffers( lPayloadSize );
            if( !lResult )
            {
                return false;
            }

            // We need to cache the payload size for future starts...
            mLastPayloadSize = lPayloadSize;
        }
        else if( mLastPayloadSize != lPayloadSize )
        {
            // The buffer need to be resized case

            // Reallocate the memory of each of the buffer to be able to 
            // deal with the new payload size
            for( int i = 0; i < RX_POOL_SIZE; i++ )
            {
                // Recalling Alloc will make a re-allocate in this case
                lResult = mBuffers[ i ]->Alloc( lPayloadSize );
                if( !lResult.IsOK() )
                {
                    for( int j = 0; j <= i; j++ )
                    {
                        delete mBuffers[ j ];
                        mBuffers[ j ] = NULL;
                    }

                    // Set mLastPayloadSize to 0 to ensure a clean re-allocation of the memory
                    mLastPayloadSize = 0;

                    return false;
                }
            }
            
            // We need to cache the payload size for future starts...
            mLastPayloadSize = lPayloadSize;
        }
    }

    // Queue the buffers into the stream
    for( int i = 0; i < RX_POOL_SIZE; i++ )
    {
        mStream.QueueBuffer( ( PvBuffer* ) mBuffers[ i ] );            
    }

    // Now configure the camera to be able to start streaming
    return true;
}

bool StreamThread::Start()
{
    bool lReturn = true;
    PvString lStr;
    CString lModeStr;
    bool lContinuous;

    if( mSetup.GetRole() != ROLE_DATA )
    {
        // Get the current acquisition mode used
        mMode->GetValue( lStr );
        lModeStr = lStr.GetUnicode();
        lContinuous = (lModeStr.Find( _T( "Continuous" ) ) >= 0 );

        // Lock the device access
        if( mTLLocked )
        {
            mTLLocked->SetValue( 1 );
        }

        // Start the internal thread to retrieve the data
        lReturn = Thread::Start();
        if( !lReturn )
        {
            return false;
        }

        // Start the stream
        if( mStart )
        {
            mStart->Execute();
        }

        if( !lContinuous )
        {
            if( mTLLocked )
            {
                mTLLocked->SetValue( 0 );
            }
        }
    }
    else
    {
        // Start the internal thread to retrieve the data
        lReturn = Thread::Start();
        if( !lReturn )
        {
            return false;
        }
    }

    return true;
}

bool StreamThread::Stop()
{
    PvBuffer *lBuffer = NULL;
    PvResult lOperationResult;

    if( mSetup.GetRole() != ROLE_DATA )
    {
        if ( !mDevice.IsConnected() )
        {        
            return true;
        }

        // Stop the streaming on the device
        if( mStop )
        {
            mStop->Execute();
        }
        if( mTLLocked )
        {
            mTLLocked->SetValue( 0 );
        }
    }

    // Stop the thread retrieving the data
    Thread::Stop();

    // Abort the stream and dequeue all the buffers
    mStream.AbortQueuedBuffers();
    while ( mStream.RetrieveBuffer( &lBuffer, &lOperationResult, 0 ).IsOK() );

    // We want to recover all the buffers, including the one
    // in the display snapshot...
    mCurrentBuffersTable->Reset( mRow, mColumn );
    // This is emptying the ready queue...
    while( mReturnBufferQueues.Pop() );

    return true;
}

DWORD StreamThread::Function()
{
    PvBuffer *lBuffer = NULL;
    PvImage* lImage;
    SmartBuffer* lSmartBuffer = NULL;
    PvResult lOperationResult;
    PvResult lResult;

    while ( !IsStopping() )
    {
        // The 100ms timeout is to allow the system to check the stop state
        lResult = mStream.RetrieveBuffer( &lBuffer, &lOperationResult, 50 );
        if ( lResult.IsOK() )
        {
    	    if( lOperationResult.IsOK() )
    	    {				
                // We will keep this image as the last acquired.
                // The mCurrentBuffersTable will manage the return of the buffer into the stream
				mCurrentBuffersTable->Set( ( SmartBuffer* ) lBuffer, mRow, mColumn );
                lBuffer = NULL;
        	}
            else if ( lOperationResult.GetCode() == PvResult::Code::BUFFER_TOO_SMALL )
            {
                // The buffer is too small to retrieve the image from the source
                // we will need to re-allocate the image to fit the 
                lImage = lBuffer->GetImage();
                if( !lImage )
                {
                    mMainWnd->PostMessage( WM_REALLOCATIONFAIL, ( WPARAM ) mRow, ( LPARAM ) mColumn );
                    return 0;
                }

                mLastPayloadSize = lImage->GetRequiredSize();
                lResult = lBuffer->Alloc( mLastPayloadSize );
                if( !lResult.IsOK() )
                {
                    mMainWnd->PostMessage( WM_REALLOCATIONFAIL, ( WPARAM ) mRow, ( LPARAM ) mColumn );
                    return 0;
                }
   			    mStream.QueueBuffer( lBuffer );
                lBuffer = NULL;
            }
            else
            {
    			mStream.QueueBuffer( lBuffer );
                lBuffer = NULL;
            }
        }

        // Requeue any available buffer
        while( lSmartBuffer = mReturnBufferQueues.Pop() )
        {
            mStream.QueueBuffer( ( PvBuffer* ) lSmartBuffer );
        }        
    }

    return 0;
}

bool StreamThread::CreateNewBuffers( unsigned int aPayloadSize )
{
    PvResult lResult;

    // First allocate all the buffers pool
    for( int i = 0; i < RX_POOL_SIZE; i++ )
    {
        mBuffers[ i ] = new SmartBuffer( &mReturnBufferQueues );
        if( !mBuffers[ i ] )
        {
			if ( i )
			{
				for( int j = 0; j < i; j++ )
				{
					delete mBuffers[ j ];
					mBuffers[ j ] = NULL;
				}
			}
			return false;
        }
    }

    // Now we can set the size of the buffers
    for( int i = 0; i < RX_POOL_SIZE; i++ )
    {
        lResult = mBuffers[ i ]->Alloc( aPayloadSize );
        if( !lResult.IsOK() )
        {
            for( int j = 0; j <= i; j++ )
            {
                delete mBuffers[ j ];
                mBuffers[ j ] = NULL;
            }
            return false;
        }
    }
    
    return true;
}

unsigned int StreamThread::GetPayloadSize( unsigned int& aWidth, unsigned int& aHeight )
{
    PvInt64 lWidthValue = 0;
    PvInt64 lHeightValue = 0;
    PvInt64 lPayloadSizeValue = 0;
    PvInt64 lPoorMansPayloadSize = 0;
    PvInt64 lPixelFormatValue = 0;
    PvInt64 lPixelSizeInBits = 0;

    PvGenInteger *lPayloadSize = dynamic_cast<PvGenInteger *>( mDevice.GetGenParameters()->Get( "PayloadSize" ) );
    PvGenInteger *lWidth = dynamic_cast<PvGenInteger *>( mDevice.GetGenParameters()->Get( "Width" ) );
    PvGenInteger *lHeight = dynamic_cast<PvGenInteger *>( mDevice.GetGenParameters()->Get( "Height" ) );
    PvGenEnum *lPixelFormat = dynamic_cast<PvGenEnum *>( mDevice.GetGenParameters()->Get( "PixelFormat" ) );

    aWidth = 0;
    aHeight = 0;

    // Get payload size
    if ( lPayloadSize != NULL )
    {
        lPayloadSize->GetValue( lPayloadSizeValue );

        // Round up to make it mod 32 (works around an issue with some devices)
        if ((lPayloadSizeValue % 32) != 0)
        {
            lPayloadSizeValue = ((lPayloadSizeValue / 32) + 1) * 32;
        }
    }

    // Get the width and the height
    lWidth->GetValue( lWidthValue );
    aWidth = ( unsigned int ) lWidthValue;
    lHeight->GetValue( lHeightValue );
    aHeight = ( unsigned int ) lHeightValue;

    // Compute poor man's payload size - for devices not maintaning PayloadSize properly
    lPixelFormat->GetValue( lPixelFormatValue );
    lPixelSizeInBits = PvGetPixelBitCount( static_cast<PvPixelType>( lPixelFormatValue ) );
    lPoorMansPayloadSize = ( lWidthValue * lHeightValue * lPixelSizeInBits ) / 8;

    PvInt64 lBestPayloadSize = max( lPayloadSizeValue, lPoorMansPayloadSize );
    if ( ( lBestPayloadSize > 0 ) && ( lBestPayloadSize < UINT_MAX ) )
    {
        return static_cast<unsigned int>( lBestPayloadSize );
    }

    // Could not compute/retrieve payload size...
    return 0;
}

void StreamThread::ShowOptions()
{
    CString lTemp;

    if ( mDevice.IsConnected() )
    {
        mGenBrowserTabbedWnd.AddGenParameterArray( mDevice.GetGenLink(), "Communication Parameters" );
        mGenBrowserTabbedWnd.AddGenParameterArray( mDevice.GetGenParameters(), "Device Parameters" );
    }
    if ( mStream.IsOpen() )
    {
        mGenBrowserTabbedWnd.AddGenParameterArray( mStream.GetParameters(), "Stream Parameters" );
    }

    // Make the title bar
	if ( mRow == 0 )
	{
		lTemp.Format( _T( "Video Source %d" ), ( mRow + 1 ) * ( mColumn + 1 ) );
	}
	else
	{
		lTemp.Format( _T( "Video Source %d" ), ( mRow + 1 ) + ( mColumn + 1 ) );
	}
    mGenBrowserTabbedWnd.SetTitle( lTemp.GetBuffer() );

    mGenBrowserTabbedWnd.ShowModeless( mMainWnd->GetSafeHwnd() );
}

void StreamThread::OnLinkDisconnected( PvDevice *aDevice )
{
    // Notify the main application that the connection is lost
    mMainWnd->PostMessage( WM_LINKDISCONNECTED, ( WPARAM ) mRow, ( LPARAM ) mColumn );
}

bool StreamThread::Store( PvConfigurationWriter& aWriter )
{
    CString lTemp;

    if( mConnected )
    {
        // Store the device
        if( mSetup.GetRole() == ROLE_CTRLDATA )
        {
            lTemp.Format( _T( "Device( %d, %d )" ), mRow, mColumn );
            aWriter.Store( &mDevice, lTemp.GetBuffer() );
        }

        // Store the stream
        lTemp.Format( _T( "Stream( %d, %d )" ), mRow, mColumn );
        aWriter.Store( &mStream, lTemp.GetBuffer() );

        // Store the setup information
        mSetup.Store( aWriter, mRow, mColumn );      
    }

    return true;
}

bool StreamThread::Restore( PvConfigurationReader& aReader )
{
    CString lTemp;

    if( mConnected )
    {
        Disconnect();
    }

    // Read the setup information first to understand how to deal with the 
    // stream and the device
    mSetup.Restore( aReader, mRow, mColumn );

    switch( mSetup.GetRole() )
    {
    case ROLE_CTRLDATA:
        lTemp.Format( _T( "Device( %d, %d )" ), mRow, mColumn );
        if( !aReader.Restore( lTemp.GetBuffer(), &mDevice ).IsOK() )
        {
            return false;
        }
    case ROLE_DATA:
        lTemp.Format( _T( "Stream( %d, %d )" ), mRow, mColumn );
        if( !aReader.Restore( lTemp.GetBuffer(), mStream ).IsOK() )
        {
            mDevice.Disconnect();
            return false;
        }
        break;
    default:
        return false;
        break;
    }

    // Now we need to make the link between the thread and the stream
    if ( mSetup.GetRole() == ROLE_CTRLDATA )
    {
        switch( mSetup.GetDestination() )
        {
        case DESTINATION_UNICAST_AUTO:
        case DESTINATION_UNICAST_SPECIFIC:
            mDevice.SetStreamDestination( mStream.GetLocalIPAddress(),  mStream.GetLocalPort() );
            break;
        case DESTINATION_MULTICAST:
            mDevice.SetStreamDestination( mSetup.GetIPAddress().GetBuffer(), mSetup.GetPort() );
            break;
        }
    }

    // Now we will retrieve the device information to be able to display it
    PvGenParameterArray *lGenDevice = mDevice.GetGenParameters();

    // IP
    PvGenInteger *lIPAddressParam = lGenDevice->GetInteger( "GevCurrentIPAddress" );
    PvInt64 lIPAddress = 0;
    lIPAddressParam->GetValue( lIPAddress );
    unsigned char *lIPPtr = reinterpret_cast<unsigned char *>( &lIPAddress );
    mIPAddress.Format( _T( "%i.%i.%i.%i" ), 
        lIPPtr[3], lIPPtr[2], lIPPtr[1], lIPPtr[0] );

    // MAC address
    PvGenInteger *lMACAddressParam = lGenDevice->GetInteger( "GevMACAddress" );
    PvInt64 lMACAddress;
    lMACAddressParam->GetValue( lMACAddress );
    unsigned char *lMACPtr = reinterpret_cast<unsigned char *>( &lMACAddress );
    mMACAddress.Format( _T( "%02X:%02X:%02X:%02X:%02X:%02X" ), 
        lMACPtr[5], lMACPtr[4], lMACPtr[3], lMACPtr[2], lMACPtr[1], lMACPtr[0] );

    // Model name
    PvGenString *lModelName = lGenDevice->GetString( "DeviceModelName" );
    PvString lModelNameStr;
    lModelName->GetValue( lModelNameStr );
    mModel = lModelNameStr.GetUnicode();

    // Device name ( User ID )
    PvGenString *lNameParam = lGenDevice->GetString( "GevDeviceUserID" );
    if ( lNameParam != NULL )
    {
        PvString lStr;
        lNameParam->GetValue( lStr );
        mUserDefinedName = lStr.GetUnicode();
    }

    mConnected = true;
    return true;
}
