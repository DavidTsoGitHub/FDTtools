#ifndef GCL_STUB_H
#define GCL_STUB_H

#include "StandardTypes.h"

void GCLStub_Reset(void);

void GCLStub_SetInputFrequency(uint32 value);
void GCLStub_SetSystemPowerMode(PowerMode value);
uint32 GCLStub_GetVehSpd_dsp(void);


#endif //GCL_STUB_H
