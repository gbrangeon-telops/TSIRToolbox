// *****************************************************************************
//
//     Copyright (c) 2013, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#pragma once

#include "ProgressDlg.h"
#include "Setup.h"

#include <PvDeviceInfoGEV.h>
#include <PvDeviceGEV.h>
#include <PvStreamGEV.h>

///
/// \class ConnectionThread
///
/// Thread responsible of connecting a device and opening a stream. Performed
/// in a thread in order not to lock the UI while this is happening. This allows
/// the UI to stay responsive.
///
class ConnectionThread : public Thread
{
public:

    ///
    /// \brief Constructor
    ///
    /// \param aSetup Setup information
    /// \param aDeviceInfo Device Information
    /// \param aDevice Connected device to be returned
    /// \param aStream Opened stream to be opened
    /// \param aParant Parent windows
    ConnectionThread( Setup *aSetup, const PvDeviceInfoGEV *aDeviceInfo, 
        PvDeviceGEV *aDevice, PvStreamGEV *aStream, CWnd* aParent );

    ///
    /// \brief Destructor
    ///
    virtual ~ConnectionThread();
    
    /// 
    /// \brief Launch the connection process
    ///
    PvResult Connect();

protected:
    
    ///
    /// \brief Thread function to execute the connection
    ///
    /// \return Always 0
    ///
    virtual DWORD Function();

private:
    // Stream setup information
    Setup* mSetup;
    
    // Device information
    const PvDeviceInfoGEV* mDeviceInfoGEV;

    // Device to connect
    PvDeviceGEV* mDeviceGEV;
    
    // Stream to open
    PvStreamGEV* mStreamGEV;
    
    // Process dialog
    ProgressDlg* mProgressDialog;
    
    // Hold the result of the connection
    PvResult mResult;

    ///
    /// \brief Will connect a device
    ///
    /// \return Error state
    ///
    PvResult ConnectDevice();
    
    ///
    /// \brief Will open a stream
    ///
    /// \return Error state
    ///
    PvResult OpenStream();
};


