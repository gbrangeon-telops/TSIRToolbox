// *****************************************************************************
//
// Copyright (c) 2010, Pleora Technologies Inc., All rights reserved.
//
// -----------------------------------------------------------------------------
//
// Display thread. Does not own the display, which belongs to the view class.
// Responsible of pumping images out of the PvPipeline.
// 
// *****************************************************************************

#include "stdafx.h"

#include "ThreadDisplay.h"

#include <limits>
#include <assert.h>

#include <PvPipeline.h>


#define GEN_INTERLACED ( "Interlaced" )
#define GEN_DEINTERLACING ( "Deinterlacing" )
#define GEN_INVERSION ( "DeinterlacingFieldInversion" )


// ==========================================================================
ThreadDisplay::ThreadDisplay( PvDevice *aDevice, PvPipeline *aPipeline )
    : mEnabled( true )
    , mTargetFPS( 30 )
    , mVSync( false )
    , mDisplayWnd( NULL )
    , mDevice( aDevice )
    , mPipeline( aPipeline )
    , mLatestBuffer( NULL )
    , mLatestOddBuffer( NULL )
    , mLatestEvenBuffer( NULL )
    , mDeinterlacingEnabled( false )
    , mFinalBuffer( NULL )
    , mKeepPartialImages( false )
{
    RegisterInvalidators();
    UpdateDeinterlacer(); 
    ResetStats();
}

// ==========================================================================
ThreadDisplay::~ThreadDisplay()
{
    UnregisterInvalidators();
}

// ==========================================================================
DWORD ThreadDisplay::Function()
{
    for ( ;; )
    {
        // Check if we were signaled to terminate
        if ( IsStopping() )
        {
            CSingleLock lLock( &mBufferMutex );
            VERIFY( lLock.Lock() );
            ReleaseLatestBuffers();
            break;
        }

        PvBuffer *lBuffer = NULL;

        // Try retrieving a buffer, using default timeout
        PvResult lResult = mPipeline->RetrieveNextBuffer( &lBuffer, -1 );
        if ( lResult.IsOK() )
        {
            if ( lBuffer->GetOperationResult().IsOK() || mKeepPartialImages )
            {
                ProcessBuffer( lBuffer );
            }
            else
            {
                mPipeline->ReleaseBuffer( lBuffer );
            }
        }
    }

    return 0;
}

// ==========================================================================
PvBuffer* ThreadDisplay::RetrieveLatestBuffer()
{
    mBufferMutex.Lock();
    return mFinalBuffer;
}

// ==========================================================================
void ThreadDisplay::ReleaseLatestBuffer()
{
    mBufferMutex.Unlock();
}

// ==========================================================================
// This method is called everytime the Interlaced, Deinterlacing or the
// DeinterlactingFieldInversion parameters are invalidated. 
//
// We simply lookup these parameters and refresh the de-interlacing 
// configuration.
//
// Right now we only subscribe to interlacing parameter invalidators. If
// this would change, we should look into only updating the de-interlacing
// configuraiton if the de-interlacing parameters are actually invalidated.
//
void ThreadDisplay::OnParameterUpdate( PvGenParameter *aParameter )
{
    UpdateDeinterlacer();
}

