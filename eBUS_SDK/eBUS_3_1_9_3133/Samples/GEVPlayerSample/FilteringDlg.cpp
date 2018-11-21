// *****************************************************************************
//
//     Copyright (c) 2008, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#include "stdafx.h"
#include "GEVPlayer.h"
#include "FilteringDlg.h"
#include "DisplayThread.h"

#include <math.h>


BEGIN_MESSAGE_MAP(FilteringDlg, CDialog)
    ON_BN_CLICKED(IDC_ENABLEDCHECK, &FilteringDlg::OnBnClickedEnabledcheck)
    ON_EN_CHANGE(IDC_RGEDIT, &FilteringDlg::OnEnChange)
    ON_EN_CHANGE(IDC_GGEDIT, &FilteringDlg::OnEnChange)
    ON_EN_CHANGE(IDC_BGEDIT, &FilteringDlg::OnEnChange)
    ON_EN_CHANGE(IDC_ROEDIT, &FilteringDlg::OnEnChange)
    ON_EN_CHANGE(IDC_GOEDIT, &FilteringDlg::OnEnChange)
    ON_EN_CHANGE(IDC_BOEDIT, &FilteringDlg::OnEnChange)
    ON_BN_CLICKED(IDC_WBBUTTON, &FilteringDlg::OnBnClickedWbbutton)
    ON_BN_CLICKED(IDC_RESETBUTTON, &FilteringDlg::OnBnClickedResetbutton)
    ON_CBN_SELCHANGE(IDC_BAYERCOMBO, &FilteringDlg::OnCbnSelchangeBayercombo)
    ON_NOTIFY(UDN_DELTAPOS, IDC_RGSPIN, &FilteringDlg::OnDeltapos)
    ON_NOTIFY(UDN_DELTAPOS, IDC_GGSPIN, &FilteringDlg::OnDeltapos)
    ON_NOTIFY(UDN_DELTAPOS, IDC_BGSPIN, &FilteringDlg::OnDeltapos)
    ON_WM_HSCROLL()
END_MESSAGE_MAP()


#define FILTERINGDLG_VERSION ( _T( "1.0.0.0" ) )

#define TAG_VERSION ( _T( "filteringdlgversion" ) )
#define TAG_FILTERRGBENABLED ( _T( "filterrgbenabled" ) )
#define TAG_REDGAIN ( _T( "redgain" ) )
#define TAG_GREENGAIN ( _T( "greengain" ) )
#define TAG_BLUEGAIN ( _T( "bluegain" ) )
#define TAG_REDOFFSET ( _T( "redoffset" ) )
#define TAG_GREENOFFSET ( _T( "greenoffset" ) )
#define TAG_BLUEOFFSET ( _T( "blueoffset" ) )
#define TAG_BAYERFILTERING ( _T( "bayerfiltering" ) )

#define VAL_TRUE ( _T( "true" ) )
#define VAL_FALSE ( _T( "false" ) )
#define VAL_BAYERSIMPLE ( _T( "simple" ) )
#define VAL_BAYER3X3 ( _T( "3x3" ) )


#define GAIN_TICK_COUNT ( 1000 )
#define GAIN_MAX ( 16 )


const double cNormalizer = log( static_cast<double>( GAIN_TICK_COUNT ) );


// =============================================================================
FilteringDlg::FilteringDlg( CWnd* pParent /* = NULL */ )
    : CDialog(FilteringDlg::IDD, pParent)
    , mDisplayThread( NULL )
    , mLoading( true )
    , mFilterRGBEnabled( false )
    , mBayerFilter( PvBayerFilterSimple )
    , mSkipSyncSpinsCount( 0 )
    , mSkipSyncSlidersCount( 0 )
{

}


// =============================================================================
FilteringDlg::~FilteringDlg()
{
}


