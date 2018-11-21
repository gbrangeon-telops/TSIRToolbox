// *****************************************************************************
//
//     Copyright (c) 2010, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#pragma once

#include "Thread.h"

#include <PvDisplayWnd.h>
#include <PvBuffer.h>
#include <PvBufferConverter.h>
#include <PvDevice.h>
#include <PvFilterDeinterlace.h>
#include <PvFPSStabilizer.h>

#include <math.h>


class ThreadDisplay : public Thread, PvGenEventSink
{
public:

    ThreadDisplay( PvDevice *aDevice, PvPipeline *aPipeline );
    ~ThreadDisplay();

    bool GetEnabled() const { return mEnabled; }
    void SetEnabled( bool aEnabled ) { mEnabled = aEnabled; }

    bool GetVSync() const { return mVSync; }
    void SetVSync( bool aVSync ) { mVSync = aVSync; }

    PvUInt32 GetTargetFPS() const { return mTargetFPS; }
    void SetTargetFPS( PvUInt32 aTarget ) { mTargetFPS = aTarget; }

    PvBuffer* RetrieveLatestBuffer();
    void ReleaseLatestBuffer();

    int GetFrameRate() { return ( mEnabled ) ? mStabilizer.GetAverage() : 0; }

    void ResetStats();

    bool GetDeinterlacingEnabled() const { return mDeinterlacingEnabled; }

    bool GetKeepPartialImages() { return mKeepPartialImages; }
    void SetKeepPartialImages( bool aKeepPartialImages ) { mKeepPartialImages = aKeepPartialImages; }

    void SetDisplay( PvDisplayWnd *aDisplay );

protected:

    void RegisterInvalidators();
    void UnregisterInvalidators();
    void UpdateDeinterlacer();

    void ReleaseLatestBuffers();
    void ProcessBuffer( PvBuffer *aBuffer );
    bool AreBlockIDsConsecutive( PvBuffer *aFirst, PvBuffer *aSecond );
    void Display( PvBuffer *aBuffer );
    bool SetLatest( PvBuffer *aBuffer );

    void OnParameterUpdate( PvGenParameter *aParameter );

    DWORD Function();

private:

    // We do not own these objects
    PvDevice *mDevice;
    PvPipeline * mPipeline;
    PvDisplayWnd *mDisplayWnd;

    CMutex mDisplayWndMutex;

    PvFilterDeinterlace mFilterDeinterlace;

    CMutex mBufferMutex;
    PvBuffer *mLatestBuffer;
    PvBuffer *mLatestEvenBuffer;
    PvBuffer *mLatestOddBuffer;

    PvBuffer *mFinalBuffer;
    PvBuffer mDeinterlacedBuffer;

    bool mEnabled;
    bool mVSync;
    bool mDeinterlacingEnabled;
    PvUInt32 mTargetFPS;

    PvFPSStabilizer mStabilizer;

    bool mKeepPartialImages;
};

