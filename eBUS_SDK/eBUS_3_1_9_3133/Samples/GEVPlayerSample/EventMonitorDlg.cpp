// *****************************************************************************
//
//     Copyright (c) 2007, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#include "stdafx.h"

#include "GEVPlayer.h"
#include "EventMonitorDlg.h"
#include "GEVPlayerDlg.h"


#define MAX_LOG_SIZE ( 256 * 1024 )


IMPLEMENT_DYNAMIC(EventMonitorDlg, CDialog)

BEGIN_MESSAGE_MAP(EventMonitorDlg, CDialog)
    ON_BN_CLICKED(IDC_CLEAR_BUTTON, &EventMonitorDlg::OnBnClickedClearButton)
    ON_WM_SIZE()
    ON_WM_GETMINMAXINFO()
    ON_WM_TIMER()
    ON_WM_SHOWWINDOW()
    ON_WM_DESTROY()
    ON_WM_CREATE()
    ON_BN_CLICKED(IDC_SAVELOGCHECKBOX, &EventMonitorDlg::OnBnClickedSaveLogCheckBox)
    ON_BN_CLICKED(IDC_SAVELOGBUTTON, &EventMonitorDlg::OnBnClickedSaveLogButton)
    ON_BN_CLICKED(IDC_GENICAMCHECKBOX, &EventMonitorDlg::OnBnClickedGenICamCheckBox)
    ON_BN_CLICKED(IDC_EVENTSCHECKBOX, &EventMonitorDlg::OnBnClickedEventsCheckBox)
    ON_BN_CLICKED(IDC_BUFFERALLCHECKBOX, &EventMonitorDlg::OnBnClickedBufferAllCheckBox)
    ON_BN_CLICKED(IDC_BUFFERERRORCHECKBOX, &EventMonitorDlg::OnBnClickedBufferErrorCheckBox)
    ON_BN_CLICKED(IDC_SERIAL_COM_LOG_ENABLED, &EventMonitorDlg::OnBnClickedSerialComLogEnabled)
END_MESSAGE_MAP()


// ==========================================================================
EventMonitorDlg::EventMonitorDlg( LogBuffer *aLogBuffer, PvGenParameterArray *aParameters, CWnd* pParent /*=NULL*/ )
    : CDialog( EventMonitorDlg::IDD, pParent )
    , mParameters( aParameters )
    , mNeedInit( TRUE )
    , mLogEditTextLength( 0 )
    , mLastUpdateTime( 0 )
    , mTimer( 0 )
    , mLogBuffer( aLogBuffer )
    , mShowSerialComLogCheck( false )
{
    DWORD lValue = GEVPlayerDlg::ReadRegistryDWORD( _T( "ShowSerialComLogCheck" ) );
    if ( lValue != 0 )
    {
        mShowSerialComLogCheck = true;
    }
}


// ==========================================================================
EventMonitorDlg::~EventMonitorDlg()
{
}


// ==========================================================================
void EventMonitorDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_LOG_EDIT, mLogEdit);
    DDX_Control(pDX, IDC_SAVELOGCHECKBOX, mSaveLogCheckBox);
    DDX_Control(pDX, IDC_SAVELOGEDIT, mSaveLogEdit);
    DDX_Control(pDX, IDC_SAVELOGBUTTON, mSaveLogButton);
    DDX_Control(pDX, IDC_GENICAMCHECKBOX, mGenICamCheckBox);
    DDX_Control(pDX, IDC_EVENTSCHECKBOX, mEventsCheckBox);
    DDX_Control(pDX, IDC_BUFFERALLCHECKBOX, mBufferAllCheckBox);
    DDX_Control(pDX, IDC_BUFFERERRORCHECKBOX, mBufferErrorCheckBox);
    DDX_Control(pDX, IDC_SERIAL_COM_LOG_ENABLED, mSerialComLogCheck);
    DDX_Control(pDX, IDC_SERIAL_COMMUNICATIONS_LABEL, mSerialComLabel);
}


