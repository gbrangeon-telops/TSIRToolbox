// *****************************************************************************
//
//     Copyright (c) 2007, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#include "stdafx.h"

#include "GEVPlayer.h"
#include "GEVPlayerDlg.h"
#include "AboutBox.h"
#include "SplashPage.h"
#include "SetupDlg.h"
#include "ImageSaveDlg.h"
#include "LoadingThread.h"
#include "ConnectionThread.h"
#include "SavingThread.h"
#include "BufferOptionsDlg.h"
#include "PvMessageBox.h"
#include "WarningDlg.h"
#include "SerialBridgeManagerWnd.h"
#include "ErrorsDlg.h"

#include <PvConfigurationWriter.h>
#include <PvConfigurationReader.h>
#include <PvSerialPortIPEngine.h>
#include <PvInterface.h>
#include <PvGenParameterArrayCL.h>
#include <PvGenStateStack.h>
#include <PvStreamInfo.h>
#include <PvDisplayThread.h>
#include <PvSystem.h>

#include <string.h>
#include <limits.h>
#include <io.h>


#define TAG_DISPLAYOPTIONS ( "displayoptions" )
#define TAG_COMMUNICATIONBROWSER ( "communicationbrowser" )
#define TAG_DEVICEBROWSER ( "devicebrowser" )
#define TAG_STREAMBROWSER ( "streambrowser" )
#define TAG_ACTIVESOURCE ( "activesource" )


static const int BUFFER_COUNT_DEFAULT = 16;
static const int BUFFER_COUNT_MIN = 4;


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


BEGIN_MESSAGE_MAP(GEVPlayerDlg, CDialog)
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    ON_WM_SIZE()
    ON_WM_GETMINMAXINFO()
    ON_BN_CLICKED(IDC_DEVICE_BUTTON, &GEVPlayerDlg::OnBnClickedDeviceButton)
    ON_BN_CLICKED(IDC_LINK_BUTTON, &GEVPlayerDlg::OnBnClickedLinkButton)
    ON_BN_CLICKED(IDC_STREAMPARAMS_BUTTON, &GEVPlayerDlg::OnBnClickedStreamparamsButton)
    ON_BN_CLICKED(IDC_CONNECT_BUTTON, &GEVPlayerDlg::OnBnClickedConnectButton)
    ON_BN_CLICKED(IDC_DISCONNECT_BUTTON, &GEVPlayerDlg::OnBnClickedDisconnectButton)
    ON_WM_CLOSE()
    ON_BN_CLICKED(IDC_START, &GEVPlayerDlg::OnBnClickedStart)
    ON_BN_CLICKED(IDC_STOP, &GEVPlayerDlg::OnBnClickedStop)
    ON_CBN_SELCHANGE(IDC_MODE, &GEVPlayerDlg::OnCbnSelchangeMode)
    ON_MESSAGE(WM_IMAGEDISPLAYED, OnImageDisplayed)
    ON_BN_CLICKED(IDC_DEVICEEVENTS_BUTTON, &GEVPlayerDlg::OnBnClickedDeviceEvents)
    ON_COMMAND(ID_FILE_LOAD, &GEVPlayerDlg::OnFileLoad)
    ON_COMMAND(ID_FILE_SAVE, &GEVPlayerDlg::OnFileSave)
    ON_COMMAND(ID_FILE_EXIT, &GEVPlayerDlg::OnFileExit)
    ON_COMMAND(ID_TOOLS_EVENTMONITOR, &GEVPlayerDlg::OnToolsEventMonitor)
    ON_COMMAND(ID_HELP_ABOUT, &GEVPlayerDlg::OnHelpAbout)
    ON_COMMAND_RANGE(ID_FILE_RECENT1, ID_FILE_RECENT10, &GEVPlayerDlg::OnRecent)
    ON_WM_INITMENU()
    ON_WM_MOVE()
    ON_COMMAND(ID_TOOLS_DISPLAYVSYNC, &GEVPlayerDlg::OnToolsDisplaysync)
    ON_WM_CTLCOLOR()
    ON_MESSAGE(WM_DISCONNECT, OnDisconnect)
    ON_MESSAGE(WM_STOPSTREAMING, OnStopStreaming)
    ON_COMMAND(ID_FILE_SAVEAS, &GEVPlayerDlg::OnFileSaveAs)
    ON_COMMAND(ID_ACCELERATOR_SAVE, &GEVPlayerDlg::OnAcceleratorSave)
    ON_COMMAND(ID_ACCELERATOR_OPEN, &GEVPlayerDlg::OnAcceleratorOpen)
    ON_COMMAND(ID_TOOLS_SETUP, &GEVPlayerDlg::OnToolsSetup)
    ON_COMMAND(ID_TOOLS_IMAGEFILTERING, &GEVPlayerDlg::OnToolsImagefiltering)
    ON_COMMAND(ID_TOOLS_SERIALCOMMUNICATIONBRIDGE, &GEVPlayerDlg::OnToolsSerialCommunicationBridge)
    ON_COMMAND(ID_TOOLS_ACTIONCOMMAND, &GEVPlayerDlg::OnToolsActionCommand)    
    ON_COMMAND(ID_TOOLS_SAVEIMAGES, &GEVPlayerDlg::OnToolsSaveimages)
    ON_COMMAND(ID_TOOLS_SAVECURRENTIMAGE, &GEVPlayerDlg::OnToolsSavecurrentimage)
    ON_MESSAGE(WM_STREAM, OnStream)
    ON_COMMAND(ID_TOOLS_IPENGINESERIALCOMMUNICATION, &GEVPlayerDlg::OnToolsSerialCommunication)
    ON_WM_TIMER()
    ON_WM_DESTROY()
    ON_COMMAND(ID_REGISTERINTERFACE, &GEVPlayerDlg::OnRegisterInterface)
    ON_COMMAND(ID_DISPLAYIMAGES_DISABLED, &GEVPlayerDlg::OnDisplayDisabled)
    ON_COMMAND(ID_DISPLAYIMAGES_30FPS, &GEVPlayerDlg::OnDisplay30FPS)
    ON_COMMAND(ID_DISPLAYIMAGES_60FPS, &GEVPlayerDlg::OnDisplay60FPS)
    ON_COMMAND(ID_TOOLS_SAVEPREFERENCES, &GEVPlayerDlg::OnToolsSavepreferences)
    ON_COMMAND(ID_TOOLS_RESTOREDEFAULTPREFERENCES, &GEVPlayerDlg::OnToolsRestoredefaultpreferences)
    ON_COMMAND(ID_TOOLS_BUFFEROPTIONS, &GEVPlayerDlg::OnToolsBufferoptions)
    ON_COMMAND(ID_TOOLS_DISPLAYPARTIALIMAGES, &GEVPlayerDlg::OnToolsDisplaypartialimages)
    ON_COMMAND(ID_TOOLS_DISPLAYCHUNKDATA, &GEVPlayerDlg::OnToolsDisplaychunkdata)
    ON_COMMAND(ID_TOOLS_SAVE_XML, &GEVPlayerDlg::OnToolsSaveXml)
    ON_MESSAGE(WM_LINKDISCONNECTED, &GEVPlayerDlg::OnLinkDisconnected)
    ON_MESSAGE(WM_LINKRECONNECTED, &GEVPlayerDlg::OnLinkReconnected)
    ON_MESSAGE(WM_ACQUISITIONSTATECHANGED, &GEVPlayerDlg::OnAcquisitionStateChanged)
    ON_CBN_SELCHANGE(IDC_COMBO_INPUT, &GEVPlayerDlg::OnCbnSelchangeComboSource)
    ON_COMMAND(ID_TOOLS_DEINTERLACING_DISABLED, &GEVPlayerDlg::OnToolsDeinterlacingDisabled)
    ON_COMMAND(ID_TOOLS_DEINTERLACING_WEAVING, &GEVPlayerDlg::OnToolsDeinterlacingWeaving)
    ON_MESSAGE(WM_UPDATESOURCE, &GEVPlayerDlg::OnUpdateSource)
    ON_MESSAGE(WM_UPDATESOURCES, &GEVPlayerDlg::OnUpdateSources)
    ON_MESSAGE(WM_UPDATEACQUISITIONMODE, &GEVPlayerDlg::OnUpdateAcquisitionMode)
END_MESSAGE_MAP()


// =============================================================================
GEVPlayerDlg::GEVPlayerDlg( CString aFileName, CWnd* pParent /* =NULL */ )
    : CDialog( GEVPlayerDlg::IDD, pParent )
    , mNeedInit( TRUE )
    , mDisplayThread( NULL )
    , mEventMonitorDlg( NULL )
    , mSerialBridgeManagerWnd( NULL )
    , mActionCommandWnd( NULL )
    , mMRUMenu( NULL )
    , mDeviceConnected( false )
    , mFileName( aFileName )
    , mPipeline( &mStream )
    , mStatusColor( SCDefault )
    , mSerial0IPEngineSupported( false )
    , mTimer( 0 )
    , mLogBuffer( NULL )
    , mRecovery( FALSE )
    , mAcquisitionStateManager( NULL )
    , mShowAcquisitionWarning( true )
    , mTerminalIPEngineWndModeless( false )
    , mUpdatingSources( FALSE )
    , mUpdatingAcquisitionMode( FALSE )
	, mPipelineReallocated( false )
{
    m_hIcon = AfxGetApp()->LoadIcon( IDR_MAINFRAME );

    mPlayButton.SetBitmap( IDB_PLAY );
    mStopButton.SetBitmap( IDB_STOP );

    mImageSaveDlg = new ImageSaveDlg( &mFilteringDlg, this );
    mLogBuffer = new LogBuffer;

    mDisplayThread = new DisplayThread( &mDisplay, &mFilteringDlg, mImageSaveDlg, mLogBuffer );

    DWORD lValue = ReadRegistryDWORD( _T( "IPEngineSerialCommunicationModeless" ) );
    if ( lValue != 0 )
    {
        mTerminalIPEngineWndModeless = true;
    }

	mPipeline.RegisterEventSink( this );
}


// =============================================================================
GEVPlayerDlg::~GEVPlayerDlg()
{
	mPipeline.UnregisterEventSink( this );

    if ( mDisplayThread != NULL )
    {
        delete mDisplayThread;
        mDisplayThread = NULL;
    }

    if ( mLogBuffer != NULL )
    {
        delete mLogBuffer;
        mLogBuffer = NULL;
    }

    if ( mMRUMenu != NULL )
    {
        delete mMRUMenu;
        mMRUMenu = NULL;
    }

    if ( mImageSaveDlg != NULL )
    {
        delete mImageSaveDlg;
        mImageSaveDlg = NULL;
    }

    if ( mAcquisitionStateManager != NULL )
    {
        delete mAcquisitionStateManager;
        mAcquisitionStateManager = NULL;
    }
}


// =============================================================================
void GEVPlayerDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_MODE, mModeCombo);
    DDX_Control(pDX, IDC_START, mPlayButton);
    DDX_Control(pDX, IDC_STOP, mStopButton);
    DDX_Control(pDX, IDC_STATUS, mStatusTextBox);
    DDX_Control(pDX, IDC_IP_EDIT, mIPEdit);
    DDX_Control(pDX, IDC_MAC_EDIT, mMACEdit);
    DDX_Control(pDX, IDC_MODEL_EDIT, mModelEdit);
    DDX_Control(pDX, IDC_MANUFACTURER_EDIT, mManufacturerEdit);
    DDX_Control(pDX, IDC_NAME_EDIT2, mNameEdit);
    DDX_Control(pDX, IDC_COMBO_INPUT, mSourceCombo);
}


// =============================================================================
BOOL GEVPlayerDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    SplashPage::Show();

    mFilteringDlg.Create( IDD_FILTERING, this );

    mYellowColor = RGB( 0xFF, 0xFF, 0x00 );
    mYellowBrush.CreateSolidBrush( mYellowColor );

    mRedColor = RGB( 0x80, 0x00, 0x00 );
    mRedBrush.CreateSolidBrush( mRedColor );

    SetStatusColor( SCDefault );

    GetClientRect( mCrt );
    mNeedInit = FALSE;

    // Set the icon for this dialog.  The framework does this automatically
    //  when the application's main window is not a dialog
    SetIcon(m_hIcon, TRUE);			// Set big icon
    SetIcon(m_hIcon, FALSE);		// Set small icon

    CRect lDisplayRect;
    GetDlgItem( IDC_DISPLAYPOS )->GetClientRect( &lDisplayRect );
    GetDlgItem( IDC_DISPLAYPOS )->ClientToScreen( &lDisplayRect );
    ScreenToClient( &lDisplayRect );
    mDisplay.Create( GetSafeHwnd(), 10000 );
    mDisplay.SetPosition( lDisplayRect.left, lDisplayRect.top, lDisplayRect.Width(), lDisplayRect.Height() );
    mDisplay.SetBackgroundColor( 0x80, 0x80, 0x80 );
    
    EnableInterface();
    
    LoadMRUFromRegistry();
    UpdateMRUMenu();

    // create a defaults persistence file if it doesn't already exist
    CString lDefaultPath = GetDefaultPath();
    SaveConfig( lDefaultPath, false );

    if ( !mFileName.IsEmpty() )
    {
        OpenConfig( mFileName );
    }
    else
    {
        // check for the existence of the sticky configuration file. If it exists, load it.
        CString lStickyPath = GetStickyPath();
        if ( _taccess( lStickyPath.GetBuffer(), 0 ) == 0 )
        {
            OpenConfig( lStickyPath );
        }
    }

    mAccel = ::LoadAccelerators(AfxGetResourceHandle(),
             MAKEINTRESOURCE(IDR_ACCELERATOR));

    // Create timer that will be used to periodically update the status bar
    mTimer = SetTimer( 1, 250, NULL );

    return TRUE;  // return TRUE  unless you set the focus to a control
}


