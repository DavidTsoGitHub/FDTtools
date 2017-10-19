#include "GCL_OutputShaftRotationSpeed.h"

// Define variables to hold stubbed GCL signals
static uint32 VehicleSpeed;
static uint16 RotationSpeed;
static uint8 SignalStatus = GCL_SIGNAL_OK;
// static uint8 HighBeamTellTaleValue = 255;

void GCLStub_Reset()
{
    // Resets the variables to their initial state
    // Called by the test fixture setup
    // SystemPowerMode = 0;
    // SystemPowerModeSignalStatus = SIGNAL_STATUS_OK;
    // HighBeamTellTaleValue = 255;
}

// Implement the GCL_Stub control methods defined in GCLStub.h
void GCLStub_SetVehicleSpeed(uint32 vehSpd)
{
    VehicleSpeed = vehSpd;
}

// Define getters to inspect the signals that are written by the application block
uint16 GCLStub_GetRotationSpeed(void)
{
    return RotationSpeed;
}

void GCL_Read_OutputShaftRotationSpeed_VehSpdFreqInput(uint32* value, uint8* status)
{
    *status = SignalStatus;
    *value = VehicleSpeed;
}

void GCL_Write_OutputShaftRotationSpeed_OutputShaftRotationSpeed(uint16 value)
{
    RotationSpeed = value;
}