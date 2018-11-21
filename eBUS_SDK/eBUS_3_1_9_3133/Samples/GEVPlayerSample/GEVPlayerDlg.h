// *****************************************************************************
//
//     Copyright (c) 2007, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#pragma once

#include "afxwin.h"

#include "ActionCommandWnd.h"
#include "DisplayThread.h"
#include "BitmapButton.h"
#include "EventMonitorDlg.h"
#include "FilteringDlg.h"
#include "SetupDlg.h"
#include "ImageSaveDlg.h"
#include "RegisterInterfaceDlg.h"
#include "LogBuffer.h"

#include <PvDeviceFinderWnd.h>
#include <PvDevice.h>
#include <PvGenParameter.h>
#include <PvGenBrowserWnd.h>
#include <PvGenBrowserTabbedWnd.h>
#include <PvBuffer.h>
#include <PvStream.h>
#include <PvPipeline.h>
#include <PvDisplayWnd.h>
#include <PvSerialBridgeManagerWnd.h>
#include <PvConfigurationReader.h>
#include <PvTerminalIPEngineWnd.h>
#include <PvAcquisitionStateManager.h>

#include <list>
#include <vector>

#include "afxcmn.h"


#define WM_IMAGEDISPLAYED ( WM_USER + 0x4431 )
#define WM_DISCONNECT ( WM_USER + 0x4432 )
#define WM_STREAM ( WM_USER + 0x4433 )
#define WM_STOPSTREAMING ( WM_USER + 0x4434 )
#define WM_LINKDISCONNECTED ( WM_USER + 0x4438 )
#define WM_LINKRECONNECTED ( WM_USER + 0x4439 )
#define WM_ACQUISITIONSTATECHANGED ( WM_USER + 0x4440 )
#define WM_UPDATESOURCE ( WM_USER + 0x4441 )
#define WM_UPDATESOURCES ( WM_USER + 0x4442 )
#define WM_UPDATEACQUISITIONMODE ( WM_USER + 0x4443 )


class PvGenStateStack;


class GEVPlayerDlg : public CDialog, PvGenEventSink, PvDeviceEventSink, PvAcquisitionStateEventSink, PvPipelineEventSink
{
public:

    GEVPlayerDlg( CString aFileName, CWnd *pParent = NULL );
    virtual ~GEVPlayerDlg();

    enum { IDD = IDD_GEVPLAYER };

    void StartStreaming();
    void StopStreaming();

    void AddDeviceParameterArray( PvGenParameterArray *aArray, const PvString &aName );
    void RemoveDeviceParameterArray( PvGenParameterArray *aArray );

    static bool IsMultiSourceTransmitter( PvDevice *aDevice );
    static DWORD ReadRegistryDWORD( const CString &aName );

protected:

    HICON m_hIcon;

    enum StatusColor { SCDefault, SCRed, SCYellow };

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
    LRESULT OnImageDisplayed( WPARAM wParam, LPARAM lParam );
    afx_msg void OnBnClickedStart();
    afx_msg void OnBnClickedStop();
    afx_msg void OnCbnSelchangeMode();
    afx_msg void OnBnClickedDeviceEvents();
    afx_msg void OnFileLoad();
    afx_msg void OnFileSave();
    afx_msg void OnFileExit();
    afx_msg void OnToolsEventMonitor();
    afx_msg void OnHelpAbout();
    afx_msg void OnInitMenu(CMenu* pMenu);
    afx_msg void OnMove(int x, int y);
    afx_msg void OnToolsDisplaysync();
    afx_msg void OnRecent(UINT nID);
    afx_msg void OnFileSaveAs();
    virtual BOOL PreTranslateMessage(MSG* pMsg);
    afx_msg void OnAcceleratorSave();
    afx_msg void OnAcceleratorOpen();
    afx_msg void OnToolsSetup();
    afx_msg void OnToolsImagefiltering();
    afx_msg void OnToolsSerialCommunicationBridge();
    afx_msg void OnToolsActionCommand();
    HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    LRESULT OnDisconnect( WPARAM wParam, LPARAM lParam );
    LRESULT OnStopStreaming( WPARAM wParam, LPARAM lParam );
    LRESULT OnStream( WPARAM wParam, LPARAM lParam );
    afx_msg void OnToolsSaveimages();
    afx_msg void OnToolsSavecurrentimage();
    afx_msg void OnToolsSerialCommunication();
    afx_msg void OnDestroy();
    afx_msg void OnTimer( UINT_PTR nIDEvent );
    afx_msg void OnRegisterInterface();
    afx_msg void OnDisplayDisabled();
    afx_msg void OnDisplay30FPS();
    afx_msg void OnDisplay60FPS();
    afx_msg void OnToolsSavepreferences();
    afx_msg void OnToolsRestoredefaultpreferences();
    afx_msg void OnToolsBufferoptions();
    afx_msg void OnToolsDisplaypartialimages();
    afx_msg void OnToolsDisplaychunkdata();
    afx_msg void OnToolsSaveXml();
    afx_msg void OnToolsDeinterlacingDisabled();
    afx_msg void OnToolsDeinterlacingWeaving();
    afx_msg void OnCbnSelchangeComboSource();
    virtual void OnOK() {}
    virtual void OnCancel() {}
    LRESULT OnLinkDisconnected( WPARAM wParam, LPARAM lParam );
    LRESULT OnLinkReconnected( WPARAM wParam, LPARAM lParam );
    LRESULT OnAcquisitionStateChanged( WPARAM wParam, LPARAM lParam );
    LRESULT OnUpdateSource( WPARAM wParam, LPARAM lParam );
    LRESULT OnUpdateSources( WPARAM wParam, LPARAM lParam );
    LRESULT OnUpdateAcquisitionMode( WPARAM wParam, LPARAM lParam );
    DECLARE_MESSAGE_MAP()

