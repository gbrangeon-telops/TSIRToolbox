// *****************************************************************************
//
//     Copyright (c) 2010, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#include "stdafx.h"

#include "PvCamHeadSerialComLogSample.h"
#include "PvCamHeadSerialComLogSampleDlg.h"

#include <string.h>
#include <vector>


#define WM_ONUPDATESERIALCOMLOG ( WM_USER + 0x7661 )
#define MAX_LOG_SIZE ( 256 * 1024 )
#define TERMINAL_COLUMNS ( 8 )

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


BEGIN_MESSAGE_MAP(PvCamHeadSerialComLogSampleDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_SIZE()
	ON_WM_GETMINMAXINFO()
	ON_BN_CLICKED(IDC_DEVICE_BUTTON, &PvCamHeadSerialComLogSampleDlg::OnBnClickedDeviceButton)
	ON_BN_CLICKED(IDC_LINK_BUTTON, &PvCamHeadSerialComLogSampleDlg::OnBnClickedLinkButton)
	ON_BN_CLICKED(IDC_STREAMPARAMS_BUTTON, &PvCamHeadSerialComLogSampleDlg::OnBnClickedStreamparamsButton)
	ON_BN_CLICKED(IDC_CONNECT_BUTTON, &PvCamHeadSerialComLogSampleDlg::OnBnClickedConnectButton)
	ON_BN_CLICKED(IDC_DISCONNECT_BUTTON, &PvCamHeadSerialComLogSampleDlg::OnBnClickedDisconnectButton)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_START, &PvCamHeadSerialComLogSampleDlg::OnBnClickedStart)
	ON_BN_CLICKED(IDC_STOP, &PvCamHeadSerialComLogSampleDlg::OnBnClickedStop)
	ON_CBN_SELCHANGE(IDC_MODE, &PvCamHeadSerialComLogSampleDlg::OnCbnSelchangeMode)
	ON_WM_MOVE()
    ON_WM_CTLCOLOR()
    ON_MESSAGE(WM_ONUPDATESERIALCOMLOG, OnUpdateSerialComLog)
END_MESSAGE_MAP()


// =============================================================================
PvCamHeadSerialComLogSampleDlg::PvCamHeadSerialComLogSampleDlg( CWnd* pParent /* =NULL */ )
	: CDialog( PvCamHeadSerialComLogSampleDlg::IDD, pParent )
	, mNeedInit( TRUE )
	, mDeviceWnd( NULL )
	, mCommunicationWnd( NULL )
	, mStreamParametersWnd( NULL )
	, mThreadDisplay( NULL )
	, mPipeline( &mStream )
    , mAcquiringImages( false )
    , mLogEditTextLength( 0 )
{
	m_hIcon = AfxGetApp()->LoadIcon( IDR_MAINFRAME );
}


// =============================================================================
PvCamHeadSerialComLogSampleDlg::~PvCamHeadSerialComLogSampleDlg()
{
}


// =============================================================================
void PvCamHeadSerialComLogSampleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MODE, mModeCombo);
	DDX_Control(pDX, IDC_CHANNEL, mChannelCombo);
	DDX_Control(pDX, IDC_START, mPlayButton);
	DDX_Control(pDX, IDC_STOP, mStopButton);
	DDX_Control(pDX, IDC_IP_EDIT, mIPEdit);
	DDX_Control(pDX, IDC_MAC_EDIT, mMACEdit);
	DDX_Control(pDX, IDC_MODEL_EDIT, mModelEdit);
	DDX_Control(pDX, IDC_MANUFACTURER_EDIT, mManufacturerEdit);
	DDX_Control(pDX, IDC_NAME_EDIT, mNameEdit);
    DDX_Control(pDX, IDC_SERIALCOMLOG_EDIT, mSerialComLogEdit);
}


