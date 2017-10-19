#include "GCL_ChargingTellTale.h"

static PowerMode SystemPowerMode = POWER_LOW;
static uint8 SystemPowerModeSignalStatus = GCL_SIGNAL_OK;
static uint8 ChargingIndicatorRequest = 0;
static uint8 ChargingIndicatorRequestSignalStatus = GCL_SIGNAL_OK;
static uint8 ChargingTellTaleValue = 255;

void GCLStub_Reset()
{
    SystemPowerMode = POWER_LOW;
	SystemPowerModeSignalStatus = GCL_SIGNAL_OK;
	ChargingIndicatorRequest = 0;
	ChargingIndicatorRequestSignalStatus = GCL_SIGNAL_OK;
	ChargingTellTaleValue = 255;
}

void GCLStub_SetPowerMode(PowerMode powerMode)
{
    SystemPowerMode = powerMode;
}

void GCLStub_SetPowerModeSignalStatus(uint8 signalStatus)
{
    SystemPowerModeSignalStatus = signalStatus;
}

void GCLStub_SetChargingSignal(uint8 chargingSignal)
{
	ChargingIndicatorRequest = chargingSignal;
}
void GCLStub_SetChargingSignalStatus(uint8 signalStatus)
{
	ChargingIndicatorRequestSignalStatus = signalStatus;
}

 uint8 GCLStub_GetChargingTellTaleValue(void)
 {
	 return ChargingTellTaleValue;
 }

void GCL_Read_ChargingTellTale_SystemPowerMode(PowerMode* value, uint8* status)
{
    *status = SystemPowerModeSignalStatus;
    *value = SystemPowerMode;
}

void GCL_Read_ChargingTellTale_ChargingIndicatorRequest(uint8* value, uint8* status)
{
	*status = ChargingIndicatorRequestSignalStatus;
    *value = ChargingIndicatorRequest;
}

void GCL_Write_ChargingTellTale_ChargingTellTaleValue(uint8 value)
{
    ChargingTellTaleValue = value;
}