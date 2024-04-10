#include <iostream>
#include <PvVersion.h>
#include <PvDevice.h>
#include <PvDeviceFinderWnd.h>
#include "BuildInfo.h"
#include "TSIRToolbox.h"
#include "FileManager.h"

using namespace std;

#define EXIT_PBXMX1 0
#define EXIT_PBXMX4 1
#define EXIT_ERROR 255

int main()
{
	PvResult result;
	FileManager fm;
	PvGenParameterArray *deviceParams;
	int64_t IPEngineDeviceID;
	int retval;

	// Get device parameters
	if (fm.Open() != IRC_SUCCESS)
	{
		cerr << "Unable to open port." << endl;
		return EXIT_FAILURE;
	}
	deviceParams = fm.GetGenParams();

	// Detect NTX-Mini platform
	deviceParams->GetIntegerValue("IPEngineDeviceID", IPEngineDeviceID);
	if (IPEngineDeviceID == 20) retval = EXIT_PBXMX1;
	else if (IPEngineDeviceID == 34) retval = EXIT_PBXMX4;
	else retval = EXIT_ERROR;

	// Return exit code to OS
	return retval;
}