// =============================================================================
void FilteringDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_ENABLEDCHECK, mRGBFilterCheckBox);
    DDX_Control(pDX, IDC_RGEDIT, mRGEdit);
    DDX_Control(pDX, IDC_GGEDIT, mGGEdit);
    DDX_Control(pDX, IDC_BGEDIT, mBGEdit);
    DDX_Control(pDX, IDC_ROEDIT, mROEdit);
    DDX_Control(pDX, IDC_GOEDIT, mGOEdit);
    DDX_Control(pDX, IDC_BOEDIT, mBOEdit);
    DDX_Control(pDX, IDC_RGSPIN, mRGSpin);
    DDX_Control(pDX, IDC_GGSPIN, mGGSpin);
    DDX_Control(pDX, IDC_BGSPIN, mBGSpin);
    DDX_Control(pDX, IDC_ROSPIN, mROSpin);
    DDX_Control(pDX, IDC_GOSPIN, mGOSpin);
    DDX_Control(pDX, IDC_BOSPIN, mBOSpin);
    DDX_Control(pDX, IDC_RGSLIDER, mRGSlider);
    DDX_Control(pDX, IDC_GGSLIDER, mGGSlider);
    DDX_Control(pDX, IDC_BGSLIDER, mBGSlider);
    DDX_Control(pDX, IDC_ROSLIDER, mROSlider);
    DDX_Control(pDX, IDC_GOSLIDER, mGOSlider);
    DDX_Control(pDX, IDC_BOSLIDER, mBOSlider);
    DDX_Control(pDX, IDC_WBBUTTON, mWBButton);
    DDX_Control(pDX, IDC_RESETBUTTON, mResetButton);
    DDX_Control(pDX, IDC_BAYERCOMBO, mBayerCombo);
    DDX_Control(pDX, IDC_GAINSLABEL, mGainsLabel);
    DDX_Control(pDX, IDC_OFFSETSLABEL, mOffsetsLabel);
    DDX_Control(pDX, IDC_REDLABEL, mRedLabel);
    DDX_Control(pDX, IDC_GREENLABEL, mGreenLabel);
    DDX_Control(pDX, IDC_BLUELABEL, mBlueLabel);
}


// =============================================================================
void FilteringDlg::OnOK()
{
    ShowWindow( SW_HIDE );
}


// =============================================================================
void FilteringDlg::OnCancel()
{
    ShowWindow( SW_HIDE );
}


// =============================================================================
BOOL FilteringDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    // Create bold font
    LOGFONT lLogFont;
    mRedLabel.GetFont()->GetLogFont( &lLogFont );
    lLogFont.lfWeight = FW_BOLD;
    mBoldFont.CreateFontIndirect( &lLogFont );

    mGainsLabel.SetFont( &mBoldFont );
    mOffsetsLabel.SetFont( &mBoldFont );
    mRedLabel.SetFont( &mBoldFont );
    mGreenLabel.SetFont( &mBoldFont );
    mBlueLabel.SetFont( &mBoldFont );

    mRGSpin.SetRange( 0, 2550 );
    mGGSpin.SetRange( 0, 2550 );
    mBGSpin.SetRange( 0, 2550 );

    mRGSlider.SetRange( 0, GAIN_TICK_COUNT );
    mGGSlider.SetRange( 0, GAIN_TICK_COUNT );
    mBGSlider.SetRange( 0, GAIN_TICK_COUNT );

    mROSpin.SetRange( -255, 255 );
    mGOSpin.SetRange( -255, 255 );
    mBOSpin.SetRange( -255, 255 );

    mROSlider.SetRange( 0, 510 );
    mGOSlider.SetRange( 0, 510 );
    mBOSlider.SetRange( 0, 510 );

    mBayerCombo.AddString( _T( "Simple" ) );
    mBayerCombo.AddString( _T( "3X3 Interpolation" ) );

    ToDialog();

    SyncSpins();
    SyncSliders();

    mLoading = false;
    mSkipSyncSpinsCount = 0;
    mSkipSyncSlidersCount = 0;

    return TRUE;
}


