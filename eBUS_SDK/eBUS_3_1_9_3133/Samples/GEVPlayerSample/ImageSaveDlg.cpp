// ImageSaveDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GEVPlayer.h"
#include "ImageSaveDlg.h"
#include "FilteringDlg.h"


// ImageSaveDlg dialog

IMPLEMENT_DYNAMIC(ImageSaveDlg, CDialog)

ImageSaveDlg::ImageSaveDlg( FilteringDlg *aFilteringDlg, CWnd* pParent /*=NULL*/ )
    : CDialog(ImageSaveDlg::IDD, pParent)
    , mDisplayThread( NULL )
    , mSaveThrottling( MAXRATE )
    , mOneOutOf( 1 )
    , mMaxRate( 100 )
    , mAverageThroughput( 1 )
    , mCapturedSince( 0 )
    , mPrevious( 0 )
    , mSaveEnabled( false )
    , mStartTime( 0 )
    , mFrames( 0 )
    , mFPS( 0 )
    , mMbps( 0 )
    , mTotalSize( 0 )
    , mFormat( FORMAT_BMP )
    , mFilteringDlg( aFilteringDlg )
    , mCount( 0 )
{
    wchar_t myPictures[MAX_PATH];
    SHGetSpecialFolderPath(NULL, myPictures,CSIDL_MYPICTURES,true);

    mSavePath = myPictures;

}

ImageSaveDlg::~ImageSaveDlg()
{
}

void ImageSaveDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_ONEOUTOF, mOneOutOfEdit);
    DDX_Control(pDX, IDC_MAXRATE, mMaxRateEdit);
    DDX_Control(pDX, IDC_AVERAGETHROUGHPUT, mAverageThroughputEdit);
    DDX_Control(pDX, IDC_LOCATION, mSavePathEdit);
    DDX_Control(pDX, IDC_IMAGESAVEENABLE, mSaveEnabledCheck);
    DDX_Control(pDX, IDC_LOCATIONGROUP, mLocationGroup);
    DDX_Control(pDX, IDC_LOCATION_BUTTON, mLocationButton);
    DDX_Control(pDX, IDC_THROTTLINGGROUP, mThrottlingGroup);
    DDX_Control(pDX, IDC_ONEOUTOF_RADIO, mOneOutOfRadio);
    DDX_Control(pDX, IDC_MAXRATE_RADIO, mMaxRateRadio);
    DDX_Control(pDX, IDC_AVERAGETHROUGHPUT_RADIO, mAverageThroughputRadio);
    DDX_Control(pDX, IDC_NOTHROTTLE_RADIO, mNoThrottleRadio);
    DDX_Control(pDX, IDC_CAPTUREDIMAGESLABEL, mCapturedImagesLabel);
    DDX_Control(pDX, IDC_MSLABEL, mMsLabel);
    DDX_Control(pDX, IDC_AVERAGELABEL, mAverageLabel);
    DDX_Control(pDX, IDC_FORMAT, mFormatCombo);
}


BEGIN_MESSAGE_MAP(ImageSaveDlg, CDialog)
    ON_BN_CLICKED(IDOK, &ImageSaveDlg::OnBnClickedOk)
    ON_BN_CLICKED(IDCANCEL, &ImageSaveDlg::OnBnClickedCancel)
    ON_BN_CLICKED(IDC_ONEOUTOF_RADIO, &ImageSaveDlg::OnBnClickedOneoutofRadio)
    ON_BN_CLICKED(IDC_MAXRATE_RADIO, &ImageSaveDlg::OnBnClickedMaxrateRadio)
    ON_BN_CLICKED(IDC_AVERAGETHROUGHPUT_RADIO, &ImageSaveDlg::OnBnClickedAveragethroughputRadio)
    ON_BN_CLICKED(IDC_NOTHROTTLE_RADIO, &ImageSaveDlg::OnBnClickedNothrottleRadio)
    ON_BN_CLICKED(IDC_LOCATION_BUTTON, &ImageSaveDlg::OnBnClickedLocationButton)
    ON_BN_CLICKED(IDC_IMAGESAVEENABLE, &ImageSaveDlg::OnBnClickedImagesaveenable)
