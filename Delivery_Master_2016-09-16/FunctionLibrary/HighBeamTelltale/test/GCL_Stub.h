#ifndef GCL_STUB_H
#define GCL_STUB_H

#include "StandardTypes.h"


void GCLStub_Reset(void);

void GCLStub_SetPowerMode(PowerMode powerMode);
void GCLStub_SetPowerModeSignalStatus(uint8 signalStatus);
void GCLStub_SetHighBeamSignal(uint8 highBeamSignal);
void GCLStub_SetHighBeamSignalStatus(uint8 signalStatus);

uint8 GCLStub_GetHighBeamTellTaleValue(void);

#endif //GCL_STUB_H