// =============================================================================
void GEVPlayerDlg::OnPaint()
{
    if (IsIconic())
    {
        CPaintDC dc(this); // device context for painting

        SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

        // Center icon in client rectangle
        int cxIcon = GetSystemMetrics(SM_CXICON);
        int cyIcon = GetSystemMetrics(SM_CYICON);
        CRect rect;
        GetClientRect(&rect);
        int x = (rect.Width() - cxIcon + 1) / 2;
        int y = (rect.Height() - cyIcon + 1) / 2;

        // Draw the icon
        dc.DrawIcon(x, y, m_hIcon);
    }
    else
    {
        CDialog::OnPaint();
    }
}


// =============================================================================
HCURSOR GEVPlayerDlg::OnQueryDragIcon()
{
    return static_cast<HCURSOR>(m_hIcon);
}


// =============================================================================
void GEVPlayerDlg::OnClose()
{
    // Make sure we cleanup before we leave
    Disconnect();

    // Close action command
    if ( mActionCommandWnd != NULL )
    {
        if ( mActionCommandWnd->GetSafeHwnd() != 0 )
        {
            mActionCommandWnd->DestroyWindow();
        }

        delete mActionCommandWnd;
        mActionCommandWnd = NULL;
    }

    DestroyWindow();
}


// =============================================================================
void GEVPlayerDlg::OnBnClickedConnectButton()
{
    // create a device finder wnd and open the select device dialog
    PvDeviceFinderWnd lFinder;
    lFinder.SetTitle( _T( "GigE Vision Device Selection" ) );

    PvResult lResult = lFinder.ShowModal();
    if ( !lResult.IsOK() || ( lFinder.GetSelected() == NULL ) )
    {
        return;
    }

    if ( lFinder.GetSelected()->GetInterface()->GetType() == PvInterfaceNetworkStack )
    {
        UINT lDontShowAgain = AfxGetApp()->GetProfileInt( _T(""), _T("DontShowNoDriverWarning"), 0 );

        if ( lDontShowAgain == 0 )
        {
            WarningDlg lNoDriverWarning;
            lNoDriverWarning.SetWarning( _T( "You have chosen to connect to a GigE Vision device through a network interface, which does not employ an eBUS driver.  eBUS drivers are recommended for optimal streaming performance." ) );
            lNoDriverWarning.SetCheckboxMessage( _T( "Don't show me this again" ) );
            lNoDriverWarning.DoModal();

            if ( lNoDriverWarning.IsChecked() )
            {
                AfxGetApp()->WriteProfileInt(_T(""), _T("DontShowNoDriverWarning"), 1 );
            }
        }
    }
    

    CWaitCursor lCursor;

    UpdateWindow();

    Connect( lFinder.GetSelected(), NULL );
}


// =============================================================================
void GEVPlayerDlg::OnBnClickedDisconnectButton()
{
    CWaitCursor lCursor;

    Disconnect();
}


// =============================================================================
void GEVPlayerDlg::OnSize(UINT nType, int cx, int cy)
{
    CDialog::OnSize(nType, cx, cy);

    CRect lR;
    this->GetWindowRect( &lR );
    // TRACE( "%i %i\n", lR.Width(), lR.Height() );

    if ( mNeedInit || cx == 0 || cy == 0 )
    {
        return;
    }

    int dx = cx - mCrt.Width();
    int dy = cy - mCrt.Height();

    GetClientRect(&mCrt);

    CRect r1;

    HDWP hDWP = ::BeginDeferWindowPos(20);

    //
    // Bottom left, just bring the group box down
    //

    int lBottomLeft[] =
    {
        IDC_CONTROL_GROUP
    };

    for ( int i = 0; i < sizeof( lBottomLeft ) / sizeof ( lBottomLeft[ 0 ] ); i++ )
    {
        CWnd *lChild = GetDlgItem( lBottomLeft[ i ] );
        if ( lChild != NULL )
        {
            lChild->GetWindowRect(&r1); ScreenToClient(&r1); 
            r1.bottom += dy;
            ::DeferWindowPos(
                hDWP, lChild->m_hWnd, NULL, 
                r1.left, r1.top, r1.Width(), r1.Height(),
                SWP_NOACTIVATE|SWP_NOZORDER);
        }
    }

    //
    // Right, make sure the display and group box fill the right part
    // of our window
    //

    int lDisplayGroup[] =
    {
        IDC_DISPLAY_GROUP, IDC_DISPLAYPOS
    };

    for ( int i = 0; i < sizeof( lDisplayGroup ) / sizeof ( lDisplayGroup[ 0 ] ); i++ )
    {
        CWnd *lChild = GetDlgItem( lDisplayGroup[ i ] );
        if ( lChild != NULL )
        {
            lChild->GetWindowRect(&r1); ScreenToClient(&r1); 
            r1.bottom += dy;
            r1.right += dx;
            ::DeferWindowPos(
                hDWP, lChild->m_hWnd, NULL, 
                r1.left, r1.top, r1.Width(), r1.Height(),
                SWP_NOACTIVATE|SWP_NOZORDER);

            if ( lDisplayGroup[i] == IDC_DISPLAYPOS )
            {
                mDisplay.SetPosition( 
                    r1.left, r1.top, 
                    r1.Width(), r1.Height() );
            }
        }
    }

    //
    // Last, the status box. Just like the display, but sticks to the bottom
    // and is not resized
    //

    CWnd *lBottomRight[] = 
    { 
        &mStatusTextBox
    };

    for ( int i = 0; i < sizeof( lBottomRight ) / sizeof ( lBottomRight[ 0 ] ); i++ )
    {
        CWnd *lChild = lBottomRight[ i ];
        if ( lChild != NULL )
        {
            lChild->GetWindowRect(&r1); ScreenToClient(&r1); 
            ::DeferWindowPos(
                hDWP, lChild->m_hWnd, NULL, 
                r1.left, r1.top + dy, r1.Width() + dx, r1.Height(),
                SWP_NOACTIVATE|SWP_NOZORDER);
        }
    }

    ::EndDeferWindowPos(hDWP);
}


// =============================================================================
void GEVPlayerDlg::OnGetMinMaxInfo( MINMAXINFO *lpMMI )
{
    lpMMI->ptMinTrackSize.x = 800;
    lpMMI->ptMinTrackSize.y = 517;
}

// =============================================================================
void GEVPlayerDlg::EnableInterface()
{
    // This method can be called really early or late when the window is not created
    if ( GetSafeHwnd() == 0 )
    {
        return;
    }

    GetDlgItem( IDC_CONNECT_BUTTON )->EnableWindow( !mDeviceConnected && !mStream.IsOpen() );
    GetDlgItem( IDC_DISCONNECT_BUTTON )->EnableWindow( mDeviceConnected || mStream.IsOpen() );

	EnableTreeBrowsers( TRUE );
	EnableControls( mDeviceConnected );
}


// =============================================================================
void GEVPlayerDlg::EnableTreeBrowsers( BOOL aEnabled )
{
    GetDlgItem( IDC_DEVICE_BUTTON )->EnableWindow( aEnabled && mDeviceConnected );
    GetDlgItem( IDC_STREAMPARAMS_BUTTON )->EnableWindow( aEnabled && mStream.IsOpen() );
    GetDlgItem( IDC_LINK_BUTTON )->EnableWindow( aEnabled && ( mSetupDlg.GetRole() != SetupDlg::RoleData ) ); 
}


// =============================================================================
void GEVPlayerDlg::EnableControls( BOOL aEnabled )
{
    bool lEnabled = aEnabled && ( mSetupDlg.GetRole() != SetupDlg::RoleMonitor );

    bool lModeEnabled = false;
    bool lStartEnabled = false;
    bool lStopEnabled = false;
    bool lSourceEnabled = false;
    
    PvAcquisitionState lAcquisitionState = PvAcquisitionStateUnknown;
    if ( mAcquisitionStateManager != NULL )
    {
        lAcquisitionState = mAcquisitionStateManager->GetState();
    }

    PvGenCommand *lStart = mDevice.GetGenParameters()->GetCommand( "AcquisitionStart" );
    PvGenCommand *lStop = mDevice.GetGenParameters()->GetCommand( "AcquisitionStop" );
    PvGenEnum *lMode = mDevice.GetGenParameters()->GetEnum( "AcquisitionMode" );
    PvGenEnum *lSource = mDevice.GetGenParameters()->GetEnum( "SourceSelector" );

    // Used to temporarily select the source in use
    PvGenStateStack lStack( mDevice.GetGenParameters() );
    PushSource( &lStack );

    if ( IsMultiSourceTransmitter( &mDevice ) )
    {
        lSourceEnabled = lEnabled &&
            ( lSource != NULL ) &&
            lSource->IsWritable() &&
            ( lAcquisitionState != PvAcquisitionStateLocked );
    }

    if ( lMode != NULL )
    {
        lModeEnabled = lEnabled &&
            lMode->IsWritable() &&
            ( lAcquisitionState != PvAcquisitionStateLocked );
    }

    if ( lStart != NULL )
    {
        lStartEnabled = lEnabled && 
            ( lAcquisitionState != PvAcquisitionStateLocked );
    }

    if ( lStop != NULL )
    {
        lStopEnabled = lEnabled && 
            ( lAcquisitionState != PvAcquisitionStateUnlocked );
    }

    mPlayButton.EnableWindow( lStartEnabled );
    mStopButton.EnableWindow( lStopEnabled );
    mModeCombo.EnableWindow( lModeEnabled );
    mSourceCombo.EnableWindow( lSourceEnabled );
}


