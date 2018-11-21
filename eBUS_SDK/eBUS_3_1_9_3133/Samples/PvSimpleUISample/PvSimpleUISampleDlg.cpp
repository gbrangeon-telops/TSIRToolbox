// *****************************************************************************
//
//     Copyright (c) 2007, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#include "stdafx.h"

#include "PvSimpleUISample.h"
#include "PvSimpleUISampleDlg.h"

#include <string.h>


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


BEGIN_MESSAGE_MAP(PvSimpleUISampleDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_SIZE()
	ON_WM_GETMINMAXINFO()
	ON_BN_CLICKED(IDC_DEVICE_BUTTON, &PvSimpleUISampleDlg::OnBnClickedDeviceButton)
	ON_BN_CLICKED(IDC_LINK_BUTTON, &PvSimpleUISampleDlg::OnBnClickedLinkButton)
	ON_BN_CLICKED(IDC_STREAMPARAMS_BUTTON, &PvSimpleUISampleDlg::OnBnClickedStreamparamsButton)
	ON_BN_CLICKED(IDC_CONNECT_BUTTON, &PvSimpleUISampleDlg::OnBnClickedConnectButton)
	ON_BN_CLICKED(IDC_DISCONNECT_BUTTON, &PvSimpleUISampleDlg::OnBnClickedDisconnectButton)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_START, &PvSimpleUISampleDlg::OnBnClickedStart)
	ON_BN_CLICKED(IDC_STOP, &PvSimpleUISampleDlg::OnBnClickedStop)
	ON_CBN_SELCHANGE(IDC_MODE, &PvSimpleUISampleDlg::OnCbnSelchangeMode)
	ON_BN_CLICKED(IDC_DEVICEEVENTS_BUTTON, &PvSimpleUISampleDlg::OnBnClickedDeviceEvents)
	ON_WM_MOVE()
    ON_WM_CTLCOLOR()
    ON_MESSAGE(WM_ACQUISITIONSTATECHANGED, &PvSimpleUISampleDlg::OnAcquisitionStateChanged)
END_MESSAGE_MAP()


// =============================================================================
PvSimpleUISampleDlg::PvSimpleUISampleDlg( CWnd* pParent /* =NULL */ )
	: CDialog( PvSimpleUISampleDlg::IDD, pParent )
	, mNeedInit( TRUE )
	, mDeviceWnd( NULL )
	, mCommunicationWnd( NULL )
	, mStreamParametersWnd( NULL )
	, mDisplayThread( NULL )
	, mPipeline( &mStream )
    , mAcquiringImages( false )
    , mAcquisitionStateManager( NULL )
{
	m_hIcon = AfxGetApp()->LoadIcon( IDR_MAINFRAME );

	// Create display thread
	mDisplayThread = new DisplayThread( &mDisplay );
}


// =============================================================================
PvSimpleUISampleDlg::~PvSimpleUISampleDlg()
{
    if ( mDisplayThread != NULL )
    {
        delete mDisplayThread;
        mDisplayThread = NULL;
    }

    if ( mAcquisitionStateManager != NULL )
    {
        delete mAcquisitionStateManager;
        mAcquisitionStateManager = NULL;
    }
}


// =============================================================================
void PvSimpleUISampleDlg::DoDataExchange(CDataExchange* pDX)
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
}


// =============================================================================
BOOL PvSimpleUISampleDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetWindowText( _T( "PvSimpleUISample" ) );

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
void PvSimpleUISampleDlg::OnPaint()
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
HCURSOR PvSimpleUISampleDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


// =============================================================================
void PvSimpleUISampleDlg::OnClose()
{
	// Make sure we cleanup before we leave
	Disconnect();

	CDialog::OnClose();
}


// =============================================================================
void PvSimpleUISampleDlg::OnBnClickedConnectButton()
{
	// create a device finder wnd and open the select device dialog
	PvDeviceFinderWnd lFinder;
	lFinder.SetTitle( _T( "GigE Vision Device Selection" ) );

    PvResult lResult = lFinder.ShowModal();

    if ( lResult.GetCode() == PvResult::Code::ABORTED )
    {
        MessageBox( _T( "Invalid selection. Please select a GigE Vision Device connected through an eBUS network card configuration." ), _T("PvSimpleUISample") );
        return;
    }

    if ( !lResult.IsOK() || ( lFinder.GetSelected() == NULL ) )
	{
		return;
	}

	CWaitCursor lCursor;

	UpdateWindow();

	Connect( lFinder.GetSelected() );
}


// =============================================================================
void PvSimpleUISampleDlg::OnBnClickedDisconnectButton()
{
	CWaitCursor lCursor;

	Disconnect();
}


// =============================================================================
void PvSimpleUISampleDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	CRect lR;
	this->GetWindowRect( &lR );
	TRACE( "%i %i\n", lR.Width(), lR.Height() );

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

	::EndDeferWindowPos(hDWP);
}