    void EnableInterface();
	void EnableTreeBrowsers( BOOL aEnabled );
	void EnableControls( BOOL aEnabled );

    void Connect( PvDeviceInfo *aDI, PvConfigurationReader *aCR );
    void Disconnect();
    void StartAcquisition();
    void StopAcquisition();
    PvUInt32 GetPayloadSize();

    void SaveDisplayOptions( PvConfigurationWriter& aWriter );
    void LoadDisplayOptions( PvConfigurationReader& aReader );

    void ShowGenWindow( PvGenBrowserWnd *aWnd, PvGenParameterArray *aParams, const CString &aTitle );
    void CloseGenWindow( PvWnd *aWnd );

    // PvGenEventSink implementation
    void OnParameterUpdate( PvGenParameter *aParameter );

    // PvDeviceEventSink implementation
    void OnLinkDisconnected( PvDevice *aDevice );
    void OnLinkReconnected( PvDevice *aDevice );
    void OnEvent( PvDevice *aDevice, PvUInt16 aEventID, PvUInt16 aChannel, PvUInt64 aBlockID, 
        PvUInt64 aTimestamp, const void *aData, PvUInt32 aDataLength );
    void OnEventGenICam( PvDevice *aDevice, PvUInt16 aEventID, PvUInt16 aChannel, PvUInt64 aBlockID, 
        PvUInt64 aTimestamp, PvGenParameterList *aData );

    // PvAcquisitionStateEventSink implementation
    void OnAcquisitionStateChanged( PvDevice* aDevice, PvStreamBase* aStream, PvUInt32 aSource, PvAcquisitionState aState );

	// PvPipelineEventSink implementation
	void OnBufferTooSmall( PvPipeline *aPipeline, bool *aReallocAll, bool *aResetStats );

    void OpenConfig( CString aLocation );
    void SaveConfig( CString aLocation, bool aSaveConnectedDevice );
    CString GetDefaultPath();
    CString GetStickyPath();

    void ReportMRU(CString aFileName);
    void SaveMRUToRegistry();
    void LoadMRUFromRegistry();
    void UpdateMRUMenu();
    void GetRecent( unsigned int aIndex );
    void SetStatusColor( StatusColor aColor );

    void PushSource( PvGenStateStack *aStack );
    LRESULT SendMessageIfPossible( UINT Msg, WPARAM wParam = 0, LPARAM lParam = 0 );

    std::list<CString> mRecentList;
    CString mFileName;
    CMenu* mMRUMenu;

    CRect mCrt;
    BOOL mNeedInit;

    CComboBox mModeCombo;
    BitmapButton mPlayButton;
    BitmapButton mStopButton;
    CEdit mStatusTextBox;

    CEdit mIPEdit;
    CEdit mMACEdit;
    CEdit mManufacturerEdit;
    CEdit mModelEdit;
    CEdit mNameEdit;

    HACCEL mAccel; // accelerator table

    PvDevice mDevice;
    PvStream mStream;
    PvPipeline mPipeline;

    StatusColor mStatusColor;

    DWORD mYellowColor;
    CBrush mYellowBrush;

    DWORD mRedColor;
    CBrush mRedBrush;

    bool mDeviceConnected;
    bool mSerial0IPEngineSupported;
	bool mPipelineReallocated;
    bool mTerminalIPEngineWndModeless;

    LogBuffer *mLogBuffer;
    DisplayThread *mDisplayThread;

    PvGenBrowserTabbedWnd mDeviceWnd;
    PvGenBrowserWnd mCommunicationWnd;
    PvGenBrowserWnd mStreamParametersWnd;
    PvTerminalIPEngineWnd mTerminalIPEngineWnd;
    PvSerialBridgeManagerWnd *mSerialBridgeManagerWnd;
    ActionCommandWnd *mActionCommandWnd;

    EventMonitorDlg *mEventMonitorDlg;
    FilteringDlg mFilteringDlg;
    SetupDlg mSetupDlg;
    ImageSaveDlg *mImageSaveDlg;
    RegisterInterfaceDlg mRegisterInterfaceDlg;

    PvDisplayWnd mDisplay;

    UINT_PTR mTimer;

    BOOL mRecovery;

    CComboBox mSourceCombo;

    PvAcquisitionStateManager* mAcquisitionStateManager;
    bool mShowAcquisitionWarning;

    // We keep this copy to refer to it from a non UI thread in OnParameterUpdate
    std::vector<PvInt64> mAvailableSources;

    // We use these flags to prevent re-entry in messages coming from OnParameterUpdate
    BOOL mUpdatingSources;
    BOOL mUpdatingAcquisitionMode;
};