// =============================================================================
void GEVPlayerDlg::Connect( PvDeviceInfo *aDI, PvConfigurationReader *aCR )
{
    ASSERT( aDI != NULL || aCR != NULL );
    if ( aDI == NULL && aCR == NULL )	
    {
        return;
    }

    if ( aDI != NULL )
    {
        PvDeviceClass lClass = aDI->GetClass();
        SetupDlg::Role lRole = mSetupDlg.GetRole();

		if ( ( lClass == PvDeviceClassReceiver ) || 
			 ( lClass == PvDeviceClassPeripheral ) ||
			 ( lClass == PvDeviceClassTransceiver ) )
        {
			// For receiver and peripheral, we make sure  role is controller only
            if ( lRole == SetupDlg::RoleCtrlData || lRole == SetupDlg::RoleData )
            {
                CString lMessage;

				if ( ( lClass == PvDeviceClassReceiver ) ||
					 ( lClass == PvDeviceClassPeripheral ) )
				{
					// Receiver and peripheral message
					lMessage = _T( "You have chosen to connect to a " );
					lMessage += ( lClass == PvDeviceClassReceiver ) ? _T( "receiver" ) : _T( "peripheral" );
					lMessage += _T( " however GEVPlayer is currently set up to connect as a " );
					lMessage += ( lRole == SetupDlg::RoleCtrlData ) ? _T( "controller and data receiver" ) : _T( "data receiver" );
					lMessage += _T( ". \r\n\r\nRe-attempt connection as a controller?" );
				}
				else if ( lClass == PvDeviceClassTransceiver )
				{
					// Transceiver message
					lMessage = _T( "You have chosen to connect to a transceiver. GEVPlayer does not fully\r\n" );
					lMessage += _T( "support transceivers in this version. Re-attempt connection as a \r\n" );
					lMessage += _T( "controller?" );
				}
				else
				{
					ASSERT( 0 );
				}

                if ( MessageBox( lMessage, _T( "GEVPlayer"), MB_YESNO | MB_ICONQUESTION ) == IDNO )
                {
                    return;
                }
                else
                {
                    mSetupDlg.SetRole( SetupDlg::RoleCtrl );
					mSetupDlg.SetDestination( SetupDlg::DestinationUnicastOther );
					mSetupDlg.SetUnicastIP( _T( "0.0.0.0" ) );
					mSetupDlg.SetUnicastPort( 0 );
                }
            }
        }
    }
    
    // Just in case we came here still connected...
    Disconnect();

    PvInt32 lPreferredSource = -1;

    // Device connection, packet size negotiation and stream opening
    // is performed in a separate thread while we display a progress
    // dialog to the user
    PvResult lResult;
    if ( aDI != NULL )
    {
        ConnectionThread lConnectionThread( &mSetupDlg, aDI, &mDevice, &mStream, this );
        lResult = lConnectionThread.Connect();
    }
    else if ( aCR != NULL )
    {
        if ( mSerialBridgeManagerWnd == NULL )
        {
            mSerialBridgeManagerWnd = new SerialBridgeManagerWnd( this );
        }

        PvStringList lErrorList;
        LoadingThread lLoadingThread( &mSetupDlg, aCR, &mDevice, &mStream, mSerialBridgeManagerWnd, &lErrorList, this );
        lResult = lLoadingThread.Load();

        if ( lErrorList.GetSize() > 0 )
        {
            ErrorsDlg lDlg;
            lDlg.DoModal( &lErrorList );
        }

        if ( mDevice.IsConnected() && IsMultiSourceTransmitter( &mDevice ) )
        {
            PvString lSourceStr;
            if ( aCR->Restore( TAG_ACTIVESOURCE, lSourceStr ).IsOK() )
            {
                sscanf_s( lSourceStr.GetAscii(), "%d", &lPreferredSource );
            }
        }
    }

    if ( !lResult.IsOK() )
    {
        PvMessageBox( this, lResult );
        Disconnect();

        return;
    }

    mDeviceConnected = mDevice.IsConnected();
    mRecovery = FALSE;

    PvSystem lSystem;
    PvDeviceInfo *lDI = aDI;

    if ( lDI == NULL )
    {
        // Get IP address of the device of interest
        PvInt64 lDeviceIPAddress = 0;
        if ( mDevice.IsConnected() )
        {
            mDevice.GetGenParameters()->GetIntegerValue( "GevCurrentIPAddress", lDeviceIPAddress );
        }
        else if ( mStream.IsOpen() )
        {
            mStream.GetParameters()->GetIntegerValue( "DeviceIPAddress", lDeviceIPAddress );
        }

        if ( lDeviceIPAddress != 0 )
        {
            CString lDeviceIPAddressString;
            lDeviceIPAddressString.Format( _T( "%I64i.%I64i.%I64i.%I64i" ),
                ( lDeviceIPAddress & 0xFF000000 ) >> 24,
                ( lDeviceIPAddress & 0x00FF0000 ) >> 16,
                ( lDeviceIPAddress & 0x0000FF00 ) >> 8,
                ( lDeviceIPAddress & 0x000000FF ) );

            // Attempt to get a device info
            lSystem.FindDevice( (LPCTSTR)lDeviceIPAddressString, &lDI );
        }
    }

    if ( lDI != NULL )
    {
        mManufacturerEdit.SetWindowText( lDI->GetVendor().GetUnicode() );
        mModelEdit.SetWindowText( lDI->GetModel().GetUnicode() );
        mIPEdit.SetWindowText( lDI->GetIPAddress().GetUnicode() );
        mMACEdit.SetWindowText( lDI->GetMACAddress().GetUnicode() );
        mNameEdit.SetWindowText( lDI->GetUserDefinedName().GetUnicode() );
    }

    if ( mAcquisitionStateManager != NULL )
    {
        delete mAcquisitionStateManager;
        mAcquisitionStateManager = NULL;
    }

    if ( mDeviceConnected )
    {
        // Create serial bridge manager, if needed (on persistence load, it may already be there)
        if ( mSerialBridgeManagerWnd == NULL )
        {
            mSerialBridgeManagerWnd = new SerialBridgeManagerWnd( this );
            mSerialBridgeManagerWnd->SetDevice( &mDevice );
        }

        // Create the event monitor *before* registering the device event sink
        ASSERT( mEventMonitorDlg == NULL );
        mEventMonitorDlg = new EventMonitorDlg( mLogBuffer, mDevice.GetGenParameters(), this );
        mEventMonitorDlg->Create( EventMonitorDlg::IDD, this );

        // Register device event sink
        mDevice.RegisterEventSink( this );

        // Register to all events of the parameters in the device's node map
        PvGenParameterArray *lGenDevice = mDevice.GetGenParameters();
        for ( PvUInt32 i = 0; i < lGenDevice->GetCount(); i++ )
        {
            lGenDevice->Get( i )->RegisterEventSink( this );
        }
    }

    if ( mDeviceConnected && ( mSetupDlg.GetRole() != SetupDlg::RoleMonitor ) )
    {
        // Fill acquisition mode combo box
        mModeCombo.ResetContent();
        PvGenEnum *lMode = mDevice.GetGenParameters()->GetEnum( "AcquisitionMode" );
        if ( lMode != NULL )
        {
            ASSERT( lMode != NULL ); // Mandatory parameter
            PvInt64 lEntriesCount = 0;
            VERIFY( lMode->GetEntriesCount( lEntriesCount ) ); // Failure here would be totally unexpected
            for ( PvUInt32 i = 0; i < lEntriesCount; i++ )
            {
                const PvGenEnumEntry *lEntry = NULL;
                VERIFY( lMode->GetEntryByIndex( i, &lEntry ) ); // Not expecting any failure on this one
                
                bool lAvailable = false;
                lEntry->IsAvailable( lAvailable );
                if ( lAvailable )
                {
                    PvString lEEName;
                    VERIFY( lEntry->GetName( lEEName ).IsOK() );

                    PvInt64 lEEValue;
                    VERIFY( lEntry->GetValue( lEEValue ).IsOK() );

                    int lIndex = mModeCombo.AddString( lEEName.GetUnicode() );
                    mModeCombo.SetItemData( lIndex, static_cast<DWORD_PTR>( lEEValue ) );
                }
            }
        }

        // Check whether IP Engine serial communication is supported or not
        mSerial0IPEngineSupported = PvSerialPortIPEngine::IsSupported( &mDevice, PvIPEngineSerial0 );
        mSerial0IPEngineSupported |= PvSerialPortIPEngine::IsSupported( &mDevice, PvIPEngineSerialBulk0 );

        PvInt64 lSource = 0;
        if ( IsMultiSourceTransmitter( &mDevice ) )
        {
            mDevice.GetGenParameters()->GetIntegerValue( "SourceSelector", lSource );
        }

        // Regardless of whether a corresponding stream is open or not, we can still pass it to the acquisition state manager
        ASSERT( mAcquisitionStateManager == NULL );
        mAcquisitionStateManager = new PvAcquisitionStateManager( &mDevice, &mStream, static_cast<PvUInt32>( lSource ) );
        mAcquisitionStateManager->RegisterEventSink( this );

        SendMessage( WM_UPDATESOURCES, 0, lPreferredSource );
        SendMessage( WM_UPDATEACQUISITIONMODE );
    }

    if ( mStream.IsOpen() )
    {
        // Ready image reception
        StartStreaming();
    }

    // Sync the UI with our new status
    EnableInterface();
}


// =============================================================================
void GEVPlayerDlg::Disconnect()
{
    // Close all configuration child windows
    CloseGenWindow( &mDeviceWnd );
    CloseGenWindow( &mStreamParametersWnd );
    mFilteringDlg.ShowWindow( SW_HIDE );
    if ( mTerminalIPEngineWnd.GetHandle() != 0 )
    {
        mTerminalIPEngineWnd.Close();
    }
    if ( mRegisterInterfaceDlg.GetSafeHwnd() != 0 )
    {
        mRegisterInterfaceDlg.EndDialog( IDOK );
    }

    // If streaming, stop streaming
    if ( mStream.IsOpen() )
    {
        StopStreaming();
        mStream.Close();
    }

    // Close serial bridge manager
    if ( mSerialBridgeManagerWnd != NULL )
    {
        mSerialBridgeManagerWnd->Close();
        delete mSerialBridgeManagerWnd;
        mSerialBridgeManagerWnd = NULL;
    }

    if ( mDevice.IsConnected() || mDeviceConnected )
    {
        // Unregister device event sink
        mDevice.UnregisterEventSink( this );

        // Close event monitor
        if ( mEventMonitorDlg != NULL )
        {
            if ( mEventMonitorDlg->GetSafeHwnd() != 0 )
            {
                mEventMonitorDlg->DestroyWindow();
            }

            delete mEventMonitorDlg;
            mEventMonitorDlg = NULL;
        }

        mDevice.Disconnect();
    }

    if ( mAcquisitionStateManager != NULL )
    {
        delete mAcquisitionStateManager;
        mAcquisitionStateManager = NULL;
    }

	mDisplay.Clear();

    // Reset device ID - can be called by the destructor when the window
    // no longer exists, be careful...
    if ( GetSafeHwnd() != 0 )
    {
        mManufacturerEdit.SetWindowText( _T( "" ) );
        mModelEdit.SetWindowText( _T( "" ) );
        mIPEdit.SetWindowText( _T( "" ) );
        mMACEdit.SetWindowText( _T( "" ) );
        mNameEdit.SetWindowText( _T( "" ) );
    }

    mStatusTextBox.SetWindowText( _T( "" ) );
    SetStatusColor( SCDefault );

    mDeviceConnected = false;
    mRecovery = FALSE;

    // Sync the UI with our new status
    EnableInterface();
    SetWindowText( L"GEVPlayer" );

    mSourceCombo.ResetContent();
    mModeCombo.ResetContent();
}


// =============================================================================
void GEVPlayerDlg::OnBnClickedDeviceButton()
{
    if ( !mDeviceConnected )
    {
        return;
    }

    if ( mDeviceWnd.GetHandle() != 0 )
    {
        // If already open, just toggle to closed...     
        CloseGenWindow( &mDeviceWnd );
        return;
    }
    
    // Create, assign parameters, set title and show modeless
    mDeviceWnd.SetTitle( "Device Control" );
    mDeviceWnd.AddGenParameterArray( mDevice.GetGenParameters(), "GEV Device" );
    for ( PvUInt32 i = 0; i < mSerialBridgeManagerWnd->GetPortCount(); i++ )
    {
        PvGenParameterArray *lArray = mSerialBridgeManagerWnd->GetGenParameterArray( i );
        if ( lArray != NULL )
        {
            PvString lName = mSerialBridgeManagerWnd->GetGenParameterArrayName( i );
            mDeviceWnd.AddGenParameterArray( lArray, lName );
        }
    }

    mDeviceWnd.ShowModeless( GetSafeHwnd() );
}


// =============================================================================
void GEVPlayerDlg::OnBnClickedDeviceEvents()
{
}


// =============================================================================
void GEVPlayerDlg::OnBnClickedLinkButton()
{
    ShowGenWindow( 
        &mCommunicationWnd, 
        mDevice.GetGenLink(), 
        _T( "Communication Control" ) );
}


// =============================================================================
void GEVPlayerDlg::OnBnClickedStreamparamsButton()
{
    if ( !mStream.IsOpen() )
    {
        return;
    }
    
    ShowGenWindow( 
        & mStreamParametersWnd, 
        mStream.GetParameters(), 
        _T( "Image Stream Control" ) );
}


// =============================================================================
void GEVPlayerDlg::ShowGenWindow( PvGenBrowserWnd *aWnd, PvGenParameterArray *aParams, const CString &aTitle )
{
    if ( aWnd->GetHandle() != 0 )
    {
        // If already open, just toggle to closed...     
        CloseGenWindow( aWnd );
        return;
    }
    
    // Create, assigne parameters, set title and show modeless
    aWnd->SetTitle( PvString( aTitle ) );
    aWnd->SetGenParameterArray( aParams );
    aWnd->ShowModeless( GetSafeHwnd() );
}


// =============================================================================
void GEVPlayerDlg::CloseGenWindow( PvWnd *aWnd )
{
    // If the window object is currently created (visible), close/destroy it
    if ( ( aWnd )->GetHandle() != 0 )
    {
        ( aWnd )->Close();
    }
}


// =============================================================================
void GEVPlayerDlg::StartStreaming()
{
    if ( !mDisplayThread->IsRunning() )
    {
        PvGenParameterArray *lParameters = mDevice.IsConnected() ?
            mDevice.GetGenParameters() :
            NULL;

        // Start display thread
        mDisplayThread->Start( &mPipeline, lParameters );
        mDisplayThread->SetPriority( THREAD_PRIORITY_ABOVE_NORMAL );
    }

    if ( !mPipeline.IsStarted() )
    {
        // Start pipeline thread
        mPipeline.Start();
    }
}


// =============================================================================
void GEVPlayerDlg::StopStreaming()
{
    if ( mDisplayThread->IsRunning() )
    {
        // Stop the display thread, but don't wait just yet
        mDisplayThread->Stop( false );

        // Stop the pipeline (if it was running)
		if ( mPipeline.IsStarted() )
		{
			mPipeline.Stop();
		}

	    // Wait on display thread to be completed
        mDisplayThread->WaitComplete();
    }

}

// =============================================================================
void GEVPlayerDlg::StartAcquisition()
{
    // Used to temporarily select the source in use
    PvGenStateStack lStack( mDevice.GetGenParameters() );
    PushSource( &lStack );

    PvGenCommand *lResetStats = mStream.GetParameters()->GetCommand( "Reset" );

    if ( mStream.IsOpen() )
    {
        PvUInt32 lPayloadSize = GetPayloadSize();
        if ( lPayloadSize > 0 )
        {
            mPipeline.SetBufferSize( lPayloadSize );
        }

        mPipeline.Reset();
    }

    lResetStats->Execute();
	mPipelineReallocated = false;

    if ( mDisplayThread->IsRunning() )
    {
        mDisplayThread->ResetStatistics();
    }

    if ( mImageSaveDlg != NULL )
    {
        mImageSaveDlg->ResetStats();
    }

    ASSERT( mAcquisitionStateManager != NULL );
    PvResult lResult = mAcquisitionStateManager->Start();
    if ( lResult.GetCode() == PvResult::Code::GENERIC_ERROR )
    {
        if ( mShowAcquisitionWarning )
        {
            WarningDlg lAcquisitionWarning;
            lAcquisitionWarning.SetWarning( lResult.GetDescription().GetUnicode() );
            lAcquisitionWarning.SetCheckboxMessage( _T( "Don't show this warning again in this session." ) );
            lAcquisitionWarning.DoModal();

            if ( lAcquisitionWarning.IsChecked() )
            {
                mShowAcquisitionWarning = false;
            }
        }
    }
    else if ( !lResult.IsOK() )
    {
        PvMessageBox( this, lResult );
    }
}


// =============================================================================
void GEVPlayerDlg::StopAcquisition()
{
    // Used to temporarily select the source in use
    PvGenStateStack lStack( mDevice.GetGenParameters() );
    PushSource( &lStack );

    ASSERT( mAcquisitionStateManager != NULL );
    PvResult lResult = mAcquisitionStateManager->Stop();
    if ( lResult.GetCode() == PvResult::Code::GENERIC_ERROR )
    {
        if ( mShowAcquisitionWarning )
        {
            WarningDlg lAcquisitionWarning;
            lAcquisitionWarning.SetWarning( lResult.GetDescription().GetUnicode() );
            lAcquisitionWarning.SetCheckboxMessage( _T( "Don't show this warning again in this session." ) );
            lAcquisitionWarning.DoModal();

            if ( lAcquisitionWarning.IsChecked() )
            {
                mShowAcquisitionWarning = false;
            }
        }
    }
    else if ( !lResult.IsOK() )
    {
        PvMessageBox( this, lResult );
    }
}


