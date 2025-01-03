// *****************************************************************************
//
// Copyright (c) 2010, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#include "stdafx.h"

#include "AboutBox.h"
#include "eBUSPlayerDlg.h"

#include <PvVersion.h>


#ifdef _PT_DEBUG_
#pragma warning ( push )
#pragma warning ( disable : 4100 )
#endif // _PT_DEBUG_


BEGIN_MESSAGE_MAP( AboutBox, CDialog )
END_MESSAGE_MAP()


#ifdef _PT_DEBUG_
#pragma warning( pop )
#endif // _PT_DEBUG_


// ==========================================================================
AboutBox::AboutBox( CWnd* pParent /*=NULL*/ )   
    : CDialog( AboutBox::IDD, pParent )
{
}


// ==========================================================================
AboutBox::~AboutBox()
{
}

// =============================================================================
void AboutBox::DoDataExchange( CDataExchange* pDX )
{
    CDialog::DoDataExchange( pDX );
    DDX_Control(pDX, IDC_APPNAME, mAppNameLabel);
    DDX_Control(pDX, IDC_PRODUCTNAME, mProductNameLabel);
    DDX_Control(pDX, IDC_COPYRIGHT, mCopyrightLabel);
    DDX_Control(pDX, IDC_COMPANY, mCompanyLabel);
}

// ==========================================================================
BOOL AboutBox::OnInitDialog()
{
    CDialog::OnInitDialog();

    // Create bold font
    LOGFONT lLogFont;
    mAppNameLabel.GetFont()->GetLogFont( &lLogFont );
    lLogFont.lfWeight = FW_BOLD;
    mBoldFont.CreateFontIndirect( &lLogFont );

    mAppNameLabel.SetFont( &mBoldFont );

    CString lAppName = _T( "eBUS Player" );
    eBUSPlayerDlg *lDlg = dynamic_cast<eBUSPlayerDlg *>( GetParent() );
    if ( lDlg != NULL )
    {
        lAppName = lDlg->GetAppName();
    }

    SetWindowText( _T( "About " ) + lDlg->GetAppName() );
    mAppNameLabel.SetWindowText( lDlg->GetAppName() );
    mProductNameLabel.SetWindowText( CString( _T( "eBUS SDK Version " ) ) + CString( NVERSION_STRING ) );
    mCopyrightLabel.SetWindowText( _T( VERSION_COPYRIGHT ) );
    mCompanyLabel.SetWindowText( _T( VERSION_COMPANY_NAME ) );

    return TRUE;
}

