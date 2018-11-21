// *****************************************************************************
//
//     Copyright (c) 2010, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#pragma once

#include <PvDevice.h>
#include <PvStream.h>
#include <PvPipeline.h>

#include "ThreadDisplay.h"
#include "Setup.h"


class PvDisplayWnd;


class NetCommandDoc : public CDocument, PvDeviceEventSink
{
protected: // create from serialization only

    NetCommandDoc();

public:

    DECLARE_DYNCREATE(NetCommandDoc)

	virtual ~NetCommandDoc();
    virtual void OnCloseDocument();

    void SetDisplay( PvDisplayWnd *aDisplay );

    void StartAcquisition();
    void StopAcquisition();

    PvDevice *GetDevice() { return &mDevice; }
    PvStream *GetStream() { return &mStream; }
    PvPipeline *GetPipeline() { return &mPipeline; }

    int GetDisplayFrameRate() { return mThreadDisplay->GetFrameRate(); }

    const CString &GetIPAddress() { return mIPAddress; }
    const CString &GetMACAddress() { return mMACAddress; }
    const CString &GetManufacturer() { return mManufacturer; }
    const CString &GetModel() { return  mModel; }
    const CString &GetName() { return  mName; }

    enum Role
    {
        RoleInvalid = -1,
        RoleDeviceTransmitter = 0,
        RoleDeviceReceiver,
        RoleDeviceTransceiver,
        RoleDevicePeripheral,
        RoleSoftwareReceiver
    };

    Role GetRole() const { return mRole; }
    const CString &GetDescription() const { return mDescription; }

protected:

    virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

    void SetComParameters();

    void OnLinkDisconnected( PvDevice *aDevice );

    PvDevice mDevice;
    PvStream mStream;
    PvPipeline mPipeline;

    ThreadDisplay *mThreadDisplay;

    CMutex mStartStreamingMutex;
    CMutex mStartAcquisitionMutex;

    Role mRole;

    CString mDescription;
    CString mIPAddress;
    CString mMACAddress;
    CString mManufacturer;
    CString mModel;
    CString mName;

    void Connect( PvDeviceInfo *aDI );
    void Disconnect();
    void StartStreaming();
    void StopStreaming();

    PvUInt32 GetPayloadSize();

    Setup mSetup;

    DECLARE_MESSAGE_MAP()

    CWnd *mMainWnd;

private:

};


