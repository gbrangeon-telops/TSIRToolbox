#include "BuiltInTestsDef.h"
#include "TDCStatusDef.h"
#include "BuildInfo.h"
#include "TSIRToolbox.h"
#include <PvVersion.h>
#include <PvDevice.h>
#include <PvDeviceFinderWnd.h>
#include <stdint.h>
#include <cstdio>
#include <windows.h>

typedef enum {
	verMajor = 0,
	verMinor,
	verSubMinor,
	verBuild
} version_t;


void DisplaySelectorValues(PvGenParameterArray *deviceParams, PvString selectorName);
void DisplayIntegerValue(PvGenParameterArray *deviceParams, PvString regName, const char *unit = nullptr);
void DisplayBooleanValue(PvGenParameterArray *deviceParams, PvString regName);
void DisplayEnumValue(PvGenParameterArray *deviceParams, PvString regName);
void DisplayIpAddressValue(PvGenParameterArray *deviceParams, PvString regName);
void DisplayStorageInfo(PvGenParameterArray *deviceParams);
void DisplayCLinkInfo(PvGenParameterArray *deviceParams);
bool DisplayError(PvGenParameterArray *deviceParams);


int main(int argc, char *argv[])
{
	PvResult result;

	printf("TS-IR Information (TSIR Toolbox v%d.%d.%d.%d%s)\n", TSIR_TOOLBOX_MAJOR_VERSION, TSIR_TOOLBOX_MINOR_VERSION, TSIR_TOOLBOX_SUBMINOR_VERSION, SVN_SOFTWARE_REV, SVN_SOFTWARE_MODIFIED);

	// Select device
	PvDeviceFinderWnd deviceFinderWnd;
	result = deviceFinderWnd.ShowModal();
	if(!result.IsOK())
	{
		// User canceled
		return 0;
	}

	const PvDeviceInfo *deviceInfo = NULL;
	deviceInfo = deviceFinderWnd.GetSelected();
	if (deviceInfo == NULL)
	{
		return 1;
	}

	// Connect to the selected GEV Device
#if (VERSION_MAJOR == 3)
	PvDevice device;
	result = device.Connect(deviceInfo);
#elif (VERSION_MAJOR == 4)|| (VERSION_MAJOR == 5)
	PvDevice *device;
	device = PvDevice::CreateAndConnect(deviceInfo, &result);
#endif

	if (!result.IsOK())
	{
		printf("Unable to connect to device.\n");
		return 1;
	}

	PvGenParameterArray *deviceParams;
#if (VERSION_MAJOR == 3)
	deviceParams = device.GetGenParameters();
#elif (VERSION_MAJOR == 4)|| (VERSION_MAJOR == 5)
	deviceParams = device->GetParameters();
#endif

	// Determine if there is a storage board
	bool externalMemoryBufferIsImplemented;
	deviceParams->GetBooleanValue("ExternalMemoryBufferIsImplemented", externalMemoryBufferIsImplemented);

	printf("\n");

	// Serial number
	int64_t deviceSerialNumber;
	deviceParams->GetIntegerValue("DeviceSerialNumber", deviceSerialNumber);
	printf("Serial Number: TEL%05d\n", (uint32_t)deviceSerialNumber);

	printf("\n");

	// Firmware version
	int64_t deviceFirmwareVersion[4];
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
	printf("XML Version: %d.%d.%d\n", (uint32_t) deviceXMLVersion[verMajor], (uint32_t) deviceXMLVersion[verMinor], (uint32_t) deviceXMLVersion[verSubMinor]);

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

		printf("DeviceBuiltInTestsResults%d: 0x%08X\n", i, deviceBuiltInTestsResults[i]);
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

//	// Device errors
//	printf("Device Errors:\n");
//	if (DisplayError(deviceParams) == false)
//	{
//		printf("No Error\n");
//	}
//	else
//	{
//		while (DisplayError(deviceParams));
//	}

	// Storaqge
	printf("Storage:\n");
	DisplayStorageInfo(deviceParams);
	putchar('\n');

	// Camera Link
	printf("Camera Link:\n");
	DisplayCLinkInfo(deviceParams);

#if (VERSION_MAJOR == 3)
	device.Disconnect();
#elif (VERSION_MAJOR == 4)|| (VERSION_MAJOR == 5)
	PvDevice::Free(device);
#endif

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


void DisplayStorageInfo(PvGenParameterArray *deviceParams)
{
	PvGenInteger *p_node = deviceParams->GetInteger("MemoryBufferTotalSpace");
	if (p_node == NULL) return;
	
	int64_t regValue;
	p_node->GetValue(regValue);
	printf("Available memory: %d GB\n", regValue >> 30);
}


void DisplayCLinkInfo(PvGenParameterArray *deviceParams)
{
	double val;
	PvGenFloat * p_node;
	PvString regDisplayName;
	PvString unit;

	DisplayEnumValue(deviceParams, "ClConfiguration");
	PvGenEnum *p_conf = deviceParams->GetEnum("ClConfiguration");
	if (p_conf == NULL) return;
	int64_t ClConfValue;
	p_conf->GetValue(ClConfValue);

	/* Set clock selector */
	PvGenEnum *p_selector = deviceParams->GetEnum("DeviceClockSelector");
	if (p_selector == NULL) return;
	p_selector->SetValue(ClConfValue);
	const PvGenEnumEntry *entry;
	p_selector->GetEntryByIndex(ClConfValue, &entry);
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