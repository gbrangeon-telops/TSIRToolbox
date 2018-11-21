#pragma once

#include "ProgressDlg.h"
#include "Setup.h"

#include <PvDeviceInfo.h>
#include <PvDevice.h>
#include <PvStream.h>


// =============================================================================
// Thread used to setup the system on a connect operation
//
class ConnectionThread : public Thread
{
public:

    ConnectionThread( Setup *aSetup, PvDeviceInfo *aDeviceInfo, PvDevice *aDevice, PvStream *aStream, CWnd* aParent );
    virtual ~ConnectionThread();
    
    PvResult Connect();

protected:

    DWORD Function();

private:

    Setup *mSetup;
    PvDeviceInfo *mDeviceInfo;
    PvDevice *mDevice;
    PvStream *mStream;
    ProgressDlg *mDlg;
    PvResult mResult;
};