// =============================================================================
void FilteringDlg::EnableInterface()
{
    BOOL lEnabled = ( mRGBFilterCheckBox.GetCheck() == BST_CHECKED );

    mRGEdit.EnableWindow( lEnabled );
    mGGEdit.EnableWindow( lEnabled );
    mBGEdit.EnableWindow( lEnabled );
    mROEdit.EnableWindow( lEnabled );
    mGOEdit.EnableWindow( lEnabled );
    mBOEdit.EnableWindow( lEnabled );

    mRGSpin.EnableWindow( lEnabled );
    mGGSpin.EnableWindow( lEnabled );
    mBGSpin.EnableWindow( lEnabled );
    mROSpin.EnableWindow( lEnabled );
    mGOSpin.EnableWindow( lEnabled );
    mBOSpin.EnableWindow( lEnabled );

    mRGSlider.EnableWindow( lEnabled );
    mGGSlider.EnableWindow( lEnabled );
    mBGSlider.EnableWindow( lEnabled );
    mROSlider.EnableWindow( lEnabled );
    mGOSlider.EnableWindow( lEnabled );
    mBOSlider.EnableWindow( lEnabled );

    mWBButton.EnableWindow( lEnabled );
    mResetButton.EnableWindow( lEnabled );

    mOffsetsLabel.EnableWindow( lEnabled );
    mGainsLabel.EnableWindow( lEnabled );
    mRedLabel.EnableWindow( lEnabled );
    mGreenLabel.EnableWindow( lEnabled );
    mBlueLabel.EnableWindow( lEnabled );
}


// =============================================================================
void FilteringDlg::OnBnClickedEnabledcheck()
{
    FromDialog();
    EnableInterface();
}


// =============================================================================
void FilteringDlg::ToDialog()
{
    mLoading = true;
   
    // Filter RGB enabled?
    mRGBFilterCheckBox.SetCheck( 
        ( mFilterRGBEnabled ) ? 
            BST_CHECKED : 
            BST_UNCHECKED );

    // Gains
    mRGEdit.SetWindowText( ToStr( mFilterRGB.GetGainR() ) );
    mGGEdit.SetWindowText( ToStr( mFilterRGB.GetGainG() ) );
    mBGEdit.SetWindowText( ToStr( mFilterRGB.GetGainB() ) );

    // Offsets
    mROEdit.SetWindowText( ToStr( mFilterRGB.GetOffsetR() ) );
    mGOEdit.SetWindowText( ToStr( mFilterRGB.GetOffsetG() ) );
    mBOEdit.SetWindowText( ToStr( mFilterRGB.GetOffsetB() ) );

    // Bayer filtering
    switch ( mBayerFilter )
    {
        case PvBayerFilterSimple:
            mBayerCombo.SetCurSel( 0 );
            break;

        case PvBayerFilter3X3:
            mBayerCombo.SetCurSel( 1 );
            break;

        default:
            ASSERT( 0 );
    }

    mLoading = false;

    // Make sure UI is setup properly!
    EnableInterface();
}


// =============================================================================
void FilteringDlg::FromDialog()
{
    if ( mLoading )
    {
        return;
    }
    
    // Filter RGB enabled?
    bool lEnabled = ( mRGBFilterCheckBox.GetCheck() == BST_CHECKED );
    mFilterRGBEnabled = lEnabled;

    // Gain R
    double lValueD = 0.0;
    if ( GetValueD( &mRGEdit, lValueD ) )
    {
        mFilterRGB.SetGainR( lValueD );
    }

    // Gain G
    if ( GetValueD( &mGGEdit, lValueD ) )
    {
        mFilterRGB.SetGainG( lValueD );
    }

    // Gain B
    if ( GetValueD( &mBGEdit, lValueD ) )
    {
        mFilterRGB.SetGainB( lValueD );
    }

    // Offset R
    PvInt32 lValueI = 0;
    if ( GetValueI( &mROEdit, lValueI ) )
    {
        mFilterRGB.SetOffsetR( lValueI );
    }

    // Offset G
    if ( GetValueI( &mGOEdit, lValueI ) )
    {
        mFilterRGB.SetOffsetG( lValueI );
    }

    // Offset B
    if ( GetValueI( &mBOEdit, lValueI ) )
    {
        mFilterRGB.SetOffsetB( lValueI );
    }

    // Bayer filtering
    int lSelected = mBayerCombo.GetCurSel();
    switch ( lSelected )
    {
        case 0:
            mBayerFilter = PvBayerFilterSimple;
            break;

        case 1:
            mBayerFilter = PvBayerFilter3X3;
            break;

        default:
            ASSERT( 0 );
    }
}


