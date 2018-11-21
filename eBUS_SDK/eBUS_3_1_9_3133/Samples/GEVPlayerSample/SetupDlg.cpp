// *****************************************************************************
//
//     Copyright (c) 2008, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#include "stdafx.h"
#include "GEVPlayer.h"
#include "SetupDlg.h"


BEGIN_MESSAGE_MAP(SetupDlg, CDialog)
    ON_BN_CLICKED(IDC_CTRLDATA, &SetupDlg::OnBnClicked)
    ON_BN_CLICKED(IDC_CTRL, &SetupDlg::OnBnClicked)
    ON_BN_CLICKED(IDC_DATA, &SetupDlg::OnBnClicked)
    ON_BN_CLICKED(IDC_MONITOR, &SetupDlg::OnBnClicked)
    ON_BN_CLICKED(IDC_UNICASTAUTO, &SetupDlg::OnBnClicked)
    ON_BN_CLICKED(IDC_UNICASTSPECIFIC, &SetupDlg::OnBnClicked)
    ON_BN_CLICKED(IDC_UNICASTOTHER, &SetupDlg::OnBnClicked)
    ON_BN_CLICKED(IDC_MULTICAST, &SetupDlg::OnBnClicked)
END_MESSAGE_MAP()


#define SETUPDLG_VERSION ( _T( "1.0.0.0" ) )

#define TAG_VERSION ( _T( "setupversion" ) )
#define TAG_ROLE ( _T( "setuprole" ) )
#define TAG_DESTINATION ( _T( "setupdestination" ) )
#define TAG_UNICASTSPECIFICPORT ( _T( "setupunicastspecificport" ) )
#define TAG_DEFAULTCHANNEL ( _T( "setupdefaultchannel" ) )
#define TAG_UNICASTIP ( _T( "setupunicastip" ) )
#define TAG_UNICASTPORT ( _T( "setupunicastport" ) )
#define TAG_MULTICASTIP ( _T( "setupmulticastip" ) )

#define TAG_MULTICASTPORT ( _T( "defaultchannel" ) )

#define VAL_ROLECTRLDATA ( _T( "ctrldata" ) )
#define VAL_ROLECTRL ( _T( "ctrl" ) )
#define VAL_ROLEDATA ( _T( "data" ) )
#define VAL_ROLEMONITOR ( _T( "monitor" ) )
#define VAL_DESTINATIONUNICASTSPECIFIC ( _T( "unicastspecific" ) )
#define VAL_DESTINATIONUNICASTAUTO ( _T( "unicastauto" ) )
#define VAL_DESTINATIONUNICASTOTHER ( _T( "unicastother" ) )
#define VAL_DESTINATIONMULTICAST ( _T( "multicast" ) )


// =============================================================================
SetupDlg::SetupDlg(CWnd* pParent /*=NULL*/)
    : CDialog(IDD_SETUP, pParent)
    , mEnabled( true )
{
}


// =============================================================================
SetupDlg::~SetupDlg()
{
}


// =============================================================================
void SetupDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_CTRLDATA, mCtrlDataRadio);
    DDX_Control(pDX, IDC_CTRL, mCtrlRadio);
    DDX_Control(pDX, IDC_DATA, mDataRadio);
    DDX_Control(pDX, IDC_MONITOR, mMonitorRadio);
    DDX_Control(pDX, IDC_UNICASTAUTO, mUnicastAutoRadio);
    DDX_Control(pDX, IDC_UNICASTSPECIFIC, mUnicastSpecificRadio);
    DDX_Control(pDX, IDC_UNICASTOTHER, mUnicastOtherRadio);
    DDX_Control(pDX, IDC_SPECIFICPORT, mUnicastSpecificPortEdit);
    DDX_Control(pDX, IDC_SPECIFICPORTLABEL, mUnicastSpecificPortLabel);
    DDX_Control(pDX, IDC_IP, mUnicastIPCtrl);
    DDX_Control(pDX, IDC_PORT, mUnicastPortEdit);
    DDX_Control(pDX, IDC_IPLABEL, mUnicastIPLabel);
    DDX_Control(pDX, IDC_PORTLABEL, mUnicastPortLabel);
    DDX_Control(pDX, IDC_MULTICAST, mMulticastRadio);
    DDX_Control(pDX, IDC_MULTICASTIP, mMulticastIPCtrl);
    DDX_Control(pDX, IDC_MULTICASTPORT, mMulticastPortEdit);
    DDX_Control(pDX, IDC_MULTICASTIPLABEL, mMulticastIPLabel);
    DDX_Control(pDX, IDC_MULTICASTPORTLABEL, mMulticastPortLabel);
    DDX_Control(pDX, IDC_CHANNEL_EDIT, mChannelEdit);
    DDX_Control(pDX, IDC_CHANNEL_LABEL, mChannelLabel);
}


