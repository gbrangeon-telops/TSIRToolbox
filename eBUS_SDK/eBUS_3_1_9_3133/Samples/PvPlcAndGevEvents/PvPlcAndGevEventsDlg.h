// PvPlcAndGevEventsDlg.h : header file
//
#include <PvDeviceFinderWnd.h>
#include <PvDevice.h>
#include <PvGenParameter.h>
#include <PvGenBrowserWnd.h>
#include <PvBuffer.h>
#include <PvStream.h>
#include <PvPipeline.h>
#include <PvDisplayWnd.h>

#pragma once

//#define USE_FVAL

struct Str_shared
{
	PvPipeline*    pvPipeline;
	PvStream*      pvStream;
	PvDisplayWnd*  pvDisplayWnd;
};


// CPvPlcAndGevEventsDlg dialog
class CPvPlcAndGevEventsDlg : public CDialog, PvGenEventSink, PvDeviceEventSink
{
// Construction
public:
	CPvPlcAndGevEventsDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_PVPLCANDGEVEVENTS_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

// Implementation
protected:

	HICON m_hIcon;
	
	int					mWidth;
	int					mHeight;
	int					mpvPixelFormat;
    int                 miPixelSize;
	PvStream			mpvStream;
	PvPipeline*			mptrPvPipeline;
	PvDevice			mpvDevice;
	PvDisplayWnd		mpvDisplayWnd;
	PvGenBrowserWnd*	mDeviceWnd;
	bool                mbCallbackregistered;
	bool                mbInited;
	bool                mbTimerOn;
	int                 miNbOfCallback;
	int                 miCurCounterValue;
	Str_shared          margParam;
 	
	void OnParameterUpdate( PvGenParameter *aParameter );
    void OnEvent( PvDevice *aDevice, PvUInt16 aEventID, PvUInt16 aChannel, PvUInt64 aBlockID, 
        PvUInt64 aTimestamp, const void *aData, PvUInt32 aDataLength );
    void Disconnect();
	
	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedOk();
	afx_msg void OnTimer(UINT_PTR nIDEvent);

};
