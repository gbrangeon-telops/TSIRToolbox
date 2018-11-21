// PvPlcAndGevEventsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "PvPlcAndGevEvents.h"
#include "PvPlcAndGevEventsDlg.h"

#include <process.h> 

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CPvPlcAndGevEventsDlg dialog
CPvPlcAndGevEventsDlg::CPvPlcAndGevEventsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPvPlcAndGevEventsDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CPvPlcAndGevEventsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CPvPlcAndGevEventsDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON1, &CPvPlcAndGevEventsDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CPvPlcAndGevEventsDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CPvPlcAndGevEventsDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CPvPlcAndGevEventsDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &CPvPlcAndGevEventsDlg::OnBnClickedButton5)
	ON_BN_CLICKED(IDOK, &CPvPlcAndGevEventsDlg::OnBnClickedOk)
	ON_WM_TIMER()
END_MESSAGE_MAP()

HANDLE      hProcessThread;
bool        g_bAcquisition;
int         g_iNbOfFrameGrabbed;
int         g_iNbOfGrabTimeout;

void Processfunc(void *arg)
{
	Str_shared*  strTemp = (Str_shared *)arg;
	PvPipeline* pvPipeline =strTemp->pvPipeline;
	PvDisplayWnd* pvDisplayWnd =strTemp->pvDisplayWnd;
	
	PvResult pvResult;
	PvResult pvResult2;

   // Time variables used for limiting display rate
	DWORD lPrevious = 0;
    DWORD lCurrent  = 0; 
    long lDelta ;
	static int m =0;

	g_iNbOfFrameGrabbed =0;
	g_iNbOfGrabTimeout  =0;
	while(g_bAcquisition )
	{
		PvBuffer *lBuffer = NULL;
        PvResult  lOperationResult;
		
		//Grab timeout here is determined by the DefaultImageTimeout
        pvResult = pvPipeline->RetrieveNextBuffer( & lBuffer, 0xFFFFFFFF, &lOperationResult );
        if ( pvResult.IsOK() )
        { 
            if (lOperationResult.IsOK())
            {
                //
                // We now have a valid buffer. This is where you would typically process the buffer.
                // -----------------------------------------------------------------------------------------
                // ...

			    // limit refresh rate to ~ 60 fps max
                lCurrent = ::GetTickCount();
                lDelta = ( lCurrent - lPrevious ) - ( 1000 / 60 );
			
		    	if ( lDelta > 0 )
			    {
				    pvDisplayWnd->Display( *lBuffer );
    				lPrevious = ::GetTickCount();
	    		}
                g_iNbOfFrameGrabbed++;
            }
            pvPipeline->ReleaseBuffer( lBuffer );
		}
		else
		{
			g_iNbOfGrabTimeout++;
		}
	}
	hProcessThread = NULL;
	_endthread();

}

// CPvPlcAndGevEventsDlg message handlers

BOOL CPvPlcAndGevEventsDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
    mbInited =false;
    mbTimerOn =false;
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CPvPlcAndGevEventsDlg::OnPaint()
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

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CPvPlcAndGevEventsDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CPvPlcAndGevEventsDlg::OnBnClickedButton1()
{  // "Select/Connect" button

	PvGenEnum* lPvGenEnum;
	PvGenInteger *lPvGenInteger;
 
	if(mbInited)
		Disconnect();

	// ********************************* //
	// Step 1: Select the device         //
	// ********************************* //
	PvDeviceFinderWnd pvDeviceFinderWnd;
	if ( !pvDeviceFinderWnd.ShowModal().IsOK() )
	{
		return;
	}
	PvDeviceInfo* pvDeviceInfo = pvDeviceFinderWnd.GetSelected();
	if( pvDeviceInfo == NULL )
	{
		return;
	}

	BeginWaitCursor();
    
	// ******************************** //
	// Step 2: Connect to the device    //
	// ******************************** //
	PvResult lResult = mpvDevice.Connect( pvDeviceInfo, PvAccessControl );
    if(!lResult.IsOK())
	{
		return;
	}
	
	// 
	// Set long Heartbeat Timeout for the debugging purpose
	// If the program is stopped at the breakpoint too long then the
	// hertbeat may expired
	//
#ifdef _DEBUG
    mpvDevice.GetGenParameters()->SetIntegerValue( "GevHeartbeatTimeout", 60000 );
#endif
    // Perform automatic packet size negociation
    lResult = mpvDevice.NegotiatePacketSize( 0, 1476 );
    if ( !lResult.IsOK() )
    {
        ::Sleep( 2500 );
    }

	// ************************************************* //
	// Step 3: Open and set stream destination    //
	// ************************************************* //
	lResult = mpvStream.Open( pvDeviceInfo->GetIPAddress() );
	if ( !lResult.IsOK() )
	{
		AfxMessageBox(_T("Could not open stream"));
		return;
	}
	mpvDevice.SetStreamDestination( mpvStream.GetLocalIPAddress(), mpvStream.GetLocalPort() );
	margParam.pvStream = &mpvStream;

    // Set the grab timeout value, default value 1000ms
	mpvStream.GetParameters()->SetIntegerValue("RequestTimeout", 1000 );

    // ************************************************* //
	// Step 4: Get image size information                //
	// ************************************************* //
	PvInt64 value;
	lPvGenInteger =mpvDevice.GetGenParameters()->GetInteger( "Width"  );
	lPvGenInteger->GetValue(value);
	 mWidth = (int)value;
	lPvGenInteger->RegisterEventSink(this);
	
	lPvGenInteger = mpvDevice.GetGenParameters()->GetInteger( "Height" );
	lPvGenInteger->GetValue(value);
	mHeight = (int)value;
	lPvGenInteger->RegisterEventSink(this);

    PvInt64 i64PixelFormat;
    lPvGenEnum = mpvDevice.GetGenParameters()->GetEnum("PixelFormat" );
	lPvGenEnum->GetValue(i64PixelFormat);
	mpvPixelFormat = (int)i64PixelFormat;
	lPvGenEnum->RegisterEventSink(this);

    // Retrive Pixel Size Byte per pixel (Bpp) information
	miPixelSize  =1;
 	miPixelSize  =((mpvPixelFormat & 0x00FF0000) >> 16)/8;

    // ****************************************************************************    //
	// Step 5: Prepare the PvPipeline                                              //
	// a) PvPipeline internally creates buffers with default value 640x480, 8bit.      //
	// b) At receiving of each image frame if the image leader tells the image size    //
	//    is larger than the buffer size then PvPipeline reallocates the proper size   //
	//    buffer. If the image size is smaller than the buffer size there is no buffer //
	//    resize.                                                                      //
	// ************************************************* //
  	mptrPvPipeline = new PvPipeline( & mpvStream );
	mptrPvPipeline->SetBufferSize( mWidth*mHeight*miPixelSize ); 
	mptrPvPipeline->SetBufferCount( 8 );
    mptrPvPipeline->Start();
    margParam.pvPipeline = mptrPvPipeline;

	// Step 6: Prepare for Display
	CRect lDisplayRect;
	GetDlgItem( IDC_DISPLAY )->GetClientRect( &lDisplayRect );
	GetDlgItem( IDC_DISPLAY )->ClientToScreen(&lDisplayRect );
	ScreenToClient( &lDisplayRect );
    mpvDisplayWnd.Create( GetSafeHwnd(), 10000 );
    mpvDisplayWnd.SetPosition( lDisplayRect.left, lDisplayRect.top, lDisplayRect.Width(), lDisplayRect.Height() );
	lResult =mpvDisplayWnd.SetBackgroundColor( 0x00, 0x00, 0x20);
	margParam.pvDisplayWnd =&mpvDisplayWnd;

	GetDlgItem(IDC_BUTTON1)->EnableWindow(true);
	GetDlgItem(IDC_BUTTON2)->EnableWindow(true);
	GetDlgItem(IDC_BUTTON3)->EnableWindow(true);
	GetDlgItem(IDC_BUTTON4)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON5)->EnableWindow(true);

	EndWaitCursor();

	g_bAcquisition         = false;
	mbCallbackregistered   = false;
    mDeviceWnd             = NULL;
    mbInited               = true;

	if(!mbTimerOn)
	{
		SetTimer(1, 2000, 0);
		mbTimerOn = true;
	}

 }

