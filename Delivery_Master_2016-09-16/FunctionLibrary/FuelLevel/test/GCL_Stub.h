#ifndef GCL_STUB_H
#define GCL_STUB_H

#include "Platform_Types.h"
#include "StandardTypes.h"

#define GCL_SIGNAL_OK         (0U)
#define GCL_SIGNAL_TIMEOUT    (1 << 0)
#define GCL_SIGNAL_ERROR      (1 << 1)

void GCLStub_Reset(void);

void GCLStub_SetPowerMode(PowerMode value, uint8 signalStatus);
void GCLStub_SetFuelLevel(uint16 fuelLevel, uint8 signalStatus);

uint8 GCLStub_GetFuelLevelWarningIndication(void);
uint16 GCLStub_GetFuelLevelGaugeValue(void);

#endif //GCL_STUB_H
