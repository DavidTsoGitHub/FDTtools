#ifndef GCL_STUB_H
#define GCL_STUB_H

#include "StandardTypes.h"

#define GCL_SIGNAL_OK         (0U)
#define GCL_SIGNAL_TIMEOUT    (1 << 0)
#define GCL_SIGNAL_ERROR      (1 << 1)


void GCLStub_Reset(void);

// Define setters to rig signals in the GCLStub for the unit test
void GCLStub_SetVehicleSpeed(uint32 vehSpd);

// Define getters to inspect the signals that are written by the application block
uint16 GCLStub_GetRotationSpeed(void);

#endif //GCL_STUB_H
