#pragma once
#include "afxwin.h"


// WarningDlg dialog

class WarningDlg : public CDialog
{
	DECLARE_DYNAMIC(WarningDlg)

public:
	WarningDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~WarningDlg();
    void SetWarning( const CString &aWarning );
    void SetCheckboxMessage( const CString &aMessage );
    bool IsChecked() { return mChecked; }

// Dialog Data
	enum { IDD = IDD_NO_DRIVER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedOk();
    CButton mDontShowAgainCheck;
    afx_msg void OnBnClickedDontShowAgain();
    CStatic mWarningMessage;

private:
    bool mChecked;
    CString mWarningMsg;
    CString mCheckboxMsg;
public:
    virtual BOOL OnInitDialog();
};