END_MESSAGE_MAP()


// ImageSaveDlg message handlers

void ImageSaveDlg::OnBnClickedOk()
{   
    CString lStrOneOutOf;
    CString lStrAverageThroughput;
    CString lStrMaxRate;
    switch( GetCheckedRadioButton( IDC_ONEOUTOF_RADIO, IDC_NOTHROTTLE_RADIO ) )
    {
        case IDC_ONEOUTOF_RADIO:
            mSaveThrottling = ONEOUTOF;
            mOneOutOfEdit.GetWindowTextW( lStrOneOutOf );
            mOneOutOf = _wtoi(lStrOneOutOf.GetBuffer());
            break;
        case IDC_MAXRATE_RADIO:
            mSaveThrottling = MAXRATE;
            mMaxRateEdit.GetWindowTextW( lStrMaxRate );
            mMaxRate = _wtoi(lStrMaxRate.GetBuffer());
            break;
        case IDC_AVERAGETHROUGHPUT_RADIO:
            mSaveThrottling = AVERAGETHROUGHPUT;
            mAverageThroughputEdit.GetWindowTextW( lStrAverageThroughput );
            mAverageThroughput = _wtoi(lStrAverageThroughput.GetBuffer());
            break;
        case IDC_NOTHROTTLE_RADIO:
            mSaveThrottling = NOTHROTTLE;
            break;
        default:
            ASSERT(FALSE);
            break;
    }

    CString lPath;
    mSavePathEdit.GetWindowTextW( lPath );
    mSavePath = lPath.GetBuffer();

    if ( mSaveEnabledCheck.GetCheck() == BST_CHECKED )
    {
		if( !ValidateImageFolder() )
		{
			return;
		}
        SetSaveEnabled( true ); 
    }
    else
    {
        SetSaveEnabled( false );
    }

    switch ( mFormatCombo.GetCurSel() )
    {
    default:
        ASSERT( 0 );

    case 0:
        mFormat = FORMAT_BMP;
        break;

    case 1:
        mFormat = FORMAT_RAW;
        break;
    }

    CDialog::OnOK();
}

void ImageSaveDlg::OnBnClickedCancel()
{
    CDialog::OnCancel();
}

BOOL ImageSaveDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    mFormatCombo.AddString( _T( "Device independent bitmap (BMP)" ) );
    mFormatCombo.AddString( _T( "Raw binary data" ) );

    switch ( mFormat )
    {
    default:
        ASSERT( 0 );

    case FORMAT_BMP:
        mFormatCombo.SetCurSel( 0 );
        break;

    case FORMAT_RAW:
        mFormatCombo.SetCurSel( 1 );
        break;
    }

    wchar_t lOneOutOf[256];
    _itow_s(mOneOutOf, lOneOutOf, 256, 10);
    mOneOutOfEdit.SetWindowTextW( lOneOutOf );
    wchar_t lMaxRate[256];
    _itow_s(mMaxRate, lMaxRate, 256, 10);
    mMaxRateEdit.SetWindowTextW( lMaxRate );
    wchar_t lAverageThroughput[256];
    _itow_s(mAverageThroughput, lAverageThroughput, 256, 10);
    mAverageThroughputEdit.SetWindowTextW( lAverageThroughput );
    switch ( mSaveThrottling )
    {
    default:
        ASSERT( 0 );

    case ONEOUTOF:
        CheckRadioButton( IDC_ONEOUTOF_RADIO, IDC_NOTHROTTLE_RADIO, IDC_ONEOUTOF_RADIO );
        mOneOutOfEdit.EnableWindow( TRUE );
        break;
    case MAXRATE:
        CheckRadioButton( IDC_ONEOUTOF_RADIO, IDC_NOTHROTTLE_RADIO, IDC_MAXRATE_RADIO );
        mMaxRateEdit.EnableWindow( TRUE );
        break;
    case AVERAGETHROUGHPUT:
        CheckRadioButton( IDC_ONEOUTOF_RADIO, IDC_NOTHROTTLE_RADIO, IDC_AVERAGETHROUGHPUT_RADIO );
        mAverageThroughputEdit.EnableWindow( TRUE );
        break;
    case NOTHROTTLE:
        CheckRadioButton( IDC_ONEOUTOF_RADIO, IDC_NOTHROTTLE_RADIO, IDC_NOTHROTTLE_RADIO );
        break;
    }

    mSavePathEdit.SetWindowTextW( mSavePath );

    mSaveEnabledCheck.SetCheck( mSaveEnabled );
    EnableInterface( mSaveEnabled );

    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

