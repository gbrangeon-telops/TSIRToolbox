// *****************************************************************************
//
//     Copyright (c) 2008, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#pragma once

#include "afxwin.h"
#include "Resource.h"

#include "Thread.h"

#include <PvStream.h>
#include <PvDevice.h>
#include <PvConfigurationReader.h>




class ProgressDlg : public CDialog
{
    DECLARE_DYNAMIC(ProgressDlg)

public:

    ProgressDlg( Thread *aThread, CWnd* aParentWnd = NULL );
    virtual ~ProgressDlg();

    virtual INT_PTR DoModal();

    void SetStatus( CString aStatus );

    void SetTitle( const CString &aTitle ) { mTitle = aTitle; }

    enum { IDD = IDD_OPENSTREAM };

protected:

    virtual void DoDataExchange(CDataExchange* pDX);
    void OnOK();
    void OnCancel();
    BOOL OnInitDialog();
    afx_msg void OnTimer( UINT_PTR nIDEvent );
    afx_msg void OnDestroy();
    DECLARE_MESSAGE_MAP()

    void Update();

    CStatic mStatusLabel;
    CBitmap mWheelBitmap;

    UINT_PTR mTimer;

    DWORD mWheelIndex;

    CString mTitle;

private:

    Thread *mThread;
    PvResult mResult;
    CString mStatus;
    CMutex mMutex;

};

