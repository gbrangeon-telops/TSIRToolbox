// *****************************************************************************
//
// Copyright (c) 2011, Pleora Technologies Inc., All rights reserved.
//
// -----------------------------------------------------------------------------

#include "stdafx.h"
#include "GEVDeviceInput.h"
#include "SetupDlg.h"
#include "ConnectionThread.h"
#include "PvMessageBox.h"

#include <PvDeviceFinderWnd.h>

GEVDeviceInput::GEVDeviceInput()
    : mPipeline( &mStream )
    , mPvStreamFilter( NULL )
    , mRole( RoleInvalid )
{

}

GEVDeviceInput::~GEVDeviceInput()
{

}


BOOL GEVDeviceInput::Init()
{
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

    return TRUE;
}

// ==========================================================================
void GEVDeviceInput::Disconnect()
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
void GEVDeviceInput::StopStreaming()
{
    CSingleLock lLock( &mStartStreamingMutex );

    if ( mPvStreamFilter != NULL )
    {
        mPvStreamFilter->Stop();

        delete mPvStreamFilter;
        mPvStreamFilter = NULL;
    }

    // Stop stream thread
    if ( mPipeline.IsStarted() )
    {
        mPipeline.Stop();
    }
}


// ==========================================================================
void GEVDeviceInput::Connect( PvDeviceInfo *aDI )
{
    ASSERT( aDI != NULL );
    if ( aDI == NULL )  
    {
        return;
    }

    // Just in case we came here still connected...
    Disconnect();

    // Propagate default com parameters to our PvDevice
    //SetComParameters();

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

    if ( mStream.IsOpen() )
    {
        // Ready image reception
        StartStreaming();
    }
}

// ==========================================================================
void GEVDeviceInput::StartStreaming()
{
    CSingleLock lLock( &mStartStreamingMutex );

    if ( mPvStreamFilter == NULL )
    {
        // Create display thread
        mPvStreamFilter = new PvStreamFilter( &mDevice, &mPipeline );

        // Start threads
        mPvStreamFilter->Start();
        mPvStreamFilter->SetPriority( THREAD_PRIORITY_ABOVE_NORMAL );
    }

    if ( !mPipeline.IsStarted() )
    {
        mPipeline.Start();
    }
}

