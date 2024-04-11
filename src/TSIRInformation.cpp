#include <PvVersion.h>
#include <PvDevice.h>
#include <PvDeviceFinderWnd.h>
#include <stdint.h>
#include <cstring>
#include <ctime>
#include <cstdio>
#include <windows.h>
#include "BuiltInTestsDef.h"
#include "TDCStatusDef.h"
#include "BuildInfo.h"
#include "TSIRToolbox.h"
#include "FileManager.h"

typedef enum {
	verMajor = 0,
	verMinor,
	verSubMinor,
	verBuild
} version_t;

/* Prototypes */
void DisplaySelectorValues(PvGenParameterArray *deviceParams, PvString selectorName);
void DisplayIntegerValue(PvGenParameterArray *deviceParams, PvString regName, const char *unit = nullptr);
void DisplayBooleanValue(PvGenParameterArray *deviceParams, PvString regName);
void DisplayEnumValue(PvGenParameterArray *deviceParams, PvString regName);
void DisplayIpAddressValue(PvGenParameterArray *deviceParams, PvString regName);
void DisplayNTxMiniInfo(PvGenParameterArray *deviceParams);
void DisplayStorageInfo(PvGenParameterArray *deviceParams);
void DisplayCLinkInfo(PvGenParameterArray *deviceParams);
bool DisplayError(PvGenParameterArray *deviceParams);

/* Globals */
int64_t deviceFirmwareVersion[4];
bool externalMemoryBufferIsImplemented;

