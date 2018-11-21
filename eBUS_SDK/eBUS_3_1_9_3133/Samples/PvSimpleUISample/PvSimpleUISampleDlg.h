// *****************************************************************************
//
//     Copyright (c) 2007, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#pragma once

#include "afxwin.h"

#include "DisplayThread.h"
#include "Resource.h"

#include <PvDeviceFinderWnd.h>
#include <PvDevice.h>
#include <PvGenParameter.h>
#include <PvGenBrowserWnd.h>
#include <PvBuffer.h>
#include <PvStream.h>
#include <PvPipeline.h>
#include <PvDisplayWnd.h>
#include <PvAcquisitionStateManager.h>


#include "afxcmn.h"


#define WM_ACQUISITIONSTATECHANGED ( WM_USER + 0x4440 )


class PvSimpleUISampleDlg : public CDialog, PvGenEventSink, PvAcquisitionStateEventSink
{
public:

	PvSimpleUISampleDlg( CWnd *pParent = NULL );
	virtual ~PvSimpleUISampleDlg();

	enum { IDD = IDD_PVSIMPLEUISAMPLE };

	void StartStreaming();
	void StopStreaming();

    bool IsAcquiringImages() const { return mAcquiringImages; }

protected:

	HICON m_hIcon;

	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnGetMinMaxInfo( MINMAXINFO *lpMMI );
	afx_msg void OnBnClickedDeviceButton();
	afx_msg void OnBnClickedLinkButton();
	afx_msg void OnBnClickedStreamparamsButton();
	afx_msg void OnBnClickedConnectButton();
	afx_msg void OnBnClickedDisconnectButton();
	afx_msg void OnClose();
	afx_msg void OnBnClickedStart();
	afx_msg void OnBnClickedStop();
	afx_msg void OnCbnSelchangeMode();
	afx_msg void OnBnClickedDeviceEvents();
	afx_msg void OnMove(int x, int y);
    LRESULT OnAcquisitionStateChanged(WPARAM wParam, LPARAM lParam);
    DECLARE_MESSAGE_MAP()

    void EnableInterface();

	void Connect( PvDeviceInfo *aDI );
	void Disconnect();
    void StartAcquisition();
    void StopAcquisition();

    void ShowGenWindow( PvGenBrowserWnd **aWnd, PvGenParameterArray *aParams, const CString &aTitle );
	void CloseGenWindow( PvGenBrowserWnd **aWnd );

    // PvGenEventSink implementation
	void OnParameterUpdate( PvGenParameter *aParameter );

    // PvAcquisitionStateEventSink implementation
    void OnAcquisitionStateChanged( PvDevice* aDevice, PvStreamBase* aStream, PvUInt32 aSource, PvAcquisitionState aState );

    PvUInt32 GetPayloadSize();

    CRect mCrt;
	BOOL mNeedInit;

	CComboBox mModeCombo;
	CComboBox mChannelCombo;
	CButton mPlayButton;
	CButton mStopButton;

	CEdit mIPEdit;
	CEdit mMACEdit;
	CEdit mManufacturerEdit;
	CEdit mModelEdit;
	CEdit mNameEdit;

	PvDevice mDevice;
    PvStream mStream;
    PvPipeline mPipeline;

    bool mAcquiringImages;

	DisplayThread *mDisplayThread;

	PvGenBrowserWnd *mDeviceWnd;
	PvGenBrowserWnd *mCommunicationWnd;
	PvGenBrowserWnd *mStreamParametersWnd;

	PvDisplayWnd mDisplay;

    PvAcquisitionStateManager *mAcquisitionStateManager;
};

