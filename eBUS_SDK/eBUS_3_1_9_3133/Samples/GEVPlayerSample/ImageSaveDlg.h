#pragma once

#include "DisplayThread.h"
#include "afxwin.h"
// ImageSaveDlg dialog
#include <PvConfigurationReader.h>
#include <PvConfigurationWriter.h>
#include <PvBufferWriter.h>


typedef enum
{
    ONEOUTOF=0,
    MAXRATE=1,
    AVERAGETHROUGHPUT=2,
    NOTHROTTLE=3

} SaveThrottleOption;

typedef enum
{
    FORMAT_BMP = 0,
    FORMAT_RAW = 1

} FormatOption;


class ImageSaveDlg : public CDialog
{
    DECLARE_DYNAMIC( ImageSaveDlg )

public:

    ImageSaveDlg( FilteringDlg *aFilteringDlg, CWnd* pParent = NULL ); 
    virtual ~ImageSaveDlg();

    enum { IDD = IDD_IMAGE_SAVE_OPTIONS };

    bool GetSaveEnabled() const;
    void SetSaveEnabled( bool aEnabled );
    bool SaveThisImage();
    bool SaveIfNecessary( PvBuffer *aRawBuffer );
    void Save( PvConfigurationWriter& aWriter );
    void Load( PvConfigurationReader& lReader );
    void SetDisplayThread( DisplayThread * aDisplay ) { mDisplayThread = aDisplay; }
    void EnableInterface( bool aEnabled );

    void ResetStats();
    double GetFPS() { return mFPS; }
    double GetMbps() { return mMbps; }
    DWORD GetFrames() { return mFrames; }
    PvInt64 GetTotalSize() { return mTotalSize / 1048576; }

protected:

    virtual void DoDataExchange(CDataExchange* pDX);
    DECLARE_MESSAGE_MAP()

private:

    bool SaveImage( PvBuffer *aBuffer );

    afx_msg void OnBnClickedOk();
    afx_msg void OnBnClickedCancel();
    virtual BOOL OnInitDialog();
    afx_msg void OnBnClickedOneoutofRadio();
    afx_msg void OnBnClickedMaxrateRadio();
    afx_msg void OnBnClickedAveragethroughputRadio();
    afx_msg void OnBnClickedNothrottleRadio();
    afx_msg void OnBnClickedLocationButton();
    afx_msg void OnBnClickedImagesaveenable();
	bool ValidateImageFolder();

    bool FolderExists( const CString lSavePath );

    bool FolderPathIsEmpty( const CString &lSavePath );

    CEdit mOneOutOfEdit;
    CEdit mMaxRateEdit;
    CEdit mAverageThroughputEdit;
    CEdit mSavePathEdit;
    CButton mSaveEnabledCheck;
    CButton mLocationGroup;
    CButton mLocationButton;
    CButton mThrottlingGroup;
    CButton mOneOutOfRadio;
    CButton mMaxRateRadio;
    CButton mAverageThroughputRadio;
    CButton mNoThrottleRadio;
    CStatic mCapturedImagesLabel;
    CStatic mMsLabel;
    CStatic mAverageLabel;
    CComboBox mFormatCombo;

    bool mSaveEnabled;
    double mFPS;
    double mMbps;
    DWORD mFrames;
    PvInt64 mTotalSize;

    SaveThrottleOption mSaveThrottling;

    DWORD mOneOutOf;
    DWORD mMaxRate;
    DWORD mAverageThroughput;

    DWORD mStartTime;

    DWORD mCapturedSince;
    DWORD mPrevious;

    FormatOption mFormat;

    DisplayThread *mDisplayThread;
    FilteringDlg *mFilteringDlg;

    PvBufferWriter mBufferWriter;

    CString mSavePath;

    DWORD mCount;
};

