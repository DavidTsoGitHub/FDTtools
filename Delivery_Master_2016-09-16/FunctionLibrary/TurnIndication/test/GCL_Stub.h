#ifndef GCL_STUB_H
#define GCL_STUB_H

#include "Platform_Types.h"

#define GCL_SIGNAL_OK         (0U)
#define GCL_SIGNAL_TIMEOUT    (1 << 0)
#define GCL_SIGNAL_ERROR      (1 << 1)


void GCLStub_Reset(void);

void GCLStub_SetRightTurnIndication(uint8 value, uint8 status);
void GCLStub_SetLeftTurnIndication(uint8 value, uint8 status);

uint8 GCLStub_GetRightTurnTelltaleValue(void);
uint8 GCLStub_GetLeftTurnTelltaleValue(void);

#endif //GCL_STUB_H