void CPvPlcAndGevEventsDlg::OnBnClickedButton2()
{   // "Acquisition Start/Stop" button
	// Confirm if the device is connected
	if ( !mpvDevice.IsConnected() )
	{
		CString str;
		str.Format(_T("Device appears to be disconnected."));  
		SetDlgItemText(IDC_STATIC, str); 
		return;
	}

 	if(!g_bAcquisition)
	{
        mpvDevice.GetGenParameters()->SetEnumValue( "AcquisitionMode" ,  "Continuous" );

		// Reset the stream
		mpvStream.GetParameters()->ExecuteCommand( "Reset" );
		
		PvResult lResult;
		lResult = mpvDevice.GetGenParameters()->SetIntegerValue( "TLParamsLocked", 1 );
		if ( !lResult.IsOK() )
		{
			CString str;
			str.Format(_T("Unable to change TLParamsLocked to 1.."));  
			SetDlgItemText(IDC_STATIC, str); 
		}

		// Start the stream
		lResult = mpvDevice.GetGenParameters()->ExecuteCommand( "AcquisitionStart" );
		if ( !lResult.IsOK() )
		{	
			CString str;
			str.Format(_T("Unable to execute AcquisitionStart command"));  
			SetDlgItemText(IDC_STATIC, str); 
		}

		// Start the image processing thread
		g_bAcquisition =true;

#ifdef USE_FVAL
	// ************************************//
	// Clear the counter using PCL_Ctrl1   //
	// Reset the callback counter
	// ************************************//
	mpvDevice.GetGenParameters()->SetBooleanValue( "PLC_ctrl1", false );
	Sleep(10);
	mpvDevice.GetGenParameters()->SetBooleanValue( "PLC_ctrl1", true);
	miNbOfCallback =0;
	
#endif
	
		hProcessThread = (HANDLE)_beginthread(Processfunc, 0, (void *)&margParam);
		GetDlgItem(IDC_BUTTON1)->EnableWindow(false);
		GetDlgItem(IDC_BUTTON3)->EnableWindow(false);
		GetDlgItem(IDC_BUTTON5)->EnableWindow(false);
		SetDlgItemText(IDC_BUTTON2, _T("Acquisition Stop"));
	}
	else
	{
		//Stop Acquisition
        g_bAcquisition =false;

		mpvDevice.GetGenParameters()->ExecuteCommand( "AcquisitionStop"  );
		
		// TLParamsLocked - Optionnal
		mpvDevice.GetGenParameters()->SetIntegerValue( "TLParamsLocked", 0 );
		Sleep(50);
		mpvStream.AbortQueuedBuffers();
		GetDlgItem(IDC_BUTTON1)->EnableWindow(true);
		GetDlgItem(IDC_BUTTON3)->EnableWindow(true);
		GetDlgItem(IDC_BUTTON5)->EnableWindow(true);	
		SetDlgItemText(IDC_BUTTON2, _T("Acquisition Start"));
	}
}

