#include "GCL_HighBeamTelltale.h"

static PowerMode SystemPowerMode = POWER_LOW;
static uint8 SystemPowerModeSignalStatus = GCL_SIGNAL_OK;
static uint8 HighBeamIndicatorRequest = 0;
static uint8 HighBeamIndicatorRequestSignalStatus = GCL_SIGNAL_OK;
static uint8 HighBeamTellTaleValue = 255;

void GCLStub_Reset()
{
    SystemPowerMode = POWER_LOW;
    SystemPowerModeSignalStatus = GCL_SIGNAL_OK;
    HighBeamIndicatorRequest = 0;
    HighBeamIndicatorRequestSignalStatus = GCL_SIGNAL_OK;
    HighBeamTellTaleValue = 255;
}

/*** GCL Control ***/
void GCLStub_SetPowerMode(PowerMode powerMode)
{
    SystemPowerMode = powerMode;
}

void GCLStub_SetPowerModeSignalStatus(uint8 signalStatus)
{
    SystemPowerModeSignalStatus = signalStatus;
}

void GCLStub_SetHighBeamSignal(uint8 signalStatus)
{
    HighBeamIndicatorRequest = signalStatus;
}

void GCLStub_SetHighBeamSignalStatus(uint8 signalStatus)
{
    HighBeamIndicatorRequestSignalStatus = signalStatus;
}

uint8 GCLStub_GetHighBeamTellTaleValue(void)
{
    return HighBeamTellTaleValue;
}

/*** GCL Stubs ***/
void GCL_Read_HighBeamTelltale_HighBeamIndicatorRequest(uint8* value, uint8* status)
{
    *value = HighBeamIndicatorRequest;
    *status = HighBeamIndicatorRequestSignalStatus;
}

void GCL_Read_HighBeamTelltale_SystemPowerMode(PowerMode* value, uint8* status)
{
    *value = SystemPowerMode;
    *status = SystemPowerModeSignalStatus;
}

void GCL_Write_HighBeamTelltale_HighBeamTellTaleValue(uint8 value)
{
    HighBeamTellTaleValue = value;
}