#include "GCLMock.h"

static uint8 powerMode  = 0;

void GCLMock_Reset()
{
    powerMode = 0;
}

uint8 GCLMock_Get_PowerMode()
{
    return powerMode;
}

void GCL_Write_Platform_PowerMode(uint8 value)
{
    powerMode = value;
}
