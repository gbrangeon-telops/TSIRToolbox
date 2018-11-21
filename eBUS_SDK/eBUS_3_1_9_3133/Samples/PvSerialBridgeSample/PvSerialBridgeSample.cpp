// *****************************************************************************
//
//     Copyright (c) 2011, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#include <PvSampleUtils.h>
#include <PvDeviceFinderWnd.h>
#include <PvDevice.h>

#include "SerialCOMPortBridge.h"
#include "CameraLinkDLLBridge.h"
#include "CLProtocol.h"

PV_INIT_SIGNAL_HANDLER();

//
// Selects and connects a device
//

bool ConnectDevice( PvDevice *aDevice )
{
    // Create a GEV Device finder dialog
    PvDeviceFinderWnd lDeviceFinderWnd;

    // Prompt the user to select a GEV Device
    lDeviceFinderWnd.ShowModal();

    // Get the connectivity information for the selected GEV Device
    PvDeviceInfo* lDeviceInfo = lDeviceFinderWnd.GetSelected();

    // If no device is selected, abort
    if( lDeviceInfo == NULL )
    {
        cout << "No device selected." << endl;
        return false;
    }

    // Connect to the GEV Device
    cout << "Connecting to " << lDeviceInfo->GetMACAddress().GetAscii() << endl;
    if ( !aDevice->Connect( lDeviceInfo ).IsOK() )
    {
        cout << "Unable to connect to " << lDeviceInfo->GetMACAddress().GetAscii() << endl;
        return false;
    }
    cout << "Successfully connected to " << lDeviceInfo->GetMACAddress().GetAscii() << endl;
    cout << endl;

    return true;
}


//
// Main function
//

int main( int argc, char* argv[] )
{
    cout << "PvSerialBridge sample - serial device control through an IP Engine" << endl << endl;

    // Select, connect a PvDevice
    PvDevice lDevice;
    if ( !ConnectDevice( &lDevice ) )
    {
        return -1;
    }

    int lSelection = 0;
    while ( ( lSelection < 1 ) || ( lSelection > 3 ) )
    {
        cout << "1. Start COM bridge" << endl;
        cout << "2. Start CameraLink DLL bridge" << endl;
        cout << "3. Build CLProtocol camera interface" << endl;
        cout << "?>";

        cin >> lSelection;
    }

    cout << "" << endl;

    switch ( lSelection )
    {
    case 1:
        SerialCOMPortBridge( &lDevice );
        break;

    case 2:
        CameraLinkDLLBridge( &lDevice );
        break;

    case 3:
        CLProtocol( &lDevice );
        break;
    }

    return 0;
}


