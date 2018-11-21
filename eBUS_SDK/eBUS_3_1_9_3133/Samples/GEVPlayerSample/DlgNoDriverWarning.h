#pragma once
#include "afxwin.h"


// DlgNoDriverWarning dialog

class DlgNoDriverWarning : public CDialog
{
	DECLARE_DYNAMIC(DlgNoDriverWarning)

public:
	DlgNoDriverWarning(CWnd* pParent = NULL);   // standard constructor
	virtual ~DlgNoDriverWarning();

// Dialog Data
	enum { IDD = IDD_NO_DRIVER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedOk();
    CButton mDontShowAgainCheck;
    afx_msg void OnBnClickedDontShowAgain();
};
