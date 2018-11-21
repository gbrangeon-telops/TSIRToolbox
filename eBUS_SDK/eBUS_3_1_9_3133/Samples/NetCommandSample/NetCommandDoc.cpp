// *****************************************************************************
//
// Copyright (c) 2010, Pleora Technologies Inc., All rights reserved.
//
// -----------------------------------------------------------------------------
//
// The document of the MDI application. Contains a device, stream and everything
// required to hold them together. There is one document for each entity in
// the NetCommand projet.
// 
// *****************************************************************************


#include "stdafx.h"
#include "NetCommand.h"

#include "NetCommandDoc.h"
#include "ConnectionThread.h"
#include "PvMessageBox.h"
#include "ThreadDisplay.h"
#include "Messages.h"
#include "SetupDlg.h"

#include <PvDeviceFinderWnd.h>
#include <PvConfigurationReader.h>
#include <PvConfigurationWriter.h>


#define BUFFER_SIZE ( 1024 )


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Default PvDevice. Never connected, only used to hold default communication parameters
extern PvDevice sDefaultDevice;


IMPLEMENT_DYNCREATE(NetCommandDoc, CDocument)

BEGIN_MESSAGE_MAP(NetCommandDoc, CDocument)
END_MESSAGE_MAP()


// ==========================================================================
NetCommandDoc::NetCommandDoc()
    : mPipeline( &mStream )
    , mThreadDisplay( NULL )
    , mRole( RoleInvalid )
    , mMainWnd( NULL )
{
    // We save this pointer, as we cannot retrieve it from PvDevice callbacks
    // comming from another thread context
    mMainWnd = ::AfxGetMainWnd();
    ASSERT( mMainWnd != NULL );
}

// ==========================================================================
NetCommandDoc::~NetCommandDoc()
{
    if ( mDevice.IsConnected() || mStream.IsOpen() )
    {
        OnCloseDocument();
    }
}

// ==========================================================================
BOOL NetCommandDoc::OnNewDocument()
{
	if ( !CDocument::OnNewDocument() )
    {
		return FALSE;
    }

    PvDeviceFinderWnd lFinder;
    lFinder.SetTitle( _T( "GigE Vision Device Selection" ) );

    PvResult lResult = lFinder.ShowModal();
    if ( !lResult.IsOK() || ( lFinder.GetSelected() == NULL ) )
    {
        return FALSE;
    }

    SetupDlg lDlg;
    lDlg.SetSetup( &mSetup );
    lDlg.SetDevice( lFinder.GetSelected() );
    if ( lDlg.DoModal() != IDOK )
    {
        return FALSE;
    }

    mSetup = *( lDlg.GetSetup() );

    CWaitCursor lCursor;
    Connect( lFinder.GetSelected() );

    ::AfxGetMainWnd()->SendMessage( WM_NEWDOCUMENT, reinterpret_cast<WPARAM>( this ) );

    m_bAutoDelete = FALSE;

    return TRUE;
}

// ==========================================================================
void NetCommandDoc::OnCloseDocument()
{
    CWaitCursor lCursor;

    ::AfxGetMainWnd()->SendMessage( WM_DOCUMENTCLOSING, reinterpret_cast<WPARAM>( this ) );
    Disconnect();

    CDocument::OnCloseDocument();
}

// ==========================================================================
void NetCommandDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

// ==========================================================================
void NetCommandDoc::Connect( PvDeviceInfo *aDI )
{
    ASSERT( aDI != NULL );
    if ( aDI == NULL )	
    {
        return;
    }

    // Just in case we came here still connected...
    Disconnect();

    // Propagate default com parameters to our PvDevice
    SetComParameters();

    // Device connection, packet size negociation and stream opening is performed in a separate 
    // thread while we display a progress dialog to the user
    PvResult lResult;
    if ( aDI != NULL )
    {
        ConnectionThread lConnectionThread( &mSetup, aDI, &mDevice, &mStream, NULL );
        lResult = lConnectionThread.Connect();
    }

    if ( !lResult.IsOK() )
    {
        PvMessageBox( NULL, lResult );
        Disconnect();

        return;
    }

    mIPAddress = aDI->GetIPAddress().GetUnicode();
    mMACAddress = aDI->GetMACAddress().GetUnicode();
    mManufacturer = aDI->GetManufacturerInfo().GetUnicode();
    mModel = aDI->GetModel().GetUnicode();
    mName = aDI->GetUserDefinedName().GetUnicode();
    mDescription = aDI->GetID().GetUnicode();

    if ( mDevice.IsConnected() )
    {
        // Register PvDevice callbacks
        mDevice.RegisterEventSink( this );

        switch ( aDI->GetClass() )
        {
        default:
            ASSERT( 0 );

        case PvDeviceClassTransmitter:
            mRole = RoleDeviceTransmitter;
            break;

        case PvDeviceClassReceiver:
            mRole = RoleDeviceReceiver;
            break;

        case PvDeviceClassTransceiver:
            mRole = RoleDeviceTransceiver;
            break;

        case PvDeviceClassPeripheral:
            mRole = RoleDevicePeripheral;
            break;
        }
    }
    else if ( mStream.IsOpen() )
    {
        mRole = RoleSoftwareReceiver;
    }

    SetTitle( mDescription );

    if ( mStream.IsOpen() )
    {
        // Ready image reception
        StartStreaming();
    }
}