// ==========================================================================
BOOL EventMonitorDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    mSerialComLogCheck.ShowWindow( mShowSerialComLogCheck );
    mSerialComLabel.ShowWindow( mShowSerialComLogCheck );

    GetClientRect( mCrt );
    mNeedInit = FALSE;

    mWindowBackgroundBrush.CreateSolidBrush( ::GetSysColor( COLOR_WINDOW ) );

    mInfoMap.clear();

    mLogEdit.SetLimitText( MAX_LOG_SIZE * 2 );
    mLogEdit.HideCaret();

    if ( mTerminalFont.GetSafeHandle() == 0 )
    {
        mTerminalFont.CreateFont( 14, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, _T( "Courier New" ) );
    }

    mLogEdit.SetFont( &mTerminalFont );

    for ( PvUInt32 i = 0; i < mParameters->GetCount(); i++ )
    {
        PvGenParameter *lParameter = mParameters->Get( i );

        // We only display invisible parameter events when in debug mode
#ifndef _DEBUG
        PvGenVisibility lVisibility = PvGenVisibilityUndefined;
        VERIFY( lParameter->GetVisibility( lVisibility ) );
        if ( lVisibility < PvGenVisibilityInvisible )
#endif // _DEBUG
        {
            PvString lName;
            VERIFY( lParameter->GetName( lName ).IsOK() );

            mInfoMap[ lName.GetUnicode() ].Initialize( lParameter, mLogBuffer );
        }
    }

    LoadData();

    return TRUE;
}


// ==============================================================================
void EventMonitorDlg::OnTimer( UINT_PTR nIDEvent )
{
    if ( nIDEvent == 1 )
    {
        CString lEntry = mLogBuffer->Empty();
        if ( lEntry.GetLength() <= 0 )
        {
            return;
        }

        // Reduce flicker by ignoring WM_PAINT
        ::SendMessage( mLogEdit.GetSafeHwnd(), WM_SETREDRAW, false, 0 );

        mLogEditTextLength += lEntry.GetLength();

        mLogEdit.SetSel( MAX_LOG_SIZE * 4, MAX_LOG_SIZE * 4, TRUE );
        mLogEdit.ReplaceSel( lEntry );

        if ( mLogEditTextLength > MAX_LOG_SIZE )
        {
            mLogEdit.SetSel( 0, lEntry.GetLength() );
            mLogEdit.ReplaceSel( _T( "" ) );
            mLogEditTextLength -= lEntry.GetLength();
        }

        // Make sure we scroll to the end. MAX_LOG_SIZE is the worst case 
        // scenario, one char per line...
        ::SendMessage( mLogEdit.GetSafeHwnd(), EM_LINESCROLL, 0, MAX_LOG_SIZE );

        ::SendMessage( mLogEdit.GetSafeHwnd(), WM_SETREDRAW, true, 0 );
    }

    CDialog::OnTimer( nIDEvent );
}


// ==========================================================================
void EventMonitorDlg::OnBnClickedClearButton()
{
    mLogBuffer->Reset();
    mLogEdit.SetWindowText( _T( "" ) );
    mLogEditTextLength = 0;
}