// =============================================================================
BOOL PvCamHeadSerialComLogSampleDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

    if ( mTerminalFont.GetSafeHandle() == 0 )
    {
        mTerminalFont.CreateFont( 14, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, _T( "Courier New" ) );
    }

    mSerialComLogEdit.SetFont( &mTerminalFont );

    SetWindowText( _T( "PvCamHeadSerialComLogSample" ) );

	GetClientRect( mCrt );
	mNeedInit = FALSE;

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// Right now we support a single, static channel
	mChannelCombo.ResetContent();
	mChannelCombo.AddString( _T( "Data Channel #0" ) );
	mChannelCombo.SetCurSel( 0 );

	CRect lDisplayRect;
	GetDlgItem( IDC_DISPLAYPOS )->GetClientRect( &lDisplayRect );
	GetDlgItem( IDC_DISPLAYPOS )->ClientToScreen( &lDisplayRect );
	ScreenToClient( &lDisplayRect );
    mDisplay.Create( GetSafeHwnd(), 10000 );
	mDisplay.SetPosition( lDisplayRect.left, lDisplayRect.top, lDisplayRect.Width(), lDisplayRect.Height() );
	mDisplay.SetBackgroundColor( 0x80, 0x80, 0x80 );
    
	EnableInterface();

    return TRUE;  // return TRUE  unless you set the focus to a control
}


// =============================================================================
void PvCamHeadSerialComLogSampleDlg::OnPaint()
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
HCURSOR PvCamHeadSerialComLogSampleDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


// =============================================================================
void PvCamHeadSerialComLogSampleDlg::OnClose()
{
	// Make sure we cleanup before we leave
	Disconnect();

	CDialog::OnClose();
}


// =============================================================================
void PvCamHeadSerialComLogSampleDlg::OnBnClickedConnectButton()
{
	// create a device finder wnd and open the select device dialog
	PvDeviceFinderWnd lFinder;
	lFinder.SetTitle( _T( "GigE Vision Device Selection" ) );

    PvResult lResult = lFinder.ShowModal();
    if ( !lResult.IsOK() || ( lFinder.GetSelected() == NULL ) )
	{
		return;
	}

	CWaitCursor lCursor;

	UpdateWindow();

	Connect( lFinder.GetSelected() );
}


// =============================================================================
void PvCamHeadSerialComLogSampleDlg::OnBnClickedDisconnectButton()
{
	CWaitCursor lCursor;

	Disconnect();
}


// =============================================================================
void PvCamHeadSerialComLogSampleDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	CRect lR;
	this->GetWindowRect( &lR );

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

    // Last, make sure the serial com log follows dx in width, moves up/down with dy
    mSerialComLogEdit.GetWindowRect( &r1 ); ScreenToClient( &r1 ); 
    ::DeferWindowPos( hDWP, mSerialComLogEdit.GetSafeHwnd(), NULL,
        r1.left, r1.top + dy, r1.Width() + dx, r1.Height(), 
        SWP_NOACTIVATE | SWP_NOZORDER );

	::EndDeferWindowPos( hDWP );
}


// =============================================================================
void PvCamHeadSerialComLogSampleDlg::OnGetMinMaxInfo( MINMAXINFO *lpMMI )
{
	lpMMI->ptMinTrackSize.x = 700;
	lpMMI->ptMinTrackSize.y = 520;
}


// =============================================================================
void PvCamHeadSerialComLogSampleDlg::EnableInterface()
{
	// This method can be called really early or late when the window is not created
	if ( GetSafeHwnd() == 0 )
	{
		return;
	}

	bool lConnected = mDevice.IsConnected();

	GetDlgItem( IDC_CONNECT_BUTTON )->EnableWindow( !lConnected );
	GetDlgItem( IDC_DISCONNECT_BUTTON )->EnableWindow( lConnected );

	GetDlgItem( IDC_DEVICE_BUTTON )->EnableWindow( lConnected );
	GetDlgItem( IDC_STREAMPARAMS_BUTTON )->EnableWindow( lConnected );

	// Special case, we want to allow setting communication paramters before we connect
	GetDlgItem( IDC_LINK_BUTTON )->EnableWindow( TRUE  ); 

	mPlayButton.EnableWindow( lConnected );

	// If not connected, disable the acquisition mode control. If enabled,
	// it will be managed automatically by events from the GenICam parameters
	if ( !lConnected )
	{
        mModeCombo.EnableWindow( FALSE );
		mStopButton.EnableWindow( FALSE );
	}
}