// ==========================================================================
void NetCommandDoc::Disconnect()
{
    if ( mDevice.IsConnected() )
    {
        mDevice.UnregisterEventSink( this );
        mDevice.Disconnect();
    }

    // If streaming, stop streaming
    if ( mStream.IsOpen() )
    {
        StopStreaming();
        mStream.Close();
    }
}

// ==========================================================================
void NetCommandDoc::StartStreaming()
{
    CSingleLock lLock( &mStartStreamingMutex );
    VERIFY( lLock.Lock() );
    if ( mThreadDisplay == NULL )
    {
        // Create display thread
        mThreadDisplay = new ThreadDisplay( &mDevice, &mPipeline );

        // Start threads
        mThreadDisplay->Start();
        mThreadDisplay->SetPriority( THREAD_PRIORITY_ABOVE_NORMAL );
    }

    if ( !mPipeline.IsStarted() )
    {
        mPipeline.Start();
    }
}

// ==========================================================================
void NetCommandDoc::StopStreaming()
{
    CSingleLock lLock( &mStartStreamingMutex );
    VERIFY( lLock.Lock() );
    if ( mThreadDisplay != NULL )
    {
        mThreadDisplay->Stop();

        delete mThreadDisplay;
        mThreadDisplay = NULL;
    }

    // Stop stream thread
    if ( mPipeline.IsStarted() )
    {
        mPipeline.Stop();
    }
}

// ==========================================================================
void NetCommandDoc::SetDisplay( PvDisplayWnd *aDisplay )
{
    CSingleLock lLock( &mStartStreamingMutex );
    VERIFY( lLock.Lock() );
    if ( mThreadDisplay != NULL )
    {
        mThreadDisplay->SetDisplay( aDisplay );
    }
}

// ==========================================================================
void NetCommandDoc::StartAcquisition()
{
    CSingleLock lLock( &mStartAcquisitionMutex );
    VERIFY( lLock.Lock() );
    PvGenEnum *lMode = dynamic_cast<PvGenEnum *>( mDevice.GetGenParameters()->Get( "AcquisitionMode" ) );
    PvGenCommand *lStart = dynamic_cast<PvGenCommand *>( mDevice.GetGenParameters()->Get( "AcquisitionStart" ) );
    PvGenInteger *lTLParamsLocked = dynamic_cast<PvGenInteger *>( mDevice.GetGenParameters()->Get( "TLParamsLocked" ) );
    PvGenCommand *lResetStats = dynamic_cast<PvGenCommand *>( mStream.GetParameters()->Get( "Reset" ) );

    if ( mStream.IsOpen() )
    {
        PvUInt32 lPayloadSize = GetPayloadSize();
        if ( lPayloadSize > 0 )
        {
            mPipeline.SetBufferSize( lPayloadSize );
        }

        mPipeline.Reset();
    }

    lResetStats->Execute();

    if ( mThreadDisplay != NULL )
    {
        mThreadDisplay->ResetStats();
    }

    PvResult lResult = PvResult::Code::NOT_INITIALIZED;

    PvString lStr;
    lMode->GetValue( lStr );
    CString lModeStr = lStr.GetUnicode();
    if ( lModeStr.Find( _T( "Continuous" ) ) >= 0 )
    {
        // We are streaming, lock the TL parameters
        if ( lTLParamsLocked != NULL ) 
        {
            lResult = lTLParamsLocked->SetValue( 1 );
        }

        lStart->Execute();
    }
    else if ( ( lModeStr.Find( _T( "Multi" ) ) >= 0 ) || 
              ( lModeStr.Find( _T( "Single" ) ) >= 0 ) )
    {
        // We are streaming, lock the TL parameters
        if ( lTLParamsLocked != NULL ) 
        {
            lTLParamsLocked->SetValue( 1 );
        }

        lResult = lStart->Execute();

        // We are done streaming, unlock the TL parameters
        if ( lTLParamsLocked != NULL ) 
        {
            lTLParamsLocked->SetValue( 0 );
        }
    }
}