// ==========================================================================
void EventMonitorDlg::OnSize(UINT nType, int cx, int cy)
{
    CDialog::OnSize(nType, cx, cy);

    CRect lR;
    this->GetWindowRect( &lR );
    
    // TRACE( _T( "%i %i\n" ), lR.Width(), lR.Height() );

    if ( mNeedInit )
    {
        return;
    }

    int dx = cx - mCrt.Width();
    int dy = cy - mCrt.Height();
    GetClientRect(&mCrt);

    CRect r1;

    HDWP hDWP = ::BeginDeferWindowPos( 20 );

    //
    // Bottom left, resizing
    //

    int lBottomLeftResizing[] =
    {
        IDC_PARAMETERS_GROUP, IDC_LISTBOX
    };

    for ( int i = 0; i < sizeof( lBottomLeftResizing ) / sizeof ( lBottomLeftResizing[ 0 ] ); i++ )
    {
        CWnd *lChild = GetDlgItem( lBottomLeftResizing[ i ] );
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
    // Bottom left, sticking to the bottom
    //

    int lBottomLeft[] =
    {
        IDC_INFO, IDC_RESET_BUTTON
    };

    for ( int i = 0; i < sizeof( lBottomLeft ) / sizeof ( lBottomLeft[ 0 ] ); i++ )
    {
        CWnd *lChild = GetDlgItem( lBottomLeft[ i ] );
        if ( lChild != NULL )
        {
            lChild->GetWindowRect(&r1); ScreenToClient(&r1); 
            r1.top += dy;
            r1.bottom += dy;
            ::DeferWindowPos(
                hDWP, lChild->m_hWnd, NULL, 
                r1.left, r1.top, r1.Width(), r1.Height(),
                SWP_NOACTIVATE|SWP_NOZORDER);
        }
    }

    //
    // Right, make sure the log and group box fill the right part
    // of our window
    //

    int lRight[] = 
    { 
        IDC_LOG_EDIT, IDC_LOG_GROUP
    };

    for ( int i = 0; i < sizeof( lRight ) / sizeof ( lRight[ 0 ] ); i++ )
    {
        CWnd *lChild = GetDlgItem( lRight[ i ] );
        if ( lChild != NULL )
        {
            lChild->GetWindowRect(&r1); ScreenToClient(&r1); 
            r1.right += dx; r1.bottom += dy;
            ::DeferWindowPos(
                hDWP, lChild->m_hWnd, NULL, 
                r1.left, r1.top, r1.Width(), r1.Height(),
                SWP_NOACTIVATE|SWP_NOZORDER);
        }
    }

    //
    // Reset button box. Just like the log, but sticks to the bottom
    // and is not resized
    //

    int lBottomRight[] = 
    { 
        IDC_CLEAR_BUTTON, IDC_SAVELOGEDIT
    };

    for ( int i = 0; i < sizeof( lBottomRight ) / sizeof ( lBottomRight[ 0 ] ); i++ )
    {
        CWnd *lChild = GetDlgItem( lBottomRight[ i ] );
        if ( lChild != NULL )
        {
            lChild->GetWindowRect(&r1); ScreenToClient(&r1); 
            ::DeferWindowPos(
                hDWP, lChild->m_hWnd, NULL, 
                r1.left, r1.top + dy, r1.Width() + dx, r1.Height(),
                SWP_NOACTIVATE|SWP_NOZORDER);
        }
    }

    // Save log check box. Sticks to bottom left, not resized
    
    int lBottomRightFixed[] = 
    { 
        IDC_SAVELOGCHECKBOX
    };

    for ( int i = 0; i < sizeof( lBottomRightFixed ) / sizeof ( lBottomRightFixed[ 0 ] ); i++ )
    {
        CWnd *lChild = GetDlgItem( lBottomRightFixed[ i ] );
        if ( lChild != NULL )
        {
            lChild->GetWindowRect(&r1); ScreenToClient(&r1); 
            ::DeferWindowPos(
                hDWP, lChild->m_hWnd, NULL, 
                r1.left, r1.top + dy, r1.Width(), r1.Height(),
                SWP_NOACTIVATE|SWP_NOZORDER);
        }
    }

    // Bottom right, "..." button
    mSaveLogButton.GetWindowRect(&r1); ScreenToClient(&r1); 
    ::DeferWindowPos(
        hDWP, mSaveLogButton.m_hWnd, NULL, 
        r1.left + dx, r1.top + dy, r1.Width(), r1.Height(),
        SWP_NOACTIVATE|SWP_NOZORDER);

    //
    // Top right check boxes
    //

    int lTopRight[] = 
    { 
        IDC_GENICAMCHECKBOX, IDC_EVENTSCHECKBOX, IDC_BUFFERERRORCHECKBOX, IDC_BUFFERALLCHECKBOX,
        IDC_SERIAL_COM_LOG_ENABLED, IDC_SERIAL_COMMUNICATIONS_LABEL
    };

    for ( int i = 0; i < sizeof( lTopRight ) / sizeof ( lTopRight[ 0 ] ); i++ )
    {
        CWnd *lChild = GetDlgItem( lTopRight[ i ] );
        if ( lChild != NULL )
        {
            lChild->GetWindowRect(&r1); ScreenToClient(&r1); 
            ::DeferWindowPos(
                hDWP, lChild->m_hWnd, NULL, 
                r1.left + dx, r1.top, r1.Width(), r1.Height(),
                SWP_NOACTIVATE|SWP_NOZORDER);
        }
    }

    ::EndDeferWindowPos(hDWP);
}


// ==========================================================================
void EventMonitorDlg::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
    lpMMI->ptMinTrackSize.x = 480;
    lpMMI->ptMinTrackSize.y = 200;
}


// ==========================================================================
void EventMonitorDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
    CDialog::OnShowWindow(bShow, nStatus);

    if ( bShow )
    {
        if ( mTimer == 0 )
        {
            mTimer = SetTimer( 1, 200, NULL );
        }
    }
    else
    {
        if ( mTimer != 0 )
        {
            KillTimer( 1 );
            mTimer = 0;
        }
    }
}


// ==========================================================================
void EventMonitorDlg::OnDestroy()
{
    CDialog::OnDestroy();

    if ( mTimer != 0 )
    {
        KillTimer( 1 );
        mTimer = 0;
    }
}


// ==========================================================================
void EventMonitorDlg::OnOK()
{
    // Do nothing
}


