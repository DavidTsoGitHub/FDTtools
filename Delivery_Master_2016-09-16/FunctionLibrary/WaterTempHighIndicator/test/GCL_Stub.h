#ifndef GCL_STUB_H
#define GCL_STUB_H

#include "StandardTypes.h"

#define GCL_SIGNAL_OK         (0U)
#define GCL_SIGNAL_TIMEOUT    (1 << 0)
#define GCL_SIGNAL_ERROR      (1 << 1)


void GCLStub_Reset(void);

void GCLStub_SetPowerMode(PowerMode powerMode);
void GCLStub_SetPowerModeSignalStatus(uint8 highBeamSignal);

void GCLStub_SetWaterTempHighIndication(uint8 value, uint8 status);
uint8 GCLStub_GetWaterTempHighTellTaleValue(void);

#endif //GCL_STUB_H