int main(int argc, char *argv[])
{
	PvResult result;
	FileManager fm;
	time_t t;
	struct tm *tm;
	long int tzhour, tzmin;
	
	// Get current date (local timezone)
	t = time(NULL);
	tm = localtime(&t);

	// Display start
	printf("TS-IR Information (TSIR Toolbox v%d.%d.%d.%d%s)\n", TSIR_TOOLBOX_MAJOR_VERSION, TSIR_TOOLBOX_MINOR_VERSION, TSIR_TOOLBOX_SUBMINOR_VERSION, SVN_SOFTWARE_REV, SVN_SOFTWARE_MODIFIED);

	// Display local time
	tzhour = -_timezone / 3600;
	tzmin = (_timezone % 3600) / 60;
	printf("Report Date: %04d-%02d-%02dT%02d:%02d:%02d%0+3ld:%02ld\n", tm->tm_year+1900, tm->tm_mon+1, tm->tm_mday, tm->tm_hour, tm->tm_min, tm->tm_sec,
		tzhour, tzmin);

	if (fm.Open() != IRC_SUCCESS)
	{
		fprintf(stderr, "Unable to open port.\n");
		return IRC_FAILURE;
	}

	// Get device parameters
	PvGenParameterArray *deviceParams = fm.GetGenParams();

	// Determine if there is a storage board
	deviceParams->GetBooleanValue("ExternalMemoryBufferIsImplemented", externalMemoryBufferIsImplemented);

	printf("\n");

	// NTx-Mini information
	DisplayNTxMiniInfo(deviceParams);
	printf("\n");
	
	// Serial number
	int64_t deviceSerialNumber;
	deviceParams->GetIntegerValue("DeviceSerialNumber", deviceSerialNumber);
	printf("Serial Number: TEL%05d\n", (uint32_t)deviceSerialNumber);

	printf("\n");

	// Firmware version
	deviceParams->GetIntegerValue("DeviceFirmwareMajorVersion", deviceFirmwareVersion[verMajor]);
	deviceParams->GetIntegerValue("DeviceFirmwareMinorVersion", deviceFirmwareVersion[verMinor]);
	deviceParams->GetIntegerValue("DeviceFirmwareSubMinorVersion", deviceFirmwareVersion[verSubMinor]);
	deviceParams->GetIntegerValue("DeviceFirmwareBuildVersion", deviceFirmwareVersion[verBuild]);
	printf("Firmware Version: %d.%d.%d.%d\n", (uint32_t) deviceFirmwareVersion[verMajor], (uint32_t) deviceFirmwareVersion[verMinor],
		(uint32_t) deviceFirmwareVersion[verSubMinor], (uint32_t) deviceFirmwareVersion[verBuild]);

	if ((deviceFirmwareVersion[verMajor] == 0) && (deviceFirmwareVersion[verMinor] == 0) &&
		(deviceFirmwareVersion[verSubMinor] == 0) && (deviceFirmwareVersion[verBuild] == 0))
	{
		// Firmware modules revision
		DisplaySelectorValues(deviceParams, "DeviceFirmwareModuleSelector");
	}

	printf("\n");

	// XML version
	int64_t deviceXMLVersion[3];
	deviceParams->GetIntegerValue("DeviceXMLMajorVersion", deviceXMLVersion[verMajor]);
	deviceParams->GetIntegerValue("DeviceXMLMinorVersion", deviceXMLVersion[verMinor]);
	deviceParams->GetIntegerValue("DeviceXMLSubMinorVersion", deviceXMLVersion[verSubMinor]);
	printf("Camera XML Version: %d.%d.%d\n", (uint32_t) deviceXMLVersion[verMajor], (uint32_t) deviceXMLVersion[verMinor], (uint32_t) deviceXMLVersion[verSubMinor]);

	printf("\n");

	// Device key validation
	if ((deviceXMLVersion[verMajor] > 11) || ((deviceXMLVersion[verMajor] == 11) && (deviceXMLVersion[verMinor] >= 3)))
	{
		int64_t deviceKeyValidationLow, deviceKeyValidationHigh;
		deviceParams->GetIntegerValue("DeviceKeyValidationLow", deviceKeyValidationLow);
		deviceParams->GetIntegerValue("DeviceKeyValidationHigh", deviceKeyValidationHigh);

		printf("Device Key Validation: 0x%08X%08X\n", (uint32_t)deviceKeyValidationHigh, (uint32_t)deviceKeyValidationLow);
	}
	else
	{
		printf("Device key is not supported.\n");
	}

	printf("\n");

	// Running time
	DisplayIntegerValue(deviceParams, "DeviceRunningTime", "s");
	DisplayIntegerValue(deviceParams, "DevicePowerOnCycles");
	DisplayIntegerValue(deviceParams, "DeviceCoolerRunningTime", "s");
	DisplayIntegerValue(deviceParams, "DeviceCoolerPowerOnCycles");

	printf("\n");

	// User settings
	DisplayBooleanValue(deviceParams, "PowerOnAtStartup");
	DisplayBooleanValue(deviceParams, "AcquisitionStartAtStartup");
	DisplayBooleanValue(deviceParams, "StealthMode");
	DisplayBooleanValue(deviceParams, "BadPixelReplacement");

	printf("\n");

	// Calibration memory configuration
	bool mem4DDRIsImplemented;
	deviceParams->GetBooleanValue("Mem4DDRIsImplemented", mem4DDRIsImplemented);
	printf("Calibration Memory: %s\n", (mem4DDRIsImplemented) ? "4 DDR3" : "2 DDR3");

	printf("\n");

	// Device status
	int64_t tdcStatus;
	deviceParams->GetIntegerValue("TDCStatus", tdcStatus);
	printf("TDCStatus: 0x%08X\n", (uint32_t) tdcStatus);
	DisplayTDCStatusBits((uint32_t) tdcStatus);
	printf("\n");

	// Device power state
	DisplayEnumValue(deviceParams, "DevicePowerState");
	printf("\n");

	// GEV Configuration
	printf("GEV Configuration:\n");
	DisplayBooleanValue(deviceParams, "GevCurrentIPConfigurationLLA");
	DisplayBooleanValue(deviceParams, "GevCurrentIPConfigurationDHCP");
	DisplayBooleanValue(deviceParams, "GevCurrentIPConfigurationPersistentIP");
	DisplayIpAddressValue(deviceParams, "GevCurrentIPAddress");
	DisplayIpAddressValue(deviceParams, "GevCurrentSubnetMask");
	DisplayIpAddressValue(deviceParams, "GevCurrentDefaultGateway");
	DisplayEnumValue(deviceParams, "GevIPConfigurationStatus");
	DisplayIpAddressValue(deviceParams, "GevPersistentIPAddress");
	DisplayIpAddressValue(deviceParams, "GevPersistentSubnetMask");
	DisplayIpAddressValue(deviceParams, "GevPersistentDefaultGateway");
	printf("\n");

	// Device built-in tests result
	uint32_t deviceBuiltInTestRegCount = BUILT_IN_TEST_PROC_REG_COUNT + BUILT_IN_TEST_OUTPUT_REG_COUNT;
	if (externalMemoryBufferIsImplemented) deviceBuiltInTestRegCount += BUILT_IN_TEST_STORAGE_REG_COUNT;

	int64_t *deviceBuiltInTestsResults = new int64_t[deviceBuiltInTestRegCount];

	for (uint32_t i = 0; i < deviceBuiltInTestRegCount; i++)
	{
		char strDeviceBuiltInTestsResultsRegName[32];
		sprintf_s(strDeviceBuiltInTestsResultsRegName, "DeviceBuiltInTestsResults%d", i + 1);
		deviceParams->GetIntegerValue(strDeviceBuiltInTestsResultsRegName, deviceBuiltInTestsResults[i]);

		printf("DeviceBuiltInTestsResults%u: 0x%08X\n", i+1, (uint32_t) deviceBuiltInTestsResults[i]);
		DisplayBuiltInTestsResults((uint32_t) deviceBuiltInTestsResults[i], i);

		printf("\n");
	}

	builtInTestResult_t procBuiltInTestGlobalResult = getBuiltInTestResult(deviceBuiltInTestsResults[BUILT_IN_TEST_PROC_GBL_RES_REG_IDX], 0);
	builtInTestResult_t outputBuiltInTestGlobalResult = getBuiltInTestResult(deviceBuiltInTestsResults[BUILT_IN_TEST_OUTPUT_GBL_RES_REG_IDX], 0);
	builtInTestResult_t storageBuiltInTestGlobalResult = BITR_Not_Applicable;
	if (externalMemoryBufferIsImplemented) storageBuiltInTestGlobalResult = getBuiltInTestResult(deviceBuiltInTestsResults[BUILT_IN_TEST_STORAGE_GBL_RES_REG_IDX], 0);

	builtInTestResult_t deviceBuiltInTestsGlobalResult = BITR_Passed;

	if ((procBuiltInTestGlobalResult == BITR_Failed) || (outputBuiltInTestGlobalResult == BITR_Failed) || (storageBuiltInTestGlobalResult == BITR_Failed))
	{
		deviceBuiltInTestsGlobalResult = BITR_Failed;
	}
	else if ((procBuiltInTestGlobalResult == BITR_Pending) || (outputBuiltInTestGlobalResult == BITR_Pending) || (storageBuiltInTestGlobalResult == BITR_Pending))
	{
		deviceBuiltInTestsGlobalResult = BITR_Pending;
	}
	printf("DeviceBuiltInTestsGlobalResult: %s (%d)\n", BuiltInTestResultToString(deviceBuiltInTestsGlobalResult), deviceBuiltInTestsGlobalResult);

	printf("\n");

	// Device serial ports
	printf("Device Serial ports:\n");
	DisplaySelectorValues(deviceParams, "DeviceSerialPortSelector");
	printf("\n");

	// Device temperatures
	printf("Device Temperatures:\n");
	DisplaySelectorValues(deviceParams, "DeviceTemperatureSelector");
	printf("\n");

	// Device voltages
	printf("Device Voltages:\n");
	DisplaySelectorValues(deviceParams, "DeviceVoltageSelector");
	printf("\n");

	// Device currents
	printf("Device Currents:\n");
	DisplaySelectorValues(deviceParams, "DeviceCurrentSelector");
	printf("\n");

	// Storaqge
	printf("Storage:\n");
	DisplayStorageInfo(deviceParams);
	putchar('\n');

	// Camera Link
	printf("Camera Link:\n");
	DisplayCLinkInfo(deviceParams);
	putchar('\n');

	// Print file list and flash filesystem space
	printf("List of files:\n");
	if (fm.FileList() != IRC_SUCCESS)
	{
		fprintf(stderr, "Failed to list file(s).\n");
		fm.Close();
		return IRC_FAILURE;
	}

	fm.Close();
	return 0;
}