// =============================================================================
bool FilteringDlg::GetValueD( CEdit *aEdit, double &aValue )
{
    CString lStr;
    aEdit->GetWindowText( lStr );

    return GetValueD( lStr, aValue );
}


// =============================================================================
bool FilteringDlg::GetValueD( const CString &aStr, double &aValue )
{
    int lCount = swscanf_s( (LPCTSTR)aStr, _T( "%lf" ), &aValue );

    return ( lCount == 1 );
}


// =============================================================================
bool FilteringDlg::GetValueI( CEdit *aEdit, PvInt32 &aValue )
{
    CString lStr;
    aEdit->GetWindowText( lStr );

    return GetValueI( lStr, aValue );
}


// =============================================================================
bool FilteringDlg::GetValueI( const CString &aStr, PvInt32 &aValue )
{
    int lCount = swscanf_s( (LPCTSTR)aStr, _T( "%i" ), &aValue );

    return ( lCount == 1 );
}


// =============================================================================
void FilteringDlg::OnEnChange()
{
    FromDialog();

    if ( mSkipSyncSpinsCount > 0 )
    {
        mSkipSyncSpinsCount--;
    }
    else
    {
        mSkipSyncSpinsCount = 0;
        SyncSpins();
    }

    if ( mSkipSyncSlidersCount > 0 )
    {
        mSkipSyncSlidersCount--;
    }
    else
    {
        mSkipSyncSlidersCount = 0;
        SyncSliders();
    }
}


// =============================================================================
CString FilteringDlg::ToStr( double aValue )
{
    CString lStr;
    lStr.Format( _T( "%.3f" ), aValue );

    return lStr;
}


// =============================================================================
CString FilteringDlg::ToStr( PvInt32 aValue )
{
    CString lStr;
    lStr.Format( _T( "%i" ), aValue );

    return lStr;
}


// =============================================================================
void FilteringDlg::OnBnClickedWbbutton()
{
    ASSERT( mDisplayThread != NULL );
    if ( mDisplayThread == NULL )
    {
        return;
    }

    mDisplayThread->WhiteBalance( &mFilterRGB );

    ToDialog();
}


// =============================================================================
void FilteringDlg::OnBnClickedResetbutton()
{
    mFilterRGB.Reset();
    ToDialog();
}


// =============================================================================
void FilteringDlg::OnCbnSelchangeBayercombo()
{
    FromDialog();
}


// =============================================================================
void FilteringDlg::ApplyFilterRGB( PvBuffer *aBuffer )
{
    mFilterRGB.Apply( aBuffer );
}


// =============================================================================
void FilteringDlg::SyncSpins()
{
    if ( ( mRGSpin.GetSafeHwnd() == NULL ) ||
         ( mGGSpin.GetSafeHwnd() == NULL ) ||
         ( mBGSpin.GetSafeHwnd() == NULL ) ||
         ( mROSpin.GetSafeHwnd() == NULL ) ||
         ( mGOSpin.GetSafeHwnd() == NULL ) ||
         ( mBOSpin.GetSafeHwnd() == NULL ) )
    {
        return;
    }

    double lValueD = 0.0;
    int lValueI = 0;
    int lLower, lUpper;

    // RGain
    if ( GetValueD( &mRGEdit, lValueD ) )
    {
        mRGSpin.GetRange32( lLower, lUpper );
        mRGSpin.SetPos( max( lLower, min( lUpper, static_cast<int>( lValueD * 10.0 + 0.5 ) ) ) );
    }

    // GGain
    if ( GetValueD( &mGGEdit, lValueD ) )
    {
        mGGSpin.GetRange32( lLower, lUpper );
        mGGSpin.SetPos( max( lLower, min( lUpper, static_cast<int>( lValueD * 10.0 + 0.5 ) ) ) );
    }

    // BGain
    if ( GetValueD( &mBGEdit, lValueD ) )
    {
        mBGSpin.GetRange32( lLower, lUpper );
        mBGSpin.SetPos( max( lLower, min( lUpper, static_cast<int>( lValueD * 10.0 + 0.5 ) ) ) );
    }

    // ROffset
    if ( GetValueI( &mROEdit, lValueI ) )
    {
        mROSpin.GetRange32( lLower, lUpper );
        mROSpin.SetPos( max( lLower, min( lUpper, lValueI ) ) );
    }

    // GOffset
    if ( GetValueI( &mGOEdit, lValueI ) )
    {
        mGOSpin.GetRange32( lLower, lUpper );
        mGOSpin.SetPos( max( lLower, min( lUpper, lValueI ) ) );
    }

    // BOffset
    if ( GetValueI( &mBOEdit, lValueI ) )
    {
        mBOSpin.GetRange32( lLower, lUpper );
        mBOSpin.SetPos( max( lLower, min( lUpper, lValueI ) ) );
    }
}


