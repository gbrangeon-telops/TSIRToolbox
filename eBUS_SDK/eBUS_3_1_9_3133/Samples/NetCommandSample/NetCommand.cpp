// *****************************************************************************
//
// Copyright (c) 2010, Pleora Technologies Inc., All rights reserved.
//
// -----------------------------------------------------------------------------
//
// The NetCommand application. Entry point for the application.
// 
// *****************************************************************************


#include "stdafx.h"
#include "afxwinappex.h"
#include "NetCommand.h"
#include "MainFrm.h"

#include "ChildFrm.h"
#include "NetCommand.h"
#include "NetCommandView.h"
#include "AboutBox.h"
#include "SplashPage.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


BEGIN_MESSAGE_MAP(NetCommandApp, CWinAppEx)
	ON_COMMAND(ID_APP_ABOUT, &NetCommandApp::OnAppAbout)
	ON_COMMAND(ID_FILE_NEW, &CWinAppEx::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinAppEx::OnFileOpen)
	ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinAppEx::OnFilePrintSetup)
END_MESSAGE_MAP()


// ==========================================================================
NetCommandApp::NetCommandApp()
{

	mbHiColorIcons = TRUE;

	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

NetCommandApp theApp;

// ==========================================================================
BOOL NetCommandApp::InitInstance()
{
	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof( InitCtrls );
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx( &InitCtrls );

	CWinAppEx::InitInstance();

	// Initialize OLE libraries
	if ( !AfxOleInit() )
	{
		AfxMessageBox( IDP_OLE_INIT_FAILED );
		return FALSE;
	}
	AfxEnableControlContainer();

    SplashPage::Show();
    
    // Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey( _T( "Local AppWizard-Generated Applications" ) );
	LoadStdProfileSettings( 4 );  // Load standard INI file options (including MRU)

	InitContextMenuManager();
	InitKeyboardManager();

	InitTooltipManager();
	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	theApp.GetTooltipManager()->SetTooltipParams( AFX_TOOLTIP_TYPE_ALL,
		RUNTIME_CLASS( CMFCToolTipCtrl ), &ttParams);

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views
	CMultiDocTemplate* pDocTemplate;
    pDocTemplate = new CMultiDocTemplate( IDR_NETCOMMANDTYPE,
		RUNTIME_CLASS( NetCommandDoc ),
		RUNTIME_CLASS( ChildFrame ), // custom MDI child frame
		RUNTIME_CLASS( NetCommandView) );
	if ( !pDocTemplate )
    {
		return FALSE;
    }
	AddDocTemplate( pDocTemplate );

	// create main MDI Frame window
	MainFrame* pMainFrame = new MainFrame;
	if ( !pMainFrame || !pMainFrame->LoadFrame( IDR_MAINFRAME ) )
	{
		delete pMainFrame;
		return FALSE;
	}
	m_pMainWnd = pMainFrame;
	// call DragAcceptFiles only if there's a suffix
	//  In an MDI app, this should occur immediately after setting mpMainWnd

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine( cmdInfo );
/*
	// Dispatch commands specified on the command line.  Will return FALSE if
	// app was launched with /RegServer, /Register, /Unregserver or /Unregister.
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;
*/
	// The main window has been initialized, so show and update it
	pMainFrame->ShowWindow( m_nCmdShow );
	pMainFrame->UpdateWindow();

	return TRUE;
}

// ==========================================================================
void NetCommandApp::OnAppAbout()
{
	AboutBox lDlg;
	lDlg.DoModal();
}

// ==========================================================================
void NetCommandApp::PreLoadState()
{
}

// ==========================================================================
void NetCommandApp::LoadCustomState()
{
}

// ==========================================================================
void NetCommandApp::SaveCustomState()
{
}