// ==========================================================================
void EventMonitorDlg::OnCancel()
{
    // This window life-cycle is managed from the outside
    ShowWindow( SW_HIDE );
}


// ==========================================================================
int EventMonitorDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if ( CDialog::OnCreate( lpCreateStruct ) == -1 )
    {
        return -1;
    }

    // This small change will hide the icon
    ModifyStyleEx( 0, WS_EX_DLGMODALFRAME );

    return 0;
}


// ==============================================================================
void EventMonitorDlg::OnBnClickedGenICamCheckBox()
{
	mLogBuffer->SetGenICamEnabled( mGenICamCheckBox.GetCheck() == BST_CHECKED );
}


// ==============================================================================
void EventMonitorDlg::OnBnClickedEventsCheckBox()
{
	mLogBuffer->SetEventsEnabled( mEventsCheckBox.GetCheck() == BST_CHECKED );
}


// ==============================================================================
void EventMonitorDlg::OnBnClickedBufferAllCheckBox()
{
	mLogBuffer->SetBufferAllEnabled( mBufferAllCheckBox.GetCheck() == BST_CHECKED );
}


// ==============================================================================
void EventMonitorDlg::OnBnClickedBufferErrorCheckBox()
{
	mLogBuffer->SetBufferErrorEnabled( mBufferErrorCheckBox.GetCheck() == BST_CHECKED );
}


// ==============================================================================
void EventMonitorDlg::OnBnClickedSaveLogCheckBox()
{
	mLogBuffer->SetWriteToFileEnabled( mSaveLogCheckBox.GetCheck() == BST_CHECKED );
	EnableInterface();
}


// ==============================================================================
void EventMonitorDlg::OnBnClickedSaveLogButton()
{
    CFileDialog lDlg( FALSE, NULL, mLogBuffer->GetFilename(), 
        OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, 
        _T( "Text files (*.txt)|All files (*.*)||" ), this );

    if ( lDlg.DoModal() == IDOK )
    {
        mLogBuffer->SetFilename( lDlg.GetPathName() );
    }

    LoadData();
}


// ==============================================================================
void EventMonitorDlg::EnableInterface()
{
	bool lEnabled = ( mSaveLogCheckBox.GetCheck() == BST_CHECKED );
    mSaveLogEdit.EnableWindow( false );
	mSaveLogButton.EnableWindow( lEnabled );
    
    if ( mShowSerialComLogCheck )
    {
        // we only want to enable this check box if it is possible to enable being notified
        // of serial communications between ip engine and camera head
        bool lEnabled = mLogBuffer->SetSerialComLogEnabled( mLogBuffer->IsSerialComLogEnabled(), mParameters );
        mSerialComLogCheck.EnableWindow( lEnabled );
        mSerialComLabel.EnableWindow( lEnabled );
    }
}


// ==============================================================================
void EventMonitorDlg::LoadData()
{
    mSerialComLogCheck.SetCheck( ( mLogBuffer->IsSerialComLogEnabled() ) ? BST_CHECKED : BST_UNCHECKED );
	mGenICamCheckBox.SetCheck( ( mLogBuffer->IsGenICamEnabled() ) ? BST_CHECKED : BST_UNCHECKED );
	mEventsCheckBox.SetCheck( ( mLogBuffer->IsEventsEnabled() ) ? BST_CHECKED : BST_UNCHECKED );
	mBufferAllCheckBox.SetCheck( ( mLogBuffer->IsBufferAllEnabled() ) ? BST_CHECKED : BST_UNCHECKED );
	mBufferErrorCheckBox.SetCheck( ( mLogBuffer->IsBufferErrorEnabled() ) ? BST_CHECKED : BST_UNCHECKED );
	mSaveLogCheckBox.SetCheck( ( mLogBuffer->IsWriteToFileEnabled() ) ? BST_CHECKED : BST_UNCHECKED );
    mSaveLogEdit.SetWindowText( mLogBuffer->GetFilename() );

	EnableInterface();
}


// ==============================================================================
void EventMonitorDlg::EnableGenICamMonitoring( bool aEnabled )
{
	std::map<CString, ParameterInfo>::iterator lIt = mInfoMap.begin();
	while ( lIt != mInfoMap.end() )
	{
		lIt->second.Enable( aEnabled );
		lIt++;
	}
}


// ==============================================================================
void EventMonitorDlg::OnBnClickedSerialComLogEnabled()
{
    mLogBuffer->SetSerialComLogEnabled( mSerialComLogCheck.GetCheck() == BST_CHECKED, mParameters );
}
