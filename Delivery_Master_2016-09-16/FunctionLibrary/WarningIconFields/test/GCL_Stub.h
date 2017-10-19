#ifndef GCL_STUB_H
#define GCL_STUB_H

#include "StandardTypes.h"

#define GCL_SIGNAL_OK         (0U)
#define GCL_SIGNAL_TIMEOUT    (1 << 0)
#define GCL_SIGNAL_ERROR      (1 << 1)


void GCLStub_Reset(void);

// Define setters to rig signals in the GCLStub for the unit test
void GCLStub_SetChargingIndication(uint8 value);
void GCLStub_SetParkBrakeIndication(uint8 value);
void GCLStub_SetBrakePressureLowIndication(uint8 value);
void GCLStub_SetEngineOilPressureIndication(uint8 value);

// Define getters to inspect the signals that are written by the application block
uint32 GCLStub_GetIcon1ID(void);
uint32 GCLStub_GetIcon2ID(void);
uint32 GCLStub_GetIcon3ID(void);
uint32 GCLStub_GetIcon4ID(void);

#endif //GCL_STUB_H
