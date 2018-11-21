#pragma once

#include "ProgressDlg.h"
#include "SetupDlg.h"
#include <PvConfigurationReader.h>
#include <PvDevice.h>
#include <PvStream.h>
#include <PvSerialBridgeManagerWnd.h>


// =============================================================================
// Thread used to bring back a persisted state
//
class LoadingThread : public Thread
{
public:

    LoadingThread( SetupDlg *aSetupDlg, PvConfigurationReader *aReader, 
        PvDevice *aDevice, PvStream *aStream, PvSerialBridgeManagerWnd *aSerialBridgeManagerWnd,
        PvStringList *aErrorList, CWnd* aParent = NULL );
    virtual ~LoadingThread();
    
    PvResult Load();

protected:

    void SetPrefix( const CString &aPrefix );

    DWORD Function();

private:

    SetupDlg *mSetupDlg;
    PvConfigurationReader *mReader;
    PvDevice *mDevice;
    PvStream *mStream;
    PvSerialBridgeManagerWnd *mSerialBridgeManagerWnd;
    PvStringList *mErrorList;

    ProgressDlg *mDlg;

    PvResult mResult;
};


