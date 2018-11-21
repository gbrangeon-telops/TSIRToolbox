// *****************************************************************************
//
//     Copyright (c) 2007, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#include "stdafx.h"
#include "GEVPlayer.h"
#include "GEVPlayerDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


BEGIN_MESSAGE_MAP(CGEVPlayerApp, CWinApp)
END_MESSAGE_MAP()


// =============================================================================
CGEVPlayerApp::CGEVPlayerApp()
{
    // TODO: add construction code here,
    // Place all significant initialization in InitInstance
}


//
// The one and only CGEVPlayerApp object
//

CGEVPlayerApp theApp;



// =============================================================================
BOOL CGEVPlayerApp::InitInstance()
{
    // InitCommonControlsEx() is required on Windows XP if an application
    // manifest specifies use of ComCtl32.dll version 6 or later to enable
    // visual styles.  Otherwise, any window creation will fail.
    INITCOMMONCONTROLSEX InitCtrls;
    InitCtrls.dwSize = sizeof(InitCtrls);
    // Set this to include all the common control classes you want to use
    // in your application.
    InitCtrls.dwICC = ICC_WIN95_CLASSES;
    InitCommonControlsEx(&InitCtrls);

    CCommandLineInfo lCommandLine;
    ParseCommandLine( lCommandLine );
    CWinApp::InitInstance();

    AfxEnableControlContainer();

    SetRegistryKey(_T("Pleora Technologies Inc"));

    GEVPlayerDlg dlg(lCommandLine.m_strFileName);
    m_pMainWnd = &dlg;

    INT_PTR nResponse = dlg.DoModal();
    if (nResponse == IDOK)
    {
        // TODO: Place code here to handle when the dialog is
        //  dismissed with OK
    }
    else if (nResponse == IDCANCEL)
    {
        // TODO: Place code here to handle when the dialog is
        //  dismissed with Cancel
    }

    // Since the dialog has been closed, return FALSE so that we exit the
    //  application, rather than start the application's message pump.
    return FALSE;
}

/*

// =============================================================================
BOOL CGEVPlayerApp::PreTranslateMessage(MSG* pMsg)
{
    if ( CWinApp::PreTranslateMessage( pMsg ) )
    {
        return( TRUE );
    }

    return PvGUI_FilterDllMsg( reinterpret_cast<MSG *>( pMsg ) );
}


// =============================================================================
BOOL CGEVPlayerApp::OnIdle(LONG lCount)
{
    if ( CWinApp::OnIdle( lCount ) )
    {
        return TRUE;
    }

    PvGUI_ProcessDllIdle();

    return FALSE;
}

*/

