// *****************************************************************************
//
//     Copyright (c) 2007, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#pragma once

#include <queue>


template<class T>
class TLockableQueue : protected std::queue<T>
{
public:

    TLockableQueue()
    {
        mQueueLock = ::CreateMutex( NULL, FALSE, NULL );
        mItemPresent = ::CreateEvent( NULL, TRUE, FALSE, NULL );
    }

    ~TLockableQueue()
    {
        ::CloseHandle( mQueueLock );
        ::CloseHandle( mItemPresent );
    }
    
    typedef value_type QueueItem_t;				
    typedef size_type QueueSize_t;
    
    // Write functions

    QueueItem_t Pop()
    {
        ::WaitForSingleObject( mQueueLock, INFINITE );
        QueueItem_t poppedItem = mQueue.front();
        mQueue.pop();
        if ( mQueue.empty() )
        {
            ::ResetEvent( mItemPresent );
        }
        ::ReleaseMutex( mQueueLock );
        return poppedItem;
    }

    void Push( QueueItem_t newItem )
    {
        ::WaitForSingleObject( mQueueLock, INFINITE );
        mQueue.push( newItem );
        ::SetEvent( mItemPresent );
        ::ReleaseMutex( mQueueLock );
    }

    void Clear()
    {
        ::WaitForSingleObject( mQueueLock, INFINITE );
        mQueue.clear();
        ::ResetEvent( mItemPresent );
        ::ReleaseMutex( mQueueLock );					
    }

    // Read/Query functions

    QueueItem_t Back()
    {
        ::WaitForSingleObject( mQueueLock, INFINITE );
        QueueItem_t backElement = mQueue.back();
        ::ReleaseMutex( mQueueLock );
        return backElement;
    }

    QueueItem_t Front()
    {
        ::WaitForSingleObject( mQueueLock, INFINITE );
        QueueItem_t frontElement = mQueue.front();
        ::ReleaseMutex( mQueueLock );
        return frontElement;
    }

    QueueSize_t Size()
    {
        ::WaitForSingleObject( mQueueLock, INFINITE );
        QueueSize_t queueSize = mQueue.size();
        ::ReleaseMutex( mQueueLock );
        return queueSize;
    }

    bool Empty()
    {
        ::WaitForSingleObject( mQueueLock, INFINITE );
        bool isEmpty = mQueue.empty();
        ::ReleaseMutex( mQueueLock );
        return isEmpty;
    }

    bool WaitForItem( DWORD timeout, QueueItem_t &aItem )
    {
        DWORD lRetVal = ::WaitForSingleObject( mItemPresent, timeout );
        if ( lRetVal != WAIT_TIMEOUT )
        {
            aItem = (QueueItem_t)Pop();
            return true;
        }

        return false;
    }

protected:

    HANDLE mItemPresent;
    HANDLE mQueueLock;

    std::queue<T> mQueue;
};

