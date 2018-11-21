// *****************************************************************************
//
//     Copyright (c) 2009, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#pragma once

#include "afxwin.h"

#include <PvDevice.h>


class RegisterInterfaceDlg : public CDialog
{
public:

    RegisterInterfaceDlg( CWnd* pParent = NULL );
    virtual ~RegisterInterfaceDlg();

    void SetDevice( PvDevice *aDevice ) { mDevice = aDevice; }

protected:

    virtual void DoDataExchange(CDataExchange* pDX);
    BOOL OnInitDialog();
    void OnOK();
    void OnCancel();
    afx_msg void OnReadButtonClicked();
    afx_msg void OnWriteButtonClicked();
    DECLARE_MESSAGE_MAP()

    void EnableInterface();

    bool ReadValue( CEdit &aEdit, PvUInt32 &aValue );
    void WriteValue( PvUInt32 aValue, CEdit &aEdit );

private:

    PvDevice *mDevice;

    CEdit mReadAddressEdit;
    CEdit mReadValueEdit;
    CEdit mReadResultEdit;
    CEdit mWriteAddressEdit;
    CEdit mWriteValueEdit;
    CEdit mWriteResultEdit;
};


