// *****************************************************************************
//
// Copyright (c) 2012, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#include <PvSampleUtils.h>
#include <PvDeviceFinderWnd.h>
#include <PvTransmitterRaw.h>
#include <PvVersion.h>

#include <sstream>

#include "VideoSource.h"

#include <TCHAR.H>

VideoSource::VideoSource( const char *aDeviceAddress )
    : mDeviceAddress( aDeviceAddress )
{
    mPipeline = new PvPipeline( &mStream );

    Gdiplus::GdiplusStartupInput lGdiplusStartupInput;
	Gdiplus::GdiplusStartup( &mGdiplusToken, &lGdiplusStartupInput, NULL );
}

VideoSource::~VideoSource()
{
    // Pipeline was dynamically allcated, release it.
    if ( mPipeline != NULL )
    {
        delete mPipeline;
        mPipeline = NULL;
    }

    // Free bitmaps.
	gBMap::iterator lIt = mMap.begin();
    while ( lIt != mMap.end() )
    {
        Gdiplus::Bitmap *lImage = ( lIt++ )->second;
        delete lImage;
		lImage = NULL;
    }

	Gdiplus::GdiplusShutdown( mGdiplusToken );
}

bool VideoSource::Connect()
{
	PvDeviceInfo *lDeviceInfo = NULL;
    PvResult lResult = PvResult::Code::OK;
	bool lRetVal = false;

#ifdef PV_GUI_NOT_AVAILABLE
	PvSystem lSystem;
    lDeviceInfo = PvSelectDevice( lSystem );
    if( lDeviceInfo == NULL )
    {
        cout << "No device selected." << endl;
		lRetVal = false;
        return lRetVal;
    }
#else
	// No device address, prompts user.
	PvDeviceFinderWnd lDeviceFinderWnd;
    lResult = lDeviceFinderWnd.ShowModal();

    if( !lResult.IsOK() )
    {
        cout << "No device selected." << endl;
        return lRetVal;
    }
    lDeviceInfo = lDeviceFinderWnd.GetSelected();
#endif // PV_GUI_NOT_AVAILABLE.

	mDeviceAddress = lDeviceInfo->GetIPAddress().GetAscii();
	// Connect device
    lResult = mDevice.Connect( mDeviceAddress.c_str() );
    if ( !lResult.IsOK() )
    {
        cout << "Unable to connect to device." << endl;
        return lRetVal;
    }


    // Open stream.
    lResult = mStream.Open( mDeviceAddress.c_str() );
    if ( !lResult.IsOK() )
    {
        cout << "Unable to open stream." << endl;
        return lRetVal;
    }
	lRetVal = true;

    return lRetVal;
}

bool VideoSource::StartAcquisition()
{
	bool lRetVal = true;
	PvResult lResult = PvResult::Code::OK;
	try
	{
		// Start pipeline.
		lResult =  mPipeline->Start();
		if ( lResult.IsOK() )
		{
			// Set streaming destination.
			lResult = mDevice.SetStreamDestination( mStream.GetLocalIPAddress(), mStream.GetLocalPort() );
			if ( lResult.IsOK() )
			{
				// Set TLParamsLocked to 1.
				lResult = mDevice.GetGenParameters()->SetIntegerValue( "TLParamsLocked", 1 );
			}
			if ( lResult.IsOK() )
			{
				// Start acquisition.
				lResult = mDevice.GetGenParameters()->ExecuteCommand( "AcquisitionStart" );
			}
		}
		if ( !lResult.IsOK() )
		{
			cout << "Failed to start acquisition process." <<  endl;
			cout << "    Error: " << lResult.GetDescription() << endl;
			lRetVal = false;
		}
	}
	catch(...)
	{
		cout << "Failed to start acquisition process. Unexpected error." << endl;
		lRetVal = false;
	}
	return lRetVal;
}

bool VideoSource::StopAcquisition()
{
	PvResult lResult = PvResult::Code::OK;
	bool lRetVal = true;
	try
	{
		// Stop acquisition.
		lResult = mDevice.GetGenParameters()->ExecuteCommand( "AcquisitionStop" );
		if ( lResult.IsOK() )
		{
			// Set TLParamsLocked to 0.
			lResult = mDevice.GetGenParameters()->SetIntegerValue( "TLParamsLocked", 0 );
			if ( lResult.IsOK() )
			{
				// Stop pipeline.
				lResult = mPipeline->Stop();
			}
		}
		if ( !lResult.IsOK() )
		{
			cout << "Failed to stop acquisition process." <<  endl;
			cout << "    Error: " << lResult.GetDescription() << endl;
			lRetVal = false;
		}
	}
	catch(...)
	{
		cout << "Failed to stop acquisition process. Unexpected error." << endl;
		lRetVal = false;
	}
	return lRetVal;
}

bool VideoSource::Disconnect()
{
	PvResult lResult = PvResult::Code::OK;
	bool lRetVal = true;
	try
	{
		if ( mStream.IsOpen() )
		{
			// Close stream.
			lResult = mStream.Close();
			if ( lResult.IsOK() )
			{
				if ( mDevice.IsConnected() )
				{
					// Disconnect the device.
					lResult = mDevice.Disconnect();
				}
			}
		}
		if ( !lResult.IsOK() )
		{
			cout << "Failed to disconnect the device." <<  endl;
			cout << "    Error: " << lResult.GetDescription() << endl;
			lRetVal = false;
		}
	}
	catch(...)
	{
		cout << "Failed to disconnect the device. Unexpected error." << endl;
		lRetVal = false;
	}

	return lRetVal;
}

