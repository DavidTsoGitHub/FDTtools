#ifndef GCL_STUB_H
#define GCL_STUB_H

#include "StandardTypes.h"

void GCLStub_Reset(void);

void GCLStub_SetCatalystTankLevel(uint8 catalystTankLevel, uint8 signalStatus);
uint8 GCLStub_GetReadCatalystTankLevelCallCount(void);
void GCLStub_SetPowerMode(PowerMode powerMode, uint8 signalStatus);

uint8 GCLStub_GetSegment0(void);
uint8 GCLStub_GetSegment1(void);
uint8 GCLStub_GetSegment2(void);
uint8 GCLStub_GetSegment3(void);

#endif //GCL_STUB_H