// =============================================================================
void PvCamHeadSerialComLogSampleDlg::Connect( PvDeviceInfo *aDI )
{
	ASSERT( aDI != NULL );
	if ( aDI == NULL )	
    {
		return;
	}
	
	// Just in case we came here still connected...
	Disconnect();

    // Device connection, packet size negociation and stream opening
	PvResult lResult = PvResult::Code::NOT_CONNECTED;

    // Connect device
	lResult = mDevice.Connect( aDI, PvAccessControl );
    if ( !lResult.IsOK() )
    {
		Disconnect();
        return;
    }

    // Perform automatic packet size negociation
    lResult = mDevice.NegotiatePacketSize( );
    if ( !lResult.IsOK() )
    {
        ::Sleep( 2500 );
    }

    // Open stream - and retry if it fails
	for ( ;; )
	{
        // Open stream
        lResult = mStream.Open( aDI->GetIPAddress() );
		if ( lResult.IsOK() )
		{
			break;
		}

		::Sleep( 1000 );
	}

    // Now that the stream is opened, set the destination on the device
    lResult = mDevice.SetStreamDestination( mStream.GetLocalIPAddress(), mStream.GetLocalPort() );
	if ( !lResult.IsOK() )
	{
		Disconnect();
		return;
	}

    // Register to all events of the parameters in the device's node map
    PvGenParameterArray *lGenDevice = mDevice.GetGenParameters();
    for ( PvUInt32 i = 0; i < lGenDevice->GetCount(); i++ )
    {
	    lGenDevice->Get( i )->RegisterEventSink( this );
    }

	PvGenInteger *lIPAddressParam = dynamic_cast<PvGenInteger *>( lGenDevice->Get( "GevCurrentIPAddress" ) );
	PvGenInteger *lMACAddressParam = dynamic_cast<PvGenInteger *>( lGenDevice->Get( "GevMACAddress" ) );
	PvGenString *lManufacturer = dynamic_cast<PvGenString *>( lGenDevice->Get( "DeviceVendorName" ) );
	PvGenString *lModelName = dynamic_cast<PvGenString *>( lGenDevice->Get( "DeviceModelName" ) );
	PvGenString *lDeviceVersion = dynamic_cast<PvGenString *>( lGenDevice->Get( "DeviceVersion" ) );
	PvGenString *lNameParam = dynamic_cast<PvGenString *>( lGenDevice->Get( "GevDeviceUserID " ) );

	// Verify all params - except name which is optional
	ASSERT( lIPAddressParam != NULL );
	ASSERT( lMACAddressParam != NULL );
	ASSERT( lManufacturer != NULL );
	ASSERT( lModelName != NULL );

	PvString lManufacturerStr, lModelNameStr, lDeviceVersionStr = "N/A";
	lManufacturer->GetValue( lManufacturerStr );
	lModelName->GetValue( lModelNameStr );

    if ( lDeviceVersion != NULL )
    {
	    lDeviceVersion->GetValue( lDeviceVersionStr );
    }

	// IP
	CString lIPStr;
	PvInt64 lIPAddress = 0;
	lIPAddressParam->GetValue( lIPAddress );
	unsigned char *lIPPtr = reinterpret_cast<unsigned char *>( &lIPAddress );
	lIPStr.Format( _T( "%i.%i.%i.%i" ), 
		lIPPtr[3], lIPPtr[2], lIPPtr[1], lIPPtr[0] );

	// MAC address
	CString lMACStr;
	PvInt64 lMACAddress;
	lMACAddressParam->GetValue( lMACAddress );
	unsigned char *lMACPtr = reinterpret_cast<unsigned char *>( &lMACAddress );
	lMACStr.Format( _T( "%02X:%02X:%02X:%02X:%02X:%02X" ), 
		lMACPtr[5], lMACPtr[4], lMACPtr[3], lMACPtr[2], lMACPtr[1], lMACPtr[0] );

	// Device name (User ID)
	CString lNameStr( "N/A" );
	if ( lNameParam != NULL )
	{
		PvString lStr;
		lNameParam->GetValue( lStr );
		lNameStr = lStr.GetUnicode();
	}

	mManufacturerEdit.SetWindowText( lManufacturerStr );
	mModelEdit.SetWindowText( lModelNameStr.GetUnicode() );
	mIPEdit.SetWindowText( lIPStr );
	mMACEdit.SetWindowText( lMACStr );
	mNameEdit.SetWindowText( lNameStr );

	// Fill acquisition mode combo box
	mModeCombo.ResetContent();
	PvGenEnum *lMode = dynamic_cast<PvGenEnum *>( mDevice.GetGenParameters()->Get( "AcquisitionMode" ) );
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

	// Set mode combo box to value currently used by the device
	PvInt64 lValue = 0;
	lMode->GetValue( lValue );
	for ( int i = 0; i < mModeCombo.GetCount(); i++ )
	{
		if ( lValue == mModeCombo.GetItemData( i ) )
		{
			mModeCombo.SetCurSel( i );
			break;
		}
	}

	PvGenCommand *lStart = dynamic_cast<PvGenCommand *>( mDevice.GetGenParameters()->Get( "AcquisitionStart" ) );
	ASSERT( lStart != NULL ); // Mandatory parameter

	PvGenCommand *lStop = dynamic_cast<PvGenCommand *>( mDevice.GetGenParameters()->Get( "AcquisitionStop" ) );
	ASSERT( lStop != NULL ); // Mandatory parameter

    mAcquiringImages = false;

    // Hookup messaging channel callback, enable events for serial com log
    EnableSerialComLog();
    mDevice.RegisterEventSink( this );

    // Force an update on all the parameters we have shortcuts to
    OnParameterUpdate( lMode );
    OnParameterUpdate( lStart );
    OnParameterUpdate( lStop );

	// Ready image reception
	StartStreaming();

	// Sync up UI
	EnableInterface();
}