void DisplaySelectorValues(PvGenParameterArray *deviceParams, PvString selectorName)
{
	PvGenEnum *p_selector = deviceParams->GetEnum(selectorName);
	if (p_selector == NULL) return;

	int64_t selectorEnumEntriesCount;
	p_selector->GetEntriesCount(selectorEnumEntriesCount);

	PvGenParameterList selectedParams;
	deviceParams->Get(selectorName)->GetSelectedParameters(selectedParams);

	int64_t selectedParamsCount = selectedParams.GetSize();
	bool displaySelectedParamName = (selectedParamsCount > 1);

	for (uint32_t i = 0; i < selectorEnumEntriesCount; i++)
	{
		const PvGenEnumEntry *entry;
		p_selector->GetEntryByIndex(i, &entry);

		PvString entryName;
		entry->GetName(entryName);

		PvString entryDisplayName;
		entry->GetDisplayName(entryDisplayName);

		p_selector->SetValue(entryName);
		Sleep(100); // Gives the time to selected registers to be invalidated. Some values are zero otherwise.

		for (uint32_t j = 0; j < selectedParamsCount; j++)
		{
			printf("%s", entryDisplayName.GetAscii());
			if (displaySelectedParamName)
			{
				PvString selectedParamDisplayName;
				selectedParams.GetItem(j)->GetDisplayName(selectedParamDisplayName);
				printf(" %s", selectedParamDisplayName.GetAscii());
			}
			printf(": ");

			PvGenType selectedParamType;
			selectedParams.GetItem(j)->GetType(selectedParamType);

			if (selectedParamType == PvGenTypeFloat)
			{
				double selectedParamValue;
				PvGenFloat *genFloat;
				PvString unit;
				deviceParams->GetFloatValue(selectedParams.GetItem(j)->GetName(), selectedParamValue);
				genFloat = deviceParams->GetFloat(selectedParams.GetItem(j)->GetName());
				genFloat->GetUnit(unit);
				printf("%0.2f %s\n", selectedParamValue, unit.GetAscii());
			}
			else if (selectedParamType == PvGenTypeInteger)
			{
				int64_t selectedParamValue;
				deviceParams->GetIntegerValue(selectedParams.GetItem(j)->GetName(), selectedParamValue);
				printf("%d\n", (uint32_t) selectedParamValue);
			}
			else if (selectedParamType == PvGenTypeEnum)
			{
				int64_t selectedParamValue;
				deviceParams->GetEnumValue(selectedParams.GetItem(j)->GetName(), selectedParamValue);

				const PvGenEnumEntry *enumEntry;
				deviceParams->GetEnum(selectedParams.GetItem(j)->GetName())->GetEntryByValue(selectedParamValue, &enumEntry);

				PvString enumDisplayName;
				enumEntry->GetDisplayName(enumDisplayName);

				printf("%s (%d)\n", enumDisplayName.GetAscii(), (uint32_t) selectedParamValue);
			}
		}
	}
}