// =============================================================================
BOOL SetupDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    // Role
    int lRole = IDC_CTRLDATA;
    switch ( mSetup.mRole )
    {
        case RoleCtrlData:
            lRole = IDC_CTRLDATA;
            break;

        case RoleCtrl:
            lRole = IDC_CTRL;
            break;

        case RoleData:
            lRole = IDC_DATA;
            break;

        case RoleMonitor:
            lRole = IDC_MONITOR;
            break;

        default:
            ASSERT( 0 );
    }

    CheckRadioButton( IDC_CTRLDATA, IDC_MONITOR, lRole );

    // Destination
    int lDestination = IDC_UNICASTAUTO;
    switch ( mSetup.mDestination )
    {
        case DestinationUnicastAuto:
            lDestination = IDC_UNICASTAUTO;
            break;

        case DestinationUnicastSpecific:
            lDestination = IDC_UNICASTSPECIFIC;
            break;

        case DestinationUnicastOther:
            lDestination = IDC_UNICASTOTHER;
            break;

        case DestinationMulticast:
            lDestination = IDC_MULTICAST;
            break;

        default:
            ASSERT( 0 );
    }

    CheckRadioButton( IDC_UNICASTAUTO, IDC_MULTICAST, lDestination );

    CString lStr;

    // Specific port
    lStr.Format( _T( "%i" ), mSetup.mUnicastSpecificPort );
    mUnicastSpecificPortEdit.SetWindowText( lStr );

    // Default channel
    lStr.Format( _T( "%i" ), mSetup.mDefaultChannel );
    mChannelEdit.SetWindowText( lStr );

    // IP
    IPStrToCtrl( mSetup.mUnicastIP, mUnicastIPCtrl );

    // Port
    lStr.Format( _T( "%i" ), mSetup.mUnicastPort );
    mUnicastPortEdit.SetWindowText( lStr );

    // Multicast IP
    IPStrToCtrl( mSetup.mMulticastIP, mMulticastIPCtrl );

    // Multicast port
    lStr.Format( _T( "%i" ), mSetup.mMulticastPort );
    mMulticastPortEdit.SetWindowText( lStr );

    EnableInterface();

    return TRUE;
}


// =============================================================================
void SetupDlg::IPStrToCtrl( const CString &aIPStr, CIPAddressCtrl &aCtrl )
{
    int lIP[ 4 ];
    int lCount = swscanf_s( aIPStr, _T( "%i.%i.%i.%i" ), lIP, lIP + 1, lIP + 2, lIP + 3 );
    ASSERT( lCount == 4 );
    if ( lCount == 4 )
    {
        BYTE lIPb[ 4 ];
        for ( int i = 0; i < 4; i++ )
        {
            ASSERT( lIP[ i ] >= 0 );
            ASSERT( lIP[ i ] <= 255 );

            lIPb[ i ] = static_cast<BYTE>( lIP[ i ] );
        }

        aCtrl.SetAddress( lIPb[ 0 ], lIPb[ 1 ], lIPb[ 2 ], lIPb[ 3 ] );
    }
}