void CPvPlcAndGevEventsDlg::OnBnClickedButton3()
{   // "Setup PLC" button

    PvGenEnum*      lPvGenEnum;
	PvResult        lResult;

	// ********************************************* //
	// Setup PLC_I0 as PLC_A4 (FVAL) or PLC_ctrl0    //
	// ********************************************* //
	lPvGenEnum = mpvDevice.GetGenParameters()->GetEnum( "PLC_I0"  );
	if(lPvGenEnum !=NULL)
	{
#ifdef USE_FVAL
		lResult = lPvGenEnum->SetValue("PLC_A4");
#else
    	lResult =lPvGenEnum->SetValue("PLC_ctrl0");
#endif
		if(!lResult.IsOK())
		{
			AfxMessageBox(_T("PLC_I0, SetValue() failed"));
			return;
		}
	}
	else
	{
		AfxMessageBox(_T("Could not access parameter PLC_I0"));
		return;
	}

	// ********************************************** //
	// Setup PLC_I4 as PLC_ctrl1                      //
	// Note: This is the default setting              //
	//       Set it here in case it has been changed  //
    // ********************************************** //
    lResult = mpvDevice.GetGenParameters()->SetEnumValue( "PLC_I4", "PLC_ctrl1" );
	if(!lResult.IsOK())
		AfxMessageBox(_T("Could not access parameter PLC_I4"));
		

	// *********************************************** //
	// Route PLC_I0 to Q7  for PLC_Interrupt_FIFO_Q7   //
	// *********************************************** //
	mpvDevice.GetGenParameters()->SetEnumValue ("PLC_Q7_Variable0", "PLC_I0" );
	mpvDevice.GetGenParameters()->SetEnumValue ("PLC_Q7_Operator0", "Or"     );
	mpvDevice.GetGenParameters()->SetEnumValue ("PLC_Q7_Variable1", "Zero"   );
	mpvDevice.GetGenParameters()->SetEnumValue ("PLC_Q7_Operator1", "Or"     );
	mpvDevice.GetGenParameters()->SetEnumValue ("PLC_Q7_Variable2", "Zero"   );
	mpvDevice.GetGenParameters()->SetEnumValue ("PLC_Q7_Operator2", "Or"     );
	mpvDevice.GetGenParameters()->SetEnumValue ("PLC_Q7_Variable3", "Zero"   );


	//**************************************************************//
	// Route PLC_I0 to Q17 which is the counter increament event    //
	//**************************************************************//
	mpvDevice.GetGenParameters()->SetEnumValue ("PLC_Q17_Variable0", "PLC_I0" );
	mpvDevice.GetGenParameters()->SetEnumValue ("PLC_Q17_Operator0", "Or"     );
	mpvDevice.GetGenParameters()->SetEnumValue ("PLC_Q17_Variable1", "Zero"   );
	mpvDevice.GetGenParameters()->SetEnumValue ("PLC_Q17_Operator1", "Or"     );
	mpvDevice.GetGenParameters()->SetEnumValue ("PLC_Q17_Variable2", "Zero"   );
	mpvDevice.GetGenParameters()->SetEnumValue ("PLC_Q17_Operator2", "Or"     );
	mpvDevice.GetGenParameters()->SetEnumValue ("PLC_Q17_Variable3", "Zero"   );


	// **************************************************************//
	// Route the PLC_I4 to Q3 which is the counter clear event    //
	// **************************************************************//
	mpvDevice.GetGenParameters()->SetEnumValue ("PLC_Q3_Variable0", "PLC_I4" );
	mpvDevice.GetGenParameters()->SetEnumValue ("PLC_Q3_Operator0", "Or"     );
	mpvDevice.GetGenParameters()->SetEnumValue ("PLC_Q3_Variable1", "Zero"   );
	mpvDevice.GetGenParameters()->SetEnumValue ("PLC_Q3_Operator1", "Or"     );
	mpvDevice.GetGenParameters()->SetEnumValue ("PLC_Q3_Variable2", "Zero"   );
	mpvDevice.GetGenParameters()->SetEnumValue ("PLC_Q3_Operator2", "Or"     );
	mpvDevice.GetGenParameters()->SetEnumValue ("PLC_Q3_Variable3", "Zero"   );
	
	//**************************************** //
	// Enable PLC_Interrupt_FIFO_Q7            //
	// *************************************** //

	// Step 1): Select the EventSelector as Q7
	mpvDevice.GetGenParameters()->SetEnumValue( "EventSelector", "PLC_Interrupt_FIFO0_Q7" );
	
	// Step 2): Enable Q7 Interrupt
  	mpvDevice.GetGenParameters()->SetEnumValue( "EventNotification", "GigEVisionEvent");
	
	//*********************************************************** //
	// Register an PvDeviceEvent sink for accessing the 
	//	PvDeviceEventSink::OnEvent() callback:
	//*********************************************************** //
  	if(! mbCallbackregistered)
	{
		mpvDevice.RegisterEventSink(this);
		mbCallbackregistered =true;
	}

	//*********************************************************** //
	// Setup Counter to monitor the Q17             //
	//*********************************************************** //
	mpvDevice.GetGenParameters()->SetEnumValue( "CounterSelector", "Counter1" );
	mpvDevice.GetGenParameters()->SetEnumValue( "CounterEventSource", "PLC_Q17_RisingEdge");
	mpvDevice.GetGenParameters()->SetEnumValue( "CounterDecrementEventSource", "Off");
    mpvDevice.GetGenParameters()->SetEnumValue( "CounterResetSource", "PLC_Q3");
	mpvDevice.GetGenParameters()->SetEnumValue( "CounterResetActivation", "RisingEdge");

	// ************************************//
	// Clear the counter using PCL_ctrl1   //
	// ************************************//
	mpvDevice.GetGenParameters()->SetBooleanValue( "PLC_ctrl1", false );
	Sleep(10);
	mpvDevice.GetGenParameters()->SetBooleanValue( "PLC_ctrl1", true);
	miNbOfCallback =0;
    miCurCounterValue = 0;

#ifndef USE_FVAL
	GetDlgItem(IDC_BUTTON4)->EnableWindow(true);
#endif
	
}