void DisplayIntegerValue(PvGenParameterArray *deviceParams, PvString regName, const char *unit)
{
	PvGenInteger *p_node = deviceParams->GetInteger(regName);
	if (p_node == NULL) return;

	PvString regDisplayName;
	p_node->GetDisplayName(regDisplayName);

	int64_t regValue;
	p_node->GetValue(regValue);

	if (unit)
		printf("%s: %d %s\n", regDisplayName.GetAscii(), (uint32_t) regValue, unit);
	else
		printf("%s: %d\n", regDisplayName.GetAscii(), (uint32_t)regValue);
}

void DisplayBooleanValue(PvGenParameterArray *deviceParams, PvString regName)
{
	PvGenBoolean *p_node = deviceParams->GetBoolean(regName);
	if (p_node == NULL) return;

	PvString regDisplayName;
	p_node->GetDisplayName(regDisplayName);

	bool regValue;
	p_node->GetValue(regValue);

	printf("%s: %s\n", regDisplayName.GetAscii(), regValue? "True": "False");
}

void DisplayEnumValue(PvGenParameterArray *deviceParams, PvString regName)
{
	PvGenEnum *p_node = deviceParams->GetEnum(regName);
	if (p_node == NULL) return;

	PvString regDisplayName;
	p_node->GetDisplayName(regDisplayName);

	int64_t regValue;
	p_node->GetValue(regValue);

	const PvGenEnumEntry *enumEntry;
	p_node->GetEntryByValue(regValue, &enumEntry);

	PvString enumDisplayName;
	enumEntry->GetDisplayName(enumDisplayName);

	printf("%s: %s (%d)\n", regDisplayName.GetAscii(), enumDisplayName.GetAscii(), (uint32_t) regValue);
}

void DisplayIpAddressValue(PvGenParameterArray *deviceParams, PvString regName)
{
	PvGenInteger *p_node = deviceParams->GetInteger(regName);
	if (p_node == NULL) return;

	PvString regDisplayName;
	p_node->GetDisplayName(regDisplayName);

	int64_t regValue;
	p_node->GetValue(regValue);

	uint8_t *regValueBytes = (uint8_t *) &regValue;

	printf("%s: %d.%d.%d.%d\n", regDisplayName.GetAscii(),
		regValueBytes[3], regValueBytes[2], regValueBytes[1], regValueBytes[0]);
}

