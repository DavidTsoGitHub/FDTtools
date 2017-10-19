#include "GCL_EngineOilPressureTellTale.h"

static PowerMode SystemPowerMode = 0;
static uint8 SystemPowerModeSignalStatus = GCL_SIGNAL_OK;
static uint8 OilPressureWarningRequest = 0;
static uint8 OilPressureWarningRequestSignalStatus = GCL_SIGNAL_OK;
static uint8 OilPressureWarningIndication = 0xFF;

void GCLStub_Reset()
{
    SystemPowerMode = POWER_LOW;
    SystemPowerModeSignalStatus = GCL_SIGNAL_OK;
    OilPressureWarningRequest = 0;
    OilPressureWarningRequestSignalStatus = GCL_SIGNAL_OK;
    OilPressureWarningIndication = 0xFF;
}

void GCLStub_SetPowerMode(PowerMode powerMode, uint8 powermodeSignalStatus)
{
    SystemPowerMode = powerMode;
    SystemPowerModeSignalStatus = powermodeSignalStatus;
}

void GCLStub_SetEngineOilPressureWarningRequest(uint8 warningRequest, uint8 warningRequestSignalStatus)
{
    OilPressureWarningRequest = warningRequest;
    OilPressureWarningRequestSignalStatus = warningRequestSignalStatus;
}

uint8 GCLStub_GetEngineOilPressureWarningIndication(void)
{
    return OilPressureWarningIndication;
}

void GCL_Read_EngineOilPressureTellTale_SystemPowerMode(PowerMode* value, uint8* status)
{
    *value = SystemPowerMode;
    *status = SystemPowerModeSignalStatus;
}

void GCL_Read_EngineOilPressureTellTale_OilPressureWarningRequest(uint8* value, uint8* status)
{
    *value = OilPressureWarningRequest;
    *status = OilPressureWarningRequestSignalStatus;
}

void GCL_Write_EngineOilPressureTellTale_OilPressureWarningIndication(uint8 value)
{
    OilPressureWarningIndication = value;
}