// =============================================================================
void GEVPlayerDlg::OnBnClickedStart()
{
    if ( !mDeviceConnected )
    {
        return;
    }

    StartAcquisition();
}


// =============================================================================
void GEVPlayerDlg::OnBnClickedStop()
{
    if ( !mDeviceConnected )
    {
        return;
    }

    SetStatusColor( SCDefault );
    mStatusTextBox.SetWindowText( _T( "" ) );

    StopAcquisition();
}


// =============================================================================
void GEVPlayerDlg::OnCbnSelchangeMode()
{
    if ( !mDeviceConnected || ( mAcquisitionStateManager == NULL ) )
    {
        return;
    }

    // No selection?
    if ( mModeCombo.GetCurSel() < 0 )
    {
        return;
    }

    // Set source selector if needed
    PvGenStateStack lStack( mDevice.GetGenParameters() );
    PushSource( &lStack );

    // Get acquisition mode parameter
    PvGenEnum *lMode = mDevice.GetGenParameters()->GetEnum( "AcquisitionMode" );
    if ( lMode == NULL )
    {
        return;
    }

    // Retrieve item data value from combo box item, assign it to acquisition mode parameter
    PvUInt64 lValue = mModeCombo.GetItemData( mModeCombo.GetCurSel() );
    PvResult lResult = lMode->SetValue( lValue );
    if ( !lResult.IsOK() )
    {
        PvMessageBox( this, lResult );
    }
}


// =============================================================================
LRESULT GEVPlayerDlg::OnImageDisplayed( WPARAM wParam, LPARAM lParam )
{
    // Stream opened, image save dlg exists, thread display is up
    if ( ( !mStream.IsOpen() ) ||
         ( mImageSaveDlg == NULL ) ||
         ( !mDisplayThread->IsRunning() ) )
    {
        return 0;
    }

    if ( mImageSaveDlg->GetSaveEnabled() )
    {
        SetStatusColor( SCRed );

		CString lAvgFPS = _T( "N/A" );
        if ( mImageSaveDlg->GetFrames() > 0 )
        {
            lAvgFPS.Format( _T( "%.01f" ), mImageSaveDlg->GetFPS() );
        }

		CString lStr;
        lStr.Format( _T( "RECORDING %d images    %I64i MB    %s FPS    %.01f Mbps"), mImageSaveDlg->GetFrames(), 
            mImageSaveDlg->GetTotalSize(), lAvgFPS, mImageSaveDlg->GetMbps() );
        mStatusTextBox.SetWindowText( lStr );
    }
    else
    {
        SetStatusColor( SCDefault );

		PvStreamInfo lInfo( &mStream );
		CString lStatistics = lInfo.GetStatistics( mDisplayThread->GetFPS() ).GetUnicode();
		CString lErrors = lInfo.GetErrors().GetUnicode();
		CString lWarnings = lInfo.GetWarnings( mPipelineReallocated ).GetUnicode();

        mStatusTextBox.SetWindowText( lStatistics + _T( "\r\n" ) + lErrors + _T( "\r\n" ) + lWarnings );
    }

    return 0;
}


// =============================================================================
void GEVPlayerDlg::OnParameterUpdate( PvGenParameter *aParameter )
{
    if ( mAcquisitionStateManager == NULL )
    {
        return;
    }

    PvString lName = aParameter->GetName();
    if ( lName == "AcquisitionMode" )
    {
        // Prevent re-entry
        if ( !mUpdatingAcquisitionMode )
        {
            if ( IsMultiSourceTransmitter( &mDevice ) )
            {
                PvInt64 lSource = 0;
                mDevice.GetGenParameters()->GetEnumValue( "SourceSelector", lSource );
                if ( lSource == mAcquisitionStateManager->GetSource() )
                {
                    // Only refresh if the source selector is that same as the source used by GEVPlayer
                    mUpdatingAcquisitionMode = TRUE;
                    SendMessageIfPossible( WM_UPDATEACQUISITIONMODE, 1 );
                }
            }
            else
            {
                // No source selector, always refresh
                mUpdatingAcquisitionMode = TRUE;
                SendMessageIfPossible( WM_UPDATEACQUISITIONMODE, 1 );
            }
        }
    }

    if ( lName == "SourceSelector" )
    {
        if ( !mUpdatingSources && IsMultiSourceTransmitter( &mDevice ) )
        {
            PvGenEnum *lSourceSelector = dynamic_cast<PvGenEnum *>( aParameter );
            if ( lSourceSelector == NULL )
            {
                return;
            }

            // Retrieve enum entry count from source selector
            PvInt64 lSourceCount = 0;
            lSourceSelector->GetEntriesCount( lSourceCount );

            // Build list of available sources
            std::vector<PvInt64> lSources;
            for ( PvInt64 i = 0; i < lSourceCount; i++ )
            {
                const PvGenEnumEntry *lEE = NULL;
                lSourceSelector->GetEntryByIndex( i, &lEE );
                if ( lEE->IsAvailable() )
                {
                    PvInt64 lValue = 0;
                    lEE->GetValue( lValue );

                    lSources.push_back( lValue );
                }
            }

            // Compare available sources vs. cached combo box content
            bool lIsTheSame = false;
            if ( lSources.size() == mAvailableSources.size() )
            {
                lIsTheSame = true;
                std::vector<PvInt64>::const_iterator lIt1 = lSources.begin();
                std::vector<PvInt64>::const_iterator lIt2 = mAvailableSources.begin();
                while ( ( lIt1 != lSources.end() ) && ( lIt2 != mAvailableSources.end() ) )
                {
                    if ( *lIt1 != *lIt2 )
                    {
                        lIsTheSame = false;
                        break;
                    }

                    lIt1++;
                    lIt2++;
                }
            }

            if ( !lIsTheSame )
            {
                mUpdatingSources = TRUE;
                PostMessage( WM_UPDATESOURCES );
            }
        }
    }
}


// =============================================================================
void GEVPlayerDlg::OnFileLoad()
{
    if ( ( mAcquisitionStateManager != NULL ) && 
         ( mAcquisitionStateManager->GetState() == PvAcquisitionStateLocked ) )
    {
        return;
    }

    CFileDialog lFileDlg( TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, L"PureGEV Configuration Files (*.pvcfg)|*.pvcfg|All files (*.*)|*.*||", this);

    lFileDlg.m_ofn.lpstrTitle = L"Open eBUS SDK Project";

    if ( lFileDlg.DoModal() == IDOK)
    {
        mFileName = lFileDlg.GetPathName();
        OpenConfig( mFileName );  
    }
}


// =============================================================================
void GEVPlayerDlg::OnFileSave()
{
    if ( ( mAcquisitionStateManager != NULL ) && 
         ( mAcquisitionStateManager->GetState() == PvAcquisitionStateLocked ) )
    {
        return;
    }

    if ( mFileName.IsEmpty() )
    {
        OnFileSaveAs();
        return;
    }
    
    SaveConfig( mFileName, true );

    ReportMRU( mFileName );
}


// =============================================================================
void GEVPlayerDlg::OnFileExit()
{
    Disconnect();
    EndDialog( IDOK );
}


// =============================================================================
void GEVPlayerDlg::OnRecent( UINT nID )
{
    unsigned int lIndex = nID - ID_FILE_RECENT1;
    std::list<CString>::iterator lIt;
    lIt = mRecentList.begin();
    unsigned int i = 0;
    while( lIt != mRecentList.end() && i <= lIndex )
    {
        if ( i == lIndex )
        {
            mFileName = *lIt;
        }
        lIt++;
        i++;
    }

    OpenConfig( mFileName );
}


// =============================================================================
void GEVPlayerDlg::OnToolsEventMonitor()
{
    if ( !mDeviceConnected )
    {
        return;
    }

    ASSERT( mEventMonitorDlg != NULL );
    if ( mEventMonitorDlg == NULL )
    {
        return;
    }

    ASSERT( mEventMonitorDlg->GetSafeHwnd() != 0 );
    if ( mEventMonitorDlg->GetSafeHwnd() == 0 )
    {
        return;
    }

    if ( mEventMonitorDlg->IsWindowVisible() )
    {
        mEventMonitorDlg->ShowWindow( SW_HIDE );
    }
    else
    {
        mEventMonitorDlg->ShowWindow( SW_SHOW );
        mEventMonitorDlg->BringWindowToTop();
        mEventMonitorDlg->SetFocus();
    }
}


// =============================================================================
void GEVPlayerDlg::OnToolsSerialCommunicationBridge()
{
    if ( !mDeviceConnected )
    {
        return;
    }

    ASSERT( mSerialBridgeManagerWnd != NULL );
    if ( mSerialBridgeManagerWnd == NULL )
    {
        return;
    }

    if ( mSerialBridgeManagerWnd->GetHandle() != 0 )
    {
        mSerialBridgeManagerWnd->Close();
    }
    else
    {
        mSerialBridgeManagerWnd->ShowModeless( GetSafeHwnd() );
    }
}

// =============================================================================
void GEVPlayerDlg::OnToolsActionCommand()
{
    if ( mActionCommandWnd == NULL )
    {
        mActionCommandWnd = new ActionCommandWnd();
    }

    if ( mActionCommandWnd->GetSafeHwnd() != 0 )
    {
        mActionCommandWnd->DestroyWindow();

        delete mActionCommandWnd;
        mActionCommandWnd = NULL;
        return;
    }

    bool lRetVal;
    CWnd lWndObject;
    CWnd *lWnd = CWnd::FromHandlePermanent( this->GetSafeHwnd() );
    if ( lWnd == NULL )
    {
        lWndObject.Attach( this->GetSafeHwnd() );
        lWnd = &lWndObject;
    }

	lRetVal = ( mActionCommandWnd->Create( ActionCommandWnd::IDD, lWnd ) != 0 ) ? true : false;

    if ( lWndObject.GetSafeHwnd() != 0 )
    {
	    lWndObject.Detach();
    }

	if ( lRetVal )
	{
		mActionCommandWnd->ShowWindow( SW_SHOW );
	}
}

// =============================================================================
void GEVPlayerDlg::OnHelpAbout()
{
	AboutBox lDlg;
	lDlg.DoModal();
}