void DisplayNTxMiniInfo(PvGenParameterArray *deviceParams)
{
	// NTx-Mini platform
	int64_t IPEngineDeviceID;
	int ntxPlatformSel = 0 ;
	const char *ntxPlatforms[] = { "PT01-PBXMX1-32XG33-v1.24.0", "PT01-PBXMX4-32XG33-v0.5.0" };
	deviceParams->GetIntegerValue("IPEngineDeviceID", IPEngineDeviceID);
	if (IPEngineDeviceID == 20) ntxPlatformSel = 0;
	if (IPEngineDeviceID == 34) ntxPlatformSel = 1;
	printf("NTx-Mini Platform: %s\n", ntxPlatforms[ntxPlatformSel]);

	// NTx-Mini XML version
	PvString gevFirstURL;
	char *s, *tok;
	int ntxMiniXmlMajorVersion;
	int ntxMiniXmlMinorVersion;
	int ntxMiniXmlSubMinorVersion;
	#if (VERSION_MAJOR == 4) || (VERSION_MAJOR == 5)
		deviceParams->GetStringValue("GevFirstURL", gevFirstURL);
	#else
		deviceParams->GetString("GevFirstURL", gevFirstURL);
	#endif
	s = _strdup(gevFirstURL.GetAscii());
	tok = strtok(s, "_");
	tok = strtok(NULL, "_");
	sscanf(tok, "%d", &ntxMiniXmlMajorVersion);
	tok = strtok(NULL, "_");
	sscanf(tok, "%d", &ntxMiniXmlMinorVersion);
	tok = strtok(NULL, "_");
	sscanf(tok, "%d", &ntxMiniXmlSubMinorVersion);
	printf("NTx-Mini XML Version: %d.%d.%d\n", ntxMiniXmlMajorVersion, ntxMiniXmlMinorVersion, ntxMiniXmlSubMinorVersion);
	free(s);
}

void DisplayStorageInfo(PvGenParameterArray *deviceParams)
{
	printf("External storage board present: %s\n", externalMemoryBufferIsImplemented ? "yes" : "no");

	if (deviceFirmwareVersion[verMajor] == 0 || deviceFirmwareVersion[verMajor] > 2 ||
		(deviceFirmwareVersion[verMajor] == 2 && deviceFirmwareVersion[verMinor] >= 6))
	{
		PvGenInteger *p_node1 = deviceParams->GetInteger("MemoryBufferTotalSpaceHigh");
		PvGenInteger *p_node2 = deviceParams->GetInteger("MemoryBufferTotalSpaceLow");
		if (p_node1 == NULL || p_node2 == NULL) return;

		int64_t highRegValue, lowRegValue;
		int64_t totalSpace;
		p_node1->GetValue(highRegValue);
		p_node2->GetValue(lowRegValue);
		totalSpace = (highRegValue << 32) | lowRegValue;

		if ((totalSpace >> 20) >= 1024)
			printf("Available memory: %lld GB\n", totalSpace >> 30);
		else
			printf("Available memory: %lld MB\n", totalSpace >> 20);
	}
	else 
		printf("Available memory: %d GB\n", externalMemoryBufferIsImplemented ? 16 : 1);
}


void DisplayCLinkInfo(PvGenParameterArray *deviceParams)
{
	double val;
	PvGenFloat * p_node;
	PvString regDisplayName;
	PvString unit;

	DisplayEnumValue(deviceParams, "ClConfiguration");

	/* Set clock selector */
	PvGenEnum *p_selector = deviceParams->GetEnum("DeviceClockSelector");
	if (p_selector == NULL) return;
	p_selector->SetValue(2);  // Camera Link
	const PvGenEnumEntry *entry;
	p_selector->GetEntryByIndex(2, &entry);
	PvString entryDisplayName;
	entry->GetDisplayName(entryDisplayName);
	printf("Device Clock Selector: %s\n", entryDisplayName.GetAscii());

	deviceParams->GetFloatValue("DeviceClockFrequency", val);
	p_node = deviceParams->GetFloat("DeviceClockFrequency");
	p_node->GetUnit(unit);
	p_node->GetDisplayName(regDisplayName);
	printf("%s: %0.1f %s\n", regDisplayName.GetAscii(), val, unit.GetAscii());
}

bool DisplayError(PvGenParameterArray *deviceParams)
{
	int64_t eventError;
	deviceParams->GetIntegerValue("EventError", eventError);

	if (eventError == 0)
	{
		return false;
	}

	int64_t eventErrorTimestamp;
	deviceParams->GetIntegerValue("EventErrorTimestamp", eventErrorTimestamp);

	int64_t eventErrorCode;
	deviceParams->GetIntegerValue("EventErrorCode", eventErrorCode);

	const PvGenEnumEntry *eventErrorCodeDescEntry;
	deviceParams->GetEnum("EventErrorCodeDesc")->GetEntryByValue(eventErrorCode, &eventErrorCodeDescEntry);

	PvString eventErrorCodeDesc;
	eventErrorCodeDescEntry->GetDescription(eventErrorCodeDesc);

	printf("%010d: %s (%d)\n",(uint32_t) eventErrorTimestamp, eventErrorCodeDesc.GetAscii(), (uint32_t) eventErrorCode);

	return true;
}