void ImageSaveDlg::OnBnClickedOneoutofRadio()
{
    mOneOutOfEdit.EnableWindow( TRUE );
    mMaxRateEdit.EnableWindow( FALSE );
    mAverageThroughputEdit.EnableWindow( FALSE );
}

void ImageSaveDlg::OnBnClickedMaxrateRadio()
{
    mOneOutOfEdit.EnableWindow( FALSE );
    mMaxRateEdit.EnableWindow( TRUE );
    mAverageThroughputEdit.EnableWindow( FALSE );
}

void ImageSaveDlg::OnBnClickedAveragethroughputRadio()
{
    mOneOutOfEdit.EnableWindow( FALSE );
    mMaxRateEdit.EnableWindow( FALSE );
    mAverageThroughputEdit.EnableWindow( TRUE );
}

void ImageSaveDlg::OnBnClickedNothrottleRadio()
{
    mOneOutOfEdit.EnableWindow( FALSE );
    mMaxRateEdit.EnableWindow( FALSE );
    mAverageThroughputEdit.EnableWindow( FALSE );
}

void ImageSaveDlg::OnBnClickedLocationButton()
{
    BROWSEINFO bi = { 0 };
    bi.lpszTitle = _T("Save images to folder");
    bi.hwndOwner = GetSafeHwnd();
    wchar_t lPath[512];

    LPITEMIDLIST pidl = SHBrowseForFolder ( &bi );
    if ( pidl != 0 )
    {
        // get the name of the folder
        if ( SHGetPathFromIDList ( pidl, lPath ) )
        {
            mSavePathEdit.SetWindowTextW( lPath );
        }

        // free memory used
        IMalloc * imalloc = 0;
        if ( SUCCEEDED( SHGetMalloc ( &imalloc )) )
        {
            imalloc->Free ( pidl );
            imalloc->Release ( );
        }
    }
}

// private
bool ImageSaveDlg::SaveImage( PvBuffer* aBuffer )
{
    CString lExt;
    PvBufferFormatType lType;

    switch ( mFormat )
    {
    default:
        ASSERT( 0 );

    case FORMAT_BMP:
        lExt = _T( ".bmp" );
        lType = PvBufferFormatBMP;
        break;

    case FORMAT_RAW:
        lExt = _T( ".bin" );
        lType = PvBufferFormatRaw;
        break;
    }

    wchar_t lFileName[MAX_PATH];
    swprintf_s( lFileName, MAX_PATH, L"%s\\%08d_%016I64X_%d%s", mSavePath.GetBuffer(), mCount++,aBuffer->GetTimestamp(), ::GetTickCount(), lExt );

    mFilteringDlg->ConfigureConverter( mBufferWriter.GetConverter() );
    mBufferWriter.Store( aBuffer, lFileName, lType );

    DWORD lDelta = ::GetTickCount() - mStartTime;
    mFrames++;

    mFPS = (double) mFrames * 1000 / (double) lDelta;

    // 1048576 bits per Mbit
	PvUInt32 lBytesPerImage = aBuffer->GetAcquiredSize();
    mMbps = static_cast<double>( lBytesPerImage ) * 8 * mFPS / 1048576.0;
    mTotalSize += lBytesPerImage;

    return true;
}