// =============================================================================
void FilteringDlg::SyncSliders()
{
    if ( ( mRGSlider.GetSafeHwnd() == NULL ) ||
         ( mGGSlider.GetSafeHwnd() == NULL ) ||
         ( mBGSlider.GetSafeHwnd() == NULL ) ||
         ( mROSlider.GetSafeHwnd() == NULL ) ||
         ( mGOSlider.GetSafeHwnd() == NULL ) ||
         ( mBOSlider.GetSafeHwnd() == NULL ) )
    {
        return;
    }

    double lValueD = 0.0;
    int lValueI = 0;
    int lLower, lUpper;

    // RGain
    if ( GetValueD( &mRGEdit, lValueD ) )
    {
        ToGainSlider( &mRGSlider, lValueD );
    }

    // GGain
    if ( GetValueD( &mGGEdit, lValueD ) )
    {
        ToGainSlider( &mGGSlider, lValueD );
    }

    // BGain
    if ( GetValueD( &mBGEdit, lValueD ) )
    {
        ToGainSlider( &mBGSlider, lValueD );
    }

    // ROffset
    if ( GetValueI( &mROEdit, lValueI ) )
    {
        mROSlider.GetRange( lLower, lUpper );
        mROSlider.SetPos( max( lLower, min( lUpper, lValueI + 255 ) ) );
    }

    // GOffset
    if ( GetValueI( &mGOEdit, lValueI ) )
    {
        mGOSlider.GetRange( lLower, lUpper );
        mGOSlider.SetPos( max( lLower, min( lUpper, lValueI + 255 ) ) );
    }

    // BOffset
    if ( GetValueI( &mBOEdit, lValueI ) )
    {
        mBOSlider.GetRange( lLower, lUpper );
        mBOSlider.SetPos( max( lLower, min( lUpper, lValueI + 255 ) ) );
    }
}


// =============================================================================
void FilteringDlg::OnDeltapos( NMHDR *pNMHDR, LRESULT *pResult )
{
    LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);

    int lNewVal = min( 2550, max( 0, pNMUpDown->iPos + pNMUpDown->iDelta ) );

    if ( pNMHDR->hwndFrom == mRGSpin.GetSafeHwnd() )
    {
        mSkipSyncSpinsCount++;
        mRGEdit.SetWindowText( ToStr( lNewVal / 10.0 ) );
    }
    else if ( pNMHDR->hwndFrom == mGGSpin.GetSafeHwnd() )
    {
        mSkipSyncSpinsCount++;
        mGGEdit.SetWindowText( ToStr( lNewVal / 10.0 ) );
    }
    else if ( pNMHDR->hwndFrom == mBGSpin.GetSafeHwnd() )
    {
        mSkipSyncSpinsCount++;
        mBGEdit.SetWindowText( ToStr( lNewVal / 10.0 ) );
    }

    *pResult = 0;
}


