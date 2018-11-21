// *****************************************************************************
//
//     Copyright (c) 2008, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#pragma once

#include "afxwin.h"

#include <PvConfigurationReader.h>
#include <PvConfigurationWriter.h>


class SetupDlg : public CDialog
{
public:

    SetupDlg(CWnd* pParent = NULL);
    virtual ~SetupDlg();

    // Persistence
    void Save( PvConfigurationWriter &aWriter );
    void Load( PvConfigurationReader &aReader );
    bool IsTheSame( PvConfigurationReader &aReader );

    enum Role
    {
        RoleInvalid = -1,
        RoleCtrlData = 0,
        RoleCtrl,
        RoleData,
        RoleMonitor
    };

    enum Destination
    {
        DestinationInvalid = -1,
        DestinationUnicastAuto = 0,
        DestinationUnicastOther = 1,
        DestinationMulticast = 2,
        DestinationUnicastSpecific = 3
    };

    void SetEnabled( bool aEnabled ) { mEnabled = aEnabled; }

    // Get accessors, to be used AFTER dialog is closed
    Role GetRole() const { return mSetup.mRole; }
    Destination GetDestination() const { return mSetup.mDestination; }
    unsigned short GetUnicastSpecificPort() const { return mSetup.mUnicastSpecificPort; }
    const CString &GetUnicastIP() const { return mSetup.mUnicastIP; }
    unsigned short GetUnicastPort() const { return mSetup.mUnicastPort; }
    const CString &GetMulticastIP() const { return mSetup.mMulticastIP; }
    unsigned short GetMulticastPort() const { return mSetup.mMulticastPort; }
    unsigned short GetDefaultChannel() const { return mSetup.mDefaultChannel; }

    // Set accessors, to be used BEFORE dialog is opened
    void SetRole( Role aRole ) { mSetup.mRole = aRole; }
    void SetDestination( Destination aDestination ) { mSetup.mDestination = aDestination; }
    void SetUnicastSpecificPort( unsigned short aPort ) { mSetup.mUnicastSpecificPort = aPort; }
    void SetUnicastIP( const CString &aIP ) { mSetup.mUnicastIP = aIP; }
    void SetUnicastPort( unsigned short aPort ) { mSetup.mUnicastPort = aPort; }
    void SetMulticastIP( const CString &aMulticastIP ) { mSetup.mMulticastIP = aMulticastIP; }
    void SetMulticastPort( unsigned short aMulticastPort ) { mSetup.mMulticastPort = aMulticastPort; }

protected:

    struct Setup
    {
        Setup()
        {
            Reset();
        }

        void Reset()
        {
            mRole = RoleCtrlData;
            mDestination = DestinationUnicastAuto;
            mUnicastSpecificPort = 0;
            mUnicastIP = _T( "0.0.0.0" );
            mUnicastPort = 0;
            mMulticastIP = _T( "239.192.1.1" );
            mMulticastPort = 1042;
            mDefaultChannel = 0;
        }
        
        Role mRole;
        Destination mDestination;
        unsigned short mUnicastSpecificPort;
        CString mUnicastIP;
        unsigned short mUnicastPort;
        CString mMulticastIP;
        unsigned short mMulticastPort;
        unsigned short mDefaultChannel;
    };

    static void Save( PvConfigurationWriter &aWriter, Setup &aSetup );
    static void Load( PvConfigurationReader &aReader, Setup &aSetup );

    static void StrToRole( const CString &aStr, Role &aRole );
    static void StrToDestination( const CString &aStr, Destination &aDestination );

    void EnableInterface();
    void IPStrToCtrl( const CString &aIPStr, CIPAddressCtrl &aCtrl );

    virtual void DoDataExchange(CDataExchange* pDX);
    BOOL OnInitDialog();
    void OnOK();
    void OnCancel();
    afx_msg void OnBnClicked();
    DECLARE_MESSAGE_MAP()

    CButton mCtrlDataRadio;
    CButton mCtrlRadio;
    CButton mDataRadio;
    CButton mMonitorRadio;
    CButton mUnicastSpecificRadio;
    CButton mUnicastAutoRadio;
    CButton mUnicastOtherRadio;
    CIPAddressCtrl mUnicastIPCtrl;
    CEdit mUnicastSpecificPortEdit;
    CStatic mChannelLabel;
    CEdit mChannelEdit;
    CEdit mUnicastPortEdit;
    CButton mMulticastRadio;
    CIPAddressCtrl mMulticastIPCtrl;
    CStatic mUnicastSpecificPortLabel;
    CStatic mUnicastIPLabel;
    CStatic mUnicastPortLabel;
    CEdit mMulticastPortEdit;
    CStatic mMulticastIPLabel;
    CStatic mMulticastPortLabel;

private:

    Setup mSetup;
    bool mEnabled;
};


