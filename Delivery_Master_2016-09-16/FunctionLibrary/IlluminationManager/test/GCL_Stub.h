#ifndef GCL_STUB_H
#define GCL_STUB_H

#include "StandardTypes.h"

#define GCL_SIGNAL_OK         (0U)
#define GCL_SIGNAL_TIMEOUT    (1 << 0)
#define GCL_SIGNAL_ERROR      (1 << 1)


void GCLStub_Reset(void);

// Define setters to rig signals in the GCLStub for the unit test
void GCLStub_SetPowerMode(uint8 powerMode);
void GCLStub_SetSignalStatus(uint8 status);
void GCLStub_SetIlluminationMode(uint8 mode);
void GCLStub_SetIlluminationLevel(uint8 level);
uint8 GCLStub_GetDisplayIlluminationLevel(void);
uint8 GCLStub_GetGaugeIlluminationLevel(void);
uint8 GCLStub_GetCurrentIlluminationLevel();


#endif //GCL_STUB_H