bool ImageSaveDlg::GetSaveEnabled() const
{
    return mSaveEnabled; 
}

void ImageSaveDlg::SetSaveEnabled( bool aEnabled )
{
    mSaveEnabled = aEnabled;
}

// called from GEVPlayerDlg
bool ImageSaveDlg::SaveThisImage()
{
    ASSERT( mDisplayThread != NULL );
    if ( mDisplayThread == NULL )
    {
        return false;
    }

    bool lResult = false;

    PvBuffer* lBuffer = mDisplayThread->RetrieveLatestBuffer();
    if ( lBuffer != NULL )
    {
        lResult = SaveImage( lBuffer );
    }
    mDisplayThread->ReleaseLatestBuffer();

    return lResult;
}

// Called from ThreadDisplay
bool ImageSaveDlg::SaveIfNecessary( PvBuffer *aRawBuffer )
{
    if ( !mSaveEnabled )
    {
        return false;
    }

    bool lSaveThisOne = false;
    double lBitsPerImage;
    double lBitsPerMs;
    DWORD lCurrent;
    switch( mSaveThrottling )
    {
    case ONEOUTOF:
        // 1 image every mOneOf captured images
        mCapturedSince++;
        if ( mCapturedSince >= mOneOutOf )
        {
            lSaveThisOne = true;
            mCapturedSince = 0;
        }      
        break;
    case MAXRATE:
        // maximum of one out of every mMaxRate ms
        lCurrent = ::GetTickCount();
        if ( lCurrent - mPrevious >= mMaxRate )
        {
            lSaveThisOne = true;
            mPrevious = lCurrent;
        }
        break;
    case AVERAGETHROUGHPUT:
        // maintain mAverageThroughput Mbits/s average 
        lBitsPerImage = aRawBuffer->GetAcquiredSize() * 8;
        lBitsPerMs = mAverageThroughput * 1048.576;
        lCurrent = ::GetTickCount();
        if ( lCurrent - mPrevious >= lBitsPerImage / lBitsPerMs )
        {
            lSaveThisOne = true;
            mPrevious = lCurrent;
        }
        break;
    case NOTHROTTLE:
        lSaveThisOne = true;
        break;
    default:
        ASSERT(FALSE);
        break;

    }

    if ( lSaveThisOne )
    {
        return SaveImage( aRawBuffer );
    }

    return false;
}


void ImageSaveDlg::Save( PvConfigurationWriter &aWriter )
{
    aWriter.Store( GetSaveEnabled() ? "1" : "0", "saveenabled" );
    
    char lOneOutOf[256];
    _itoa_s(mOneOutOf, lOneOutOf, 256, 10);
    char lMaxRate[256];
    _itoa_s(mMaxRate, lMaxRate, 256, 10);
    char lAverageThroughput[256];
    _itoa_s(mAverageThroughput, lAverageThroughput, 256, 10);
    char lSaveThrottleOption[256];
    _itoa_s((int)mSaveThrottling, lSaveThrottleOption, 256, 10);

    aWriter.Store( lOneOutOf, "oneoutof" );
    aWriter.Store( lMaxRate, "maxrate" );
    aWriter.Store( lAverageThroughput, "averagethroughput" );
    aWriter.Store( lSaveThrottleOption, "savethrottleoption" );

    

    aWriter.Store( mSavePath.GetBuffer(), "savepath" );
    aWriter.Store( mFormat == 0 ? "bmp" : "raw", "saveformat" );
}

