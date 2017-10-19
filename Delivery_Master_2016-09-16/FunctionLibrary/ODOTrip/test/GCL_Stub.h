#ifndef GCL_STUB_H
#define GCL_STUB_H

#include "StandardTypes.h"
#include "GCL_Types.h"

void GCLStub_Reset(void);

void GCLStub_SetPowerMode(PowerMode value, uint8 signalStatus);
void GCLStub_SetFuelLevel(uint16 fuelLevel, uint8 signalStatus);

void GCLStub_SetFrequency(uint32 freq);
uint32 GCLStub_GetCurrentOdo(void);

uint32 GCLStub_GetCurrentTripA(void);
uint32 GCLStub_GetCurrentTripB(void);

#endif //GCL_STUB_H
