// *****************************************************************************
//
//     Copyright (c) 2007, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#include "stdafx.h"
#include "ConnectionThread.h"
#include "GEVPlayerDlg.h"

#include <PvInterface.h>


ConnectionThread::ConnectionThread( SetupDlg *aSetupDlg, PvDeviceInfo *aDeviceInfo, PvDevice *aDevice, PvStream *aStream, CWnd* aParent, const PvString &aLocalIPAddress, const PvString &aDeviceIPAddress )
    : mSetupDlg( aSetupDlg )
    , mDeviceInfo( aDeviceInfo )
    , mDevice( aDevice )
    , mStream( aStream )
    , mLocalIPAddress( aLocalIPAddress )
    , mDeviceIPAddress( aDeviceIPAddress )
    , mTitle( _T( "Connection Progress" ) )
{
    mDlg = new ProgressDlg( this, aParent );
}

ConnectionThread::~ConnectionThread()
{
    if ( mDlg != NULL )
    {
        delete mDlg;
        mDlg = NULL;
    }
}

PvResult ConnectionThread::Connect()
{
    ASSERT( mDlg != NULL );
    mDlg->SetTitle( mTitle );
    mDlg->DoModal();

    return mResult;
}

void ConnectionThread::SetStreamDestination( const CString &aIpAddress, PvUInt16 aPort, PvUInt16 aChannel )
{
	ASSERT( mDevice != NULL );
	ASSERT( mStream != NULL );

    // Bug 2132: only set destination if different than 0.0.0.0:0
    if ( ( aIpAddress == _T( "0.0.0.0" ) ) &&
         ( aPort == 0 ) )
    {
		return;
	}

	// Bug 3202: don't set destination on receiver, peripheral or transceiver (for now)
    if ( mDeviceInfo != NULL )
    {
	    if ( ( mDeviceInfo->GetClass() == PvDeviceClassReceiver ) ||
		     ( mDeviceInfo->GetClass() == PvDeviceClassPeripheral ) ||
		     ( mDeviceInfo->GetClass() == PvDeviceClassTransceiver ) )
	    {
		    return;
	    }
    }

	mDevice->SetStreamDestination( (LPCTSTR)aIpAddress, aPort, aChannel );
}

DWORD ConnectionThread::Function()
{
    PvResult lResult = PvResult::Code::OK;
    PvUInt16 lChannel = mSetupDlg->GetDefaultChannel();
    
    if ( mDeviceInfo != NULL )
    {
        mDeviceIPAddress = mDeviceInfo->GetIPAddress();
        mLocalIPAddress = mDeviceInfo->GetInterface()->GetIPAddress();
    }

    try
    {
        if ( ( mSetupDlg->GetRole() == SetupDlg::RoleCtrlData ) ||
             ( mSetupDlg->GetRole() == SetupDlg::RoleCtrl ) ||
             ( mSetupDlg->GetRole() == SetupDlg::RoleMonitor ) )
        {
            if ( !mDevice->IsConnected() && mDeviceInfo != NULL )
            {
                // Connect device
                mDlg->SetStatus( _T( "Building GenICam interface..." ) );
                lResult = mDevice->Connect( mDeviceInfo, 
                    ( mSetupDlg->GetRole() != SetupDlg::RoleMonitor ) ? 
                        PvAccessControl : 
                        PvAccessReadOnly );
                if ( !lResult.IsOK() )
                {
                    mResult = lResult;
                    return lResult.GetCode();
                }
            }

            if ( mSetupDlg->GetRole() == SetupDlg::RoleCtrlData )
            {
                // Determine whether this device is multisource (does that genicam feature exist)
                if ( GEVPlayerDlg::IsMultiSourceTransmitter( mDevice ) )
                {
                    // If multisource feature exists, get the channel corresponding to the currently selected source
                    // Keep in mind there's also the SourceComponentSelector feature that can be configured using the device parameter browser
                    PvInt64 lValue = 0;
                    PvResult lResult = mDevice->GetGenParameters()->GetIntegerValue( "SourceStreamChannel", lValue );
                    if ( lResult.IsOK() )
                    {
                        lChannel = static_cast<PvUInt16>( lValue );
                    }
                    else
                    {
                        MessageBox( mDlg->GetSafeHwnd(), 
                            _T( "SourceSelector feature present, but SourceStreamChannel feature missing. Defaulting to channel 0." ), 
                            _T( "Warning" ), MB_ICONWARNING | MB_OK );
                    }
                }

                // Negotiate packet size once for all channels
                if ( ( mDeviceInfo != NULL ) && ( mDeviceInfo->GetClass() == PvDeviceClassTransmitter ) )
                {
                    NegotiatePacketSize();
                }
            }
        }

        // Open stream - and retry if it fails
        if ( ( mSetupDlg->GetRole() == SetupDlg::RoleCtrlData ) ||
             ( mSetupDlg->GetRole() == SetupDlg::RoleData ) )
        {
            CString lString;
            lString.Format( _T( "Opening eBUS stream on channel %d..." ), lChannel );
            mDlg->SetStatus( lString );

            // Open stream
            if ( mSetupDlg->GetDestination() == SetupDlg::DestinationUnicastAuto )
            {
                lResult = mStream->Open( 
                    mDeviceIPAddress, 0, lChannel, mLocalIPAddress );
            }
            else if ( mSetupDlg->GetDestination() == SetupDlg::DestinationUnicastSpecific )
            {
                lResult = mStream->Open(
                    mDeviceIPAddress, mSetupDlg->GetUnicastSpecificPort(), lChannel, mLocalIPAddress );
            }
            else if ( mSetupDlg->GetDestination() == SetupDlg::DestinationMulticast )
            {
                lResult = mStream->Open(
                    mDeviceIPAddress,
                    (LPCTSTR)mSetupDlg->GetMulticastIP(), 
                    mSetupDlg->GetMulticastPort(), lChannel, mLocalIPAddress );
            }
            else
            {
                ASSERT( 0 );
            }

            if ( !lResult.IsOK() )
            {
                mResult = lResult;
                return lResult.GetCode();
            }
        }

        // Now that the stream is opened, set the destination on the device
        if ( ( mSetupDlg->GetRole() == SetupDlg::RoleCtrlData ) ||
             ( mSetupDlg->GetRole() == SetupDlg::RoleCtrl ) )
        {
            if ( ( mSetupDlg->GetDestination() == SetupDlg::DestinationUnicastAuto ) ||
                 ( mSetupDlg->GetDestination() == SetupDlg::DestinationUnicastSpecific ) )
            {
				SetStreamDestination( mStream->GetLocalIPAddress().GetUnicode(), mStream->GetLocalPort(), lChannel );
            }
            else if ( mSetupDlg->GetDestination() == SetupDlg::DestinationUnicastOther )
            {
                SetStreamDestination( mSetupDlg->GetUnicastIP(), mSetupDlg->GetUnicastPort(), lChannel );
            }
            else if ( mSetupDlg->GetDestination() == SetupDlg::DestinationMulticast )
            {
                SetStreamDestination( (LPCTSTR)mSetupDlg->GetMulticastIP(), mSetupDlg->GetMulticastPort(), lChannel );
            }
        }
    }
    catch ( ... )
    {
        lResult = PvResult( PvResult::Code::ABORTED, "Unexpected error" );
    }

    mResult = lResult;
    return lResult.GetCode();
}