// =============================================================================
void PvSimpleUISampleDlg::OnGetMinMaxInfo( MINMAXINFO *lpMMI )
{
	lpMMI->ptMinTrackSize.x = 800;
	lpMMI->ptMinTrackSize.y = 497;
}


// =============================================================================
void PvSimpleUISampleDlg::EnableInterface()
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
	GetDlgItem( IDC_COMMUNICATION_BUTTON )->EnableWindow( TRUE  ); 

    bool lLocked = false;
    if ( mAcquisitionStateManager != NULL )
    {
        lLocked = mAcquisitionStateManager->GetState() == PvAcquisitionStateLocked;
    }

	mPlayButton.EnableWindow( lConnected && !lLocked );
	mStopButton.EnableWindow( lConnected && lLocked );

	// If not connected, disable the acquisition mode control. If enabled,
	// it will be managed automatically by events from the GenICam parameters
	if ( !lConnected || lLocked )
	{
        mModeCombo.EnableWindow( FALSE );
	}
}


// =============================================================================
void PvSimpleUISampleDlg::Connect( PvDeviceInfo *aDI )
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
    mDevice.SetStreamDestination( mStream.GetLocalIPAddress(), mStream.GetLocalPort() );

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

	PvString lManufacturerStr, lModelNameStr, lDeviceVersionStr = "N/A";
	lGenDevice->GetString( "DeviceVendorName", lManufacturerStr );
	lGenDevice->GetString( "DeviceModelName", lModelNameStr );
	lGenDevice->GetString( "DeviceVersion", lDeviceVersionStr );

	// IP
	CString lIPStr;
	PvInt64 lIPAddress = 0;
	lGenDevice->GetIntegerValue( "GevCurrentIPAddress", lIPAddress );
	unsigned char *lIPPtr = reinterpret_cast<unsigned char *>( &lIPAddress );
	lIPStr.Format( _T( "%i.%i.%i.%i" ), 
		lIPPtr[3], lIPPtr[2], lIPPtr[1], lIPPtr[0] );

	// MAC address
	CString lMACStr;
	PvInt64 lMACAddress;
	lGenDevice->GetIntegerValue( "GevMACAddress", lMACAddress );
	unsigned char *lMACPtr = reinterpret_cast<unsigned char *>( &lMACAddress );
	lMACStr.Format( _T( "%02X:%02X:%02X:%02X:%02X:%02X" ), 
		lMACPtr[5], lMACPtr[4], lMACPtr[3], lMACPtr[2], lMACPtr[1], lMACPtr[0] );

	// Device name (User ID)
	PvString lStr = "N/A";
	lGenDevice->GetString( "GevDeviceUserID", lStr );
	CString lNameStr = lStr.GetUnicode();

	mManufacturerEdit.SetWindowText( lManufacturerStr );
	mModelEdit.SetWindowText( lModelNameStr.GetUnicode() );
	mIPEdit.SetWindowText( lIPStr );
	mMACEdit.SetWindowText( lMACStr );
	mNameEdit.SetWindowText( lNameStr );

	// Fill acquisition mode combo box
	mModeCombo.ResetContent();
	PvGenEnum *lMode = lGenDevice->GetEnum( "AcquisitionMode" );
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

    // Create acquisition state manager
    ASSERT( mAcquisitionStateManager == NULL );
    mAcquisitionStateManager = new PvAcquisitionStateManager( &mDevice, &mStream );
    mAcquisitionStateManager->RegisterEventSink( this );

    mAcquiringImages = false;

    // Force an update on all the parameters on acquisition mode
    OnParameterUpdate( lMode );

	// Ready image reception
	StartStreaming();

	// Sync up UI
	EnableInterface();
}


