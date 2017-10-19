#ifndef GCL_STUB_H
#define GCL_STUB_H

#include "StandardTypes.h"

void GCLStub_Reset(void);

void GCLStub_SetButton1State(uint8 buttonState);
void GCLStub_SetButton2State(uint8 buttonState);
void GCLStub_SetButton3State(uint8 buttonState);
void GCLStub_SetButton4State(uint8 buttonState);

uint8 GCLStub_GetButton1Event(void);
uint8 GCLStub_GetButton2Event(void);
uint8 GCLStub_GetButton3Event(void);
uint8 GCLStub_GetButton4Event(void);

#endif //GCL_STUB_H
