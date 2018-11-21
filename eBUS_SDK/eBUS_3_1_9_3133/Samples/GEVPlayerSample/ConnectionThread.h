#pragma once

#include "ProgressDlg.h"
#include "SetupDlg.h"
#include <PvDeviceInfo.h>
#include <PvDevice.h>
#include <PvStream.h>

// =============================================================================
// Thread used to setup the system on a connect operation
//
class ConnectionThread : public Thread
{
public:

    ConnectionThread( SetupDlg *aSetupDlg, PvDeviceInfo *aDeviceInfo, PvDevice *aDevice, PvStream *aStream, CWnd* aParent, const PvString &aLocalIPAddress = "", const PvString &aDeviceIPAddress = "" );
    virtual ~ConnectionThread();
    
    PvResult Connect();

    void SetTitle( const CString &aTitle ) { mTitle = aTitle; }

protected:

	void SetStreamDestination( const CString &aIPAddress, PvUInt16 aPort, PvUInt16 aChannel );
    void NegotiatePacketSize();

    DWORD Function();

private:

    SetupDlg *mSetupDlg;
    PvDeviceInfo *mDeviceInfo;
    PvDevice *mDevice;
    PvStream *mStream;
    ProgressDlg *mDlg;
    PvResult mResult;
    PvString mLocalIPAddress;
    PvString mDeviceIPAddress;

    CString mTitle;
};

