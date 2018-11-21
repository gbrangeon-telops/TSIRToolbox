// *****************************************************************************
//
//     Copyright (c) 2010, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#pragma once

#include "afxwin.h"

#include "Thread.h"
#include "Resource.h"

#include <PvDeviceFinderWnd.h>
#include <PvDevice.h>
#include <PvGenParameter.h>
#include <PvGenBrowserWnd.h>
#include <PvBuffer.h>
#include <PvStream.h>
#include <PvPipeline.h>
#include <PvDisplayWnd.h>

#include "afxcmn.h"


class PvCamHeadSerialComLogSampleDlg : public CDialog, PvGenEventSink, PvDeviceEventSink
{
public:

	PvCamHeadSerialComLogSampleDlg( CWnd *pParent = NULL );
	virtual ~PvCamHeadSerialComLogSampleDlg();

	enum { IDD = IDD_PVCAMHEADSERIALCOMLOGSAMPLE_DIALOG };

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
	afx_msg void OnMove(int x, int y);
    LRESULT OnUpdateSerialComLog( WPARAM wParam, LPARAM lParam );
    DECLARE_MESSAGE_MAP()

    void EnableInterface();

	void Connect( PvDeviceInfo *aDI );
	void Disconnect();
    void EnableSerialComLog( bool aEnable = true );
    void StartAcquisition();
    void StopAcquisition();

    void ShowGenWindow( PvGenBrowserWnd **aWnd, PvGenParameterArray *aParams, const CString &aTitle );
	void CloseGenWindow( PvGenBrowserWnd **aWnd );

    // PvGenEventSink implementation
	void OnParameterUpdate( PvGenParameter *aParameter );

    // PvDeviceEventSink implementation
    void OnEvent( PvDevice *aDevice, 
        PvUInt16 aEventID, PvUInt16 aChannel, PvUInt16 aBlockID, PvUInt64 aTimestamp, 
        const void *aData, PvUInt32 aDataLength );

    void ProcessEvent( PvUInt64 aTimestamp, const unsigned char *aData, PvUInt32 aDataLength );
    PvUInt32 FindNext( const unsigned char *aData, PvUInt32 aDataLength, const unsigned char *aPattern, PvUInt32 aPatternLength );
    CString Unpack( const char *aPattern, const unsigned char *aData, PvUInt32 aDataLength );

	CRect mCrt;
	BOOL mNeedInit;

    int mLogEditTextLength;
    CString mLogBuffer;
    CMutex mLogBufferMutex;
    CFont mTerminalFont;

	CComboBox mModeCombo;
	CComboBox mChannelCombo;
	CButton mPlayButton;
	CButton mStopButton;

	CEdit mIPEdit;
	CEdit mMACEdit;
	CEdit mManufacturerEdit;
	CEdit mModelEdit;
	CEdit mNameEdit;
	CEdit mSerialComLogEdit;

	PvDevice mDevice;
    PvStream mStream;
    PvPipeline mPipeline;

    bool mAcquiringImages;

	Thread *mThreadDisplay;

	PvGenBrowserWnd *mDeviceWnd;
	PvGenBrowserWnd *mCommunicationWnd;
	PvGenBrowserWnd *mStreamParametersWnd;

	PvDisplayWnd mDisplay;
};