// =============================================================================
void PvCamHeadSerialComLogSampleDlg::Disconnect()
{
	// If streaming, stop streaming
	StopStreaming();

    // Unregister all events of the parameters in the device's node map
    PvGenParameterArray *lGenDevice = mDevice.GetGenParameters();
    for ( PvUInt32 i = 0; i < lGenDevice->GetCount(); i++ )
    {
	    lGenDevice->Get( i )->UnregisterEventSink( this );
    }

    // Disable serial com events, unregister callbacks
    EnableSerialComLog( false );
    mDevice.UnregisterEventSink( this );

    // Close all configuration child windows
	CloseGenWindow( &mDeviceWnd );
	CloseGenWindow( &mCommunicationWnd );
	CloseGenWindow( &mStreamParametersWnd );

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

	mDevice.Disconnect();
	mStream.Close();
	
	EnableInterface();
}


// =============================================================================
void PvCamHeadSerialComLogSampleDlg::EnableSerialComLog( bool aEnabled /* = true */ )
{
    if ( !mDevice.IsConnected() )
    {
        return;
    }

    // Get required parameters
    PvGenEnum *lEventSelector = dynamic_cast<PvGenEnum *>( mDevice.GetGenParameters()->Get( "EventSelector" ) );
    PvGenEnum *lEventNotification = dynamic_cast<PvGenEnum *>( mDevice.GetGenParameters()->Get( "EventNotification" ) );
    if ( ( lEventSelector == NULL ) || ( lEventNotification == NULL ) )
    {
		MessageBox( _T( "Unable to retrieve GenICam parameters required to enable serial com log." ), _T( "Error" ), 
			MB_OK | MB_ICONINFORMATION );
    }

    // Select IPEngineCamHeadSerialComLog
    PvResult lResult = lEventSelector->SetValue( "IPEngineCamHeadSerialComLog" );
    if ( !lResult.IsOK() )
    {
		MessageBox( _T( "Error selecting IPEngineCamHeadSerialComLog" ), _T( "Error" ), 
			MB_OK | MB_ICONINFORMATION );
    }

    // Enable event
    lResult = lEventNotification->SetValue( aEnabled ? "GigEVisionEvent" : "Off" );
    if ( !lResult.IsOK() )
    {
        lResult = lEventNotification->SetValue( aEnabled ? "On" : "Off" );
        if ( !lResult.IsOK() )
        {
		    MessageBox( _T( "Unable to set event notification to GigEVisionEvent" ), _T( "Error" ), 
			    MB_OK | MB_ICONINFORMATION );
        }
    }
}