// =============================================================================
void SetupDlg::EnableInterface()
{
    if ( !mEnabled )
    {
        // Master disable, not touching the state of the UI
        mCtrlDataRadio.EnableWindow( false );
        mCtrlRadio.EnableWindow( false );
        mDataRadio.EnableWindow( false );
        mMonitorRadio.EnableWindow( false );
        mUnicastAutoRadio.EnableWindow( false );
        mUnicastSpecificRadio.EnableWindow( false );
        mUnicastOtherRadio.EnableWindow( false );
        mMulticastRadio.EnableWindow( false );
        mUnicastSpecificPortEdit.EnableWindow( false );
        mChannelEdit.EnableWindow( false );
        mChannelLabel.EnableWindow( false );
        mUnicastSpecificPortLabel.EnableWindow( false );
        mUnicastIPCtrl.EnableWindow( false );
        mUnicastPortEdit.EnableWindow( false );
        mUnicastIPLabel.EnableWindow( false );
        mUnicastPortLabel.EnableWindow( false );
        mMulticastIPCtrl.EnableWindow( false );
        mMulticastPortEdit.EnableWindow( false );
        mMulticastIPLabel.EnableWindow( false );
        mMulticastPortLabel.EnableWindow( false );

        return;
    }

    int lRole = GetCheckedRadioButton( IDC_CTRLDATA, IDC_MONITOR );
    int lDest = GetCheckedRadioButton( IDC_UNICASTAUTO, IDC_MULTICAST );
    
    bool lCtrlData = ( lRole == IDC_CTRLDATA );
    bool lCtrl = ( lRole == IDC_CTRL );
    bool lData = ( lRole == IDC_DATA );

    // Stream radio buttons
    mUnicastAutoRadio.EnableWindow( ( lCtrlData || lData ) && mEnabled );
    mUnicastSpecificRadio.EnableWindow( ( lCtrlData || lData ) && mEnabled );
    mUnicastOtherRadio.EnableWindow( lCtrl && mEnabled );
    mMulticastRadio.EnableWindow( ( lCtrl || lData || lCtrlData ) && mEnabled );
    mChannelEdit.EnableWindow( ( lData || lCtrlData ) && mEnabled );
    mChannelLabel.EnableWindow( ( lData || lCtrlData ) && mEnabled );

    // Make sure we check the first enabled option (if needed)
    bool lValid = false;
    lValid |= ( lDest == IDC_UNICASTAUTO ) && mUnicastAutoRadio.IsWindowEnabled();
    lValid |= ( lDest == IDC_UNICASTSPECIFIC ) && mUnicastSpecificRadio.IsWindowEnabled();
    lValid |= ( lDest == IDC_UNICASTOTHER ) && mUnicastOtherRadio.IsWindowEnabled();
    lValid |= ( lDest == IDC_MULTICAST ) && mMulticastRadio.IsWindowEnabled();
    if ( !lValid )
    {
        if ( mUnicastAutoRadio.IsWindowEnabled() )
        {
            CheckRadioButton( IDC_UNICASTAUTO, IDC_MULTICAST, IDC_UNICASTAUTO );
        }
        else if ( mUnicastSpecificRadio.IsWindowEnabled() )
        {
            CheckRadioButton( IDC_UNICASTAUTO, IDC_MULTICAST, IDC_UNICASTSPECIFIC );
        }
        else if ( mUnicastOtherRadio.IsWindowEnabled() )
        {
            CheckRadioButton( IDC_UNICASTAUTO, IDC_MULTICAST, IDC_UNICASTOTHER  );
        }
        else if ( mMulticastRadio.IsWindowEnabled() )
        {
            CheckRadioButton( IDC_UNICASTAUTO, IDC_MULTICAST, IDC_MULTICAST );
        }

        // ...refresh!
        lRole = GetCheckedRadioButton( IDC_CTRLDATA, IDC_MONITOR );
        lDest = GetCheckedRadioButton( IDC_UNICASTAUTO, IDC_MULTICAST );
    }

    // Unicast specific port
    mUnicastSpecificPortEdit.EnableWindow( ( lDest == IDC_UNICASTSPECIFIC ) && mUnicastSpecificRadio.IsWindowEnabled() && mEnabled );
    mUnicastSpecificPortLabel.EnableWindow( mUnicastSpecificPortEdit.IsWindowEnabled() );

    // Unicast other IP/port
    mUnicastIPCtrl.EnableWindow( ( lDest == IDC_UNICASTOTHER ) && mUnicastOtherRadio.IsWindowEnabled() && mEnabled );
    mUnicastPortEdit.EnableWindow( mUnicastIPCtrl.IsWindowEnabled() );
    mUnicastIPLabel.EnableWindow( mUnicastIPCtrl.IsWindowEnabled() );
    mUnicastPortLabel.EnableWindow( mUnicastIPCtrl.IsWindowEnabled() );

    // Multicast IP/port
    mMulticastIPCtrl.EnableWindow( ( lDest == IDC_MULTICAST ) && mMulticastRadio.IsWindowEnabled() && mEnabled );
    mMulticastPortEdit.EnableWindow( mMulticastIPCtrl.IsWindowEnabled() );
    mMulticastIPLabel.EnableWindow( mMulticastIPCtrl.IsWindowEnabled() );
    mMulticastPortLabel.EnableWindow( mMulticastIPCtrl.IsWindowEnabled() );
}


