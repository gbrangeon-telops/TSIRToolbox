// *****************************************************************************
//
//     Copyright (c) 2007, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#include "stdafx.h"

#include "DisplayThread.h"
#include "GEVPlayerDlg.h"
#include "ImageSaveDlg.h"
#include "FilteringDlg.h"

#include <limits>
#include <assert.h>


///
/// \brief Constructor
///

DisplayThread::DisplayThread( 
        PvDisplayWnd *aDisplayWnd, FilteringDlg *aFilteringDlg, 
        ImageSaveDlg* aImageSaveDlg, LogBuffer *aLogBuffer )
    : mDisplayWnd( aDisplayWnd )
    , mLogBuffer( aLogBuffer )
    , mFilteringDlg( aFilteringDlg )
    , mImageSaveDlg( aImageSaveDlg )
{
    mFilteringDlg->SetDisplayThread( this );
    mImageSaveDlg->SetDisplayThread( this );
}


///
/// \brief Destructor
///

DisplayThread::~DisplayThread()
{
    mFilteringDlg->SetDisplayThread( NULL );
    mImageSaveDlg->SetDisplayThread( NULL );
}


///
/// \brief Perform white balance on the current buffer then update display
///

void DisplayThread::WhiteBalance( PvFilterRGB *aFilterRGB )
{
    // Get the latest buffer - this locks the display thread so we can use the buffer safely
    PvBuffer *lBuffer = RetrieveLatestBuffer();
    if ( lBuffer == NULL )
    {
        return;
    }

    PvImage *lFinalImage = lBuffer->GetImage();

    PvBuffer lBufferDisplay;
	PvImage *lImageDisplay = lBufferDisplay.GetImage();
    lImageDisplay->Alloc( lFinalImage->GetWidth(), lFinalImage->GetHeight(), PV_PIXEL_WIN_RGB32 );

    // Convert last good buffer to RGB, one-time use converter
    PvBufferConverter lConverter( 1 );
    lConverter.Convert( lBuffer, &lBufferDisplay );

    aFilterRGB->WhiteBalance( &lBufferDisplay );

    OnBufferDisplay( lBuffer );

    // Important: release the buffer to unlock the display thread
    ReleaseLatestBuffer();
}


///
/// \brief Callback from PvDisplayThread
///

void DisplayThread::OnBufferDisplay( PvBuffer *aBuffer )
{
    // Configure converter than display buffer
    mFilteringDlg->ConfigureConverter( mDisplayWnd->GetConverter() );
    mDisplayWnd->Display( *aBuffer, GetVSyncEnabled() );
}


///
/// \brief Buffer logging callback. Just add to buffer log.
///

void DisplayThread::OnBufferLog( const PvString &aLog )
{
    mLogBuffer->Log( aLog.GetUnicode() );
}


///
/// \brief Callback from PvDisplayThread
///

void DisplayThread::OnBufferRetrieved( PvBuffer *aBuffer )
{
    SetBufferLogErrorEnabled( mLogBuffer->IsBufferErrorEnabled() );
    SetBufferLogAllEnabled( mLogBuffer->IsBufferAllEnabled() );
}


///
/// \brief Callback from PvDisplayThread
///

void DisplayThread::OnBufferDone( PvBuffer *aBuffer )
{
    // Give the buffer to the image saving dialog - will save if necessary
    mImageSaveDlg->SaveIfNecessary( aBuffer );
}


///
/// \brief Callback from PvDisplayThread
///

void DisplayThread::OnBufferTextOverlay( const PvString &aText )
{
    mDisplayWnd->SetTextOverlay( aText );
}