// =============================================================================
void GEVPlayerDlg::OnMove(int x, int y)
{
    CDialog::OnMove(x, y);

    mDisplay.UpdateBounds();
}

    
// =============================================================================
void GEVPlayerDlg::OnInitMenu(CMenu* pMenu)
{
    CDialog::OnInitMenu(pMenu);
    bool lStreaming = ( mAcquisitionStateManager != NULL ) && ( mAcquisitionStateManager->GetState() == PvAcquisitionStateLocked );

    int lCountI = static_cast<int>( pMenu->GetMenuItemCount() );
    for ( int i = 0; i < lCountI; i++ )
    {
        UINT lID = pMenu->GetMenuItemID( i );
        if ( lID == -1 )
        {
            CMenu *lSubMenu = pMenu->GetSubMenu( i );
            int lCountJ = static_cast<int>( lSubMenu->GetMenuItemCount() );
            for ( int j = 0; j < lCountJ; j++ )
            {  
                UINT lID = lSubMenu->GetMenuItemID( j );
                if ( lID != -1 )
                {
                    switch ( lID )
                    {
                    case ID_TOOLS_SAVEPREFERENCES:
                    case ID_TOOLS_RESTOREDEFAULTPREFERENCES:
                        lSubMenu->EnableMenuItem( j, MF_BYPOSITION | 
                            ( ( !mRecovery ) ? 
                                MF_ENABLED : 
                                MF_GRAYED ) );
                        break;

                    case ID_TOOLS_EVENTMONITOR:
                        lSubMenu->EnableMenuItem( j, MF_BYPOSITION | 
                            ( ( ( mEventMonitorDlg != NULL ) && !mRecovery ) ? 
                                MF_ENABLED : 
                                MF_GRAYED ) );
                        lSubMenu->CheckMenuItem( j, MF_BYPOSITION |
                            ( ( ( mEventMonitorDlg != NULL ) && mEventMonitorDlg->IsWindowVisible() ) ? 
                                MF_CHECKED : 
                                MF_UNCHECKED ) );
                        break;

                    case ID_TOOLS_DISPLAYVSYNC:
                        lSubMenu->EnableMenuItem( j, MF_BYPOSITION | 
                            ( ( mDisplayThread->IsRunning() ) ? 
                                MF_ENABLED : 
                                MF_GRAYED ) );
                        lSubMenu->CheckMenuItem( j, MF_BYPOSITION | 
                                ( mDisplayThread->GetVSyncEnabled() ? 
                                MF_CHECKED : 
                                MF_UNCHECKED ) );
                        break;

                    case ID_TOOLS_DISPLAYPARTIALIMAGES:
                        lSubMenu->EnableMenuItem( j, MF_BYPOSITION | 
                            ( ( mDisplayThread->IsRunning() ) ? 
                                MF_ENABLED : 
                                MF_GRAYED ) );
                        lSubMenu->CheckMenuItem( j, MF_BYPOSITION | 
                                ( mDisplayThread->GetKeepPartialImagesEnabled() ? 
                                MF_CHECKED : 
                                MF_UNCHECKED ) );
                        break;

                    case ID_TOOLS_DISPLAYCHUNKDATA:
                        lSubMenu->EnableMenuItem( j, MF_BYPOSITION | 
                            ( ( mDisplayThread->IsRunning() ) ? 
                                MF_ENABLED : 
                                MF_GRAYED ) );
                        lSubMenu->CheckMenuItem( j, MF_BYPOSITION | 
                                ( mDisplayThread->GetDisplayChunkDataEnabled() ? 
                                MF_CHECKED : 
                                MF_UNCHECKED ) );
                        break;

                    case ID_TOOLS_SAVEIMAGES:
                        lSubMenu->EnableMenuItem( j, MF_BYPOSITION | 
                            ( ( mDisplayThread->IsRunning() ) ? 
                                MF_ENABLED : 
                                MF_GRAYED ) );
                        break;

                    case ID_TOOLS_SAVECURRENTIMAGE:
                        lSubMenu->EnableMenuItem( j, MF_BYPOSITION | 
                            ( ( mDisplayThread->IsRunning() ) ? 
                                MF_ENABLED : 
                                MF_GRAYED ) );
                        break;

                    case ID_TOOLS_BUFFEROPTIONS:
                        lSubMenu->EnableMenuItem( j, MF_BYPOSITION | 
                            ( ( mDisplayThread->IsRunning() ) ? 
                                MF_ENABLED : 
                                MF_GRAYED ) );
                        break;

                    case ID_FILE_SAVEAS:
                    case ID_FILE_SAVE:
                        lSubMenu->EnableMenuItem( j, MF_BYPOSITION | 
                            ( ( ( mDeviceConnected || mStream.IsOpen() ) && !mRecovery && !lStreaming ) ? 
                                MF_ENABLED : 
                                MF_GRAYED ) );
                        break;

                    case ID_FILE_LOAD:
                        lSubMenu->EnableMenuItem( j, MF_BYPOSITION | 
                            ( ( ( mCommunicationWnd.GetHandle() == 0 ) && 
                                ( mDeviceWnd.GetHandle() == 0 ) && 
                                ( mStreamParametersWnd.GetHandle() == 0 ) && 
                                ( !mRecovery && !lStreaming ) ) ? 
                                MF_ENABLED : 
                                MF_GRAYED ) );
                        break;

                    case ID_TOOLS_IMAGEFILTERING:
                        lSubMenu->EnableMenuItem( j, MF_BYPOSITION | 
                            ( ( mStream.IsOpen() ) ? 
                                MF_ENABLED : 
                                MF_GRAYED ) );
                        lSubMenu->CheckMenuItem( j, MF_BYPOSITION |
                            ( ( mFilteringDlg.IsWindowVisible() ) ? 
                                MF_CHECKED : 
                                MF_UNCHECKED ) );
                        break;

                    case ID_TOOLS_IPENGINESERIALCOMMUNICATION:
                        lSubMenu->EnableMenuItem( j, MF_BYPOSITION |
                            ( ( mDevice.IsConnected() && mSerial0IPEngineSupported && !mRecovery ) ? 
                                MF_ENABLED : 
                                MF_GRAYED ) );
                        lSubMenu->CheckMenuItem( j, MF_BYPOSITION |
                            ( ( mTerminalIPEngineWnd.GetHandle() != 0 ) ?
                                MF_CHECKED : 
                                MF_UNCHECKED ) );
                        break;

                    case ID_TOOLS_SAVE_XML:
                        lSubMenu->EnableMenuItem( j, MF_BYPOSITION |
                            ( ( mDevice.IsConnected() && !mRecovery ) ? 
                                MF_ENABLED : 
                                MF_GRAYED ) );
                        break;

                    case ID_TOOLS_SERIALCOMMUNICATIONBRIDGE:
                        lSubMenu->EnableMenuItem( j, MF_BYPOSITION | 
                            ( ( ( mSerialBridgeManagerWnd != NULL ) && !mRecovery ) ? 
                                MF_ENABLED : 
                                MF_GRAYED ) );
                        lSubMenu->CheckMenuItem( j, MF_BYPOSITION |
                            ( ( ( mSerialBridgeManagerWnd != NULL ) && ( mSerialBridgeManagerWnd->GetHandle() != 0 ) ) ? 
                                MF_CHECKED : 
                                MF_UNCHECKED ) );
                        break;
                    case ID_TOOLS_ACTIONCOMMAND:
                        lSubMenu->CheckMenuItem( j, MF_BYPOSITION |
                            ( ( ( mActionCommandWnd != NULL ) && ( mActionCommandWnd->GetSafeHwnd() != 0 ) ) ? 
                                MF_CHECKED : 
                                MF_UNCHECKED ) );
                        break;
                    }
                }
                else
                {
                    CMenu *lSubMenu2 = lSubMenu->GetSubMenu( j );
                    int lCountK = static_cast<int>( lSubMenu2->GetMenuItemCount() );
                    for ( int k = 0; k < lCountK; k++ )
                    {  
                        UINT lID = lSubMenu2->GetMenuItemID( k );
                        switch ( lID )
                        {
                        case ID_DISPLAYIMAGES_DISABLED:
                            lSubMenu2->EnableMenuItem( k, MF_BYPOSITION | 
                                ( ( mDisplayThread->IsRunning() ) ? 
                                    MF_ENABLED : 
                                    MF_GRAYED ) );
                            lSubMenu2->CheckMenuItem( k, MF_BYPOSITION |
                                    ( ( mDisplayThread->GetTargetFPS() == 0 ) ?
                                    MF_CHECKED : 
                                    MF_UNCHECKED ) );
                            break;

                        case ID_DISPLAYIMAGES_30FPS:
                            lSubMenu2->EnableMenuItem( k, MF_BYPOSITION | 
                                ( ( mDisplayThread->IsRunning() ) ? 
                                    MF_ENABLED : 
                                    MF_GRAYED ) );
                            lSubMenu2->CheckMenuItem( k, MF_BYPOSITION |
                                ( ( mDisplayThread->GetTargetFPS() == 30 ) ?
                                    MF_CHECKED : 
                                    MF_UNCHECKED ) );
                            break;

                        case ID_DISPLAYIMAGES_60FPS:
                            lSubMenu2->EnableMenuItem( k, MF_BYPOSITION | 
                                ( ( mDisplayThread->IsRunning() ) ? 
                                    MF_ENABLED : 
                                    MF_GRAYED ) );
                            lSubMenu2->CheckMenuItem( k, MF_BYPOSITION |
                                ( ( mDisplayThread->GetTargetFPS() == 60 ) ?
                                    MF_CHECKED : 
                                    MF_UNCHECKED ) );
                            break;

                        case ID_TOOLS_DEINTERLACING_DISABLED:
                            lSubMenu2->EnableMenuItem( k, MF_BYPOSITION | 
                                ( ( mDisplayThread->IsRunning() ) ? 
                                    MF_ENABLED : 
                                    MF_GRAYED ) );
                            lSubMenu2->CheckMenuItem( k, MF_BYPOSITION |
                                ( ( mDisplayThread->GetDeinterlacing() == PvDeinterlacingDisabled ) ?
                                    MF_CHECKED : 
                                    MF_UNCHECKED ) );
                            break;

                        case ID_TOOLS_DEINTERLACING_WEAVING:
                            lSubMenu2->EnableMenuItem( k, MF_BYPOSITION | 
                                ( ( mDisplayThread->IsRunning() ) ? 
                                    MF_ENABLED : 
                                    MF_GRAYED ) );
                            lSubMenu2->CheckMenuItem( k, MF_BYPOSITION |
                                ( ( mDisplayThread->GetDeinterlacing() == PvDeinterlacingWeaving ) ?
                                    MF_CHECKED : 
                                    MF_UNCHECKED ) );
                            break;

                        case ID_FILE_RECENT1:
                        case ID_FILE_RECENT2:
                        case ID_FILE_RECENT3:
                        case ID_FILE_RECENT4:
                        case ID_FILE_RECENT5:
                        case ID_FILE_RECENT6:
                        case ID_FILE_RECENT7:
                        case ID_FILE_RECENT8:
                        case ID_FILE_RECENT9:
                        case ID_FILE_RECENT10:
                            lSubMenu2->EnableMenuItem( k, MF_BYPOSITION | 
                                ( ( !mRecovery && !lStreaming ) ? 
                                    MF_ENABLED : 
                                    MF_GRAYED ) );
                            break;
                        }
                    }
                }
            }
        }
    }
}


// =============================================================================
void GEVPlayerDlg::OnToolsDisplaysync()
{
    mDisplayThread->SetVSyncEnabled( !mDisplayThread->GetVSyncEnabled() );
}


// =============================================================================
HBRUSH GEVPlayerDlg::OnCtlColor( CDC* pDC, CWnd* pWnd, UINT nCtlColor )
{
    switch ( pWnd->GetDlgCtrlID() )
    {
    case IDC_STATUS:    
        switch ( mStatusColor )
        {
        case SCDefault:
            break;

        case SCYellow:
            pDC->SetBkColor( mYellowColor );
            return mYellowBrush;

        case SCRed:
            pDC->SetTextColor( RGB( 0xFF, 0xFF, 0xFF ) );
            pDC->SetBkColor( mRedColor );
            return mRedBrush;
        }
        break;
    }

    return CDialog::OnCtlColor( pDC, pWnd, nCtlColor );
}


// =============================================================================
void GEVPlayerDlg::OnLinkDisconnected( PvDevice *aDevice )
{
    PostMessage( WM_LINKDISCONNECTED );    
}


// =============================================================================
LRESULT GEVPlayerDlg::OnLinkDisconnected( WPARAM wParam, LPARAM lParam )
{
    mRecovery = TRUE;

    SendMessage( WM_STOPSTREAMING );
    SendMessage( WM_DISCONNECT );

    return 0;
}

    
// =============================================================================
void GEVPlayerDlg::OnLinkReconnected( PvDevice *aDevice )
{
    PostMessage( WM_LINKRECONNECTED );
}


// =============================================================================
LRESULT GEVPlayerDlg::OnLinkReconnected( WPARAM wParam, LPARAM lParam )
{
    PvGenInteger *lInteger = mDevice.GetGenParameters()->GetInteger( "GevCurrentIPAddress" );
    if ( lInteger != NULL )
    {
        PvInt64 lValue;
        lInteger->GetValue( lValue );

        CString lIP;
        lIP.Format( _T( "%I64i.%I64i.%I64i.%I64i" ),
            ( lValue & 0xFF000000 ) >> 24,
            ( lValue & 0x00FF0000 ) >> 16,
            ( lValue & 0x0000FF00 ) >> 8,
            ( lValue & 0x000000FF ) );

        mIPEdit.SetWindowText( lIP );
    }

    bool lStreaming = false;
    if ( mAcquisitionStateManager != NULL )
    {
        lStreaming = mAcquisitionStateManager->GetState() == PvAcquisitionStateLocked;
    }

    // Important - this will, amongst other, make sure TLParamsLocked is set to 0!
    StopAcquisition();

    if ( mSerialBridgeManagerWnd != NULL )
    {
        mSerialBridgeManagerWnd->Recover();
    }

    SendMessage( WM_STREAM, lStreaming ? 1 : 0 );

    SetStatusColor( SCDefault );
    mStatusTextBox.SetWindowText( _T( "" ) );

    mRecovery = FALSE;

    return 0;
}


// =============================================================================
LRESULT GEVPlayerDlg::OnStream( WPARAM wParam, LPARAM lParam )
{
    if ( !mDevice.IsConnected() )
    {
        return 0;
    }
    
    PvGenStateStack lStack( mDevice.GetGenParameters() );
    PushSource( &lStack );

    PvInt64 lChannel = 0;
    if ( IsMultiSourceTransmitter( &mDevice ) )
    {
        mDevice.GetGenParameters()->GetIntegerValue( "SourceStreamChannel", lChannel );
    }

    if ( mSetupDlg.GetRole() == SetupDlg::RoleCtrlData )
    {
        //
        // Only re-open stream if control and data.
        //     - If control only, no need for stream
        //     - If data only, there is no recovery required
        //     - If monitor, well there is no stream!
        //

        PvGenInteger *lInteger = mDevice.GetGenParameters()->GetInteger( "GevCurrentIPAddress" );
        ASSERT( lInteger != NULL );

        PvInt64 lValue;
        lInteger->GetValue( lValue );

        CString lIP;
        lIP.Format( _T( "%I64i.%I64i.%I64i.%I64i" ),
            ( lValue & 0xFF000000 ) >> 24,
            ( lValue & 0x00FF0000 ) >> 16,
            ( lValue & 0x0000FF00 ) >> 8,
            ( lValue & 0x000000FF ) );

        PvResult lResult = PvResult::Code::GENERIC_ERROR;
        switch ( mSetupDlg.GetDestination() )
        {
            case SetupDlg::DestinationUnicastAuto:
                lResult = mStream.Open( (LPCTSTR)lIP );
                break;

            case SetupDlg::DestinationUnicastSpecific:
                lResult = mStream.Open( (LPCTSTR)lIP, 
                    mSetupDlg.GetUnicastSpecificPort(), (PvUInt16)lChannel );
                break;

            case SetupDlg::DestinationMulticast:
                lResult = mStream.Open( (LPCTSTR)lIP, 
                    (LPCTSTR)mSetupDlg.GetMulticastIP(), mSetupDlg.GetMulticastPort(), (PvUInt16)lChannel );
                break;

            default:
                ASSERT( 0 );
        }

        if ( lResult.IsOK() )
        {
            mDevice.SetStreamDestination( mStream.GetLocalIPAddress(), mStream.GetLocalPort(), (PvUInt16)lChannel );
            StartStreaming();

            if ( wParam != 0 )
            {
                StartAcquisition();
            }
        }
    }
    else if ( mSetupDlg.GetRole() == SetupDlg::RoleCtrl )
    {
        //
        // Just controlling: set the destination as configured, start acquisition if needed
        //

        PvResult lResult = PvResult::Code::GENERIC_ERROR;
        switch ( mSetupDlg.GetDestination() )
        {
            case SetupDlg::DestinationUnicastOther:
                lResult = mDevice.SetStreamDestination( (LPCTSTR)mSetupDlg.GetUnicastIP(), 
                    mSetupDlg.GetUnicastPort(), (PvUInt16)lChannel );
                break;

            case SetupDlg::DestinationMulticast:
                lResult = mDevice.SetStreamDestination( (LPCTSTR)mSetupDlg.GetMulticastIP(), 
                    mSetupDlg.GetMulticastPort(), (PvUInt16)lChannel );
                break;

            default:
                ASSERT( 0 );
        }

        if ( lResult.IsOK() )
        {
            ASSERT( mAcquisitionStateManager != NULL );
            if ( mAcquisitionStateManager->GetState() == PvAcquisitionStateLocked )
            {
                StartAcquisition();
            }
        }
    }

    SetStatusColor( SCDefault );
    mStatusTextBox.SetWindowText( _T( "" ) );

	EnableInterface();

    return 0;
}


