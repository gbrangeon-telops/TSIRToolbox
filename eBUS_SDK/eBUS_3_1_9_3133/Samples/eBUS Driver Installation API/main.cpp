// *****************************************************************************
//
// Copyright (c) 2010, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#include <EbInstaller.h>

#include <stdio.h>
#include <iostream>
using namespace std;

//
// Class inheriting from EbInstallerEventSink for install callbacks
//

class EventSink : public EbInstallerEventSink
{
	// Callback on progress % - just printf
    virtual void OnUpdateProgress( PtUInt32 aProgress )
    {
        cout << aProgress << " percent complete" << endl;
    }

	// Callback on status message - just printf
    virtual void OnStatusMessage( PtString aMsg )
    {
        cout << aMsg.GetAscii() << endl;
    }
};


int main( void )
{
	// THE installer object
	EbInstaller lInstaller;

	// Event sink used for callbacks
	EventSink lSink;
	lInstaller.RegisterEventSink( &lSink );

	// Initialize installer
	PtResult lResult = lInstaller.Initialize();
	if ( !lResult.IsOK() )
	{
		cout << "Unable to initialize installer: " << lResult.GetCodeString().GetAscii() << ", " << lResult.GetDescription().GetAscii() << endl;

		// Failure to initialize installer, not much we can do...
		return -1;
	}
    
    while (true)
    {
        const EbNetworkAdapter* lAdapter = NULL;
        const EbDriver* lDriver = NULL;
        unsigned int i = 0;
        int lAdapterIndex = -1;
        int lDriverIndex = -1;
        char lSelected = 0;

		cout << endl;
		cout << "Type 'u' to uninstall all, 'l' to list network adapters and drivers installed on the system, 'i' to install a driver on a specific card, 'q' to quit. " << endl;
        std::cin >> lSelected;

        switch(lSelected)
        {
        case 'q': 
			// Quit. Just do that...
			// -------------------------------------------------------------
            return 0;

        case 'l':
			// List available network adapters and drivers
			// -------------------------------------------------------------

			// Go through installer's adapter list
            cout << "Available adapters:" << endl;
            for ( i = 0; i < lInstaller.GetNetworkAdapterCount(); i++ )
            {
                lAdapter = lInstaller.GetNetworkAdapter( i );
                if ( lAdapter != NULL )
                {
					// Display adapter information
                    cout << i << ": Adapter " << lAdapter->GetMACAddress().GetAscii();

					// eBUS driver on this adapter?
                    lDriver = lAdapter->GetCurrentDriver();
                    if ( lDriver != NULL )
                    {
						// Has an eBUS driver, indicate which one
                        cout << "has driver " << lDriver->GetName().GetAscii();
                    }
                    if ( lAdapter->IsRebootNeeded() )
                    {
						// Output if an install/uninstall operation requires reboot on that adapter
                        cout << " and requires a reboot.";
                    }

                    cout << endl;
                }
                
            }

			// Go through available drivers
			cout << endl;
            cout << "Available drivers:" << endl;
            for ( i = 0; i < lInstaller.GetDriverCount(); i++ )
            {
                lDriver = lInstaller.GetDriver( i );
                if ( lDriver != NULL )
                {
					// Display dirver information
					cout << i << " " << lDriver->GetDisplayName().GetAscii() << " - " << lDriver->GetDescription().GetAscii()<< endl;
                }
            }

            break;

        case 'i':
			// Install driver
			// -------------------------------------------------------------

            if ( lInstaller.IsRebootNeeded() )
            {
                cout << "A reboot is required before proceeding" << endl;
                break;
            }

			// Ask for adapter index (from list)
            cout << "Type adapter index" << endl;
			cin >> lAdapterIndex;
			// Ask for driver index (from list)
            cout << "Type driver index" << endl;
			cin >> lDriverIndex;
			// Get pointers on adapter, driver
            lAdapter = lInstaller.GetNetworkAdapter( lAdapterIndex );
            lDriver = lInstaller.GetDriver( lDriverIndex );

			// Validate adapter
            if ( lAdapter == NULL )
            {
                cout << "Invalid adapter selection." << endl;
                break;
            }

			// Validate driver
            if ( lDriver == NULL || !lAdapter->IsDriverSupported( lDriver ))
            {
                cout << "Invalid driver selection." << endl;
                break;
            }
            
			// Install driver
            lResult = lInstaller.Install( lAdapter, lDriver );
            if( lResult == PtResult::Code::OK )
            {
				// Success
                cout << "Installation complete." << endl;
            }
            else if ( lResult == PtResult::Code::REBOOT_AND_RECALL )
            {
				// Success: reboot and call the driver installation tool (or this sample) to complete installation
                cout << "The installation process must resume after a reboot. Please reboot and perform the same action again after reboot to complete the installation." << endl;
            }
            else if ( lResult == PtResult::Code::REBOOT_NEEDED )
            {
				// Success: reboot to complete
                cout << "Please reboot your PC before using this network adapter" << endl;
            }
            else
            {
				// Installation failed
				cout << "Installation failed: " << lResult.GetCodeString().GetAscii() << " " << lResult.GetDescription().GetAscii() << endl;
				cout << "Please reboot your PC in order to ensure that your network adapter is restored to a valid state." << endl;
            }

            break;

        case 'u':
			// Install all drivers
			// -------------------------------------------------------------

			// Uninstall all drivers
			lResult = lInstaller.UninstallAll();
            if( lResult == PtResult::Code::OK )
            {
				// Success
                cout << "Installation complete." << endl;
            }
            else if ( lResult == PtResult::Code::REBOOT_NEEDED )
            {
				// Success: reboot required
                cout << "Reboot required." << endl;
            }
            else
            {
				// Uninstallation not successfull
				cout << "Uninstallation did not complete successfully: " << lResult.GetCodeString().GetAscii() << " " << lResult.GetDescription().GetAscii() << endl;
            }

			break;

        }
    }

    return 0;
}