// =============================================================================
void FilteringDlg::OnHScroll( UINT nSBCode, UINT nPos, CScrollBar* pScrollBar )
{
    CDialog::OnHScroll( nSBCode, nPos, pScrollBar );

    if ( pScrollBar->GetSafeHwnd() == mRGSlider.GetSafeHwnd() )
    {
        mSkipSyncSlidersCount++;
        mRGEdit.SetWindowText( ToStr( FromGainSlider( &mRGSlider ) ) );
    }
    else if ( pScrollBar->GetSafeHwnd() == mGGSlider.GetSafeHwnd() )
    {
        mSkipSyncSlidersCount++;
        mGGEdit.SetWindowText( ToStr( FromGainSlider( &mGGSlider ) ) );
    }
    else if ( pScrollBar->GetSafeHwnd() == mBGSlider.GetSafeHwnd() )
    {
        mSkipSyncSlidersCount++;
        mBGEdit.SetWindowText( ToStr( FromGainSlider( &mBGSlider ) ) );
    }
    else if ( pScrollBar->GetSafeHwnd() == mROSlider.GetSafeHwnd() )
    {
        mSkipSyncSlidersCount++;
        int lPos = mROSlider.GetPos();
        mROEdit.SetWindowText( ToStr( lPos - 255 ) );
    }
    else if ( pScrollBar->GetSafeHwnd() == mGOSlider.GetSafeHwnd() )
    {
        mSkipSyncSlidersCount++;
        int lPos = mGOSlider.GetPos();
        mGOEdit.SetWindowText( ToStr( lPos - 255 ) );
    }
    else if ( pScrollBar->GetSafeHwnd() == mBOSlider.GetSafeHwnd() )
    {
        mSkipSyncSlidersCount++;
        int lPos = mBOSlider.GetPos();
        mBOEdit.SetWindowText( ToStr( lPos - 255 ) );
    }
}


// =============================================================================
double FilteringDlg::FromGainSlider( CSliderCtrl *aSlider )
{
    double lPos = min( static_cast<double>( GAIN_TICK_COUNT - 1 ), aSlider->GetPos() );
    double lLog = log( GAIN_TICK_COUNT - lPos );
    double lValue = GAIN_MAX - lLog / cNormalizer * GAIN_MAX;

    return lValue;
}


// =============================================================================
void FilteringDlg::ToGainSlider( CSliderCtrl *aSlider, double &aValue )
{
    double lV1 = ( static_cast<double>( GAIN_MAX ) - aValue ) / static_cast<double>( GAIN_MAX ) * cNormalizer;
    double lV2 = static_cast<double>( GAIN_TICK_COUNT ) - exp( lV1 );

    int lMin, lMax;
    aSlider->GetRange( lMin, lMax );

    int lValue = max( lMin, min( lMax, static_cast<int>( lV2 + 0.5 ) ) );

    aSlider->SetPos( lValue );
}


// =============================================================================
void FilteringDlg::Save( PvConfigurationWriter &aWriter )
{
    CString lStr;

    // Save a version string, just in case we need it in the future
    aWriter.Store( FILTERINGDLG_VERSION, TAG_VERSION );

    // RGB filter enabled
    aWriter.Store( mFilterRGBEnabled ? VAL_TRUE : VAL_FALSE, TAG_FILTERRGBENABLED );

    // Gains
    aWriter.Store( (LPCTSTR)ToStr( mFilterRGB.GetGainR() ), TAG_REDGAIN );
    aWriter.Store( (LPCTSTR)ToStr( mFilterRGB.GetGainG() ), TAG_GREENGAIN );
    aWriter.Store( (LPCTSTR)ToStr( mFilterRGB.GetGainB() ), TAG_BLUEGAIN );

    // Offsets
    aWriter.Store( (LPCTSTR)ToStr( mFilterRGB.GetOffsetR() ), TAG_REDOFFSET );
    aWriter.Store( (LPCTSTR)ToStr( mFilterRGB.GetOffsetG() ), TAG_GREENOFFSET );
    aWriter.Store( (LPCTSTR)ToStr( mFilterRGB.GetOffsetB() ), TAG_BLUEOFFSET );

    // Bayer filtering
    lStr = VAL_BAYERSIMPLE;
    switch ( mBayerFilter )
    {
        case PvBayerFilterSimple:
            lStr = VAL_BAYERSIMPLE;
            break;

        case PvBayerFilter3X3:
            lStr = VAL_BAYER3X3;
            break;

        default:
            ASSERT( 0 );
    }
    aWriter.Store( (LPCTSTR)lStr, TAG_BAYERFILTERING );
}