void ImageSaveDlg::Load( PvConfigurationReader &aReader )
{
    PvString lSaveEnabled;
    aReader.Restore(PvString("saveenabled"), lSaveEnabled);
    if (strcmp(lSaveEnabled.GetAscii(),"1") == 0 )
    {
        mSaveEnabled = true;
    }
    else
    {
        mSaveEnabled = false;
    }

    PvString lOneOutOf;
    aReader.Restore( PvString( "oneoutof" ), lOneOutOf );
    mOneOutOf = atoi( lOneOutOf.GetAscii() );

    PvString lMaxRate;
    aReader.Restore( PvString( "maxrate" ), lMaxRate );
    mMaxRate = atoi( lMaxRate.GetAscii() );

    PvString lAverageThroughput;
    aReader.Restore( PvString( "averagethroughput" ), lAverageThroughput );
    mAverageThroughput = atoi(lAverageThroughput.GetAscii());

    PvString lSaveThrottleOption;
    aReader.Restore( PvString( "savethrottleoption" ), lSaveThrottleOption );
    mSaveThrottling = (SaveThrottleOption)atoi( lSaveThrottleOption.GetAscii() );

    PvString lSavePath;
    aReader.Restore( PvString( "savepath" ), lSavePath );
    mSavePath = lSavePath.GetUnicode();

    PvString lFormat;
    aReader.Restore( PvString( "saveformat" ), lFormat );
    CString lStr( lFormat.GetUnicode() );
    CString lLower = lStr.MakeLower();
    if ( lLower == _T( "bmp" ) )
    { 
        mFormat = FORMAT_BMP;
    }
    else if ( lLower == _T( "raw" ) )
    {
        mFormat = FORMAT_RAW;
    }
    else
    {
        // Default. Can be expected as the user can end up manually
        // editing the persistence file.
        mFormat = FORMAT_BMP;
    }
}


void ImageSaveDlg::OnBnClickedImagesaveenable()
{
    EnableInterface( mSaveEnabledCheck.GetCheck() == BST_CHECKED );
} 

void ImageSaveDlg::EnableInterface( bool aEnabled )
{
    mOneOutOfEdit.EnableWindow( aEnabled );
    mMaxRateEdit.EnableWindow( aEnabled );
    mAverageThroughputEdit.EnableWindow( aEnabled );
    mSavePathEdit.EnableWindow( aEnabled );
    mLocationGroup.EnableWindow( aEnabled );
    mLocationButton.EnableWindow( aEnabled );
    mThrottlingGroup.EnableWindow( aEnabled );
    mOneOutOfRadio.EnableWindow( aEnabled );
    mMaxRateRadio.EnableWindow( aEnabled );
    mAverageThroughputRadio.EnableWindow( aEnabled );
    mNoThrottleRadio.EnableWindow( aEnabled );
    mCapturedImagesLabel.EnableWindow( aEnabled );
    mMsLabel.EnableWindow( aEnabled );
    mAverageLabel.EnableWindow( aEnabled );
    mFormatCombo.EnableWindow( aEnabled );
}

void ImageSaveDlg::ResetStats()
{
    mFPS = 0.0;
    mMbps = 0.0;
    mFrames = 0;
    mTotalSize = 0;
    mStartTime = ::GetTickCount();
}

bool ImageSaveDlg::ValidateImageFolder()
{
    CString lSavePath;
	mSavePathEdit.GetWindowText( lSavePath );
    if( FolderPathIsEmpty( lSavePath ) )
    {
        return false;
    }
    
    return FolderExists( lSavePath );   
}

bool ImageSaveDlg::FolderPathIsEmpty( const CString &lSavePath )
{
    if( lSavePath.IsEmpty() )
    {
        AfxMessageBox( L"Location to save images is empty.  Please insert a valid path.", MB_ICONERROR);
        return true;
    }

    return false;
}

bool ImageSaveDlg::FolderExists( const CString lSavePath )
{
    if( !PathFileExists( lSavePath ) )
    {
        CString lErrorMsg;
        lErrorMsg.Format( L"The path \"%s\" does not exist.  Please insert a valid path.", lSavePath );
        AfxMessageBox( lErrorMsg, MB_ICONERROR);
        return false;

    }

    return true;
}