void CPvPlcAndGevEventsDlg::OnBnClickedButton4()
{   // "Generate 1 pulse" button

	// ************************************//
	// Generate 1 pulse                    //
	// ************************************//
    mpvDevice.GetGenParameters()->SetBooleanValue( "PLC_ctrl0", false );
	Sleep(10);
	mpvDevice.GetGenParameters()->SetBooleanValue( "PLC_ctrl0", true);
}

void CPvPlcAndGevEventsDlg::OnBnClickedButton5()
{   // "Device control" button

	if ( !mpvDevice.IsConnected() )
	{
		return;
	}
	
	if(mDeviceWnd !=NULL)
	{	
		if( mDeviceWnd->GetHandle() != 0 )
			return;
		else
			mDeviceWnd->Close();
	}

	mDeviceWnd  = new PvGenBrowserWnd;
	mDeviceWnd ->SetTitle( PvString( "Device control parameters" ) );
	mDeviceWnd ->SetGenParameterArray( mpvDevice.GetGenParameters() );
	mDeviceWnd->SetVisibility(PvGenVisibilityGuru);
	mDeviceWnd ->ShowModeless(GetSafeHwnd() );
}

void CPvPlcAndGevEventsDlg::OnParameterUpdate( PvGenParameter *aParameter )
{
	PvString lName;
	bool bBufferResize =false;

	if ( !aParameter->GetName( lName ).IsOK() )
	{
		ASSERT( 0 ); // Totally unexpected	
		return;
	}

	if (  lName == "Width"  )
	{	
		PvInt64 value;
        mpvDevice.GetGenParameters()->GetIntegerValue( "Width", value );
		if(mWidth != (int)value)
		{
			mWidth = (int)value;
			bBufferResize  =true;
		}

	}
	else if (  lName == "Height" )
	{	
		PvInt64 value;
		mpvDevice.GetGenParameters()->GetIntegerValue( "Height", value );
		if( mHeight != (int)value)
		{	
			mHeight = (int)value;
			bBufferResize  =true;
		}
	}
	else if (  lName == "PixelFormat" )
	{
		PvInt64 i64PixelFormat;
		mpvDevice.GetGenParameters()->GetEnumValue( "PixelFormat", i64PixelFormat );
		if(mpvPixelFormat != (int)i64PixelFormat)
		{
			mpvPixelFormat = (int)i64PixelFormat;
			miPixelSize =1;
			miPixelSize =((mpvPixelFormat & 0x00FF0000) >> 16)/8;
			bBufferResize  =true;
		}
	}
	else
	{
		CString str;
		str.Format(_T("An unregistered parameter update: %s updated"), lName.GetAscii());  
		::AfxMessageBox(str);
	}

  	if(bBufferResize)
	{
		if( mptrPvPipeline->IsStarted())
			mptrPvPipeline->Stop();
		mptrPvPipeline->SetBufferSize( mWidth*mHeight*miPixelSize ); 
		mptrPvPipeline->Start();
	}

}

