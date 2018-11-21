// DlgNoDriverWarning.cpp : implementation file
//

#include "stdafx.h"
#include "GEVPlayer.h"
#include "DlgNoDriverWarning.h"


// DlgNoDriverWarning dialog

IMPLEMENT_DYNAMIC(DlgNoDriverWarning, CDialog)

DlgNoDriverWarning::DlgNoDriverWarning(CWnd* pParent /*=NULL*/)
	: CDialog(DlgNoDriverWarning::IDD, pParent)
{

}

DlgNoDriverWarning::~DlgNoDriverWarning()
{
}

void DlgNoDriverWarning::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_DONT_SHOW_AGAIN, mDontShowAgainCheck);
}


BEGIN_MESSAGE_MAP(DlgNoDriverWarning, CDialog)
    ON_BN_CLICKED(IDOK, &DlgNoDriverWarning::OnBnClickedOk)
    ON_BN_CLICKED(IDC_DONT_SHOW_AGAIN, &DlgNoDriverWarning::OnBnClickedDontShowAgain)
END_MESSAGE_MAP()


// DlgNoDriverWarning message handlers

void DlgNoDriverWarning::OnBnClickedOk()
{
    if ( mDontShowAgainCheck.GetCheck() )
    {
        AfxGetApp()->WriteProfileInt(_T(""), _T("DontShowNoDriverWarning"), 1 );
    }

    OnOK();
}

void DlgNoDriverWarning::OnBnClickedDontShowAgain()
{
    // TODO: Add your control notification handler code here
}