void ConnectionThread::NegotiatePacketSize()
{
    bool lEnabledValue = true;
    mDevice->GetGenLink()->GetBooleanValue( "AutoNegotiation", lEnabledValue ) ;

    PvInt64 lUserPacketSizeValue = 1476;
    mDevice->GetGenLink()->GetIntegerValue( "DefaultPacketSize", lUserPacketSizeValue );

    if ( lEnabledValue )
    {
        PvInt64 lPacketSizeValue = 0;
        mDevice->GetGenParameters()->GetIntegerValue( "GevSCPSPacketSize", lPacketSizeValue );

        // Perform automatic packet size negotiation
        mDlg->SetStatus( _T( "Optimizing streaming packet size..." ) );
        PvResult lResult = mDevice->NegotiatePacketSize( 0, static_cast<PvUInt32>( lPacketSizeValue ) );
        if ( !lResult.IsOK() )
        {
            CString lString;
            lString.Format( _T( "WARNING: Streaming packet size negotiation failure, using original value of %d bytes!" ), lPacketSizeValue );
            mDlg->SetStatus( lString );
            ::Sleep( 3000 );
        }
    }
    else
    {
        PvResult lResult = mDevice->SetPacketSize( static_cast<PvUInt32>( lUserPacketSizeValue ) );
        if ( !lResult.IsOK() )
        {
            // Last resort...
            mDevice->SetPacketSize( 576 );
        }
        
        CString lNewStr;
        if ( lResult.IsOK() )
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
        mDlg->SetStatus( lNewStr );
        ::Sleep( 3000 );
    }

    //
    // If more than one channel, propagate packet in effect size to other channels
    //

    PvInt64 lChannelCount = 0;
    PvResult lResult = mDevice->GetGenParameters()->GetIntegerValue( "GevStreamChannelCount", lChannelCount );
    if ( !lResult.IsOK() || ( lChannelCount < 2 ) )
    {
        // If we can't read the GevStreamChannelCount or of its less than 2, we are done here
        return;
    }

    PvInt64 lPacketSizeValue = 0;
    lResult = mDevice->GetGenParameters()->GetIntegerValue( "GevSCPSPacketSize", lPacketSizeValue );
    if ( !lResult.IsOK() )
    {
        mDlg->SetStatus( 
            _T( "WARNING: Failed to propagate GevSCPSPacketSize on all streaming channels. " )
            _T( "Could not read back streaming channel 0 packet size. " ) );
        ::Sleep( 3000 );
        return;
    }

    for ( PvInt64 i = 1; i < lChannelCount; i++ )
    {
        lResult = mDevice->SetPacketSize( static_cast<PvUInt32>( lPacketSizeValue ), static_cast<PvUInt32>( i ) );
        if ( !lResult.IsOK() )
        {
            mDlg->SetStatus( _T( "WARNING: Failed to propagate GevSCPSPacketSize on all streaming channels. " ) );
            ::Sleep( 3000 );
            return;
        }
    }
}



