#pragma once
#include "afxwin.h"

#include <PvActionCommand.h>

#include "resource.h"

// Specialised CEdit to handle dec and hex
class CHexEdit : public CEdit
{
public:
    CHexEdit() {}
    virtual ~CHexEdit() {}

    void SetValue( PvUInt32 aValue, bool aHex );
    void SetValue( PvUInt64 aValue, bool aHex );

    PvUInt32 GetValueUInt32();
    PvUInt64 GetValueUInt64();

    void Cut();
    afx_msg void OnChar(UINT aChar, UINT aRepCnt, UINT aFlags);
    afx_msg LRESULT OnPaste( WPARAM awParam, LPARAM alParam );
    afx_msg void OnKillFocus( CWnd* pcNewReceiver );
    DECLARE_MESSAGE_MAP()

protected:
    void ValidateValues();
};

// ActionCommand dialog
class ActionCommandWnd : public CDialog
{
	DECLARE_DYNAMIC(ActionCommandWnd)

public:
	ActionCommandWnd(CWnd* pParent = NULL);   // standard constructor
	virtual ~ActionCommandWnd();

// Dialog Data
	enum { IDD = IDD_ACTIONCOMMAND };

private:
    PvActionCommand mActionCommand;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
    CHexEdit mDeviceKey;
    CHexEdit mGroupKey;
    CHexEdit mGroupMask;
    CHexEdit mScheduledTime;
    CButton mRequestAcknowledgesCheckBox;
    CButton mScheduled;
    CStatic mActionTimeLabel;
    CButton mSend;
    CListBox mAcknowledges;
    afx_msg void OnBnClickedCheckscheduled();
    afx_msg void OnBnClickedButtonsend();
    virtual BOOL OnInitDialog();
    CCheckListBox mInterfaces;
    virtual void OnOK();
    virtual void OnCancel();
};
