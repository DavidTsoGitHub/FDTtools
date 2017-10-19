#include "GCL_FuelLevel.h"
#include "GCL_Stub.h"

static uint16 fuelLevel = 0;
static uint8 fuelLevelSignalStatus = GCL_SIGNAL_OK;

static uint8 powerMode = POWER_IGNITION_OFF;
static uint8 powerModeSignalStatus = GCL_SIGNAL_OK;

static uint8 fuelLevelWarningIndication = 0;
static uint16 fuelLevelGaugeValue = 0;

void GCLStub_Reset()
{
    fuelLevel = 0;
    fuelLevelSignalStatus = GCL_SIGNAL_OK;
    powerMode = POWER_IGNITION_OFF;
    powerModeSignalStatus = GCL_SIGNAL_OK;
}

void GCLStub_SetPowerMode(PowerMode value, uint8 signalStatus)
{
    powerMode = value;
    powerModeSignalStatus = signalStatus;
}

void GCLStub_SetFuelLevel(uint16 value, uint8 signalStatus)
{
    fuelLevel = value;
    fuelLevelSignalStatus = signalStatus;
}

uint8 GCLStub_GetFuelLevelWarningIndication(void)
{
    return fuelLevelWarningIndication;
}

uint16 GCLStub_GetFuelLevelGaugeValue(void)
{
    return fuelLevelGaugeValue;
}

void GCL_Read_FuelLevel_PowerMode(PowerMode* value, uint8* status)
{
    *value = powerMode;
    *status = powerModeSignalStatus;
}

void GCL_Read_FuelLevel_FuelLevel(uint16* value, uint8* status)
{
    *value = fuelLevel;
    *status = fuelLevelSignalStatus;
}

void GCL_Write_FuelLevel_FuelLevelGaugeValue(uint16 value)
{
    fuelLevelGaugeValue = value;
}

void GCL_Write_FuelLevel_FuelLevelWarningIndication(uint8 value)
{
    fuelLevelWarningIndication = value;
}