// =============================================================================
void PvCamHeadSerialComLogSampleDlg::OnBnClickedDeviceButton()
{
	if ( !mDevice.IsConnected() )
	{
		return;
	}

	ShowGenWindow( 
		&mDeviceWnd, 
		mDevice.GetGenParameters(), 
		_T( "GEV Device Control" ) );
}


// =============================================================================
void PvCamHeadSerialComLogSampleDlg::OnBnClickedLinkButton()
{
	ShowGenWindow( 
		&mCommunicationWnd, 
		mDevice.GetGenLink(), 
		_T( "Communication Control" ) );
}


// =============================================================================
void PvCamHeadSerialComLogSampleDlg::OnBnClickedStreamparamsButton()
{
	if ( !mDevice.IsConnected() )
	{
		return;
	}
	
	ShowGenWindow( 
		& mStreamParametersWnd, 
		mStream.GetParameters(), 
		_T( "Image Stream Control" ) );
}


// =============================================================================
void PvCamHeadSerialComLogSampleDlg::ShowGenWindow( PvGenBrowserWnd **aWnd, PvGenParameterArray *aParams, const CString &aTitle )
{
	if ( ( *aWnd ) != NULL )
	{
		if ( ( *aWnd )->GetHandle() != 0 )
		{
			CWnd lWnd;
			lWnd.Attach( ( *aWnd )->GetHandle() );

			// Window already visible, give it focus and bring it on top
			lWnd.BringWindowToTop();
			lWnd.SetFocus();

			lWnd.Detach();
			return;
		}

		// Window object exists but was closed/destroyed. Free it before re-creating
		CloseGenWindow( aWnd );
	}

	// Create, assigne parameters, set title and show modeless
	( *aWnd ) = new PvGenBrowserWnd;
	( *aWnd )->SetTitle( PvString( aTitle ) );
	( *aWnd )->SetGenParameterArray( aParams );
	( *aWnd )->ShowModeless( GetSafeHwnd() );
}


// =============================================================================
void PvCamHeadSerialComLogSampleDlg::CloseGenWindow( PvGenBrowserWnd **aWnd )
{
	// If the window object does not even exist, do nothing
	if ( ( *aWnd ) == NULL )
	{
		return;
	}

	// If the window object exists and is currently created (visible), close/destroy it
	if ( ( *aWnd )->GetHandle() != 0 )
	{
		( *aWnd )->Close();
	}

	// Finally, release the window object
	delete ( *aWnd );
	( *aWnd ) = NULL;
}


// =============================================================================
void PvCamHeadSerialComLogSampleDlg::StartStreaming()
{
	// Create display thread
	mThreadDisplay = new Thread( this, &mDisplay, &mDevice, &mPipeline );

	// Start threads
	mThreadDisplay->Start();
	mThreadDisplay->SetPriority( THREAD_PRIORITY_ABOVE_NORMAL );

    // Start pipeline
    mPipeline.Start();
}


// =============================================================================
void PvCamHeadSerialComLogSampleDlg::StopStreaming()
{
    // Stop display thread
	if ( mThreadDisplay != NULL )
	{
		mThreadDisplay->Stop();

		delete mThreadDisplay;
		mThreadDisplay = NULL;
	}

	// Stop stream thread
    if ( mPipeline.IsStarted() )
    {
		mPipeline.Stop();
    }
}

