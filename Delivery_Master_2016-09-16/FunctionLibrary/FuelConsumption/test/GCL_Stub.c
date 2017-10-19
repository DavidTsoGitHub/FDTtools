#include "GCL_FuelConsumption.h"

// Define variables to hold stubbed GCL signals
static PowerMode SystemPowerMode = 0;
static uint16 FilteredInstantaniousFuelConsumption;
static uint16 InstantaneousFuelConsumption;
static uint16 FuelRate;
static uint16 VehicleSpeed;
static uint32 TotalFuelConsumption;
static uint32 TripFuelConsumption;
static uint32 TripDistance;
static uint32 TripDistanceCentimeters;
static boolean ResetSignal;

void GCLStub_Reset()
{
    // Resets the variables to their initial state
    // Called by the test fixture setup
    SystemPowerMode = POWER_LOW;
    FuelRate = 0;
    InstantaneousFuelConsumption = 0;
    TotalFuelConsumption = 0;
    VehicleSpeed = 0;
    FilteredInstantaniousFuelConsumption = 0;
    TripFuelConsumption = 0;
    TripDistance = 0;
    TripDistanceCentimeters = 0;
    ResetSignal = FALSE;
}

// Implement the GCL_Stub control methods defined in GCLStub.h
void GCL_Write_FuelConsumption_FilteredInstantaniousFuelConsumption(uint16 fuelConsumption)
{
    FilteredInstantaniousFuelConsumption = fuelConsumption;
}

void GCL_Read_FuelConsumption_InstantaneousFuelConsumption(uint16* fuelConsumption, uint8* u8SignalStatus)
{
    *fuelConsumption = InstantaneousFuelConsumption;
    *u8SignalStatus = GCL_SIGNAL_OK;
}

void GCL_Read_FuelConsumption_SystemPowerMode(PowerMode* sysPwrMode, uint8* u8SignalStatus)
{
    *sysPwrMode = SystemPowerMode;
    *u8SignalStatus = GCL_SIGNAL_OK;
}

void GCL_Read_FuelConsumption_FuelRate(uint16* fuelRate, uint8* u8SignalStatus)
{
    *fuelRate = FuelRate;
    *u8SignalStatus = GCL_SIGNAL_OK;
}

void GCL_Read_FuelConsumption_TripDistance(uint32* tripDistance, uint8* u8SignalStatus)
{
    *tripDistance = TripDistance;
    *u8SignalStatus = GCL_SIGNAL_OK;
}

void GCL_Read_FuelConsumption_ReadResetSignal(boolean* resetSignal, uint8* u8SignalStatus)
{
    *resetSignal = ResetSignal;
    *u8SignalStatus = GCL_SIGNAL_OK;
}

void GCL_Write_FuelConsumption_TotalFuelConsumption(uint32 value)
{
    TotalFuelConsumption = value;
}

void GCL_Write_FuelConsumption_TripFuelConsumption(uint32 value)
{
    TripFuelConsumption = value;
}
// Implement the GCL_FuelConsumption_(Read|Write)_<SignalName>(...) methods that will be called by the function block.
// Use the stubbed GCL signal variables in the top of this file

void GCLStub_SetPowerMode(PowerMode powerMode)
{
    SystemPowerMode = powerMode;

}

void GCLStub_SetInstantaneousFuelConsumption(uint16 value)
{
    InstantaneousFuelConsumption = value;
}

uint32 GCLStub_GetFilteredInstantaniousFuelConsumption()
{
    return FilteredInstantaniousFuelConsumption;
}

void GCLStub_SetFuelRate(uint16 value)
{
    FuelRate = value;
}

void GCLStub_SetVehicleSpeed(uint16 value)
{
    VehicleSpeed = value;
}

uint32 GCLStub_GetTotalFuelConsumption()
{
    return TotalFuelConsumption;
}

uint32 GCLStub_GetTripFuelConsumption(void)
{
    return TripFuelConsumption;
}

void GCLStub_IncreaseTripDistance(uint32 distanceToAdd)
{
    TripDistanceCentimeters += distanceToAdd;
    //uint32 tmpDistance = (TripDistanceCentimeters / 10000) << 8;
    
    if (TripDistanceCentimeters > 10000)
    {
        TripDistance += 1;
        //printf("TripDistance == 0x%X\n", TripDistance);
        TripDistanceCentimeters -= 10000;
    }
    
}

void GCLStub_SetResetSignal(boolean status)
{
    ResetSignal = status;
}