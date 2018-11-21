// *****************************************************************************
//
//     Copyright (c) 2008, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#pragma once


#include <PvBufferConverter.h>
#include <PvFilterRGB.h>
#include <PvConfigurationWriter.h>
#include <PvConfigurationReader.h>

class DisplayThread;


class FilteringDlg : public CDialog
{
public:

    FilteringDlg( CWnd* pParent = NULL );
    virtual ~FilteringDlg();

    // Persistence
    void Save( PvConfigurationWriter &aWriter );
    void Load( PvConfigurationReader &aReader );

    void SetDisplayThread( DisplayThread *aDisplayThread ) { mDisplayThread = aDisplayThread; }

    PvBayerFilterType GetBayerFilter() const { return mBayerFilter; }
    bool IsFilterRGBEnabled() const { return mFilterRGBEnabled; }

    void ApplyFilterRGB( PvBuffer *aBuffer );
    void ConfigureConverter( PvBufferConverter &aConverter );

    enum { IDD = IDD_FILTERING };

protected:

    CButton mRGBFilterCheckBox;
    CEdit mRGEdit;
    CEdit mGGEdit;
    CEdit mBGEdit;
    CEdit mROEdit;
    CEdit mGOEdit;
    CEdit mBOEdit;
    CSpinButtonCtrl mRGSpin;
    CSpinButtonCtrl mGGSpin;
    CSpinButtonCtrl mBGSpin;
    CSpinButtonCtrl mROSpin;
    CSpinButtonCtrl mGOSpin;
    CSpinButtonCtrl mBOSpin;
    CSliderCtrl mRGSlider;
    CSliderCtrl mGGSlider;
    CSliderCtrl mBGSlider;
    CSliderCtrl mROSlider;
    CSliderCtrl mGOSlider;
    CSliderCtrl mBOSlider;
    CButton mWBButton;
    CButton mResetButton;
    CComboBox mBayerCombo;
    CStatic mGainsLabel;
    CStatic mOffsetsLabel;
    CStatic mRedLabel;
    CStatic mGreenLabel;
    CStatic mBlueLabel;

    CFont mBoldFont;

    virtual void DoDataExchange(CDataExchange* pDX);
    BOOL PreTranslateMessage(MSG *pMsg);
    afx_msg void OnOK();
    afx_msg void OnCancel();
    BOOL OnInitDialog();
    afx_msg void OnBnClickedEnabledcheck();
    afx_msg void OnEnChange();
    afx_msg void OnBnClickedWbbutton();
    afx_msg void OnBnClickedResetbutton();
    afx_msg void OnCbnSelchangeBayercombo();
    afx_msg void OnDeltapos(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
    DECLARE_MESSAGE_MAP()

    DisplayThread *mDisplayThread;

    void EnableInterface();
    bool GetValueD( CEdit *aEdit, double &aValue );
    bool GetValueD( const CString &aStr, double &aValue );
    bool GetValueI( CEdit *aEdit, PvInt32 &aValue );
    bool GetValueI( const CString &aStr, PvInt32 &aValue );
    double FromGainSlider( CSliderCtrl *aSlider );
    void ToGainSlider( CSliderCtrl *aSlider, double &aValue );

    void ToDialog();
    void FromDialog();
    CString ToStr( double aValue );
    CString ToStr( PvInt32 aValue );
    void SyncSpins();
    void SyncSliders();

    PvFilterRGB mFilterRGB;
    bool mFilterRGBEnabled;
    PvBayerFilterType mBayerFilter;

    bool mLoading;
    int mSkipSyncSpinsCount;
    int mSkipSyncSlidersCount;
};
