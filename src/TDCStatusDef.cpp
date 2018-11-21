#include "TDCStatusDef.h"
#include "Utils.h"
#include <cstdio>

#define TSIR_TDC_STATUS_BIT_COUNT numof(TSIR_TDC_STATUS_BIT)

void DisplayTDCStatusBits(uint32_t tdcStatusRegValue)
{
	char *tdcStatusLabels[] = {
		"WaitingForCooler",
		"WaitingForSensor",
		"WaitingForInit",
		"Reserved",
		"WaitingForICU",
		"WaitingForNDFilter",
		"WaitingForCalibrationInit",
		"WaitingForFilterWheel",
		"WaitingForArm",
		"WaitingForValidParameters",
		"AcquisitionStarted",
		"Reserved",
		"WaitingForCalibrationData",
		"WaitingForCalibrationActualization",
		"WaitingForOutputFPGA",
		"WaitingForPower",
		"WaitingForFlashSettingsInit"
	};

	for (uint32_t i = 0; i < numof(tdcStatusLabels); i++)
	{
		printf("Bit%02d: %s: %d\n", i, tdcStatusLabels[i], bitIsOn(tdcStatusRegValue, i));
	}

}