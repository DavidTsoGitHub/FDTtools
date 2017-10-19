#ifndef GCL_STUB_H
#define GCL_STUB_H

#include "Platform_Types.h"

void GCLStub_Reset(void);

void GCLStub_SetButtonRaw(uint8 buttonRaw);

uint8 GCLStub_GetButtonEventUp(void);
uint8 GCLStub_GetButtonEventDown(void);
uint8 GCLStub_GetButtonEventLeft(void);
uint8 GCLStub_GetButtonEventRight(void);
uint8 GCLStub_GetButtonEventSet(void);
uint8 GCLStub_GetButtonEventClear(void);

#endif //GCL_STUB_H
