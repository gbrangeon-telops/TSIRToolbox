#pragma once

#include "ProgressDlg.h"
#include "SetupDlg.h"
#include <PvConfigurationWriter.h>
#include <PvDevice.h>
#include <PvStream.h>
#include <PvSerialBridgeManagerWnd.h>


// =============================================================================
// Thread used to save a persisted state
//
class SavingThread : public Thread
{
public:

    SavingThread( SetupDlg *aSetupDlg, PvConfigurationWriter *aWriter, 
        PvDevice *aDevice, PvStream *aStream, PvSerialBridgeManagerWnd *aSerialBridgeManagerWnd,
        PvStringList *aErrorList, CWnd* aParent = NULL );
    virtual ~SavingThread();

    PvResult Save();

protected:

    void SetPrefix( const CString &aPrefix );

    DWORD Function();

private:

    SetupDlg *mSetupDlg;
    PvConfigurationWriter *mWriter;
    PvDevice *mDevice;
    PvStream *mStream;
    PvSerialBridgeManagerWnd *mSerialBridgeManagerWnd;
    PvStringList *mErrorList;

    ProgressDlg *mDlg;

};