// =============================================================================
void FilteringDlg::Load( PvConfigurationReader &aReader )
{
    PvResult lResult;
    PvString lPvStr;
    double lValueD = 0.0;
    int lValueI = 0;

    // Always load from a blank setup!
    mFilterRGBEnabled = false;
    mFilterRGB.Reset();
    mBayerFilter = PvBayerFilterSimple;

    // RGB filter enabled
    lResult = aReader.Restore( PvString( TAG_FILTERRGBENABLED ), lPvStr );
    if ( lResult.IsOK() )
    {
        mFilterRGBEnabled = ( lPvStr == VAL_TRUE );
    }

    // Gains
    lResult = aReader.Restore( PvString( TAG_REDGAIN ), lPvStr );
    if ( lResult.IsOK() )
    {
        if ( GetValueD( lPvStr.GetUnicode(), lValueD ) )
        {
            mFilterRGB.SetGainR( lValueD );
        }
    }
    lResult = aReader.Restore( PvString( TAG_GREENGAIN ), lPvStr );
    if ( lResult.IsOK() )
    {
        if ( GetValueD( lPvStr.GetUnicode(), lValueD ) )
        {
            mFilterRGB.SetGainG( lValueD );
        }
    }
    lResult = aReader.Restore( PvString( TAG_BLUEGAIN ), lPvStr );
    if ( lResult.IsOK() )
    {
        if ( GetValueD( lPvStr.GetUnicode(), lValueD ) )
        {
            mFilterRGB.SetGainB( lValueD );
        }
    }

    // Offsets
    lResult = aReader.Restore( PvString( TAG_REDOFFSET ), lPvStr );
    if ( lResult.IsOK() )
    {
        if ( GetValueI( lPvStr.GetUnicode(), lValueI ) )
        {
            mFilterRGB.SetOffsetR( lValueI );
        }
    }
    lResult = aReader.Restore( PvString( TAG_GREENOFFSET ), lPvStr );
    if ( lResult.IsOK() )
    {
        if ( GetValueI( lPvStr.GetUnicode(), lValueI ) )
        {
            mFilterRGB.SetOffsetG( lValueI );
        }
    }
    lResult = aReader.Restore( PvString( TAG_BLUEOFFSET ), lPvStr );
    if ( lResult.IsOK() )
    {
        if ( GetValueI( lPvStr.GetUnicode(), lValueI ) )
        {
            mFilterRGB.SetOffsetB( lValueI );
        }
    }

    // Bayer filtering
    lResult = aReader.Restore( PvString( TAG_BAYERFILTERING ), lPvStr );
    if ( lResult.IsOK() )
    {
        if ( lPvStr == VAL_BAYERSIMPLE )
        {
            mBayerFilter = PvBayerFilterSimple;
        }
        else if ( lPvStr == VAL_BAYER3X3 )
        {
            mBayerFilter = PvBayerFilter3X3;
        }
    }

    ToDialog();
}


// =============================================================================
BOOL FilteringDlg::PreTranslateMessage(MSG *pMsg)
{
    // Flip up and down on horizontal scrollbars, see bug 1729
    HWND lFocus = GetFocus()->GetSafeHwnd();
    if ( ( lFocus == mRGSlider.GetSafeHwnd() ) ||
         ( lFocus == mGGSlider.GetSafeHwnd() ) ||
         ( lFocus == mBGSlider.GetSafeHwnd() ) ||
         ( lFocus == mROSlider.GetSafeHwnd() ) ||
         ( lFocus == mGOSlider.GetSafeHwnd() ) ||
         ( lFocus == mBOSlider.GetSafeHwnd() ) )
    {
        if ( ( pMsg->message == WM_KEYDOWN ) || ( pMsg->message == WM_KEYUP ) )
        {
            switch ( pMsg->wParam )
            {
                case VK_UP:
                    pMsg->wParam = VK_DOWN;
                    break;

                case VK_DOWN:
                    pMsg->wParam = VK_UP;
                    break;

                case VK_NEXT:
                    pMsg->wParam = VK_PRIOR;
                    break;

                case VK_PRIOR:
                    pMsg->wParam = VK_NEXT;
                    break;
            }
        }
    }

    return CDialog::PreTranslateMessage( pMsg );
}


// =============================================================================
void FilteringDlg::ConfigureConverter( PvBufferConverter &aConverter )
{
    aConverter.SetBayerFilter( GetBayerFilter() );
    if ( IsFilterRGBEnabled() )
    {
        aConverter.SetRGBFilter( mFilterRGB );
    }
    else
    {
        aConverter.ResetRGBFilter();
    }
}



