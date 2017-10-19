#ifndef GCL_STUB_H
#define GCL_STUB_H

#include "Platform_Types.h"
#include "StandardTypes.h"

void GCLStub_Reset(void);

void GCLStub_SetVehSpd(uint32 vehicleSpeed);
void GCLStub_SetParkBrakeSwitch(uint8 parkBrakeSwitchStatus);
void GCLStub_SetSystemPowerMode(PowerMode powerMode);

uint8 GCLStub_GetParkBrakeIndicatorCheckMessageActive(void);
uint8 GCLStub_GetParkBrakeIndicatorValue(void);

#endif //GCL_STUB_H
