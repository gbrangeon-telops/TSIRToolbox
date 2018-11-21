// *****************************************************************************
//
//     Copyright (c) 2012, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#include "stdafx.h"

#include "ConnectionThread.h"
#include "Constants.h"

#include <PvInterface.h>


ConnectionThread::ConnectionThread( Setup *aSetup, PvDeviceInfo *aDeviceInfo, 
    PvDevice *aDevice, PvStream *aStream, CWnd* aParent )
        : mSetup( aSetup )
        , mDeviceInfo( aDeviceInfo )
        , mDevice( aDevice )
        , mStream( aStream )
        , mProgressDialog( NULL )
{
    mProgressDialog = new ProgressDlg( this, aParent );
}

ConnectionThread::~ConnectionThread()
{
    if( mProgressDialog )
    {
        delete mProgressDialog;
        mProgressDialog = NULL;
    }
}

PvResult ConnectionThread::Connect()
{
    mProgressDialog->DoModal();

    return mResult;
}

DWORD ConnectionThread::Function()
{
    try
    {
        // Connect the device if any device required
        if ( mSetup->GetRole() == ROLE_CTRLDATA )
        {
            mResult = ConnectDevice();
            if( !mResult.IsOK() )
            {
                return 0;
            }
        }

        // Open stream - and retry if it fails
        mResult = OpenStream();
        if( !mResult.IsOK() )
        {
            return 0;
        }

        // Now we need to make the link between the thread and the stream
        if ( mSetup->GetRole() == ROLE_CTRLDATA )
        {
            switch( mSetup->GetDestination() )
            {
            case DESTINATION_UNICAST_AUTO:
            case DESTINATION_UNICAST_SPECIFIC:
                mDevice->SetStreamDestination( mStream->GetLocalIPAddress(),  mStream->GetLocalPort() );
                break;
            case DESTINATION_MULTICAST:
                mDevice->SetStreamDestination( mSetup->GetIPAddress().GetBuffer(), mSetup->GetPort() );
                break;
            }
        }
    }
    catch ( ... )
    {
        mResult = PvResult( PvResult::Code::ABORTED, "Unexpected error" );
    }
    return 0;
}

PvResult ConnectionThread::ConnectDevice()
{
    PvResult lResult;

    mProgressDialog->SetStatus( _T( "Building GenICam interface..." ) );
    lResult = mDevice->Connect( mDeviceInfo, PvAccessControl );
    if ( !lResult.IsOK() )
    {
        return lResult.GetCode();
    }

    bool lEnabledValue = true;
    PvGenBoolean *lEnabled = dynamic_cast<PvGenBoolean *>( mDevice->GetGenLink()->Get( "AutoNegotiation" ) );
    if ( lEnabled != NULL )
    {
        lEnabled->GetValue( lEnabledValue );
    }

    PvInt64 lUserPacketSizeValue = 1476;
    PvGenInteger *lUserPacketSize = dynamic_cast<PvGenInteger *>( mDevice->GetGenLink()->Get( "DefaultPacketSize" ) );
    if ( ( lUserPacketSize != NULL ) && lUserPacketSize->IsReadable() )
    {
        lUserPacketSize->GetValue( lUserPacketSizeValue );
    }

    if ( lEnabledValue )
    {
        // Perform automatic packet size negociation
        mProgressDialog->SetStatus( _T( "Optimizing streaming packet size..." ) );
        lResult = mDevice->NegotiatePacketSize( 0, 1476 );
        if ( !lResult.IsOK() )
        {
            mProgressDialog->SetStatus( _T( "WARNING: streaming packet size optimization failure, using 1476 bytes!" ) );
            ::Sleep( 3000 );
        }
    }
    else
    {
        bool lManualPacketSizeSuccess = false;

        // Start by figuring out if we can use GenICam to set the packet size
        bool lUseGenICam = false;
        PvGenInteger *lPacketSize = dynamic_cast<PvGenInteger *>( mDevice->GetGenLink()->Get( "GevSCPSPacketSize" ) );
        if ( lPacketSize != NULL )
        {
            bool lWritable = false;
            PvResult lResult = lPacketSize->IsWritable( lWritable );
            if ( lResult.IsOK() && lWritable )
            {
                lUseGenICam = true;
            }
        }
        
        if ( lUseGenICam )
        {
            // Setting packet size through the GenICam interface
            PvResult lResult = lPacketSize->SetValue( lUserPacketSizeValue );
            if ( lResult.IsOK() )
            {
                lManualPacketSizeSuccess = true;
            }
            else
            {
                // Last resort default...
                lPacketSize->SetValue( 576 );
                lManualPacketSizeSuccess = false;
            }
        }
        else
        {
            // Direct write, for GenICam challenged devices...
            PvResult lResult = mDevice->WriteRegister( 0x0D04, (PvUInt32)lUserPacketSizeValue );
            if ( lResult.IsOK() )
            {
                lManualPacketSizeSuccess = true;
            }
            else
            {
                // Last resort default...
                mDevice->WriteRegister( 0x0D04, 576 );
                lManualPacketSizeSuccess = false;
            }
        }
        
        CString lNewStr;
        if ( lManualPacketSizeSuccess )
        {
            lNewStr.Format( 
                _T( "A packet of size of %i bytes was configured for streaming. You may experience issues " )
                _T( "if your system configuration cannot support this packet size." ), 
                lUserPacketSizeValue);
        }
        else
        {                        
            lNewStr.Format( _T( "WARNING: could not set streaming packet size to %i bytes, using %i bytes!" ),
                           lUserPacketSizeValue, 576 );
        }
        mProgressDialog->SetStatus( lNewStr );
        ::Sleep( 3000 );
    }

    return PvResult::Code::OK;
}

PvResult ConnectionThread::OpenStream()
{
    PvResult lResult;

    mProgressDialog->SetStatus( _T( "Opening eBUS stream to device..." ) );

    // Open stream
    switch( mSetup->GetDestination() )
    {
    case DESTINATION_UNICAST_AUTO:
        lResult = mStream->Open( mDeviceInfo->GetIPAddress(), 0, 0, 
            mDeviceInfo->GetInterface()->GetIPAddress() );
        break;
    case DESTINATION_UNICAST_SPECIFIC:
        lResult = mStream->Open( mDeviceInfo->GetIPAddress(), mSetup->GetPort(), 
            0, mDeviceInfo->GetInterface()->GetIPAddress() );
        break;
    case DESTINATION_MULTICAST:
        lResult = mStream->Open( mDeviceInfo->GetIPAddress(), mSetup->GetIPAddress().GetBuffer(),
            mSetup->GetPort(), 0, mDeviceInfo->GetInterface()->GetIPAddress() );
        break;
    default:
        ASSERT( 0 );
        break;
    }

    return lResult;
}