// =============================================================================
void SetupDlg::OnBnClicked()
{
    EnableInterface();
}


// =============================================================================
void SetupDlg::OnOK()
{
    if ( mEnabled )
    {
        mSetup.mRole = RoleInvalid;
        mSetup.mDestination = DestinationInvalid;
        
        int lRole = GetCheckedRadioButton( IDC_CTRLDATA, IDC_MONITOR );
        switch ( lRole )
        {
            case IDC_CTRLDATA:
                mSetup.mRole = RoleCtrlData;
                break;

            case IDC_CTRL:
                mSetup.mRole = RoleCtrl;
                break;

            case IDC_DATA:
                mSetup.mRole = RoleData;
                break;

            case IDC_MONITOR:
                mSetup.mRole = RoleMonitor;
                break;

            default:
                ASSERT( 0 );
        }

        int lDest = GetCheckedRadioButton( IDC_UNICASTAUTO, IDC_MULTICAST );
        switch ( lDest )
        {
            case IDC_UNICASTAUTO:
                mSetup.mDestination = DestinationUnicastAuto;
                break;

            case IDC_UNICASTSPECIFIC:
                mSetup.mDestination = DestinationUnicastSpecific;
                break;

            case IDC_UNICASTOTHER:
                mSetup.mDestination = DestinationUnicastOther;
                break;

            case IDC_MULTICAST:
                mSetup.mDestination = DestinationMulticast;
                break;

            default:
                ASSERT( 0 );
        }

        BYTE lIP[ 4 ];
        int lPort = 0;
        int lChannel = 0;
        int lCount = 0;
        CString lStr;

        mUnicastSpecificPortEdit.GetWindowText( lStr );
        swscanf_s( (LPCTSTR)lStr, _T( "%i" ), &lPort );
        mSetup.mUnicastSpecificPort = static_cast<unsigned short>( lPort );

        mChannelEdit.GetWindowText( lStr );
        lCount = swscanf_s( (LPCTSTR)lStr, _T( "%i" ), &lChannel );
        if ( lCount != 1 )
        {
            MessageBox( _T( "Error parsing default channel." ), _T( "GEVPlayer" ),
                MB_OK | MB_ICONINFORMATION );
            return;
        }
        if ( ( lChannel < 0 ) || ( lChannel > 511 ) )
        {
            MessageBox( _T( "Default channel out of range. It must be between 0 and 511 inclusively." ), _T( "GEVPlayer" ),
                MB_OK | MB_ICONINFORMATION );
            return;
        }
        mSetup.mDefaultChannel = static_cast<unsigned short>( lChannel );

        mUnicastIPCtrl.GetAddress( lIP[ 0 ], lIP[ 1 ], lIP[ 2 ], lIP[ 3 ] );
        mSetup.mUnicastIP.Format( _T( "%i.%i.%i.%i" ), lIP[ 0 ], lIP[ 1 ], lIP[ 2 ], lIP[ 3 ] );

        mUnicastPortEdit.GetWindowText( lStr );
        lCount = swscanf_s( (LPCTSTR)lStr, _T( "%i" ), &lPort );
        if ( lCount != 1 )
        {
            MessageBox( _T( "Error parsing unicast port." ), _T( "GEVPlayer" ),
                MB_OK | MB_ICONINFORMATION );
            return;
        }
        mSetup.mUnicastPort = static_cast<unsigned short>( lPort );

        mMulticastIPCtrl.GetAddress( lIP[ 0 ], lIP[ 1 ], lIP[ 2 ], lIP[ 3 ] );
        mSetup.mMulticastIP.Format( _T( "%i.%i.%i.%i" ), lIP[ 0 ], lIP[ 1 ], lIP[ 2 ], lIP[ 3 ] );

        mMulticastPortEdit.GetWindowText( lStr );
        lCount = swscanf_s( (LPCTSTR)lStr, _T( "%i" ), &lPort );
        if ( lCount != 1 )
        {
            MessageBox( _T( "Error parsing multicast port." ), _T( "GEVPlayer" ),
                MB_OK | MB_ICONINFORMATION );
            return;
        }
        mSetup.mMulticastPort = static_cast<unsigned short>( lPort );
    }

    CDialog::OnOK();
}