void CPvPlcAndGevEventsDlg::OnEvent( PvDevice *aDevice, PvUInt16 aEventID, PvUInt16 aChannel, PvUInt64 aBlockID, 
    PvUInt64 aTimestamp, const void *aData, PvUInt32 aDataLength )
{
    if(aEventID ==36869)
 		miNbOfCallback++;
	else
	{
		CString str;
	    str.Format(_T("Unexpected EventID %d, Channel %d, BlockID %d, Timestamp %d"), aEventID, aChannel, aBlockID, aTimestamp);
	    AfxMessageBox(str);
	}
}

void CPvPlcAndGevEventsDlg::OnBnClickedOk()
{
	if(! mbInited)
	{
		OnOK();
		return;
	}
	
	Disconnect();
	OnOK();
}


void CPvPlcAndGevEventsDlg::Disconnect()
{
	PvResult lResult;

	if ( mDeviceWnd !=NULL)
	{
		if( mDeviceWnd->GetHandle() != 0 )
			mDeviceWnd->Close();
	}

	if(g_bAcquisition)
	{
		OnBnClickedButton2();
	}

	//Unregister the PvDeviceEventSink
	if(mbCallbackregistered)
		mpvDevice.UnregisterEventSink(this);

	if(mbTimerOn)
	{
		KillTimer(1);
		mbTimerOn =false;
	}

    // Unregister image parameters                     
	PvGenInteger* lPvGenInteger;
	lPvGenInteger = mpvDevice.GetGenParameters()->GetInteger( "Width" );
	lPvGenInteger->UnregisterEventSink(this);
	lPvGenInteger = mpvDevice.GetGenParameters()->GetInteger( "Height" );
	lPvGenInteger->UnregisterEventSink(this);
    PvGenEnum* lPvGenEnum;
	lPvGenEnum = mpvDevice.GetGenParameters()->GetEnum("PixelFormat" );
	lPvGenEnum->UnregisterEventSink(this);
	
	mpvDevice.Disconnect();	
	
	if( mptrPvPipeline->IsStarted())
		mptrPvPipeline->Stop();
   
	mpvStream.Close();
  
}

void CPvPlcAndGevEventsDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
    CString str;
    if(mbCallbackregistered)
	{
		PvInt64 value;
		mpvDevice.GetGenParameters()->GetIntegerValue( "CounterValue", value);
	  	miCurCounterValue =(int) value;
		if(g_bAcquisition)
			str.Format(_T("Grabbed frames =%i, Grab timeout =%i, PLC interrupt callback =%i, Counter value =%i"), g_iNbOfFrameGrabbed, g_iNbOfGrabTimeout, miNbOfCallback, miCurCounterValue);  
		else
			str.Format(_T("Received PLC interrupt callback =%i, Counter value =%i"), miNbOfCallback, miCurCounterValue);  
	
		SetDlgItemText(IDC_STATIC, str); 
	}
	else
	{
		if(g_bAcquisition)
			str.Format(_T("Grabbed frames =%i, Grab timeout =%i"), g_iNbOfFrameGrabbed, g_iNbOfGrabTimeout);  

		SetDlgItemText(IDC_STATIC, str); 
	}
	__super::OnTimer(nIDEvent);
}
