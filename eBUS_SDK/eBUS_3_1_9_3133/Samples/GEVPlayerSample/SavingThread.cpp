#include "stdafx.h"

#include "SavingThread.h"


#define SERIALBRIDGE_PERSISTENCE_PREFIX ( _T( "SerialBridge" ) )


// ============================================================================
SavingThread::SavingThread( SetupDlg *aSetupDlg, PvConfigurationWriter *aWriter, 
    PvDevice *aDevice, PvStream *aStream, PvSerialBridgeManagerWnd *aSerialBridgeManagerWnd,
    PvStringList *aErrorList, CWnd* aParent )
    : mSetupDlg( aSetupDlg )
    , mWriter( aWriter )
    , mDevice( aDevice )
    , mStream( aStream )
    , mSerialBridgeManagerWnd( aSerialBridgeManagerWnd )
    , mErrorList( aErrorList )
{
    mDlg = new ProgressDlg( this, aParent );
}


// ============================================================================
SavingThread::~SavingThread()
{
    if ( mDlg != NULL )
    {
        delete mDlg;
        mDlg = NULL;
    }
}


// ============================================================================
PvResult SavingThread::Save()
{
    mDlg->DoModal();
    return GetReturnValue();
}


// ============================================================================
DWORD SavingThread::Function()
{
    PvResult lResult = PvResult::Code::NOT_CONNECTED;

    try
    {
        ASSERT( mDevice != NULL );
        ASSERT( mStream != NULL );
        ASSERT( mDlg != NULL );

        if ( ( mSetupDlg->GetRole() == SetupDlg::RoleCtrlData ) ||
             ( mSetupDlg->GetRole() == SetupDlg::RoleCtrl ) ||
             ( mSetupDlg->GetRole() == SetupDlg::RoleMonitor ) )
        {
            mDlg->SetStatus( _T( "Saving the device parameters..." ) );

            // Save device configuration
            SetPrefix( _T( "Device state save error. " ) );
            lResult = mWriter->Store( mDevice );
            if ( !lResult.IsOK() )
            {
                return lResult.GetCode();
            }

            // Save serial bridge configuration
            if ( mSerialBridgeManagerWnd != NULL )
            {
                mDlg->SetStatus( _T( "Saving serial communication bridge configuration..." ) );

                // Get persistence data
                PvPropertyList lList;
                lResult = mSerialBridgeManagerWnd->Save( lList );
                if ( !lResult.IsOK() )
                {
                    return lResult.GetCode();
                }

                // Write to persistence file
                mWriter->Store( &lList, _T( "SerialBridgeManager" ) );

                // Save camera configuration (if any)
                for ( PvUInt32 i = 0; i < mSerialBridgeManagerWnd->GetPortCount(); i++ )
                {
                    PvGenParameterArray *lNodeMap = mSerialBridgeManagerWnd->GetGenParameterArray( i );
                    if ( lNodeMap != NULL )
                    {
                        PvString lPortName = mSerialBridgeManagerWnd->GetPortName( i );

                        CString lStatus;
                        lStatus.Format( _T( "Saving configuration of camera on %s..." ), lPortName.GetUnicode() );
                        mDlg->SetStatus( lStatus );

                        CString lStorageName = lPortName.GetUnicode();
                        lStorageName += _T( "Camera" );

                        CString lPrefix;
                        lPrefix.Format( _T( "Camera (%s) state save error. " ), lPortName.GetUnicode() );

                        SetPrefix( lPrefix );
                        mWriter->Store( lNodeMap, (LPCTSTR)lStorageName );
                    }
                }
            }
        }

        if ( ( mSetupDlg->GetRole() == SetupDlg::RoleCtrlData ) ||
            ( mSetupDlg->GetRole() == SetupDlg::RoleData ) )
        {
            mDlg->SetStatus( _T( "Saving the stream parameters..." ) );

            // Save stream configuration
            SetPrefix( _T( "Stream parameters state save error. " ) );
            lResult = mWriter->Store( mStream );
            if ( !lResult.IsOK() )
            {
                return lResult.GetCode();
            }
        }
    }
    catch ( ... )
    {
        lResult = PvResult::Code::ABORTED;
    }

    return lResult.GetCode();
}


// ============================================================================
void SavingThread::SetPrefix( const CString &aPrefix )
{
    PvString lPrefix( (LPCTSTR)aPrefix );
    mWriter->SetErrorList( mErrorList, lPrefix );
}


