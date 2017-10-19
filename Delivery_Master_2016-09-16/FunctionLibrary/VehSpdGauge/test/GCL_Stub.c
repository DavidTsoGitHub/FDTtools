#include "GCL_VehSpdGauge.h"

static PowerMode SystemPowerMode = POWER_LOW;
static uint32 InputFrequency = 0;
static uint16 VehSpd_dsp = 0;

void GCLStub_Reset()
{
    SystemPowerMode = POWER_LOW;
    InputFrequency = 0;
    VehSpd_dsp = 0;
}

void GCLStub_SetInputFrequency(uint32 value)
{
    InputFrequency = value;
}

void GCLStub_SetSystemPowerMode(uint8 value)
{
    SystemPowerMode = value;
}

uint16 GCLStub_GetVehSpd_dsp(void)
{
    return VehSpd_dsp;
}

void GCL_Read_VehSpdGauge_InputFrequency(uint32* value, uint8* status)
{
    *status = GCL_SIGNAL_OK;
    *value = InputFrequency;
}

void GCL_Read_VehSpdGauge_SystemPowerMode(PowerMode* value, uint8* status)
{
    *status = GCL_SIGNAL_OK;
    *value = SystemPowerMode;
}

void GCL_Write_VehSpdGauge_VehSpdGaugeValue(uint16 value)
{
    VehSpd_dsp = value;
}
