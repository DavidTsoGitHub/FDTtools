#ifndef GCL_STUB_H
#define GCL_STUB_H

#include "StandardTypes.h"

#define GCL_SIGNAL_OK         (0U)
#define GCL_SIGNAL_TIMEOUT    (1 << 0)
#define GCL_SIGNAL_ERROR      (1 << 1)


void GCLStub_Reset(void);

// Define setters to rig signals in the GCLStub for the unit test


void GCLStub_SetPowerMode(PowerMode powerMode);
void GCLStub_SetInstantaneousFuelConsumption(uint16 value);
uint32 GCLStub_GetFilteredInstantaniousFuelConsumption(void);
uint32 GCLStub_GetTotalFuelConsumption(void);
uint32 GCLStub_GetTripFuelConsumption(void);

void GCLStub_SetFuelRate(uint16 value);
void GCLStub_SetVehicleSpeed(uint16 value);
void GCLStub_IncreaseTripDistance(uint32 distanceToAdd);
void GCLStub_SetResetSignal(boolean status);
// void GCLStub_SetPowerMode(uint8 powerMode);
// void GCLStub_SetPowerModeSignalStatus(uint8 highBeamSignal);

// Define getters to inspect the signals that are written by the application block
// uint8 GCLStub_GetHighBeamTellTaleValue(void);

#endif //GCL_STUB_H