// ==========================================================================
void NetCommandDoc::StopAcquisition()
{
    CSingleLock lLock( &mStartAcquisitionMutex );
    VERIFY( lLock.Lock() );

    PvResult lResult;

    // Execute AcquisitionStop command, if it exists
    PvGenCommand *lStop = dynamic_cast<PvGenCommand *>( mDevice.GetGenParameters()->Get( "AcquisitionStop" ) );
    if ( lStop != NULL )
    {
        lResult = lStop->Execute();
    }

    // TLParamsLocked, if it exists
    PvGenInteger *lTLParamsLocked = dynamic_cast<PvGenInteger *>( mDevice.GetGenParameters()->Get( "TLParamsLocked" ) );
    if ( lTLParamsLocked != NULL )
    {
        lResult = lTLParamsLocked->SetValue( 0 );
    }
}

// ==========================================================================
PvUInt32 NetCommandDoc::GetPayloadSize()
{
    if ( !mDevice.IsConnected() )
    {
        return 0;
    }

    PvGenInteger *lPayloadSize = dynamic_cast<PvGenInteger *>( mDevice.GetGenParameters()->Get( "PayloadSize" ) );
    PvGenInteger *lWidth = dynamic_cast<PvGenInteger *>( mDevice.GetGenParameters()->Get( "Width" ) );
    PvGenInteger *lHeight = dynamic_cast<PvGenInteger *>( mDevice.GetGenParameters()->Get( "Height" ) );
    PvGenEnum *lPixelFormat = dynamic_cast<PvGenEnum *>( mDevice.GetGenParameters()->Get( "PixelFormat" ) );

    // Get payload size
    PvInt64 lPayloadSizeValue = 0;
    if ( lPayloadSize != NULL )
    {
        lPayloadSize->GetValue( lPayloadSizeValue );
    }

    // Compute poor man's payload size - for devices not maintaning PayloadSize properly
    PvInt64 lPoorMansPayloadSize = 0;
    if ( ( lWidth != NULL ) && ( lHeight != NULL ) && ( lPixelFormat != NULL ) )
    {
        PvInt64 lWidthValue, lHeightValue;
        lWidth->GetValue( lWidthValue );
        lHeight->GetValue( lHeightValue );

        PvInt64 lPixelFormatValue;
        lPixelFormat->GetValue( lPixelFormatValue );
        PvInt64 lPixelSizeInBits = PvGetPixelBitCount( static_cast<PvPixelType>( lPixelFormatValue ) );

        lPoorMansPayloadSize = ( lWidthValue * lHeightValue * lPixelSizeInBits ) / 8;
    }

    // Take max, let pipeline know what the payload size is
    PvInt64 lBestPayloadSize = max( lPayloadSizeValue, lPoorMansPayloadSize );
    if ( ( lBestPayloadSize > 0 ) && ( lBestPayloadSize < UINT_MAX ) )
    {
        // Round up to make it mod 32 (works around an issue with some devices)
        if ( ( lBestPayloadSize % 32 ) != 0 )
        {
            lBestPayloadSize = ( ( lBestPayloadSize / 32 ) + 1 ) * 32;
        }

        return static_cast<PvUInt32>( lBestPayloadSize );
    }

    // Could not compute/retrieve payload size...
    return 0;
}

// ==========================================================================
void NetCommandDoc::SetComParameters()
{
    // Get temp path
    WCHAR lTempPath[ BUFFER_SIZE ] = { 0 };
    if ( ::GetTempPath( BUFFER_SIZE, lTempPath ) == 0 )
    {
		TRACE0( "Failed to retrieve temp path\n" );
        return;
    }

    // Get temp file name
    WCHAR lTempFile[ BUFFER_SIZE ] = { 0 };
    if ( ::GetTempFileName( lTempPath, _T( "" ), 0, lTempFile ) == 0 )
    {
		TRACE0( "Failed to create temp file name\n" );
        return;
    }

    // Write default com parameters to file
    PvConfigurationWriter lWriter;
    VERIFY( lWriter.Store( sDefaultDevice.GetGenLink() ).IsOK() );
    if ( !lWriter.Save( lTempFile ).IsOK() )
    {
		TRACE0( "Unable to save com parameters to temporary file\n" );
        return;
    }

    // Retrieve file content in doc's PvDevice com parameters
    PvConfigurationReader lReader;
    VERIFY( lReader.Load( lTempFile ).IsOK() );
    if ( !lReader.Restore( 0, *( mDevice.GetGenLink() ) ).IsOK() )
    {
		TRACE0( "Unable to retrieve com parameters from temporary file\n" );
        return;
    }

    // Delete temp file
    if ( !::DeleteFile( lTempFile ) )
    {
		TRACE0( "Error deleting temp file\n" );
    }
}

// ==========================================================================
void NetCommandDoc::OnLinkDisconnected( PvDevice *aDevice )
{
    ASSERT( mMainWnd != NULL );
    mMainWnd->PostMessage( WM_LINKDISCONNECTED, reinterpret_cast<WPARAM>( this ) );
}

