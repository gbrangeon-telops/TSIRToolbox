// *****************************************************************************
//
//     Copyright (c) 2012, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#include "stdafx.h"

#include "Thread.h"


// ==========================================================================
Thread::Thread()
    : mHandle( INVALID_HANDLE_VALUE )
    , mID( 0 )
    , mStop( false )
    , mReturnValue( 0 )
    , mStartEvent( INVALID_HANDLE_VALUE )
{
}

// ==========================================================================
Thread::~Thread()
{
    if ( mHandle != INVALID_HANDLE_VALUE )
    {
        Stop();
    }

    if( mStartEvent != INVALID_HANDLE_VALUE )
    {
        ::CloseHandle( mStartEvent );
        mStartEvent = INVALID_HANDLE_VALUE;
    }
}

// ==========================================================================
bool Thread::Start()
{
    if( mStartEvent == INVALID_HANDLE_VALUE )
    {
        mStartEvent = ::CreateEvent( NULL, TRUE, FALSE, NULL );
        if( mStartEvent == INVALID_HANDLE_VALUE )
        {
            return false;
        }
    }

    mStop = false;
    mHandle = ::CreateThread(
        NULL,				// Security attributes
        0,					// Stack size, 0 is default
        Link,				// Start address
        this,				// Parameter
        0,					// Creation flags
        &mID );				// Thread ID

    ::WaitForSingleObject( mStartEvent, INFINITE );

    return true;
}

// ==========================================================================
bool Thread::Stop()
{
    if( mHandle != INVALID_HANDLE_VALUE )
    {
        mStop = true;
        DWORD lRetVal = ::WaitForSingleObject( mHandle, INFINITE );
        ASSERT( lRetVal != WAIT_TIMEOUT  );

        ::CloseHandle( mHandle );
        mHandle = INVALID_HANDLE_VALUE;

        ::ResetEvent( mStartEvent );
        mID = 0;
    }

    return true;
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

    ::SetEvent( lThis->mStartEvent );

    lThis->mReturnValue = lThis->Function();
    return lThis->mReturnValue;
}

// ==========================================================================
DWORD Thread::GetReturnValue()
{
    return mReturnValue;
}

bool Thread::IsStarted()
{
    return ( mHandle != INVALID_HANDLE_VALUE );
}