// =============================================================================
LRESULT GEVPlayerDlg::OnDisconnect( WPARAM wParam, LPARAM lParam )
{
    // Is reconnecting enabled?
    PvGenBoolean *lLinkRecoveryEnabled = mDevice.GetGenLink()->GetBoolean( "LinkRecoveryEnabled"  );
    ASSERT( lLinkRecoveryEnabled != NULL );

    bool lEnabled = false;
    lLinkRecoveryEnabled->GetValue( lEnabled );

    if ( lEnabled )
    {
        SetStatusColor( SCYellow );
        mStatusTextBox.SetWindowText( _T( "Connection to device lost..." ) );

		CloseGenWindow( &mDeviceWnd );
		CloseGenWindow( &mCommunicationWnd );
		CloseGenWindow( &mStreamParametersWnd );

        if ( mSerialBridgeManagerWnd->GetHandle() != 0 )
        {
            mSerialBridgeManagerWnd->Close();
        }

		EnableTreeBrowsers( FALSE );
		EnableControls( FALSE );
    }
    else
    {
        SetStatusColor( SCDefault );
        mStatusTextBox.SetWindowText( _T( "" ) );

        CWaitCursor lCursor;
        Disconnect();

        MessageBox( _T( "Connection to device lost." ), _T( "GEVPlayer" ), MB_OK | MB_ICONINFORMATION );
    }

    return 0;
}


// =============================================================================
LRESULT GEVPlayerDlg::OnStopStreaming( WPARAM wParam, LPARAM lParam )
{
    if ( mStream.IsOpen() )
    {
        StopStreaming();
        mStream.Close();
    }

    return 0;
}


// =============================================================================
void GEVPlayerDlg::OnToolsSetup()
{
    mSetupDlg.SetEnabled( !mDeviceConnected && !mStream.IsOpen() );
    mSetupDlg.DoModal();
    mSetupDlg.DestroyWindow();

    EnableInterface();
}


// =============================================================================
void GEVPlayerDlg::OnToolsImagefiltering()
{
    if ( mFilteringDlg.IsWindowVisible() )
    {
        mFilteringDlg.ShowWindow( SW_HIDE );
    }
    else
    {
        mFilteringDlg.ShowWindow( SW_SHOW );
        mFilteringDlg.BringWindowToTop();
        mFilteringDlg.SetFocus();
    }
}


// =============================================================================
void GEVPlayerDlg::OnFileSaveAs()
{
    if ( ( mAcquisitionStateManager != NULL ) && 
         ( mAcquisitionStateManager->GetState() == PvAcquisitionStateLocked ) )
    {
        return;
    }

    CFileDialog lFileDlg( FALSE, L".pvcfg", NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, L"PureGEV Configuration Files (*.pvcfg)|*.pvcfg|All files (*.*)|*.*||", this);

    lFileDlg.m_ofn.lpstrTitle = L"Save eBUS SDK Project";

    if ( lFileDlg.DoModal() == IDOK)
    {
        mFileName = lFileDlg.GetPathName(); // This is your selected file name with path
        
        OnFileSave();
    }
}


// =============================================================================
void GEVPlayerDlg::OpenConfig( CString aFileName )
{
    if ( mCommunicationWnd.GetHandle() != 0 )
    {
        MessageBox( _T( "Please close the communication control window before loading a new configuration.") );
        return;
    }
    if ( mStreamParametersWnd.GetHandle() != 0 )
    {
        MessageBox( _T( "Please close the image stream control window before loading a new configuration.") );
        return;
    }
    if ( mDeviceWnd.GetHandle() != 0 )
    {
        MessageBox( _T( "Please close the Device control window before loading a new configuration.") );
        return;
    }

    // Create reader, load file
    PvConfigurationReader lReader;
    PvResult lResult = lReader.Load( aFileName.GetBuffer() );
    if ( !lResult.IsOK() )
    {
        PvMessageBox( this, lResult );
        return;
    }

    // we're loading a new persistence file. Report it now so that it's visible in the title bar while we're loading.
    if ( !mFileName.IsEmpty() && aFileName == mFileName )
    {
        ReportMRU( aFileName );
    }

    // Check if roles are the same if not, we must disconnect first
    if ( ( mDeviceConnected || mStream.IsOpen() ) && 
         !mSetupDlg.IsTheSame( lReader ) )
    {
        if ( MessageBox( _T( "Setup of this configuration file does not match the current ")
                         _T( "connection.\r\n\r\nIn order to proceed with the operation, ")
                         _T( "the GigE Vision Device will be disconnected.\r\n\r\nContinue ")
                         _T( "restoring configuration?"), 
                         _T( "GEVPlayer" ), MB_YESNO | MB_ICONQUESTION ) == IDNO )
        {
            return;
        }    

        Disconnect();
    }

    mSetupDlg.Load( lReader );
    mFilteringDlg.Load( lReader );
    
    // Data is in log buffer, but the UI is event monitor. Once the data is loaded,
    // notify the event monitor to refresh itself
    mLogBuffer->Load( lReader );
    if ( mEventMonitorDlg != NULL )
    {
        mEventMonitorDlg->LoadData();
    }

    if ( lReader.GetGenParameterArrayCount() > 0 )
    {
        lReader.Restore( "communication", *mDevice.GetGenLink() );
    }

    if ( lReader.GetDeviceCount() > 0 || lReader.GetStreamCount() > 0 )
    {
        // If we're connected, just apply the settings. Otherwise connect from the settings in the persistence file.
        if ( mDeviceConnected )
        {
            if ( MessageBox( _T( "Opening a file now will apply the settings to the currently connected " )
                             _T( "GigE Vision Device. \r\n\r\nIf you wish to connect to a different GigEVision Device associated " )
                             _T( "with the file, disconnect first, then open the file. \r\n\r\nContinue opening file?" ), 
                             _T( "GEVPlayer"), MB_YESNO | MB_ICONQUESTION ) == IDNO )
            {
                return;
            }

            PvStringList lErrorList;
            LoadingThread lLoadingThread( &mSetupDlg, &lReader, &mDevice, &mStream, mSerialBridgeManagerWnd, &lErrorList, this );
            lLoadingThread.Load();

            if ( lErrorList.GetSize() > 0 )
            {
                ErrorsDlg lDlg;
                lDlg.DoModal( &lErrorList );
            }

            if ( mDevice.IsConnected() && IsMultiSourceTransmitter( &mDevice ) )
            {
                PvString lSourceStr;
                if ( lReader.Restore( TAG_ACTIVESOURCE, lSourceStr ).IsOK() )
                {
                    PvInt32 lValue = 0;
                    sscanf_s( lSourceStr.GetAscii(), "%d", &lValue );

                    SendMessage( WM_UPDATESOURCES, 0, lValue );
                }
            }
        }
        else
        {
            Connect( NULL, &lReader );
        }

        mDeviceConnected = mDevice.IsConnected();
    }

    LoadDisplayOptions( lReader );

    mImageSaveDlg->Load( lReader );

	BufferOptionsDlg::Load( lReader, &mPipeline );
    PvPropertyList lCommunicationBrowserOptions;
    if ( lReader.Restore( TAG_COMMUNICATIONBROWSER, lCommunicationBrowserOptions ).IsOK() )
    {
        mCommunicationWnd.Load( lCommunicationBrowserOptions );        
    }
    PvPropertyList lDeviceBrowserOptions;
    if ( lReader.Restore( TAG_DEVICEBROWSER, lDeviceBrowserOptions ).IsOK() )
    {
        mDeviceWnd.Load( lDeviceBrowserOptions );        
    }
    PvPropertyList lStreamBrowserOptions;
    if ( lReader.Restore( TAG_STREAMBROWSER, lStreamBrowserOptions ).IsOK() )
    {
        mStreamParametersWnd.Load( lStreamBrowserOptions );        
    }

    EnableInterface();
}


// =============================================================================
void GEVPlayerDlg::ReportMRU(CString aFileName)
{
    //typedef CStringT< TCHAR, StrTraitATL< TCHAR > > CAtlString;
    CString resToken;
    int curPos= 0;
    CString lName;
    resToken= aFileName.Tokenize(_T("\\"),curPos);
    while (resToken != "")
    {
        lName = resToken;
        resToken= aFileName.Tokenize(_T("\\"),curPos);
    };
    SetWindowText( lName + _T(" - GEVPlayer") );

    // Try to find the item in our current list

    std::list<CString>::iterator lIt;
    std::list<CString>::iterator lToRemove;

    lIt = mRecentList.begin();
    bool lRemove = false;
    while (lIt != mRecentList.end())
    {
        if (*lIt == aFileName)
        {
            // If match, remove it, will be added as top latter
            lToRemove = lIt;
            lRemove = true;
        }
        lIt++;
    }

    if ( lRemove )
    {
        mRecentList.erase(lToRemove);
    }

    // Whether we removed already existing or not, insert at top
    mRecentList.push_front(aFileName);

    // trim the end (we don't want more than 10)
    if ( mRecentList.size() > 10 )
    {
        lIt = mRecentList.begin();
        std::list<CString>::iterator lLast;
        while (lIt != mRecentList.end())
        {
            lLast = lIt;
            lIt++;
        }
        // erase the back, not the front
        mRecentList.erase( lLast );
    }

    // Save current MRU to registry
    SaveMRUToRegistry();

    UpdateMRUMenu();
}


// =============================================================================
void GEVPlayerDlg::SaveMRUToRegistry()
{
    // iterate through the list

    std::list<CString>::iterator lIt;

    lIt = mRecentList.begin();

    CWinApp* pApp = AfxGetApp();

    UINT lCount = 0;
    while( lIt != mRecentList.end() ) 
    {
        wchar_t lNumber[4];
        swprintf_s(lNumber, 4, L"%d", lCount);

        wchar_t lToRegistry[MAX_PATH];
        swprintf_s(lToRegistry, MAX_PATH, L"%s", lIt->GetBuffer());
        BOOL lRet = pApp->WriteProfileString(_T("MRU"), lNumber, lToRegistry);
        lIt++;
        lCount++;
    }

}


// =============================================================================
void GEVPlayerDlg::LoadMRUFromRegistry()
{

    CString lData;
    DWORD lRet = 1;
    UINT lCount = 0;

    CWinApp* pApp = AfxGetApp();
    while( lData.Compare(_T("default")) != 0 )
    {
        wchar_t lNumber[4];
        swprintf_s(lNumber, 4, _T("%d"), lCount);
        lData = pApp->GetProfileString( _T("MRU"), lNumber, _T("default") );
        if ( lData.Compare(_T("default")) != 0 )
        {
           mRecentList.push_back( lData ); 
        }
        lCount++;
    }
}
 

// =============================================================================
void GEVPlayerDlg::UpdateMRUMenu()
{
    CMenu* mmenu = GetMenu();
    CMenu* lSubMenu = mmenu->GetSubMenu(0);
    if ( mMRUMenu == NULL )
    {
        mMRUMenu = new CMenu();
        mMRUMenu->CreatePopupMenu();
        lSubMenu->InsertMenu( 1, MF_BYPOSITION | MF_POPUP, (UINT_PTR)mMRUMenu->m_hMenu, L"Open Recent" );
    }
    if ( !mRecentList.empty() )
    {
        std::list<CString>::iterator lIt;
        lIt = mRecentList.begin();
        unsigned int i = 0;
        while( lIt != mRecentList.end() )
        {
            mMRUMenu->DeleteMenu( ID_FILE_RECENT1 + i, MF_BYCOMMAND );

            wchar_t lRecentString[MAX_PATH];
            swprintf_s( lRecentString, MAX_PATH, L"%d - %s", i+1, lIt->GetBuffer());

            mMRUMenu->AppendMenu(MF_STRING | MF_ENABLED, ID_FILE_RECENT1 + i, lRecentString );
            i++;
            lIt++;
        }
    }

}


// =============================================================================
BOOL GEVPlayerDlg::PreTranslateMessage(MSG* pMsg)
{
    if (WM_KEYFIRST <= pMsg->message && pMsg->message
                <= WM_KEYLAST)
        if (mAccel && ::TranslateAccelerator
            (m_hWnd, mAccel, pMsg))
            return TRUE;

    return CDialog::PreTranslateMessage(pMsg);
}


// =============================================================================
void GEVPlayerDlg::OnAcceleratorSave()
{
    OnFileSave();
}


