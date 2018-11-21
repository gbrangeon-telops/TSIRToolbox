// *****************************************************************************
//
//     Copyright (c) 2011, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#pragma once

#include "GEVPlayerDlg.h"


///
/// \brief Simple class used to receive serial bridge manager events, forwards them to GEVPlayerDlg
///

class SerialBridgeManagerWnd : public PvSerialBridgeManagerWnd
{
public:

    SerialBridgeManagerWnd( GEVPlayerDlg *aGEVPlayerDlg )
        : mGEVPlayerDlg( aGEVPlayerDlg )
    {
    }

    virtual ~SerialBridgeManagerWnd()
    {
    }

protected:

    void OnParameterArrayCreated( PvGenParameterArray *aArray, const PvString &aName )
    {
        mGEVPlayerDlg->AddDeviceParameterArray( aArray, aName );
    }

    void OnParameterArrayDeleted( PvGenParameterArray *aArray )
    {
        mGEVPlayerDlg->RemoveDeviceParameterArray( aArray );
    }

private:

    GEVPlayerDlg *mGEVPlayerDlg;
};


