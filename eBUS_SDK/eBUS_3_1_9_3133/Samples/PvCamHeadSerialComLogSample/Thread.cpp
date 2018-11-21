// *****************************************************************************
//
//     Copyright (c) 2010, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#include "stdafx.h"

#include "Thread.h"
#include "PvCamHeadSerialComLogSampleDlg.h"


// ==========================================================================
Thread::Thread( PvCamHeadSerialComLogSampleDlg *aDlg, PvDisplayWnd *aDisplayWnd, PvDevice *aDevice, PvPipeline *aPipeline )
    : mMainDlg( aDlg )
    , mDisplayWnd( aDisplayWnd )
    , mDevice( aDevice )
    , mPipeline( aPipeline )
	, mHandle( 0 )
	, mID( 0 )
	, mStop( false )
	, mReturnValue( 0 )
{
}

// ==========================================================================
Thread::~Thread()
{
	if ( mHandle != INVALID_HANDLE_VALUE )
	{
		Stop();
	}
}

// ==========================================================================
void Thread::Start()
{
	mHandle = ::CreateThread(
		NULL,				// Security attributes
		0,					// Stack size, 0 is default
		Link,				// Start address
		this,				// Parameter
		0,					// Creation flags
		&mID );				// Thread ID
}

// ==========================================================================
void Thread::Stop()
{
	ASSERT( mHandle != INVALID_HANDLE_VALUE );

	mStop = true;
	DWORD lRetVal = ::WaitForSingleObject( mHandle, INFINITE );
	ASSERT( lRetVal != WAIT_TIMEOUT  );

	::CloseHandle( mHandle );
	mHandle = INVALID_HANDLE_VALUE;

	mID = 0;
}

// ==========================================================================
void Thread::SetPriority( int aPriority )
{
	ASSERT( mHandle != INVALID_HANDLE_VALUE );
	::SetThreadPriority( mHandle, aPriority );
}

// ==========================================================================
int Thread::GetPriority() const
{
	ASSERT( mHandle != INVALID_HANDLE_VALUE );
	return ::GetThreadPriority( mHandle );
}

// ==========================================================================
bool Thread::IsStopping() const
{
	ASSERT( mHandle != INVALID_HANDLE_VALUE );
	return mStop;
}

// ==========================================================================
bool Thread::IsDone()
{
	if ( ( mHandle == INVALID_HANDLE_VALUE ) ||
		 ( mID == 0 ) )
	{
		return true;
	}

	return ( ::WaitForSingleObject( mHandle, 0 ) == WAIT_OBJECT_0 );
}

// ==========================================================================
unsigned long WINAPI Thread::Link( void *aParam )
{
	Thread *lThis = reinterpret_cast<Thread *>( aParam );
	lThis->mReturnValue = lThis->Function();
	return lThis->mReturnValue;
}

// ==========================================================================
DWORD Thread::GetReturnValue()
{
	return mReturnValue;
}

// ==========================================================================
DWORD Thread::Function()
{
	ASSERT( mDisplayWnd != NULL );
	ASSERT( mMainDlg != NULL );

    // Timestamp used to limit display rate
	DWORD lPrevious = 0;

	for ( ;; )
	{
		// Check if we were signaled to terminate
        if ( IsStopping() )
		{
			break;
		}

		PvBuffer *lBuffer = NULL;
        PvResult  lOperationResult;

        // Try retrieving a buffer, using default timeout
        PvResult lResult = mPipeline->RetrieveNextBuffer( &lBuffer, 0xFFFFFFFF, &lOperationResult );
        if ( lResult.IsOK() )
        {
            if ( lOperationResult.IsOK() )
            {
                //
                // We now have a valid buffer. This is where you would typically process the buffer.
                // -----------------------------------------------------------------------------------------
                // ...

	            // We limit refresh rate to ~ 30 fps max
                DWORD lCurrent = ::GetTickCount();
	            long lDelta = ( lCurrent - lPrevious ) - ( 1000 / 60 );
	            if ( lDelta > 0 )
	            {
		            lPrevious = ::GetTickCount();

                    // Display buffer
                    mDisplayWnd->Display( *lBuffer );
    
                 }
            }

            // Release buffer back to the pipeline
            mPipeline->ReleaseBuffer( lBuffer );
		}
	}

	return 0;
}