// =============================================================================
void PvCamHeadSerialComLogSampleDlg::StartAcquisition()
{
	PvGenEnum *lMode = dynamic_cast<PvGenEnum *>( mDevice.GetGenParameters()->Get( "AcquisitionMode" ) );
	PvGenCommand *lStart = dynamic_cast<PvGenCommand *>( mDevice.GetGenParameters()->Get( "AcquisitionStart" ) );
	PvGenInteger *lTLParamsLocked = dynamic_cast<PvGenInteger *>( mDevice.GetGenParameters()->Get( "TLParamsLocked" ) );
	PvGenInteger *lPayloadSize = dynamic_cast<PvGenInteger *>( mDevice.GetGenParameters()->Get( "PayloadSize" ) );
	PvGenCommand *lResetStats = dynamic_cast<PvGenCommand *>( mStream.GetParameters()->Get( "Reset" ) );

    // Try reading payload size from device
    PvInt64 lPayloadSizeValue = 0;
    if ( lPayloadSize != NULL )
    {
        lPayloadSize->GetValue( lPayloadSizeValue );
    }

    // If payload size is valid, force buffers re-alloc - better than 
    // adjusting as images are comming in
    if ( lPayloadSizeValue > 0 )
    {
        mPipeline.SetBufferSize( static_cast<PvUInt32>( lPayloadSizeValue ) );
    }

    // Never hurts to start streaming on a fresh pipeline/stream...
    mPipeline.Reset();

    // Reset stream statistics
    lResetStats->Execute();

	PvResult lResult = PvResult::Code::NOT_INITIALIZED;

	PvString lStr;
	lMode->GetValue( lStr );
	CString lModeStr = lStr.GetUnicode();
	if ( lModeStr.Find( _T( "Continuous" ) ) >= 0 )
	{
		// We are streaming, lock the TL parameters
		if ( lTLParamsLocked != NULL ) 
		{
			lResult = lTLParamsLocked->SetValue( 1 );
		}

		lStart->Execute();
        mAcquiringImages = true;
	}
	else if ( ( lModeStr.Find( _T( "Multi" ) ) >= 0 ) || 
			  ( lModeStr.Find( _T( "Single" ) ) >= 0 ) )
	{
		// We are streaming, lock the TL parameters
		if ( lTLParamsLocked != NULL ) 
		{
			lTLParamsLocked->SetValue( 1 );
		}

		lResult = lStart->Execute();

		// We are done streaming, unlock the TL parameters
		if ( lTLParamsLocked != NULL ) 
		{
			lTLParamsLocked->SetValue( 0 );
		}
	}
}


// =============================================================================
void PvCamHeadSerialComLogSampleDlg::StopAcquisition()
{
	PvGenCommand *lStop = dynamic_cast<PvGenCommand *>( mDevice.GetGenParameters()->Get( "AcquisitionStop" ) );
	ASSERT( lStop != NULL ); // Mandatory parameter

	PvResult lResult = lStop->Execute();

    // TLParamsLocked - Optionnal
	PvGenInteger *lTLParamsLocked = dynamic_cast<PvGenInteger *>( mDevice.GetGenParameters()->Get( "TLParamsLocked" ) );
	if ( lTLParamsLocked != NULL )
	{
		lResult = lTLParamsLocked->SetValue( 0 );
	}
}


// =============================================================================
void PvCamHeadSerialComLogSampleDlg::OnBnClickedStart()
{
    if ( !mDevice.IsConnected() )
    {
        return;
    }

    StartAcquisition();

	EnableInterface();
}


// =============================================================================
void PvCamHeadSerialComLogSampleDlg::OnBnClickedStop()
{
    if ( !mDevice.IsConnected() )
    {
        return;
    }

    mAcquiringImages = false;

    StopAcquisition();

	EnableInterface();
}


// =============================================================================
void PvCamHeadSerialComLogSampleDlg::OnCbnSelchangeMode()
{
    if ( !mDevice.IsConnected() )
    {
        return;
    }

	if ( mModeCombo.GetCurSel() < 0 )
	{
		return;
	}

	PvGenEnum *lMode = dynamic_cast<PvGenEnum *>( mDevice.GetGenParameters()->Get( "AcquisitionMode" ) );
	ASSERT( lMode != NULL ); // Mandatory parameter

	PvUInt64 lValue = mModeCombo.GetItemData( mModeCombo.GetCurSel() );
	PvResult lResult = lMode->SetValue( lValue );
	if ( !lResult.IsOK() )
	{
		MessageBox( _T( "Unable to set AcquisitionMode value." ), _T( "Error" ), 
			MB_OK | MB_ICONINFORMATION );
	}
}