// ==========================================================================
void ThreadDisplay::UpdateDeinterlacer()
{
    // Get all related interlacing parameters
    PvGenBoolean *lInterlaced = dynamic_cast<PvGenBoolean *>( mDevice->GetGenParameters()->Get( GEN_INTERLACED ) );
    PvGenEnum *lDeinterlacing = dynamic_cast<PvGenEnum *>( mDevice->GetGenParameters()->Get( GEN_DEINTERLACING ) );
    PvGenBoolean *lInversion = dynamic_cast<PvGenBoolean *>( mDevice->GetGenParameters()->Get( GEN_INVERSION ) );

    // If the Interlaced and Deinterlacing features exist then
    mDeinterlacingEnabled = false;
    if ( ( lInterlaced != NULL ) && ( lDeinterlacing != NULL ) && ( lInversion != NULL ) )
    {
        // Read Interlaced value
        bool lInterlacedValue = false;
        PvResult lResult = lInterlaced->GetValue( lInterlacedValue );
        if ( !lResult.IsOK() )
        {
            return;
        }

        // Read Deinterlacing value
        PvString lDeinterlacingValue;
        lResult = lDeinterlacing->GetValue( lDeinterlacingValue );
        if ( !lResult.IsOK() )
        {
            return;
        
        }

        // If Interlaced = True and Deinterlacing = False then
        if ( lInterlacedValue && ( lDeinterlacingValue == "Off" ) )
        {
            // Perform de-interlacing... 
            mDeinterlacingEnabled = true;

            // Read DeinterlacingFieldInversionValue
            bool lInversionValue = false;
            lResult = lInversion->GetValue( lInversionValue );
            if ( lResult.IsOK() )
            {
                // ... according to the DeinterlacingFieldInversion feature
                if ( lInversionValue != mFilterDeinterlace.GetFieldInversion() )
                {
                    mFilterDeinterlace.SetFieldInversion( lInversionValue );
                }
            }
        }
    }
}

// ==========================================================================
void ThreadDisplay::RegisterInvalidators()
{
    // Register invalidators for interlacing support
    if ( mDevice != NULL )
    {
        PvGenParameter *lParam = mDevice->GetGenParameters()->Get( GEN_INTERLACED );
        if ( lParam != NULL )
        {
            lParam->RegisterEventSink( this );
        }

        lParam = mDevice->GetGenParameters()->Get( GEN_DEINTERLACING );
        if ( lParam != NULL )
        {
            lParam->RegisterEventSink( this );
        }

        lParam = mDevice->GetGenParameters()->Get( GEN_INVERSION );
        if ( lParam != NULL )
        {
            lParam->RegisterEventSink( this );
        }
    }
}

// ==========================================================================
void ThreadDisplay::UnregisterInvalidators()
{
    // Register invalidators for interlacing support
    if ( mDevice != NULL )
    {
        PvGenParameter *lParam = mDevice->GetGenParameters()->Get( GEN_INTERLACED );
        if ( lParam != NULL )
        {
            lParam->UnregisterEventSink( this );
        }

        lParam = mDevice->GetGenParameters()->Get( GEN_DEINTERLACING );
        if ( lParam != NULL )
        {
            lParam->UnregisterEventSink( this );
        }

        lParam = mDevice->GetGenParameters()->Get( GEN_INVERSION );
        if ( lParam != NULL )
        {
            lParam->UnregisterEventSink( this );
        }
    }
}

// ==========================================================================
void ThreadDisplay::ResetStats()
{
    mStabilizer.Reset();
}

// ==========================================================================
void ThreadDisplay::ReleaseLatestBuffers()
{
    if ( mLatestBuffer != NULL )
    {
        mPipeline->ReleaseBuffer( mLatestBuffer );
        mLatestBuffer = NULL;
    }

    if ( mLatestOddBuffer != NULL )
    {
        mPipeline->ReleaseBuffer( mLatestOddBuffer );
        mLatestOddBuffer = NULL;
    }

    if ( mLatestEvenBuffer != NULL )
    {
        mPipeline->ReleaseBuffer( mLatestEvenBuffer );
        mLatestEvenBuffer = NULL;
    }
}

// ==========================================================================
bool ThreadDisplay::AreBlockIDsConsecutive( PvBuffer *aFirst, PvBuffer *aSecond )
{
    PvUInt64 lFirst = aFirst->GetBlockID();
    PvUInt64 lSecond = aSecond->GetBlockID();
    PvUInt64 lExpected = lFirst + 1;
	bool lExtendedID = aFirst->IsExtendedID();

    if( ( lExtendedID && lFirst == 0xFFFFFFFFFFFFFFFFULL )
        || ( !lExtendedID && lFirst == 0xFFFFULL ) )
    {
        lExpected = 1;
    }
    return ( lSecond == lExpected );
}

