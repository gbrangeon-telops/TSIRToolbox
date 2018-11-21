// *****************************************************************************
//
//     Copyright (c) 2011, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#include "CLProtocol.h"

#include <PvSampleUtils.h>
#include <PvSerialBridge.h>
#include <PvGenParameterArrayCL.h>
#include <PvDeviceAdapter.h>


#define CLDLL_PORTNAME ( "COM1" )
#define CLDLL_FULLPORTNAME ( "Pleora#MyPort" )

#define IPENGINEPORT ( PvDeviceSerialBulk0 )


void CLProtocol( PvDevice *aDevice )
{
    PvDeviceGEV* lDeviceGEV = dynamic_cast<PvDeviceGEV*>( aDevice );
    if ( lDeviceGEV != NULL )
    {
        // Using its GenICam interface, configure your device serial port
        // to match what is expected/required by the camera
        PvGenParameterArray *lDeviceParams = lDeviceGEV->GetParameters();
        // ...
    }

    // Create PvDevice adpater, used to interface the serial communication library
    PvDeviceAdapter lAdapter( aDevice );

    // Start the Camera Link DLL bridge. Only use the port name.
    PvSerialBridge lBridge;
    PvResult lResult = lBridge.Start( CLDLL_PORTNAME, &lAdapter, IPENGINEPORT );
    if ( !lResult.IsOK() )
    {
        cout << "Unable to start bridge: " << lResult.GetCodeString().GetAscii() << " " << lResult.GetDescription().GetAscii() << endl;
        return;
    }

    cout << "Camera Link DLL bridge started" << endl;

    // Retrieve all available templates
    PvStringList lTemplateList;
    PvGenParameterArrayCL::GetDeviceTemplates( lTemplateList );
    if ( lTemplateList.GetSize() <= 0 )
    {
        // If no templates are available, it means no CLProtocol DLL was available.
        // Either none are present on the system or they don't have their paths 
        // registered in the GENICAM_CLPROTOCOL environment variable.
        cout << "No templates. No CLPRotocol DLL available." << endl;
        return;
    }

    // Select template
    cout << "Templates:" << endl;
    for ( uint32_t i = 0; i < lTemplateList.GetSize(); i++ )
    {
        cout << i << ": " << lTemplateList[ i ]->GetAscii() << endl;
    }
    cout << "Template?>";
    int lTemplateSelection;
    cin >> lTemplateSelection;
    PvString lTemplate = *lTemplateList[ lTemplateSelection ];

    // Probe, turn templates into device ID. Use the fully qualified port name, including the company name - Pleora
    PvString lDeviceID = PvGenParameterArrayCL::ProbeDevice( CLDLL_FULLPORTNAME, lTemplate );
    if ( lDeviceID.GetLength() <= 0 )
    {
        cout << "Probe failed" << endl;
        return;
    }
    cout << "Device ID: " << lDeviceID.GetAscii() << endl;

    // Instantiate, connect Camera Link GenICam interface
    PvGenParameterArrayCL lArray;
    lResult = lArray.Connect( CLDLL_FULLPORTNAME, lDeviceID );
    if ( !lResult.IsOK() )
    {
        // Possible causes of error:
        // 1. The camera does not match the selected template
        // 2. The Camera Link DLL is not properly configured:
        //        The CLSERIALPATH registry string under HKEY_LOCAL_MACHINE\SOFTWARE\CameraLink
        //        is not defined or not pointing the the clserpte.dll eBUS SDK Camera Link DLL path
        cout << "Unable to connect Camera Link GenICam interface: " << lResult.GetCodeString().GetAscii() << " " << lResult.GetDescription().GetAscii() << endl;
        return;
    }

    // Retrieve XML IDs
    PvStringList lXMLIDsList;
    lArray.GetXMLIDs( lXMLIDsList );

    // Select XML
    cout << "XML IDs:" << endl;
    for ( uint32_t i = 0; i < lXMLIDsList.GetSize(); i++ )
    {
        cout << i << ": " << lXMLIDsList[ i ]->GetAscii();
    }
    cout << "XML ID?>";
    int lXMLIDSelection;
    cin >> lXMLIDSelection;
    PvString lXMLID = *lXMLIDsList[ lXMLIDSelection ];

    // Build node map
    cout << "Building node map..." << endl;
    lResult = lArray.Build( lXMLID );
    if ( !lResult.IsOK() )
    {
        cout << "Unable to build the Camera Link GenICam interface: " << lResult.GetCodeString().GetAscii() << " " << lResult.GetDescription().GetAscii() << endl;
        return;
    }

    cout << "Node map successfully built!" << endl;

    // Traverse node map until the user presses a key
    uint32_t lCount = lArray.GetCount();
    bool lDoubleBreak = false;
    for ( ;; )
    {
        for ( uint32_t i = 0; i < lCount; i++ )
        {
            PvGenParameter *lParam = lArray.Get( i );

            PvString lValue, lName, lCategory;
            lParam->GetName( lName );
            lParam->ToString( lValue );
            lParam->GetCategory( lCategory );
         
            cout << lCategory.GetAscii() << "\\" << lName.GetAscii() << ": " << lValue.GetAscii() << endl;

            if ( PvKbHit() )
            {
                lDoubleBreak = true;
                break;
            }
        }

        if ( lDoubleBreak )
        {
            break;
        }
    }

    cout << endl;
}