// =============================================================================
void PvCamHeadSerialComLogSampleDlg::OnParameterUpdate( PvGenParameter *aParameter )
{
	bool bBufferResize =false;
	PvString lName;
	if ( !aParameter->GetName( lName ).IsOK() )
	{
		ASSERT( 0 ); // Totally unexpected	
		return;
	}

	// If the AcquisitionStop node is updated, make sure to reflect its new state on the UI
	if ( ( lName == "AcquisitionStop" ) &&
		 ( mStopButton.GetSafeHwnd() != 0 ) )
	{
		bool lAvailable = false;
		if ( aParameter->IsAvailable( lAvailable ).IsOK() )
		{
			mStopButton.EnableWindow( lAvailable );
		}
	}

	if ( ( lName == "AcquisitionMode" ) &&
		 ( mModeCombo.GetSafeHwnd() != 0 ) )
	{
		bool lAvailable = false, lWritable = false;
		VERIFY( aParameter->IsAvailable( lAvailable ).IsOK() );
		if ( lAvailable )
		{
			VERIFY( aParameter->IsWritable( lWritable ).IsOK() );
		}

		mModeCombo.EnableWindow( lAvailable && lWritable );

		PvGenEnum *lEnum = dynamic_cast<PvGenEnum *>( aParameter );
		ASSERT( lEnum != NULL );

		if ( lEnum != NULL )
		{
			PvInt64 lEEValue = 0;
			VERIFY( lEnum->GetValue( lEEValue ) );		

			for ( int i = 0; i < mModeCombo.GetCount(); i++ )
			{
				DWORD_PTR lData = mModeCombo.GetItemData( i );
				if ( lData == lEEValue )
				{
					mModeCombo.SetCurSel( i );
					break;
				}
			}
		}
	}
}


// =============================================================================
void PvCamHeadSerialComLogSampleDlg::OnEvent( PvDevice *aDevice, 
    PvUInt16 aEventID, PvUInt16 aChannel, PvUInt16 aBlockID, PvUInt64 aTimestamp, 
    const void *aData, PvUInt32 aDataLength )
{
    if ( ( aEventID == 0x9009 ) && ( aData != NULL ) && ( aDataLength > 0 ) )
    {
        ProcessEvent( aTimestamp, reinterpret_cast<const unsigned char *>( aData ), aDataLength );
    }
}


// =============================================================================
void PvCamHeadSerialComLogSampleDlg::ProcessEvent( PvUInt64 aTimestamp, const unsigned char *aData, PvUInt32 aDataLength )
{
    const char *cPatterns[] = { "Tx: ", "Rx: " };

    CString lBuffer;
    lBuffer.Format( _T( "----- Timestamp: %016llX\r\n" ), aTimestamp );

    PvUInt32 lStart = 0;
    PvUInt32 lEnd = 0;

    for ( int i = 0; ; i++ )
    {
        // Alternate between patterns
        const char *lPattern = cPatterns[ i % 2 ];
        size_t lLength = strlen( lPattern );

        // Find Tx/Rx
        PvUInt32 lIndex = FindNext( aData + lStart, aDataLength - lStart, 
            reinterpret_cast<const unsigned char *>( lPattern ), static_cast<PvUInt32>( lLength ) );

        // Set end to end of data or new found tag
        lEnd = ( lIndex <= 0 ) ? aDataLength : ( lIndex + lStart ) - 1;

        // If we have a start, unpack
        if ( lStart > 0 )
        {
            // Unpack Rx
            const char *lPattern = cPatterns[ ( i + 1 ) % 2 ]; // Compensate, we're always late by one
            lBuffer += Unpack( lPattern, aData + lStart, lEnd - lStart );
        }

        // If nothing found, end of data
        if ( lIndex <= 0 )
        {
            break;
        }

        // Update start before goign for another round
        lStart = ( lIndex + lStart ) + lLength;
        lEnd = 0;
    }
    
    mLogBufferMutex.Lock();
    /////////////////////////////////////////////

    mLogBuffer += lBuffer;
    mLogBuffer += "\r\n";

    /////////////////////////////////////////////
    mLogBufferMutex.Unlock();

    PostMessage( WM_ONUPDATESERIALCOMLOG );
}


// =============================================================================
PvUInt32 PvCamHeadSerialComLogSampleDlg::FindNext( const unsigned char *aData, PvUInt32 aDataLength, 
    const unsigned char *aPattern, PvUInt32 aPatternLength )
{
    PvUInt32 j = 0;
    const unsigned char *lData = aData;
    for ( PvUInt32 i = 0; i < aDataLength; i++ )
    {
        if ( lData[ i ] == aPattern[ j ] )
        {
            j++;
            if ( j == aPatternLength )
            {
                return ( i - j + 1 );
            }
        }
        else
        {
            j = 0;
        }
    }

    return 0;
}


