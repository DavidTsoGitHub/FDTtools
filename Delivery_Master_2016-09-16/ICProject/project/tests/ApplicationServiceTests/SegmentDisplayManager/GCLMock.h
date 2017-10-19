#ifndef GCLMOCK_H
#define GCLMOCK_H

#include "StandardTypes.h"

void GCLMock_Reset(void);
void GCLMock_SetDisplayValue(uint32 displayValue);
void GCLMock_SetPowerMode(PowerMode powerMode);
void GCLMock_SetDotSegmentOn();
void GCLMock_SetDotSegmentOff();
void GCLMock_SetT1SegmentOn();
void GCLMock_SetT1SegmentOff();


#endif