#include "GCL_ParkBrakeIndicator.h"
#include "StandardTypes.h"

static uint16 VehSpd = 0;
static uint8 ParkBrakeSwitch = 0;
static uint8 ParkBrakeCheckMessageActive = 0xFF;
static uint8 ParkBrakeIndicatorValue = 0xFF;
static PowerMode SystemPowerMode = POWER_IGNITION_ON;

void GCLStub_Reset()
{
    VehSpd = 0;
    ParkBrakeSwitch = 0;
    ParkBrakeCheckMessageActive = 0x00;
	ParkBrakeIndicatorValue = 0x00; 
    SystemPowerMode = POWER_IGNITION_ON;
}

void GCLStub_SetVehSpd(uint16 vehSpd)
{
    VehSpd = vehSpd;
}

void GCLStub_SetParkBrakeSwitch(uint8 parkBrakeSwitchStatus)
{
    ParkBrakeSwitch = parkBrakeSwitchStatus;
}

void GCLStub_SetSystemPowerMode(PowerMode powerMode)
{
    SystemPowerMode = powerMode;
}

uint8 GCLStub_GetParkBrakeIndicatorCheckMessageActive(void)
{
    return ParkBrakeCheckMessageActive;
}

uint8 GCLStub_GetParkBrakeIndicatorValue(void)
{
    return ParkBrakeIndicatorValue;
}

void GCL_Read_ParkBrakeIndicator_VehSpd(uint16* value, uint8* status)
{
    *value = VehSpd;
    *status = GCL_SIGNAL_OK;
}

void GCL_Read_ParkBrakeIndicator_ParkBrakeSwitch(uint8* value, uint8* status)
{
    *value = ParkBrakeSwitch;
    *status = GCL_SIGNAL_OK;
}

void GCL_Read_ParkBrakeIndicator_SystemPowerMode(PowerMode* value, uint8* status)
{
    *value = SystemPowerMode;
    *status = GCL_SIGNAL_OK;
}

void GCL_Write_ParkBrakeIndicator_ParkBrakeIndicatorCheckMessageActive(uint8 value)
{
    ParkBrakeCheckMessageActive = value;
}

void GCL_Write_ParkBrakeIndicator_ParkBrakeIndicatorValue(uint8 value)
{
	ParkBrakeIndicatorValue = value;
}