// =============================================================================
void GEVPlayerDlg::OnAcceleratorOpen()
{
    OnFileLoad();
}


// =============================================================================
void GEVPlayerDlg::OnToolsSaveimages()
{
    if ( !mDisplayThread->IsRunning() )
    {
        return;
    }
    mImageSaveDlg->DoModal();
}


// =============================================================================
void GEVPlayerDlg::OnToolsSavecurrentimage()
{
    ASSERT( mDisplayThread->IsRunning() );
    if ( !mDisplayThread->IsRunning() )
    {
        return;
    }
    mImageSaveDlg->SaveThisImage();
}


// =============================================================================
void GEVPlayerDlg::SetStatusColor( StatusColor aColor )
{
    mStatusColor = aColor;
    mStatusTextBox.Invalidate();
}

// =============================================================================
void GEVPlayerDlg::OnEvent( PvDevice *aDevice, PvUInt16 aEventID, PvUInt16 aChannel, PvUInt64 aBlockID, 
    PvUInt64 aTimestamp, const void *aData, PvUInt32 aDataLength )
{
    // Event monitor is expected to be created when the device is connected
    ASSERT( mEventMonitorDlg != NULL ); 
    if ( ( aEventID == 0x9009 ) && ( aData != NULL ) && ( aDataLength > 0 ) )
    {
        if ( mLogBuffer->IsSerialComLogEnabled() )
        {
            mLogBuffer->CamHeadSerialComLog( (const unsigned char*)aData, aDataLength, aTimestamp );
        }
    }
}


// =============================================================================
void GEVPlayerDlg::OnEventGenICam( PvDevice *aDevice, PvUInt16 aEventID, PvUInt16 aChannel, PvUInt64 aBlockID, 
    PvUInt64 aTimestamp, PvGenParameterList *aData )
{
    // Event monitor is expected to be created when the device is connected
    ASSERT( mEventMonitorDlg != NULL ); 
    if ( mLogBuffer->IsEventsEnabled() )
    {
        // Basic event log
        CString lLog;
        lLog.Format( _T( "Event ID: 0x%X Channel: 0x%X BlockID: 0x%X, Timestamp: 0x%X" ),
            aEventID, aChannel, aBlockID, aTimestamp );

        // Add parameters
        if ( aData != NULL )
        {
            // Is there a parameter containing "DataLength"? Get its value to query only subset of defined IRegister
            PvInt64 lDataLength = 0;
            PvGenParameter *lP = aData->GetFirst();
            while ( lP != NULL )
            {
                CString lName = lP->GetName();
                if ( lName.Find( _T( "DataLength" ) ) >= 0 )
                {
                    PvGenInteger *lPI = dynamic_cast<PvGenInteger *>( lP );
                    if ( lPI != NULL )
                    {
                        lPI->GetValue( lDataLength );
                    }

                    break;
                }

                lP = aData->GetNext();
            }

            PvGenParameter *lParameter = aData->GetFirst();
            while ( lParameter != NULL )
            {
                lLog += _T( "\r\n                " );
                lLog += lParameter->GetName().GetUnicode();
                lLog += _T( ": " );

                PvGenRegister *lRegister = dynamic_cast<PvGenRegister *>( lParameter );
                if ( lRegister == NULL )
                {
                    lLog += lParameter->ToString().GetUnicode();
                }
                else
                {
                    lLog += "\r\n";

                    if ( lDataLength <= 0 )
                    {
                        lRegister->GetLength( lDataLength );
                    }

                    PvUInt32 lBufferLength = static_cast<PvUInt32>( lDataLength );
                    while ( ( lBufferLength % 4 ) != 0 )
                    {
                        lBufferLength++;
                    }

                    PvUInt8 *lData = new PvUInt8[ lBufferLength ];

                    PvResult lResult = lRegister->Get( lData, static_cast<PvUInt32>( lBufferLength ) );
                    lLog += LogBuffer::Unpack( "                    ", lData, static_cast<PvUInt32>( lDataLength ), 0 );

                    delete []lData;
                    lData = NULL;
                }

                lParameter = aData->GetNext();
            }
        }

        mLogBuffer->Log( lLog );
    }
}


// =============================================================================
void GEVPlayerDlg::OnToolsSerialCommunication()
{
    if ( !mTerminalIPEngineWndModeless ) 
    {
        CloseGenWindow( &mCommunicationWnd );
        CloseGenWindow( &mDeviceWnd );
        CloseGenWindow( &mStreamParametersWnd );

        mTerminalIPEngineWnd.SetDevice( &mDevice );
        mTerminalIPEngineWnd.ShowModal( GetSafeHwnd() );
    }
    else
    {
        if ( mTerminalIPEngineWnd.GetHandle() != 0 )
        {
            mTerminalIPEngineWnd.Close();
        }
        else
        {
            mTerminalIPEngineWnd.SetDevice( &mDevice );
            mTerminalIPEngineWnd.ShowModeless( GetSafeHwnd() );
        }
    }
}


// ==============================================================================
void GEVPlayerDlg::OnTimer( UINT_PTR nIDEvent )
{
    if ( nIDEvent == 1 )
    {
        // Periodically refresh the status, helps catching
        // the last image in a sequence if it is filtered out
        // by the display frame rate limiter
        SendMessage( WM_IMAGEDISPLAYED );
    }

    CDialog::OnTimer( nIDEvent );
}


// ==========================================================================
void GEVPlayerDlg::OnDestroy()
{
    CDialog::OnDestroy();

    if ( mTimer != 0 )
    {
        KillTimer( 1 );
        mTimer = 0;
    }
}

// ==========================================================================
void GEVPlayerDlg::OnRegisterInterface()
{
    if ( !mDevice.IsConnected() )
    {
        return;
    }

    CloseGenWindow( &mCommunicationWnd );
    CloseGenWindow( &mDeviceWnd );
    CloseGenWindow( &mStreamParametersWnd );

    mRegisterInterfaceDlg.SetDevice( &mDevice );
    mRegisterInterfaceDlg.DoModal();
    mRegisterInterfaceDlg.DestroyWindow();
}

// ==========================================================================
PvUInt32 GEVPlayerDlg::GetPayloadSize()
{
    if ( !mDevice.IsConnected() )
    {
        return 0;
    }

    // Used to temporarily select the source in use
    PvGenStateStack lStack( mDevice.GetGenParameters() );
    PushSource( &lStack );

    PvGenInteger *lPayloadSize = mDevice.GetGenParameters()->GetInteger( "PayloadSize" );
    PvGenInteger *lWidth = mDevice.GetGenParameters()->GetInteger( "Width" );
    PvGenInteger *lHeight = mDevice.GetGenParameters()->GetInteger( "Height" );
    PvGenEnum *lPixelFormat = mDevice.GetGenParameters()->GetEnum( "PixelFormat" );

    // Get payload size
    PvInt64 lPayloadSizeValue = 0;
    if ( lPayloadSize != NULL )
    {
        lPayloadSize->GetValue( lPayloadSizeValue );
    }

    // Compute poor man's payload size - for devices not maintaining PayloadSize properly
    PvInt64 lPoorMansPayloadSize = 0;
    if ( ( lWidth != NULL ) && ( lHeight != NULL ) && ( lPixelFormat != NULL ) )
    {
        PvInt64 lWidthValue, lHeightValue;
        lWidth->GetValue( lWidthValue );
        lHeight->GetValue( lHeightValue );

        PvInt64 lPixelFormatValue;
        lPixelFormat->GetValue( lPixelFormatValue );
        PvInt64 lPixelSizeInBits = PvGetPixelBitCount( static_cast<PvPixelType>( lPixelFormatValue ) );

        lPoorMansPayloadSize = ( lWidthValue * lHeightValue * lPixelSizeInBits ) / 8;
    }

    // Take max
    PvInt64 lBestPayloadSize = max( lPayloadSizeValue, lPoorMansPayloadSize );
    if ( ( lBestPayloadSize > 0 ) && ( lBestPayloadSize < UINT_MAX ) )
    {
        // Round up to make it mod 32 (works around an issue with some devices)
        if ( ( lBestPayloadSize % 32 ) != 0 )
        {
            lBestPayloadSize = ( ( lBestPayloadSize / 32 ) + 1 ) * 32;
        }

        return static_cast<PvUInt32>( lBestPayloadSize );
    }
    
    // Could not compute/retrieve payload size...
    return 0;
}

// ==========================================================================
void GEVPlayerDlg::OnDisplayDisabled()
{
    mDisplayThread->SetTargetFPS( 0 );
}

// ==========================================================================
void GEVPlayerDlg::OnDisplay30FPS()
{
    mDisplayThread->SetTargetFPS( 30 );
}

// ==========================================================================
void GEVPlayerDlg::OnDisplay60FPS()
{
    mDisplayThread->SetTargetFPS( 60 );
}

// =============================================================================
CString GEVPlayerDlg::GetStickyPath()
{
    CString lLocation;
    TCHAR lAppData[MAX_PATH];
    if ( SHGetSpecialFolderPath(      
        GetSafeHwnd(),
        lAppData,
        CSIDL_APPDATA,
        TRUE ) == TRUE )
    {
        lLocation += lAppData;
    }
    lLocation += __T( "\\Pleora" );
    ::CreateDirectory( lLocation, NULL );
    lLocation += _T( "\\GEVPlayer.pvcfg" );
    return lLocation;
}

// =============================================================================
CString GEVPlayerDlg::GetDefaultPath()
{
    CString lLocation;
    TCHAR lAppData[MAX_PATH];
    if ( SHGetSpecialFolderPath(      
        GetSafeHwnd(),
        lAppData,
        CSIDL_APPDATA,
        TRUE ) == TRUE )
    {
        lLocation += lAppData;
    }
    lLocation += __T( "\\Pleora" );
    ::CreateDirectory( lLocation, NULL );
    lLocation += _T( "\\GEVPlayerDefaults.pvcfg" );
    return lLocation;
}

// =============================================================================
void GEVPlayerDlg::OnToolsSavepreferences()
{
    SaveConfig( GetStickyPath(), false );
}

// =============================================================================
void GEVPlayerDlg::SaveConfig( CString aLocation, bool aSaveConnectedDevice )
{
    PvConfigurationWriter lWriter;
    lWriter.Store( "1", "gevplayerconfig" ); 

    SaveDisplayOptions( lWriter );

    mImageSaveDlg->Save( lWriter );
    mSetupDlg.Save( lWriter );
    mFilteringDlg.Save( lWriter );
    mLogBuffer->Save( lWriter );

    // save visibility settings for each PvGenBrowserWnd
    PvPropertyList lPropertyList;
    mCommunicationWnd.Save( lPropertyList );
    lWriter.Store( &lPropertyList, TAG_COMMUNICATIONBROWSER );
    mDeviceWnd.Save( lPropertyList );
    lWriter.Store( &lPropertyList, TAG_DEVICEBROWSER );
    mStreamParametersWnd.Save( lPropertyList );
    lWriter.Store( &lPropertyList, TAG_STREAMBROWSER );

    PvUInt32 lCount = mPipeline.GetBufferCount();
    if ( lCount < BUFFER_COUNT_MIN )
    {
        lCount = BUFFER_COUNT_DEFAULT;
    }

	BufferOptionsDlg::Save( lWriter, &mPipeline );

    if ( aSaveConnectedDevice )
    {
        if ( mSourceCombo.GetCurSel() >= 0 )
        {
            DWORD_PTR lSource = mSourceCombo.GetItemData( mSourceCombo.GetCurSel() );

            CString lSourceStr;
            lSourceStr.Format( _T( "%d" ), lSource );

            lWriter.Store( (LPCTSTR)lSourceStr, TAG_ACTIVESOURCE );
        }

        if ( mEventMonitorDlg != NULL )
        {
		    mEventMonitorDlg->EnableGenICamMonitoring( false );
        }

        PvStringList lErrorList;
        SavingThread lSavingThread( &mSetupDlg, &lWriter, &mDevice, &mStream, mSerialBridgeManagerWnd, &lErrorList, this );
        lSavingThread.Save();

        if ( mEventMonitorDlg != NULL )
        {
            mEventMonitorDlg->EnableGenICamMonitoring( true );
        }

        if ( lErrorList.GetSize() > 0 )
        {
            ErrorsDlg lDlg;
            lDlg.DoModal( &lErrorList );
        }
    }
    else
    {
        PvResult lResult = lWriter.Store( mDevice.GetGenLink(), "communication" );
        if ( !lResult.IsOK() )
        {
            MessageBox( _T( "Failed to store the communication parameters. This may indicate that the GenAPI parameters failed to initialize properly.") );
        }
    }

    lWriter.Save( aLocation.GetBuffer() );
}


// =============================================================================
void GEVPlayerDlg::OnToolsRestoredefaultpreferences()
{
    // load the default file
    OpenConfig( GetDefaultPath() );

    // save the default settings so they are persisted next time
    SaveConfig( GetStickyPath(), false );
}


// =============================================================================
// Saves display configuration to file
//
void GEVPlayerDlg::SaveDisplayOptions( PvConfigurationWriter& aWriter )
{
    // Fill property list with display options
    PvPropertyList lList;
    mDisplayThread->Save( lList );

    // Save property list
    aWriter.Store( &lList, TAG_DISPLAYOPTIONS );
}


// =============================================================================
// Retrieves display configuration from file
//
void GEVPlayerDlg::LoadDisplayOptions( PvConfigurationReader& aReader )
{
    // Load display options into property list
    PvPropertyList lList;
    if ( aReader.Restore( TAG_DISPLAYOPTIONS, lList ).IsOK() )
    {
        // Load property list into display thread
        mDisplayThread->Load( lList );
    }
}