// =============================================================================
void SetupDlg::OnCancel()
{
    CDialog::OnCancel();
}


// =============================================================================
void SetupDlg::Save( PvConfigurationWriter &aWriter )
{
    Save( aWriter, mSetup );
}


// =============================================================================
void SetupDlg::Save( PvConfigurationWriter &aWriter, Setup &aSetup )
{
    CString lStr;

    // Save a version string, just in case we need it in the future
    aWriter.Store( SETUPDLG_VERSION, TAG_VERSION );

    // Role mRole;
    lStr = VAL_ROLECTRLDATA;
    switch ( aSetup.mRole )
    {
        case RoleCtrlData:
            lStr = VAL_ROLECTRLDATA;
            break;

        case RoleCtrl:
            lStr = VAL_ROLECTRL;
            break;

        case RoleData:
            lStr = VAL_ROLEDATA;
            break;

        case RoleMonitor:
            lStr = VAL_ROLEMONITOR;
            break;

        default:
            ASSERT( 0 );
    }
    aWriter.Store( (LPCTSTR)lStr, TAG_ROLE );

    // Destination mDestination;
    lStr = VAL_DESTINATIONUNICASTAUTO;
    switch ( aSetup.mDestination )
    {
        case DestinationUnicastAuto:
            lStr = VAL_DESTINATIONUNICASTAUTO;
            break;

        case DestinationUnicastSpecific:
            lStr = VAL_DESTINATIONUNICASTSPECIFIC;
            break;

        case DestinationUnicastOther:
            lStr = VAL_DESTINATIONUNICASTOTHER;
            break;

        case DestinationMulticast:
            lStr = VAL_DESTINATIONMULTICAST;
            break;

        default:
            ASSERT( 0 );
    }
    aWriter.Store( (LPCTSTR)lStr, TAG_DESTINATION );

    // unsigned short mUnicastSpecificPort;
    lStr.Format( _T( "%i" ), aSetup.mUnicastSpecificPort );
    aWriter.Store( (LPCTSTR)lStr, TAG_UNICASTSPECIFICPORT );

    // unsigned short mDefaultChannel;
    lStr.Format( _T( "%i" ), aSetup.mDefaultChannel );
    aWriter.Store( (LPCTSTR)lStr, TAG_DEFAULTCHANNEL );

    // CString mUnicastIP;
    aWriter.Store( (LPCTSTR)aSetup.mUnicastIP, TAG_UNICASTIP );

    // unsigned short mUnicastPort;
    lStr.Format( _T( "%i" ), aSetup.mUnicastPort );
    aWriter.Store( (LPCTSTR)lStr, TAG_UNICASTPORT );

    // CString mMulticastIP;
    aWriter.Store( (LPCTSTR)aSetup.mMulticastIP, TAG_MULTICASTIP );

    // unsigned short mMulticastPort;
    lStr.Format( _T( "%i" ), aSetup.mMulticastPort );
    aWriter.Store( (LPCTSTR)lStr, TAG_MULTICASTPORT );
}


// =============================================================================
void SetupDlg::Load( PvConfigurationReader &aReader )
{
    Load( aReader, mSetup );
}


