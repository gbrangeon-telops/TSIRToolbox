// *****************************************************************************
//
//     Copyright (c) 2007, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#pragma once

#include "LockableQueue.h"
#include "PvBuffer.h"


class Thread
{
public:

    Thread();
    ~Thread();

    void Start();

	void SignalStop(); // but don't block...
    void Stop(); // signal and blocks until the thread is gone

    void SetPriority( int aPriority );
    int GetPriority() const;

    bool IsDone();
    DWORD GetReturnValue();

protected:

    static unsigned long WINAPI Link( void *aParam );
    virtual DWORD Function() = 0;

    bool IsStopping() const;

private:

    HANDLE mHandle;
    DWORD mID;

    bool mStop;

    DWORD mReturnValue;
};
