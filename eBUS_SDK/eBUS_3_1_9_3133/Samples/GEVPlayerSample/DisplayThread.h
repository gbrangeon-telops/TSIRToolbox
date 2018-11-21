// *****************************************************************************
//
//     Copyright (c) 2007, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#pragma once

#include "resource.h"

#include "LogBuffer.h"

#include <PvDisplayThread.h>
#include <PvDisplayWnd.h>


class ImageSaveDlg;
class FilteringDlg;


class DisplayThread : public PvDisplayThread
{
public:

    DisplayThread( 
        PvDisplayWnd *aDisplayWnd, FilteringDlg *aFilteringDlg, 
        ImageSaveDlg* aImageSaveDlg, LogBuffer *aLogBuffer );
    ~DisplayThread();

    void WhiteBalance( PvFilterRGB *aFilterRGB );

protected:

    void Log( PvBuffer *aBuffer, PvUInt64 aLastBlockID );

    void OnBufferRetrieved( PvBuffer *aBuffer );
    void OnBufferDisplay( PvBuffer *aBuffer );
    void OnBufferDone( PvBuffer *aBuffer );
    void OnBufferLog( const PvString &aLog );
    void OnBufferTextOverlay( const PvString &aText );

private:

    LogBuffer *mLogBuffer;

    PvDisplayWnd *mDisplayWnd;
    FilteringDlg *mFilteringDlg;
    ImageSaveDlg *mImageSaveDlg;
};
