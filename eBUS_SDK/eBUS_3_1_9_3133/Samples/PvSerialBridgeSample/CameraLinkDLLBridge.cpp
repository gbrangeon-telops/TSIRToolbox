// *****************************************************************************
//
//     Copyright (c) 2011, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#include "CameraLinkDLLBridge.h"

#include <PvSampleUtils.h>
#include <PvSerialBridge.h>


#define CLDLL_PORTNAME ( "MyPort" )

#define IPENGINEPORT ( PvIPEngineSerial0 )


void CameraLinkDLLBridge( PvDevice *aDevice )
{
    // Using its GenICam interface, configure your IP Engine serial port
    // to match what is expected/required by the camera
    PvGenParameterArray *lDeviceParams = aDevice->GetGenParameters();
    // ...

    // Start the Camera Link DLL bridge
    PvSerialBridge lBridge;
    PvResult lResult = lBridge.Start( CLDLL_PORTNAME, aDevice, IPENGINEPORT );
    if ( !lResult.IsOK() )
    {
        cout << "Unable to start bridge: " << lResult.GetCodeString().GetAscii() << " " << lResult.GetDescription().GetAscii() << endl;
        return;
    }

    cout << "Camera Link DLL bridge started" << endl;

    // Let the bridge run until a key is pressed, provides stats
    while ( !PvKbHit() )
    {
		cout << "Tx: " << lBridge.GetBytesSentToDevice() << " Rx: " << lBridge.GetBytesReceivedFromDevice() << "\r";
        ::Sleep( 50 );
    }

    cout << endl;
}