// =============================================================================
void SetupDlg::Load( PvConfigurationReader &aReader, Setup &aSetup )
{
    PvResult lResult;
    PvString lPvStr;

    // Always load from a blank setup!
    aSetup.Reset();

    // Role mRole;
    lResult = aReader.Restore( PvString( TAG_ROLE ), lPvStr );
    if ( lResult.IsOK() )
    {
        StrToRole( lPvStr.GetUnicode(), aSetup.mRole );
    }

    // Destination mDestination;
    lResult = aReader.Restore( PvString( TAG_DESTINATION ), lPvStr );
    if ( lResult.IsOK() )
    {
        StrToDestination( lPvStr.GetUnicode(), aSetup.mDestination );
    }

    // unsigned short mUnicastSpecificPort;
    lResult = aReader.Restore( PvString( TAG_UNICASTSPECIFICPORT ), lPvStr );
    if ( lResult.IsOK() )
    {
        int lPort;
        swscanf_s( lPvStr.GetUnicode(), _T( "%i" ), &lPort );
        aSetup.mUnicastSpecificPort = static_cast<unsigned short>( lPort );
    }

    // unsigned short mDefaultChannel;
    lResult = aReader.Restore( PvString( TAG_DEFAULTCHANNEL ), lPvStr );
    if ( lResult.IsOK() )
    {
        int lDefaultChannel;
        swscanf_s( lPvStr.GetUnicode(), _T( "%i" ), &lDefaultChannel );
        aSetup.mDefaultChannel = static_cast<unsigned short>( lDefaultChannel );
    }

    // CString mUnicastIP;
    lResult = aReader.Restore( PvString( TAG_UNICASTIP ), lPvStr );
    if ( lResult.IsOK() )
    {
        aSetup.mUnicastIP = lPvStr.GetUnicode();
    }

    // unsigned short mUnicastPort;
    lResult = aReader.Restore( PvString( TAG_UNICASTPORT ), lPvStr );
    if ( lResult.IsOK() )
    {
        int lPort;
        swscanf_s( lPvStr.GetUnicode(), _T( "%i" ), &lPort );
        aSetup.mUnicastPort = static_cast<unsigned short>( lPort );
    }

    // CString mMulticastIP;
    lResult = aReader.Restore( PvString( TAG_MULTICASTIP ), lPvStr );
    if ( lResult.IsOK() )
    {
        aSetup.mMulticastIP = lPvStr.GetUnicode();
    }

    // unsigned short mMulticastPort;
    lResult = aReader.Restore( PvString( TAG_MULTICASTPORT ), lPvStr );
    if ( lResult.IsOK() )
    {
        int lPort;
        swscanf_s( lPvStr.GetUnicode(), _T( "%i" ), &lPort );
        aSetup.mMulticastPort = static_cast<unsigned short>( lPort );
    }
}


// =============================================================================
bool SetupDlg::IsTheSame( PvConfigurationReader &aReader )
{
    // Load a local setup
    Setup lSetup;
    Load( aReader, lSetup );

    // Start with the assumption that they are the same
    bool lSame = true;

    // Try to invalidate assumption with destination, role
    lSame &= lSetup.mDestination == mSetup.mDestination;
    lSame &= lSetup.mRole == mSetup.mRole;
    lSame &= lSetup.mDefaultChannel == mSetup.mDefaultChannel;

    // Only if destination is unicast specific, compare destination
    if ( lSetup.mDestination == DestinationUnicastSpecific )
    {
        lSame &= lSetup.mUnicastSpecificPort == mSetup.mUnicastSpecificPort;
    }

    // Only if destination is unicast other, compare destination
    if ( lSetup.mDestination == DestinationUnicastOther )
    {
        lSame &= lSetup.mUnicastIP == mSetup.mUnicastIP;
        lSame &= lSetup.mUnicastPort == mSetup.mUnicastPort;
    }

    // Only if destination is multicast, compare destination
    if ( lSetup.mDestination == DestinationMulticast )
    {
        lSame &= lSetup.mMulticastIP == mSetup.mMulticastIP;
        lSame &= lSetup.mMulticastPort == mSetup.mMulticastPort;
    }

    // Return conclusion!
    return lSame;
}


// =============================================================================
void SetupDlg::StrToRole( const CString &aStr, Role &aRole )
{
    if ( aStr == VAL_ROLECTRLDATA )
    {
        aRole = RoleCtrlData;
    }
    else if ( aStr == VAL_ROLEDATA )
    {
        aRole = RoleData;
    }
    else if ( aStr == VAL_ROLECTRL )
    {
        aRole = RoleCtrl;
    }
    else if ( aStr == VAL_ROLEMONITOR )
    {
        aRole = RoleMonitor;
    }
}


// =============================================================================
void SetupDlg::StrToDestination( const CString &aStr, Destination &aDestination )
{
    if ( aStr == VAL_DESTINATIONUNICASTAUTO )
    {
        aDestination = DestinationUnicastAuto;
    }
    else if ( aStr == VAL_DESTINATIONUNICASTSPECIFIC )
    {
        aDestination = DestinationUnicastSpecific;
    }
    else if ( aStr == VAL_DESTINATIONUNICASTOTHER )
    {
        aDestination = DestinationUnicastOther;
    }
    else if ( aStr == VAL_DESTINATIONMULTICAST )
    {
        aDestination = DestinationMulticast;
    }
}