// =============================================================================
CString PvCamHeadSerialComLogSampleDlg::Unpack( const char *aPattern, const unsigned char *aData, PvUInt32 aDataLength )
{
    CString lBuffer;

    size_t lLength = strlen( aPattern );
    for ( size_t i = 0; i < lLength; i++ )
    {
        lBuffer += aPattern[ i ];
    }

    std::vector<unsigned char> lLine;
    PvUInt32 i = 0;
    for ( ;; ) // Skip NULLs
    {
        unsigned char lChar = aData[ i ];

        // Format, append hex value
        CString lHex;
        lHex.Format( _T( "%02X " ), lChar );
        lBuffer += lHex;

        // 0x20: chars below or equal to space, cannot be displayed
        // 0x7F: delete
        // 0xAF: extended ASCII, cannot be displayed
        if ( ( lChar <= 0x20 ) || ( lChar > 0x7F ) || ( lChar == 127 ) )
        {
            lChar = '.';
        }

        // Save character for latter
        lLine.push_back( lChar );

        // Next character, skipping NULL inter-char
        i += 2;

        // If end of terminal line reached - or end of data output readable chars
        if ( ( ( ( i / 2 ) % TERMINAL_COLUMNS ) == 0 ) || ( i >= aDataLength ) )
        {
            int lMissing = TERMINAL_COLUMNS - ( ( ( i - 2 ) / 2 ) % TERMINAL_COLUMNS );
            for ( int k = 0; k < lMissing + 1; k++ )
            {
                lBuffer += _T( "   " );
            }
            
            // Flush characters buffer
            std::vector<unsigned char>::iterator lIt = lLine.begin();
            while ( lIt != lLine.end() )
            {
                lBuffer += *lIt;
                lIt++;
            }
            lLine.clear();

            // Still data? If so, start new line
            if ( i < aDataLength )
            {
                // New line
                lBuffer += _T( "\r\n" );

                // Align data
                for ( size_t k = 0; k < lLength; k++ )
                {
                    lBuffer += _T( " " );
                }
            }
        }

        // Reached end of data?
        if ( i >= aDataLength )
        {
            break;
        }
    }

    lBuffer += _T( "\r\n" );

    return lBuffer;
}


// =============================================================================
void PvCamHeadSerialComLogSampleDlg::OnMove(int x, int y)
{
	CDialog::OnMove(x, y);

    mDisplay.UpdateBounds();
}


// =============================================================================
LRESULT PvCamHeadSerialComLogSampleDlg::OnUpdateSerialComLog( WPARAM wParam, LPARAM lParam )
{
    mLogBufferMutex.Lock();
    /////////////////////////////////////////////

    CString lEntry = mLogBuffer;
    mLogBuffer = _T( "" );

    /////////////////////////////////////////////
    mLogBufferMutex.Unlock();

    if ( lEntry.GetLength() <= 0 )
    {
        return 0;
    }

    // Reduce flicker by ignoring WM_PAINT
    ::SendMessage( mSerialComLogEdit.GetSafeHwnd(), WM_SETREDRAW, false, 0 );

    mLogEditTextLength += lEntry.GetLength();

    mSerialComLogEdit.SetSel( MAX_LOG_SIZE * 4, MAX_LOG_SIZE * 4, TRUE );
    mSerialComLogEdit.ReplaceSel( lEntry );

    if ( mLogEditTextLength > MAX_LOG_SIZE )
    {
        mSerialComLogEdit.SetSel( 0, lEntry.GetLength() );
        mSerialComLogEdit.ReplaceSel( _T( "" ) );
        mLogEditTextLength -= lEntry.GetLength();
    }

    // Make sure we scroll to the end. MAX_LOG_SIZE is the worst case 
    // scenario, one char per line...
    ::SendMessage( mSerialComLogEdit.GetSafeHwnd(), EM_LINESCROLL, 0, MAX_LOG_SIZE );

    ::SendMessage( mSerialComLogEdit.GetSafeHwnd(), WM_SETREDRAW, true, 0 );

    return 0;
}

