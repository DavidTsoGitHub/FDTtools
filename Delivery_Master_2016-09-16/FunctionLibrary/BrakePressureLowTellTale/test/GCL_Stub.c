#include "GCL_BrakePressureLowTellTale.h"
#include "GCL_Stub.h"

// Define variables to hold stubbed GCL signals
static PowerMode SystemPowerMode = POWER_IGNITION_OFF;
static uint8 SystemPowerModeSignalStatus = GCL_SIGNAL_OK;

static uint8 SignalStatus = GCL_SIGNAL_OK;

static uint16 FrontCircuitPressure = 0xFFFF;
static uint16 RearCircuitPressure = 0xFFFF;

static uint16 FrontReservoirPressure = 0xFFFF;
static uint16 RearReservoirPressure = 0xFFFF;
static uint16 ParkBrakeReservoirPressure = 0xFFFF;

static uint8 BrakePressureLowTellTaleValue = 0xFF;

void GCLStub_Reset()
{
    // Resets the variables to their initial state
    // Called by the test fixture setup
    SystemPowerMode = POWER_IGNITION_OFF;
    SystemPowerModeSignalStatus = GCL_SIGNAL_OK;
	SignalStatus = GCL_SIGNAL_OK;
	FrontReservoirPressure = 0xFFFF;
    BrakePressureLowTellTaleValue = 0xFF;
}

// Implement the GCL_Stub control methods defined in GCLStub.h
void GCLStub_SetPowerMode(PowerMode mode)
{
	SystemPowerMode = mode;
}

void GCLStub_SetFrontReservoirPressure(uint16 value)
{
	FrontReservoirPressure = value;
}

void GCLStub_SetRearReservoirPressure(uint16 value)
{
	RearReservoirPressure = value;
}

void GCLStub_SetParkBrakeReservoirPressure(uint16 value)
{
	ParkBrakeReservoirPressure = value;	
}

uint8 GCLStub_GetBrakePressureLowTellTaleValue()
{
	return BrakePressureLowTellTaleValue;
}

void GCLStub_SetFrontCircuitPressure(uint16 value)
{
    FrontCircuitPressure = value;
}
void GCLStub_SetRearCircuitPressure(uint16 value)
{
    RearCircuitPressure = value;
}

// Implement the GCL_BrakePressureLowTellTale_(Read|Write)_<SignalName>(...) methods that will be called by the function block.
// Use the stubbed GCL signal variables in the top of this file
void GCL_Read_BrakePressureLowTellTale_SystemPowerMode(PowerMode* powerMode, uint8* u8SignalStatus)
{
	*u8SignalStatus = SystemPowerModeSignalStatus;
	*powerMode = SystemPowerMode;
}

void GCL_Read_BrakePressureLowTellTale_FrontCircuitPressure(uint16* value, uint8* u8SignalStatus)
{
	*u8SignalStatus = GCL_SIGNAL_OK;
	*value = FrontCircuitPressure;
}

void GCL_Read_BrakePressureLowTellTale_RearCircuitPressure(uint16* value, uint8* u8SignalStatus)
{
	*u8SignalStatus = GCL_SIGNAL_OK;
	*value = RearCircuitPressure;
}

void GCL_Read_BrakePressureLowTellTale_FrontReservoirPressure(uint16* value, uint8* u8SignalStatus)
{
	*u8SignalStatus = GCL_SIGNAL_OK;
	*value = FrontReservoirPressure;
}

void GCL_Read_BrakePressureLowTellTale_RearReservoirPressure(uint16* value, uint8* u8SignalStatus)
{
	*u8SignalStatus = GCL_SIGNAL_OK;
	*value = RearReservoirPressure;
}

void GCL_Read_BrakePressureLowTellTale_ParkBrakeReservoirPressure(uint16* value, uint8* u8SignalStatus)
{
	*u8SignalStatus = GCL_SIGNAL_OK;
	*value = ParkBrakeReservoirPressure;
}

void GCL_Write_BrakePressureLowTellTale_BrakePressureLowIndicatorValue(uint8 value)
{
	BrakePressureLowTellTaleValue = value;
}