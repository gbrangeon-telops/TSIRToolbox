// *****************************************************************************
//
//     Copyright (c) 2011, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#include "SerialCOMPortBridge.h"

#include <PvSampleUtils.h>
#include <PvSerialBridge.h>


#define BRIDGECOMPORT ( "COM10" )
#define BRIDGECOMPORT_BAUDRATE ( 9600 )
#define BRIDGECOMPORT_PARITY ( PvParityNone )
#define BRIDGECOMPORT_BYTESIZE ( 8 )
#define BRIDGECOMPORT_STOPBITS ( 1 )

#define IPENGINEPORT ( PvIPEngineSerial0 )


void SerialCOMPortBridge( PvDevice *aDevice )
{
    // Using its GenICam interface, configure your IP Engine serial port
    // to match what is expected/required by the camera
    PvGenParameterArray *lDeviceParams = aDevice->GetGenParameters();
    // ...

    // Prepare COM port configuration
    PvSerialPortConfiguration lConfig;
    lConfig.mBaudRate = BRIDGECOMPORT_BAUDRATE;
    lConfig.mParity = BRIDGECOMPORT_PARITY;
    lConfig.mByteSize = BRIDGECOMPORT_BYTESIZE;
    lConfig.mStopBits = BRIDGECOMPORT_STOPBITS;

    // Start the bridge on COM10, UART0 on the device
    PvSerialBridge lBridge;
    PvResult lResult = lBridge.Start( BRIDGECOMPORT, lConfig, aDevice, IPENGINEPORT );
    if ( !lResult.IsOK() )
    {
		cout << "Unable to start bridge: " << lResult.GetCodeString().GetAscii() << " " << lResult.GetDescription().GetAscii() << endl;
        return;
    }

    cout << "Serial COM port bridge started on " << BRIDGECOMPORT << endl;

    // Let the bridge run until a key is pressed, provides stats
    while ( !PvKbHit() )
    {
		cout << "Tx: " << lBridge.GetBytesSentToDevice() << " Rx: " << lBridge.GetBytesReceivedFromDevice() << "\r";

        ::Sleep( 50 );
    }

    cout << "" << endl;
}