// =============================================================================
void PvSimpleUISampleDlg::Disconnect()
{
    // Unregister all events of the parameters in the device's node map
    PvGenParameterArray *lGenDevice = mDevice.GetGenParameters();
    for ( PvUInt32 i = 0; i < lGenDevice->GetCount(); i++ )
    {
	    lGenDevice->Get( i )->UnregisterEventSink( this );
    }

    // Close all configuration child windows
	CloseGenWindow( &mDeviceWnd );
	CloseGenWindow( &mCommunicationWnd );
	CloseGenWindow( &mStreamParametersWnd );

	// If streaming, stop streaming
	StopStreaming();

    // Release acquisition state manager
    if ( mAcquisitionStateManager != NULL )
    {
        delete mAcquisitionStateManager;
        mAcquisitionStateManager = NULL;
    }

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
void PvSimpleUISampleDlg::OnBnClickedDeviceButton()
{
	if ( !mDevice.IsConnected() )
	{
		return;
	}

	ShowGenWindow( 
		&mDeviceWnd, 
		mDevice.GetGenParameters(), 
		_T( "Device Control" ) );
}


// =============================================================================
void PvSimpleUISampleDlg::OnBnClickedDeviceEvents()
{
}


// =============================================================================
void PvSimpleUISampleDlg::OnBnClickedLinkButton()
{
	ShowGenWindow( 
		&mCommunicationWnd, 
		mDevice.GetGenLink(), 
		_T( "Communication Control" ) );
}


// =============================================================================
void PvSimpleUISampleDlg::OnBnClickedStreamparamsButton()
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
void PvSimpleUISampleDlg::ShowGenWindow( PvGenBrowserWnd **aWnd, PvGenParameterArray *aParams, const CString &aTitle )
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
void PvSimpleUISampleDlg::CloseGenWindow( PvGenBrowserWnd **aWnd )
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
void PvSimpleUISampleDlg::StartStreaming()
{
	// Start threads
    mDisplayThread->Start( &mPipeline, mDevice.GetGenParameters() );
	mDisplayThread->SetPriority( THREAD_PRIORITY_ABOVE_NORMAL );

    // Start pipeline
    mPipeline.Start();
}


// =============================================================================
void PvSimpleUISampleDlg::StopStreaming()
{
    // Stop display thread
	if ( mDisplayThread != NULL )
	{
		mDisplayThread->Stop( false );
    }

	// Stop stream thread
    if ( mPipeline.IsStarted() )
    {
		mPipeline.Stop();
    }

    // Wait for display thread to be stopped
	if ( mDisplayThread != NULL )
	{
        mDisplayThread->WaitComplete();
	}
}


// =============================================================================
void PvSimpleUISampleDlg::StartAcquisition()
{
    // Get payload size from device
    PvInt64 lPayloadSizeValue = GetPayloadSize();

    // If payload size is valid, force buffers re-alloc - better than 
    // adjusting as images are comming in
    if ( lPayloadSizeValue > 0 )
    {
        mPipeline.SetBufferSize( static_cast<PvUInt32>( lPayloadSizeValue ) );
    }

    // Never hurts to start streaming on a fresh pipeline/stream...
    mPipeline.Reset();

    // Reset stream statistics
    mStream.GetParameters()->ExecuteCommand( "Reset" );

    ASSERT( mAcquisitionStateManager != NULL );
    mAcquisitionStateManager->Start();
}


// =============================================================================
void PvSimpleUISampleDlg::StopAcquisition()
{
    ASSERT( mAcquisitionStateManager != NULL );
    mAcquisitionStateManager->Stop();
}


// =============================================================================
void PvSimpleUISampleDlg::OnBnClickedStart()
{
    if ( !mDevice.IsConnected() )
    {
        return;
    }

    StartAcquisition();

	EnableInterface();
}


// =============================================================================
void PvSimpleUISampleDlg::OnBnClickedStop()
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
void PvSimpleUISampleDlg::OnCbnSelchangeMode()
{
    if ( !mDevice.IsConnected() )
    {
        return;
    }

	if ( mModeCombo.GetCurSel() < 0 )
	{
		return;
	}

	PvGenParameterArray *lDeviceParams = mDevice.GetGenParameters();

	PvUInt64 lValue = mModeCombo.GetItemData( mModeCombo.GetCurSel() );
	PvResult lResult = lDeviceParams->SetEnumValue( "AcquisitionMode", lValue );
	if ( !lResult.IsOK() )
	{
		MessageBox( _T( "Unable to set AcquisitionMode value." ), _T( "Error" ), 
			MB_OK | MB_ICONINFORMATION );
	}
}


// =============================================================================
void PvSimpleUISampleDlg::OnParameterUpdate( PvGenParameter *aParameter )
{
	bool bBufferResize =false;
	PvString lName;
	if ( !aParameter->GetName( lName ).IsOK() )
	{
		ASSERT( 0 ); // Totally unexpected	
		return;
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
void PvSimpleUISampleDlg::OnMove(int x, int y)
{
	CDialog::OnMove(x, y);

    mDisplay.UpdateBounds();
}


// ==========================================================================
PvUInt32 PvSimpleUISampleDlg::GetPayloadSize()
{
    if ( !mDevice.IsConnected() )
    {
        return 0;
    }

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

    // Compute poor man's payload size - for devices not maintaning PayloadSize properly
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


// =============================================================================
void PvSimpleUISampleDlg::OnAcquisitionStateChanged( PvDevice* aDevice, PvStreamBase* aStream, PvUInt32 aSource, PvAcquisitionState aState )
{
    PostMessage( WM_ACQUISITIONSTATECHANGED );
}


// =============================================================================
LRESULT PvSimpleUISampleDlg::OnAcquisitionStateChanged( WPARAM wParam, LPARAM lParam )
{
    EnableInterface();
    return 0;
}
