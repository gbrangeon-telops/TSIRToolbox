#include "BuiltInTestsDef.h"
#include <cstdio>
#include <string>

#define TSIR_DEVICE_BUILT_IN_TEST_COUNT numof(TSIR_DEVICE_BUILT_IN_TEST)



char *BuiltInTestResultToString(builtInTestResult_t builtInTestResult)
{
	static char *builtInTestResultLabels[] = {
		"Not Applicable",
		"Pending",
		"Passed",
		"Failed"
	};

	return builtInTestResultLabels[builtInTestResult];
}

void DisplayBuiltInTestsResults(uint32_t builtInTestsResultsRegValue, uint32_t builtInTestsResultsRegIndex)
{
	char *builtInTestsLabels[] = {
		// DeviceBuiltInTestsResults1
		"BuiltInTestsGlobalResult",
		"BuiltInTestsVerification",
		"DebugTerminalInitialization",
		"TimerInitialization",
		"InterruptControllerInitialization",
		"NetworkInterfaceInitialization",
		"GenICamManagerInitialization",
		"PowerManagerInitialization",
		"LedControllerInitialization",
		"FlashSettingsManagerInitialization",
		"FileSystemInitialization",
		"FileManagerInitialization",
		"QSPIFlashInterfaceInitialization",
		"FirmwareUpdaterInitialization",
		"FANControllerInitialization",
		"EHDRIControllerInitialization",

		// DeviceBuiltInTestsResults2
		"ExposureTimeControllerInitialization",
		"TriggerControllerInitialization",
		"GPSInterfaceInitialization",
		"MotorControllerInterfaceInitialization",
		"FWControllerInitialization",
		"NDFControllerInitialization",
		"CalibrationDataFlowInitialization",
		"MGTInterfaceInitialization",
		"ICUControllerInitialization",
		"ADCControllerInitialization",
		"AECControllerInitialization",
		"MemoryBufferControllerInitialization",
		"FlashDynamicValuesInitialization",
		"InterruptControllerStartup",
		"FirmwareReleaseInfoInitialization",
		"NetworkHostsReady",

		// DeviceBuiltInTestsResults3
		"NetworkHostsSynchronization",
		"FirmwareRevisionsConsistency",
		"SensorControllerInitialization",
		"SensorControllerDetection",
		"CoolerVoltageVerification",
		"CoolerCurrentVerification",
		"Cooldown",
		"ActualizationDataAcquisition",
		"FlashSettingsFileLoading",
		"CalibrationFilesLoading",
		"DeviceKeyValidation",
		"SensorInitialization",
		"DeviceSerialPortsInitialization",
		"BoardRevisionValidation",
		"MotorizedLensInitialization",
		"AutofocusModuleInitialization",

		// DeviceBuiltInTestsResults4
		"FrameBufferInitialization",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",

		// DeviceBuiltInTestsResults5
		"BuiltInTestsGlobalResult",
		"BuiltInTestsVerification",
		"TimerInitialization",
		"InterruptControllerInitialization",
		"NetworkInterfaceInitialization",
		"GenICamManagerInitialization",
		"LedControllerInitialization",
		"QSPIFlashInerfaceInitialization",
		"FirmwareUpdaterInitialization",
		"FANControllerInitialization",
		"ADCControllerInitialization",
		"InterruptControllerStartup",
		"AGCControllerInitialization",
		"CLinkInterfaceInitialization",
		"SDIInterfaceInitialization",
		"NetworkHostsSynchronization",

		// DeviceBuiltInTestsResults6
		"DebugTerminalInitialization",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",

		// DeviceBuiltInTestsResults7
		"BuiltInTestsGlobalResult",
		"BuiltInTestsVerification",
		"TimerInitialization",
		"InterruptControllerInitialization",
		"NetworkInterfaceInitialization",
		"GenICamManagerInitialization",
		"QSPIFlashInerfaceInitialization",
		"FirmwareUpdaterInitialization",
		"MGTInterfaceInitialization",
		"ADCControllerInitialization",
		"MemoryBufferControllerInitialization",
		"InterruptControllerStartup",
		"TestPointInitialization",
		"NetworkHostsSynchronization",
		"DebugTerminalInitialization",
		"GPIOControllerInitialization",
		
		// DeviceBuiltInTestsResults8
		"I2CInterfaceInitialization",
		"MemoryConfigurationCheck",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		""
	};

	uint32_t idxBuiltInTest = 0;

	while ((idxBuiltInTest < BUILT_IN_TEST_COUNT_IN_REG) &&
		(strlen(builtInTestsLabels[(builtInTestsResultsRegIndex * BUILT_IN_TEST_COUNT_IN_REG) + idxBuiltInTest]) > 0))
	{
		builtInTestResult_t builtInTestResult = getBuiltInTestResult(builtInTestsResultsRegValue, idxBuiltInTest);
		printf("%s: %s (%d)\n", builtInTestsLabels[(builtInTestsResultsRegIndex * BUILT_IN_TEST_COUNT_IN_REG) + idxBuiltInTest], BuiltInTestResultToString(builtInTestResult), (uint32_t) builtInTestResult);

		idxBuiltInTest++;
	}
}