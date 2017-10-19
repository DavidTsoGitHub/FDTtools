#ifndef GCL_STUB_H
#define GCL_STUB_H

#include "StandardTypes.h"

#define GCL_SIGNAL_OK         (0U)
#define GCL_SIGNAL_TIMEOUT    (1 << 0)
#define GCL_SIGNAL_ERROR      (1 << 1)


void GCLStub_Reset(void);

// Define setters to rig signals in the GCLStub for the unit test
// void GCLStub_SetPowerMode(uint8 powerMode);
// void GCLStub_SetPowerModeSignalStatus(uint8 highBeamSignal);

void GCLStub_SetPowerMode(PowerMode mode);

void GCLStub_SetFrontCircuitPressure(uint16 value);
void GCLStub_SetRearCircuitPressure(uint16 value);

void GCLStub_SetFrontReservoirPressure(uint16 value);
void GCLStub_SetRearReservoirPressure(uint16 value);
void GCLStub_SetParkBrakeReservoirPressure(uint16 value);

// Define getters to inspect the signals that are written by the application block
uint8 GCLStub_GetBrakePressureLowTellTaleValue(void);

#endif //GCL_STUB_H
