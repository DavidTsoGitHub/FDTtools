#include "GCL_IlluminationManager.h"

static PowerMode SystemPowerMode = 0;
static uint8 SignalStatus = GCL_SIGNAL_OK;

static uint8 IlluminationMode = 0;
static uint8 IlluminationLevel = 0;

static uint8 DisplayIlluminationLevel = 0;
static uint8 GaugeIlluminationLevel = 0;

static uint8 CurrentIlluminationLevel = 0;

void GCLStub_Reset()
{
    SystemPowerMode = POWER_LOW;
    SignalStatus = GCL_SIGNAL_OK;
    IlluminationMode = 0;
    IlluminationLevel = 0;
    DisplayIlluminationLevel = 0;
    GaugeIlluminationLevel = 0;
    CurrentIlluminationLevel = 0;
}

void GCLStub_SetPowerMode(PowerMode powerMode)
{
    SystemPowerMode = powerMode;
}

void GCLStub_SetSignalStatus(uint8 status)
{
    SignalStatus = status;
}

void GCL_Read_IlluminationManager_SystemPowerMode(PowerMode* value, uint8* status)
{
    *value = SystemPowerMode;
    *status = SignalStatus;
}

void GCLStub_SetIlluminationMode(uint8 mode)
{
    IlluminationMode = mode;
}

void GCL_Read_IlluminationManager_IlluminationMode(uint8* mode, uint8* status)
{
    *mode = IlluminationMode;
    *status = SignalStatus;
}

void GCLStub_SetIlluminationLevel(uint8 level)
{
    IlluminationLevel = level;
}

void GCL_Read_IlluminationManager_IlluminationLevel(uint8* level, uint8* status)
{
    *level = IlluminationLevel;
    *status = SignalStatus;
}

uint8 GCLStub_GetDisplayIlluminationLevel(void)
{
    return DisplayIlluminationLevel;
}

uint8 GCLStub_GetGaugeIlluminationLevel(void)
{
    return GaugeIlluminationLevel;
}

void GCL_Write_IlluminationManager_DisplayIlluminationLevel(uint8 level)
{
    DisplayIlluminationLevel = level;
}

void GCL_Write_IlluminationManager_GaugeIlluminationLevel(uint8 level)
{
    GaugeIlluminationLevel = level;
}

void GCL_Write_IlluminationManager_CurrentIlluminationLevel(uint8 level)
{
    CurrentIlluminationLevel = level;
}

uint8 GCLStub_GetCurrentIlluminationLevel()
{
    return CurrentIlluminationLevel;
}
