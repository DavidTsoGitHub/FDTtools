#include "GCL_ODOTrip.h"
#include "GCL_Stub.h"

// GCL_Stub.h
// ----------------------------------------------------------------------------

static uint8 powerMode = POWER_IGNITION_OFF;
static uint8 powerModeSignalStatus = GCL_SIGNAL_OK;

static uint32 currentODO = 0;
static uint32 currentTripA = 0;
static uint32 currentTripB = 0;
static uint32 currentfreq = 0;

void GCLStub_Reset()
{
    powerMode = POWER_IGNITION_OFF;
    powerModeSignalStatus = GCL_SIGNAL_OK;
    
    currentODO = 0;
    currentfreq = 0;
    currentTripA = 0;
    currentTripB = 0;
}

void GCLStub_SetPowerMode(PowerMode value, uint8 signalStatus)
{
    powerMode = value;
    powerModeSignalStatus = signalStatus;
}

void GCLStub_SetFrequency(uint32 freq)
{
    currentfreq = freq<<8;
}

uint32 GCLStub_GetCurrentFrequency()
{
    return currentfreq;
}

uint32 GCLStub_GetCurrentOdo(void)
{
    return currentODO;
}

uint32 GCLStub_GetCurrentTripA(void)
{
    return currentTripA;
}

uint32 GCLStub_GetCurrentTripB(void)
{
    return currentTripB;
}


// GCL_ODOTrip.h
// ----------------------------------------------------------------------------

void GCL_Read_ODOTrip_VehSpdFrequency(uint32* value, uint8* status)
{
    *value =  currentfreq;
    *status = GCL_SIGNAL_OK;
}

void GCL_Read_ODOTrip_SystemPowerMode(PowerMode* value, uint8* status)
{
    *value = powerMode;
    *status = GCL_SIGNAL_OK;
}

void GCL_Read_ODOTrip_TripResetA(uint8* value, uint8* status)
{}

void GCL_Read_ODOTrip_TripResetB(uint8* value, uint8* status)
{}

void GCL_Write_ODOTrip_CurrentODOValue(uint32 value)
{
    currentODO = value;
}

void GCL_Write_ODOTrip_CurrentTripValueA(uint32 value)
{
    currentTripA = value;
}

void GCL_Write_ODOTrip_CurrentTripValueB(uint32 value)
{
    currentTripB = value;
}

/*** Signal indication callbacks ***/
//void ODOTrip_TripResetA_Indication(void)
//{}

//void ODOTrip_TripResetB_Indication(void)
//{}
