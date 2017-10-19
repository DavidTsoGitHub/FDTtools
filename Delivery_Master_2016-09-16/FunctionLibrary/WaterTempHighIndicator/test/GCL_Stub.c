#include "GCL_WaterTempHighIndicator.h"

static PowerMode SystemPowerMode = POWER_LOW;
static uint8 SystemPowerModeSignalStatus = GCL_SIGNAL_OK;
static uint8 WaterHighTempTelltaleValue = 255;
static uint8 WaterHighTempIndicator = 255;
static uint8 WaterHighTempIndicatorStatus = GCL_SIGNAL_OK;

void GCLStub_Reset()
{
    SystemPowerMode = POWER_LOW;
    SystemPowerModeSignalStatus = GCL_SIGNAL_OK;
	WaterHighTempTelltaleValue = 0;
	WaterHighTempIndicator = 0;
    WaterHighTempIndicatorStatus = GCL_SIGNAL_OK;
}

void GCLStub_SetPowerMode(PowerMode powerMode)
{
	SystemPowerMode = powerMode;
}

void GCLStub_SetPowerModeSignalStatus(uint8 signalStatus)
{
	SystemPowerModeSignalStatus = signalStatus;
}

void GCLStub_SetWaterTempHighIndication(uint8 value, uint8 status)
{
	WaterHighTempIndicator = value;
	WaterHighTempIndicatorStatus = status;
}

void GCL_Read_WaterTempHighIndicator_WaterTempHighIndication(uint8* value, uint8* status)
{
	*value = WaterHighTempIndicator;
	*status = WaterHighTempIndicatorStatus;
}

void GCL_Write_WaterTempHighIndicator_WaterTempHighTelltaleValue(uint8 value)
{
	WaterHighTempTelltaleValue = value;
}

void GCL_Read_WaterTempHighIndicator_SystemPowerMode(PowerMode* value, uint8* status)
{
	*value = SystemPowerMode;
	*status = SystemPowerModeSignalStatus;
}

uint8 GCLStub_GetWaterTempHighTellTaleValue(void)
{
	return WaterHighTempTelltaleValue;
}
