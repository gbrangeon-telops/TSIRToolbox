#include "stdafx.h"

#include "LoadingThread.h"


#define SERIALBRIDGE_PERSISTENCE_PREFIX ( _T( "SerialBridge" ) )


// ============================================================================
LoadingThread::LoadingThread( SetupDlg *aSetupDlg, PvConfigurationReader *aReader, 
    PvDevice *aDevice, PvStream *aStream, PvSerialBridgeManagerWnd *aSerialBridgeManagerWnd,
    PvStringList *aErrorList, CWnd* aParent )
    : mSetupDlg( aSetupDlg )
    , mReader( aReader )
    , mDevice( aDevice )
    , mStream( aStream )
    , mSerialBridgeManagerWnd( aSerialBridgeManagerWnd )
    , mErrorList( aErrorList )
{
    mDlg = new ProgressDlg( this, aParent );
}


// ============================================================================
LoadingThread::~LoadingThread()
{
    if ( mDlg != NULL )
    {
        delete mDlg;
        mDlg = NULL;
    }
}


// ============================================================================
PvResult LoadingThread::Load()
{
    ASSERT( mDlg != NULL );
    mDlg->DoModal();

    return mResult;
}


// ============================================================================
DWORD LoadingThread::Function()
{
    PvResult lResult = PvResult::Code::NOT_CONNECTED;
    PvUInt16 lChannel = mSetupDlg->GetDefaultChannel();

    try
    {
        ASSERT( mDevice != NULL );
        ASSERT( mStream != NULL );

        if ( ( mSetupDlg->GetRole() == SetupDlg::RoleCtrlData ) ||
             ( mSetupDlg->GetRole() == SetupDlg::RoleCtrl ) ||
             ( mSetupDlg->GetRole() == SetupDlg::RoleMonitor ) )
        {   
            if ( mDevice->IsConnected() )
            {
                mDlg->SetStatus( _T( "Restoring device parameters from file..." ) );
            }
            else
            {
                mDlg->SetStatus( _T( "Connecting to device and restoring it from file..." ) );
            }

            SetPrefix( _T( "Device restore error. " ) );
            lResult = mReader->Restore( 0, mDevice );
            if ( !lResult.IsOK() )
            {
                mResult = lResult;
                return lResult.GetCode();
            }

            if ( mSerialBridgeManagerWnd != NULL )
            {
                CString lPrefix = SERIALBRIDGE_PERSISTENCE_PREFIX;
                int lPrefixLength = lPrefix.GetLength();

                mDlg->SetStatus( _T( "Restoring serial communication bridge configuration..." ) );

                PvPropertyList lList;
                mReader->Restore( _T( "SerialBridgeManager" ), lList );

                // Let the serial bridge manager to perform the load
                mSerialBridgeManagerWnd->SetDevice( mDevice );
                mSerialBridgeManagerWnd->Load( lList, *mErrorList );

                // Go through ports, try finding persistence data for node maps (if any)
                for ( PvUInt32 i = 0; i < mSerialBridgeManagerWnd->GetPortCount(); i++ )
                {
                    PvGenParameterArray *lNodeMap = mSerialBridgeManagerWnd->GetGenParameterArray( i );
                    if ( lNodeMap != NULL )
                    {
                        PvString lPortName = mSerialBridgeManagerWnd->GetPortName( i );

                        CString lStatus;
                        lStatus.Format( _T( "Restoring configuration of camera on %s..." ), lPortName.GetUnicode() );
                        mDlg->SetStatus( lStatus );

                        CString lStorageName = lPortName.GetUnicode();
                        lStorageName += _T( "Camera" );

                        CString lPrefix;
                        lPrefix.Format( _T( "Camera (%s) restore error. " ), lPortName.GetUnicode() );

                        SetPrefix( lPrefix );
                        mReader->Restore( (LPCTSTR)lStorageName, *lNodeMap );
                    }
                }
            }
        }

        if ( ( mSetupDlg->GetRole() == SetupDlg::RoleCtrlData ) ||
             ( mSetupDlg->GetRole() == SetupDlg::RoleData ) )
        {
            if ( mStream->IsOpen() )
            {
                mDlg->SetStatus( _T( "Restoring stream parameters from file..." ) );
            }
            else
            {
                mDlg->SetStatus( _T( "Opening a stream and restoring it from file..." ) );
            }

            SetPrefix( _T( "Stream restore error. " ) );
            lResult = mReader->Restore( 0, *mStream );

            if ( lResult.IsOK() )
            {
                // obtain the proper channel from the stream we restored
                PvInt64 lValue;
                PvGenInteger* lChannelID = mStream->GetParameters()->GetInteger( "ChannelID" );
                lChannelID->GetValue( lValue );
                lChannel = (PvUInt16) lValue;
            }
        }

        if ( !lResult.IsOK() )
        {
            mResult = lResult;
            return lResult.GetCode();
        }

        // Now that the stream is opened, set the destination on the device
        if ( ( mSetupDlg->GetRole() == SetupDlg::RoleCtrlData ) ||
             ( mSetupDlg->GetRole() == SetupDlg::RoleCtrl ) )
        {
            if ( ( mSetupDlg->GetDestination() == SetupDlg::DestinationUnicastAuto ) ||
                 ( mSetupDlg->GetDestination() == SetupDlg::DestinationUnicastSpecific ) )
            {
                mDevice->SetStreamDestination( mStream->GetLocalIPAddress(), mStream->GetLocalPort(), lChannel );
            }
            else if ( mSetupDlg->GetDestination() == SetupDlg::DestinationUnicastOther )
            {
                mDevice->SetStreamDestination( (LPCTSTR)mSetupDlg->GetUnicastIP(), mSetupDlg->GetUnicastPort(), lChannel );
            }
            else if ( mSetupDlg->GetDestination() == SetupDlg::DestinationMulticast )
            {
                mDevice->SetStreamDestination( (LPCTSTR)mSetupDlg->GetMulticastIP(), mSetupDlg->GetMulticastPort(), lChannel );
            }
        }
    }
    catch ( ... )
    {
        lResult = PvResult( PvResult::Code::ABORTED, "Unexpected error loading persistence file" );
    }

    mResult = lResult;
    return lResult.GetCode();
}


// ============================================================================
void LoadingThread::SetPrefix( const CString &aPrefix )
{
    PvString lPrefix( (LPCTSTR)aPrefix );
    mReader->SetErrorList( mErrorList, lPrefix );
}