bool VideoSource::FillBuffer( PvBuffer *aBuffer, PvTransmitterRaw *aTransmitter )
{
    // Get input buffer from pipeline (stream in).
    PvBuffer *lInBuffer = NULL;
	PvResult lOperationResult;

    // Retrieve next buffer
    PvResult lResult = mPipeline->RetrieveNextBuffer( &lInBuffer, 1000, &lOperationResult );
    if ( !lResult.IsOK() )
    {
		return false;
	}

	if( lOperationResult.IsOK() )
	{
		// Get image by ID (one for every buffer, attached for the buffer).
		Gdiplus::Bitmap *lImage = GetImageForBuffer( aBuffer );

		if ( lImage == NULL )
		{
			return false;
		}

		PvUInt32 lWidth = GetWidth();
		PvUInt32 lHeight = GetHeight();

		// Lock bitmap data.
		Gdiplus::BitmapData lBitmapData;
		Gdiplus::Rect lRect(0, 0, lWidth, lHeight );
		lImage->LockBits( &lRect, Gdiplus::ImageLockModeRead, lImage->GetPixelFormat(), &lBitmapData );

		// Attach image to output buffer with conversion-happy pixel type.
		aBuffer->GetImage()->Attach( (byte*)lBitmapData.Scan0, 
			lWidth, lHeight, PvPixelBGR8 );

		// Copy in buffer into image attached to out buffer using eBUS SDK pixel converters.
		mConverter.Convert( lInBuffer, aBuffer, false );
		lImage->UnlockBits( &lBitmapData );

		// Draw (transform part of the sample) on image attached to out buffer.
		Draw( aTransmitter, lImage );
	}

	// Release buffer back to pipeline.
	mPipeline->ReleaseBuffer( lInBuffer );

	return lOperationResult.IsOK();
}

PvUInt32 VideoSource::GetWidth()
{
    PvInt64 lWidth;
    mDevice.GetGenParameters()->GetIntegerValue( "Width", lWidth );

    return static_cast<PvUInt32>( lWidth );
}

PvUInt32 VideoSource::GetHeight()
{
    PvInt64 lHeight;
    mDevice.GetGenParameters()->GetIntegerValue( "Height", lHeight );

    return static_cast<PvUInt32>( lHeight );
}

PvPixelType VideoSource::GetPixelFormat()
{
    PvInt64 lFormat;
    mDevice.GetGenParameters()->GetEnumValue( "PixelFormat", lFormat );

    return static_cast<PvPixelType>( lFormat );
}

Gdiplus::Bitmap *VideoSource::GetImageForBuffer( PvBuffer *aBuffer )
{
    PvUInt32 lID = static_cast<PvUInt32>( aBuffer->GetID() );
    Gdiplus::Bitmap *lImage = NULL;

    PvUInt32 lWidth = GetWidth();
    PvUInt32 lHeight = GetHeight();
	PvUInt32 lImageSize = aBuffer->GetImage()->GetImageSize();
	PvUInt32 lBufAcqSize = aBuffer->GetAcquiredSize();

	lImageSize = lWidth * lHeight;

    if ( mMap.find( lID ) == mMap.end() )
    {
        // No buffer found at map position.
    }
    else
    {
        lImage = mMap[ lID ];
		if ( lImage != NULL )
		{
			if ( ( lImage->GetWidth() != lWidth ) ||
				( lImage->GetHeight() != lHeight ) )
			{
				// Detach from PvBuffer.
				aBuffer->Detach();

				// Image found, but not the right size.
				delete lImage;
				lImage = NULL;
			}
		}
    }

    if ( lImage == NULL )
    {
		// Create a new image of the right size.
		lImage  = new Gdiplus::Bitmap( lWidth, lHeight, PixelFormat24bppRGB );
        mMap[ lID ] = lImage;
    }

    return lImage;
}

void VideoSource::Draw( PvTransmitterRaw *aTransmitter, Gdiplus::Bitmap *aImage )
{
	if ( aImage == NULL )
	{
		return;
	}

	// Create a Graphics object that is associated with the image.
	Gdiplus::Graphics *lImageGraphics = Gdiplus::Graphics::FromImage( aImage );
	if ( lImageGraphics == NULL )
	{
		return;
	}

	HDC lDC = lImageGraphics->GetHDC();
	Gdiplus::Graphics lGraphics( lDC );

	Gdiplus::FontFamily lFontFamily( L"Arial" );
	Gdiplus::Font lFont( &lFontFamily, 24, Gdiplus::FontStyleRegular, Gdiplus::UnitPixel );
	Gdiplus::PointF lPointF( 25.0F, 25.0F );
	Gdiplus::SolidBrush lSolidBrush( Gdiplus::Color( 0x00, 0xFF, 0xFF ) );

    stringstream lSS;

    lSS << "PvTransformAndTransmitSample" << endl;
    lSS << PRODUCT_NAME << " " << NVERSION_STRING << endl << endl;

    lSS << fixed << setprecision( 1 );
    lSS << "Transmitted " << aTransmitter->GetBlocksTransmitted() << " blocks" << endl;

    lSS << fixed << setprecision( 1 );
    lSS << aTransmitter->GetAverageTransmissionRate() << " (" << aTransmitter->GetInstantaneousTransmissionRate() << ") FPS" << endl;

    lSS << fixed << setprecision( 1 );
    lSS << aTransmitter->GetAveragePayloadThroughput() / 1000000.0f <<
        " (" << aTransmitter->GetInstantaneousPayloadThroughput() / 1000000.0f << ") Mb/s" << endl;

	PvString lString( lSS.str().c_str() );
	lGraphics.DrawString( lString.GetUnicode(), lString.GetLength(), &lFont, lPointF, &lSolidBrush );

	lImageGraphics->ReleaseHDC( lDC );
	delete lImageGraphics;
}