// =============================================================================
void GEVPlayerDlg::OnToolsBufferoptions()
{
    BufferOptionsDlg lBufferOptions( this, &mPipeline );
	lBufferOptions.DoModal();
}


// =============================================================================
void GEVPlayerDlg::OnToolsDisplaypartialimages()
{
    mDisplayThread->SetKeepPartialImagesEnabled( !mDisplayThread->GetKeepPartialImagesEnabled() );
}


// =============================================================================
void GEVPlayerDlg::OnToolsDisplaychunkdata()
{
    mDisplayThread->SetDisplayChunkDataEnabled( !mDisplayThread->GetDisplayChunkDataEnabled() );
}

// =============================================================================
void GEVPlayerDlg::OnToolsSaveXml()
{
	if ( !mDeviceConnected )
	{
		return;
	}

	PvString lFilename;
	if ( !mDevice.GetDefaultGenICamXMLFilename( lFilename ).IsOK() )
	{
		lFilename = "Default.xml";
	}

	CFileDialog lFileDlg( FALSE, _T( ".xml" ), lFilename.GetUnicode(), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, 
		_T( "GenICam XML Files (*.xml)|*.xml|All files (*.*)|*.*||" ), this );
    lFileDlg.m_ofn.lpstrTitle = _T( "Save Device XML" );

    if ( lFileDlg.DoModal() == IDOK )
    {
        mDevice.DumpGenICamXML( lFileDlg.GetPathName().GetBuffer() );
    }
   
}


// =============================================================================
void GEVPlayerDlg::AddDeviceParameterArray( PvGenParameterArray *aArray, const PvString &aName )
{
    if ( mDeviceWnd.GetHandle() != 0 )
    {
        mDeviceWnd.AddGenParameterArray( aArray, aName );
    }
}


// =============================================================================
void GEVPlayerDlg::RemoveDeviceParameterArray( PvGenParameterArray *aArray )
{
    if ( mDeviceWnd.GetHandle() != 0 )
    {
        for ( PvUInt32 i = 0; i < mDeviceWnd.GetParameterArrayCount(); i++ )
        {
            if ( mDeviceWnd.GetGenParameterArray( i ) == aArray )
            {
                mDeviceWnd.RemoveGenParameterArray( i );
                break;
            }
        }
    }
}


// =============================================================================
void GEVPlayerDlg::OnCbnSelchangeComboSource()
{
    SendMessage( WM_UPDATESOURCE );
}


// =============================================================================
void GEVPlayerDlg::OnAcquisitionStateChanged( PvDevice* aDevice, PvStreamBase* aStream, PvUInt32 aSource, PvAcquisitionState aState )
{
	SendMessageIfPossible( WM_ACQUISITIONSTATECHANGED );
}


// =============================================================================
LRESULT GEVPlayerDlg::OnAcquisitionStateChanged( WPARAM wParam, LPARAM lParam )
{
    // In case the event has been received after the object has been released
    if ( mAcquisitionStateManager != NULL )
    {
        EnableControls( mDeviceConnected );
    }

    return 0;
}


// =============================================================================
DWORD GEVPlayerDlg::ReadRegistryDWORD( const CString &aName )
{
    HKEY hKey = 0;
    DWORD lType = REG_DWORD;
    DWORD lSize = 4;
    DWORD lValue = 0;
    LONG lWinResult = RegOpenKeyEx( HKEY_LOCAL_MACHINE, _T( "SOFTWARE\\Pleora Technologies Inc\\GEVPlayer" ), 0L,  KEY_READ, &hKey );
    if ( lWinResult == ERROR_SUCCESS )
    {
        lWinResult = RegQueryValueEx( hKey, aName, NULL, &lType, (LPBYTE)&lValue, &lSize );
        if ( lWinResult == ERROR_SUCCESS )
        {
            return lValue;
        }

        ::CloseHandle( hKey );
    }

    return 0;
}


// =============================================================================
void GEVPlayerDlg::OnToolsDeinterlacingDisabled()
{
    mDisplayThread->SetDeinterlacing( PvDeinterlacingDisabled );
}


// =============================================================================
void GEVPlayerDlg::OnToolsDeinterlacingWeaving()
{
    mDisplayThread->SetDeinterlacing( PvDeinterlacingWeaving );
}


// =============================================================================
LRESULT GEVPlayerDlg::OnUpdateSource( WPARAM wParam, LPARAM lParam )
{
    // Since we get here through the message queue, things may have happened since
    // the message was posted
    if ( !mDevice.IsConnected() )
    {
        return 0;
    }

    PvGenEnum *lSourceSelector = mDevice.GetGenParameters()->GetEnum( "SourceSelector" );
    ASSERT( lSourceSelector != NULL );

    ASSERT( mAcquisitionStateManager != NULL );

    PvUInt32 lOldSource = mAcquisitionStateManager->GetSource();
    DWORD_PTR lNewSource = mSourceCombo.GetItemData( mSourceCombo.GetCurSel() );

    // If no source change, return
    if ( lOldSource == lNewSource )
    {
        return 0;
    }

    // Reset current destination
    if ( mDevice.IsConnected() )
    {
        PvInt64 lOldChannel = 0;
        mDevice.ResetStreamDestination( (PvUInt32)lOldChannel );
    }

    // Obtain the local IP address and device IP address
    PvString lLocalIPAddress = mStream.GetLocalIPAddress();
    PvGenInteger *lGenDeviceIPAddress = mStream.GetParameters()->GetInteger( "DeviceIPAddress" );
    ASSERT( lGenDeviceIPAddress != NULL );

    PvInt64 lValue = 0;
    lGenDeviceIPAddress->GetValue( lValue );

    CString lDeviceIPAddress;
    lDeviceIPAddress.Format( _T( "%I64i.%I64i.%I64i.%I64i" ),
        ( lValue & 0xFF000000 ) >> 24,
        ( lValue & 0x00FF0000 ) >> 16,
        ( lValue & 0x0000FF00 ) >> 8,
        ( lValue & 0x000000FF ) );

    // Stop streaming
    StopAcquisition();
    if ( mStream.IsOpen() )
    {
        StopStreaming();
        mStream.Close();
    }

    // Reset acquisition state manager
    delete mAcquisitionStateManager;
    mAcquisitionStateManager = NULL;

    // Clear the display
    mDisplay.Clear();

    // Select the new input
    PvResult lResult = lSourceSelector->SetValue( lNewSource );
    if ( !lResult.IsOK() )
    {
        PvMessageBox( this, lResult );
    }

    // Spawn connection thread to open new stream, set streaming destination
    ConnectionThread lConnectionThread( &mSetupDlg, NULL, &mDevice, &mStream, this, lLocalIPAddress, (LPCTSTR)lDeviceIPAddress );
    lConnectionThread.SetTitle( _T( "Changing Source" ) );
    lResult = lConnectionThread.Connect();
    if ( !lResult.IsOK() )
    {
        PvMessageBox( this, lResult );
        Disconnect();
        return 0;
    }

    // Regardless of whether a corresponding stream is open or not, we can still pass it to the acquisition state manager
    ASSERT( mAcquisitionStateManager == NULL );
    mAcquisitionStateManager = new PvAcquisitionStateManager( &mDevice, &mStream, static_cast<PvUInt32>( lNewSource ) );
    mAcquisitionStateManager->RegisterEventSink( this );

    if ( mStream.IsOpen() )
    {
        // Ready image reception
        StartStreaming();
    }

    EnableControls( TRUE );

    // Make sure we update the acquisition mode combo box
    SendMessage( WM_UPDATEACQUISITIONMODE );
	
    return 0;
}


// =============================================================================
LRESULT GEVPlayerDlg::OnUpdateSources( WPARAM wParam, LPARAM lParam )
{
    if ( mAcquisitionStateManager == NULL )
    {
        // Not ready yet...
        mUpdatingSources = FALSE;
        return 0;
    }

    PvGenEnum *lSourceSelector = mDevice.GetGenParameters()->GetEnum( "SourceSelector" );
    if ( lSourceSelector == NULL )
    {
        mAvailableSources.clear();
        mSourceCombo.ResetContent();
        mUpdatingSources = FALSE;
        return 0;
    }

    PvInt64 lSourceCount = 0;
    lSourceSelector->GetEntriesCount( lSourceCount );

    PvUInt32 lActiveSource = mAcquisitionStateManager->GetSource();

    // Fill source selector combo box
    mAvailableSources.clear();
    mSourceCombo.ResetContent();
    for ( PvInt64 i = 0; i < lSourceCount; i++ )
    {
        const PvGenEnumEntry *lEE = NULL;
        lSourceSelector->GetEntryByIndex( i, &lEE );
        
        if ( lEE->IsAvailable() )
        {
            PvString lDisplayName;
            lEE->GetDisplayName( lDisplayName );

            PvInt64 lValue = 0;
            lEE->GetValue( lValue );

            int lIndex = mSourceCombo.AddString( lDisplayName.GetUnicode() );
            mSourceCombo.SetItemData( lIndex, static_cast<DWORD_PTR>( lValue ) );
            mAvailableSources.push_back( lValue );

            // Same as active source, select
            if ( lActiveSource == static_cast<PvUInt32>( lValue ) )
            {
                mSourceCombo.SetCurSel( lIndex );
            }
        }
    }

    // If we have a preferred selection, attempt ot find, select it
    if ( ( lParam >= 0 ) && ( mSourceCombo.GetCount() > 0 ) )
    {
        for ( int i = 0; i < mSourceCombo.GetCount(); i++ )
        {
            if ( mSourceCombo.GetItemData( i ) == static_cast<DWORD_PTR>( lParam ) )
            {
                mSourceCombo.SetCurSel( i );
                PostMessage( WM_UPDATESOURCE );
            }
        }
    }

    // If no selelection, select first available
    if ( ( mSourceCombo.GetCount() > 0 ) && ( mSourceCombo.GetCurSel() < 0 ) )
    {
        mSourceCombo.SetCurSel( 0 );
        PostMessage( WM_UPDATESOURCE );
    }

    mUpdatingSources = FALSE;

    return 0;
}


// =============================================================================
// wParam is a flag that when set to one resets the BOOL used to prevent
// reentry with the GenApi OnParameterUpdate.
//
// When not called from GenApi OnParameterUpdate (Send or PostMessage) the 
// flag is left alone (not reset) in case there is a OnParameterUpdate message 
// already in the message queue that would depend on it being set.
//
LRESULT GEVPlayerDlg::OnUpdateAcquisitionMode( WPARAM wParam, LPARAM lParam )
{
    if ( mAcquisitionStateManager == NULL )
    {
        // Not ready yet...
        if ( wParam == 1 )
        {
            mUpdatingAcquisitionMode = FALSE;
        }
        return 0;
    }

    PvGenEnum *lEnum = mDevice.GetGenParameters()->GetEnum( "AcquisitionMode" );
    if ( lEnum != NULL )
    {
        PvGenStateStack lStack( mDevice.GetGenParameters() );
        PushSource( &lStack );

        // Set mode combo box to value currently used by the device
        PvInt64 lValue = 0;
        lEnum->GetValue( lValue );
        for ( int i = 0; i < mModeCombo.GetCount(); i++ )
        {
            if ( lValue == mModeCombo.GetItemData( i ) )
            {
                if ( mModeCombo.GetCurSel() != i )
                {
                    mModeCombo.SetCurSel( i );
                }
                break;
            }
        }

        PvAcquisitionState lAcquisitionState = mAcquisitionStateManager->GetState();

        mModeCombo.EnableWindow( 
            lEnum->IsWritable() && 
            ( lAcquisitionState != PvAcquisitionStateLocked ) );
    }

    if ( wParam == 1 )
    {
        mUpdatingAcquisitionMode = FALSE;
    }

    return 0;
}


// =============================================================================
bool GEVPlayerDlg::IsMultiSourceTransmitter( PvDevice *aDevice )
{
	if ( ( aDevice != NULL ) && aDevice->IsConnected() )
	{
		PvGenEnum *lEnum = aDevice->GetGenParameters()->GetEnum( "SourceSelector" );
		PvGenEnum *lDeviceClass = aDevice->GetGenParameters()->GetEnum( "GevDeviceClass" );

		if ( ( lEnum != NULL ) && ( lDeviceClass != NULL ) )
		{
			PvString lClass;
			if ( lDeviceClass->GetValue( lClass ).IsOK() )
			{
				return ( lClass == "Transmitter" );
			}
		}
	}

	return false;
}


// =============================================================================
void GEVPlayerDlg::OnBufferTooSmall( PvPipeline *aPipeline, bool *aReallocAll, bool *aResetStats )
{
	*aReallocAll = true;
	*aResetStats = true;

	// Save event for status warnings
	mPipelineReallocated = true;
}


// =============================================================================
void GEVPlayerDlg::PushSource( PvGenStateStack *aStack )
{
    // If multi source, push the active source on the stack
    if ( IsMultiSourceTransmitter( &mDevice ) )
    {
        if ( mAcquisitionStateManager != NULL )
        {
            // Select source (will be reset when lStack goes out of scope)
            aStack->SetEnumValue( "SourceSelector", mAcquisitionStateManager->GetSource() );
        }
    }
}


// =============================================================================
LRESULT GEVPlayerDlg::SendMessageIfPossible( UINT Msg, WPARAM wParam, LPARAM lParam )
{
    if ( AfxGetApp()->m_nThreadID == ::GetCurrentThreadId() )
    {
        // Same thread, use send (ends up being a direct call)
        return SendMessage( Msg, wParam, lParam );
    }

    
    // Different thread, we want to use post to prevent deadlocks
    return PostMessage( Msg, wParam, lParam );
}