// ==========================================================================
void ThreadDisplay::ProcessBuffer( PvBuffer *aBuffer )
{
    // Based on regular or interlaced status, set latest buffer
    if ( !SetLatest( aBuffer ) )
    {
        return;
    }

    // Display and main dialog refresh throttled by FPS stabilizer
    if ( mStabilizer.IsTimeToDisplay( mTargetFPS ) ) 
    {
        // Only display if the display option is enabled!
        if ( mEnabled )
        {
            CSingleLock lLock( &mBufferMutex );
            VERIFY( lLock.Lock() );
            Display( mFinalBuffer );
        }

        // Send notification to main dialog to update status
        // mMainDlg->PostMessage( WM_IMAGEDISPLAYED );
    }
}

// ==========================================================================
void ThreadDisplay::Display( PvBuffer *aBuffer )
{
    CSingleLock lLock( &mDisplayWndMutex );
    VERIFY( lLock.Lock() );
    
    // Display buffer
    if ( mDisplayWnd != NULL )
    {
        mDisplayWnd->Display( *aBuffer, mVSync );
    }
}

// ==========================================================================
bool ThreadDisplay::SetLatest( PvBuffer *aBuffer )
{
    CSingleLock lLock( &mBufferMutex );
    VERIFY( lLock.Lock() );

    PvImage *lImage = aBuffer->GetImage();
    ASSERT( lImage != NULL );

    // since we've added the "keep partial images" option, this is a safeguard to make sure
    // we don't try to display a buffer with 0 width or height
    if ( ( lImage->GetWidth() == 0 ) || 
		 ( lImage->GetHeight() == 0 ) || 
		 ( aBuffer->GetAcquiredSize() > aBuffer->GetSize() ) || 
		 ( lImage->GetRequiredSize() > aBuffer->GetSize() ) )
    {
        mPipeline->ReleaseBuffer( aBuffer );
        return false;
    }

    if ( lImage->IsInterlacedOdd() )
    {
        // Save/replace Odd temp buffer, wait for matching Even
        ReleaseLatestBuffers();
        mLatestOddBuffer = aBuffer;
        return false;
    }
    else if ( lImage->IsInterlacedEven() )
    {
        // Do we have an Odd buffer? Is the Odd buffer matching this one?
        if ( ( mLatestOddBuffer != NULL ) && ( AreBlockIDsConsecutive( mLatestOddBuffer, aBuffer ) ) )
        {
            mLatestEvenBuffer = aBuffer;
        }
        else
        {
            mPipeline->ReleaseBuffer( aBuffer );
            return false;
        }
    }
    else
    {
        // No interlacing, just keep this buffer
        ReleaseLatestBuffers();
        mLatestBuffer = aBuffer;
    }

    if ( mLatestBuffer != NULL )
    {
        // Single buffer: progressive scan or old iPORT ANL de-interlacing
        mFinalBuffer = mLatestBuffer;
        if ( mDeinterlacingEnabled )
        {
            // De-interlacing required for old iPORT ANL
            mFilterDeinterlace.Apply( mFinalBuffer, &mDeinterlacedBuffer );
            mFinalBuffer = &mDeinterlacedBuffer;
            
            // Final pointing on de-interlaced buffer, we can release
            ReleaseLatestBuffers();
        }
    }
    else if ( ( mLatestOddBuffer != NULL ) && ( mLatestEvenBuffer != NULL ) )
    {
        // Odd and Even buffers available: new iPORT ANL-Pro de-interlacing
        mFilterDeinterlace.Apply( mLatestOddBuffer, mLatestEvenBuffer, &mDeinterlacedBuffer );
        mFinalBuffer = &mDeinterlacedBuffer;

        // Final pointing on de-interlaced buffer, we can release
        ReleaseLatestBuffers();
    }

    return true;
}

// ==========================================================================
void ThreadDisplay::SetDisplay( PvDisplayWnd *aDisplay ) 
{ 
    CSingleLock lLock( &mDisplayWndMutex );
    VERIFY( lLock.Lock() );
    mDisplayWnd = aDisplay; 
}

