// *****************************************************************************
//
//     Copyright (c) 2012, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#include "stdafx.h"
#include "GEVPlayer.h"
#include "BufferOptionsDlg.h"
#include "GEVPlayerDlg.h"

#include <PvConfigurationReader.h>
#include <PvConfigurationWriter.h>


#define BUFFER_COUNT_DEFAULT ( 16 );
#define WARNING_MEM_THRESHOLD ( 1 * 1024 * 1024 * 1024 ) // 1 GB


#define BUFFEROPTIONSDLG_VERSION ( "1.0.0.0" )

#define TAG_VERSION ( "bufferoptionsdlgversion" )
#define TAG_STREAMINGBUFFERCOUNT ( "streamingbuffercount" )
#define TAG_STREAMINGDEFAULTBUFFERSIZE ( "streamingdefaultbuffersize" )
#define TAG_AUTORESIZE ( "streamingautoresize" )


IMPLEMENT_DYNAMIC(BufferOptionsDlg, CDialog)

BEGIN_MESSAGE_MAP(BufferOptionsDlg, CDialog)
    ON_BN_CLICKED(IDOK, &BufferOptionsDlg::OnBnClickedOk)
END_MESSAGE_MAP()


///
/// \brief Constructor.
///

BufferOptionsDlg::BufferOptionsDlg( GEVPlayerDlg *aGEVPlayerDlg, PvPipeline *aPipeline )
	: CDialog( BufferOptionsDlg::IDD, aGEVPlayerDlg )
    , mPipeline( aPipeline )
	, mGEVPlayerDlg( aGEVPlayerDlg )
{

}


///
/// \brief Destructor.
///

BufferOptionsDlg::~BufferOptionsDlg()
{
}


///
/// \brief Dialog objects binding
///

void BufferOptionsDlg::DoDataExchange(CDataExchange* pDX)
{
	DDX_Control(pDX, IDC_BUFFERCOUNT, mBufferCountEdit);
	DDX_Control(pDX, IDC_BUFFERSIZE, mBufferSizeEdit);
    DDX_Control(pDX, IDC_AUTORESIZECHECK, mAutoResizeCheck);
	CDialog::DoDataExchange(pDX);
}


///
/// \brief OnOK handler
///

void BufferOptionsDlg::OnBnClickedOk()
{
	PvUInt32 lCount = 0;
	if ( !EditToUInt32( mBufferCountEdit, lCount ) )
	{
		// Parsing error
		MessageBox( _T( "Invalid buffer count value." ), _T( "Error" ), MB_ICONERROR | MB_OK );
		return;
	}
	if ( lCount <= 0 )
	{
		MessageBox( _T( "Buffer count cannot be zero." ), _T( "Error" ), MB_ICONERROR | MB_OK );
		return;
	}

	PvUInt32 lDefaultBufferSize = 0;
	if ( !EditToUInt32( mBufferSizeEdit, lDefaultBufferSize ) )
	{
		// Parsing error
		MessageBox( _T( "Invalid default buffer size value." ), _T( "Error" ), MB_ICONERROR | MB_OK );
		return;
	}
	if ( lDefaultBufferSize <= 0 )
	{
		MessageBox( _T( "Default buffer size cannot be zero." ), _T( "Error" ), MB_ICONERROR | MB_OK );
		return;
	}

    bool lAutoResize = true;
    if ( mAutoResizeCheck.GetCheck() != BST_CHECKED )
    {
        lAutoResize = false;
    }

	if ( ( lCount == mOriginalBufferCount ) && 
		 ( lDefaultBufferSize == mOriginalBufferSize ) &&
         ( lAutoResize == mOriginalAutoResize ) )
	{
		// Nothing changed, nothing to do...
		OnOK();
		return;
	}

	// If *A LOT* of memory is about to be allocated, ask the user first
	PvUInt64 lTotal = lDefaultBufferSize * lCount;
	if ( lTotal >= WARNING_MEM_THRESHOLD )
	{
		double lGB = static_cast<double>( lTotal ) / static_cast<double>( 1024 * 1024 * 1024 );
		
		CString lMessage;
		lMessage.Format( _T( "%.1f GB of memory will be required by the acquisition pipeline. Are you sure you want to continue?" ), lGB );
		int lReturn = MessageBox( lMessage, _T( "Warning" ), MB_ICONWARNING | MB_YESNO );
        if ( lReturn != IDYES )
        {
            return;
        }
	}

	// Apply parameters
	mGEVPlayerDlg->StopStreaming();
	mPipeline->SetDefaultBufferSize( lDefaultBufferSize );
    PvResult lResult = mPipeline->SetBufferCount( lCount );
    mPipeline->SetHandleBufferTooSmall( lAutoResize );
	mGEVPlayerDlg->StartStreaming();
    if ( !lResult.IsOK() )
    {   
        CString lMessage;
        lMessage.Format( _T( "%s\r\n\r\nCould not change buffer count to requested value.\r\nOnly %i buffers could be allocated." ), 
            lResult.GetCodeString().GetUnicode(), mPipeline->GetBufferCount() );

        MessageBox( lMessage, _T( "Error" ), MB_ICONERROR | MB_OK );

        return;
    }

    OnOK();
}


///
/// \brief Converts a string to integer
///

bool BufferOptionsDlg::StrToUInt32( const CString &aString, PvUInt32 &aValue )
{
	aValue = 0;
	int lScanCount = swscanf_s( (LPCTSTR)aString, _T( "%d" ), &aValue, sizeof( aValue ) );
	if ( lScanCount != 1 )
	{
		return false;
	}

	return true;
}


///
/// \brief Reads an integer from a CEdit
///

bool BufferOptionsDlg::EditToUInt32( CEdit &aEdit, PvUInt32 &aValue )
{
	CString lString;
	aEdit.GetWindowText( lString );

	return StrToUInt32( lString, aValue );
}


///
/// \brief Dialog initialization
///

BOOL BufferOptionsDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    mOriginalBufferCount = mPipeline->GetBufferCount();
	mOriginalBufferSize = mPipeline->GetDefaultBufferSize();
    mOriginalAutoResize = mPipeline->GetHandleBufferTooSmall();

	CString lBufferCountStr;
	lBufferCountStr.Format( _T( "%i" ), mOriginalBufferCount );

	mBufferCountEdit.SetWindowText( lBufferCountStr );

	CString lBufferSizeStr;
	lBufferSizeStr.Format( _T( "%i" ), mOriginalBufferSize );

	mBufferSizeEdit.SetWindowText( lBufferSizeStr );

    mAutoResizeCheck.SetCheck( mOriginalAutoResize ? BST_CHECKED : BST_UNCHECKED );

    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}


///
/// \brief Saves buffer options of a pipeline to a configuration writer.
///

void BufferOptionsDlg::Save( PvConfigurationWriter &aWriter, PvPipeline *aPipeline )
{
    CString lStr;

    // Save a version string, just in case we need it in the future
    aWriter.Store( BUFFEROPTIONSDLG_VERSION, TAG_VERSION );

    // Buffer count
	lStr.Format( _T( "%d" ), aPipeline->GetBufferCount() );
    aWriter.Store( (LPCTSTR)lStr, TAG_STREAMINGBUFFERCOUNT );

    // Default buffer size
	lStr.Format( _T( "%d" ), aPipeline->GetDefaultBufferSize() );
    aWriter.Store( (LPCTSTR)lStr, TAG_STREAMINGDEFAULTBUFFERSIZE );

    // Auto resizing of buffers
    lStr = aPipeline->GetHandleBufferTooSmall() ? "true" : "false";
    aWriter.Store( (LPCTSTR)lStr, TAG_AUTORESIZE );
}


///
/// \brief Reads buffer options from a configuration reader and applies them to a pipeline
///

void BufferOptionsDlg::Load( PvConfigurationReader &aReader, PvPipeline *aPipeline )
{
    PvResult lResult;
    PvString lPvStr;
    double lValueD = 0.0;
    int lValueI = 0;
    bool lAutoResize = true;

    PvUInt32 lBufferCount = aPipeline->GetBufferCount();
    lResult = aReader.Restore( TAG_STREAMINGBUFFERCOUNT, lPvStr );
    if ( lResult.IsOK() )
    {
        PvUInt32 lValue = 0;
        if ( StrToUInt32( CString( lPvStr.GetUnicode() ), lValue ) && ( lValue > 0 ) )
        {
        	lBufferCount = lValue;
        }
    }

    PvUInt32 lBufferSize = aPipeline->GetDefaultBufferSize();
    lResult = aReader.Restore( TAG_STREAMINGDEFAULTBUFFERSIZE, lPvStr );
    if ( lResult.IsOK() )
    {
    	PvUInt32 lValue = 0;
    	if ( StrToUInt32( CString( lPvStr.GetUnicode() ), lValue ) && ( lValue > 0 ) )
    	{
    		lBufferSize = lValue;
    	}
    }

    lAutoResize = aPipeline->GetHandleBufferTooSmall();
    lResult = aReader.Restore( TAG_AUTORESIZE, lPvStr );
    if ( lResult.IsOK() )
    {
        // Comparing to false as the default, usual behaviour is true
        lAutoResize = lPvStr != "false";
    }

    aPipeline->SetDefaultBufferSize(lBufferSize);
    aPipeline->SetBufferCount( lBufferCount );
    aPipeline->SetHandleBufferTooSmall( lAutoResize );
}



