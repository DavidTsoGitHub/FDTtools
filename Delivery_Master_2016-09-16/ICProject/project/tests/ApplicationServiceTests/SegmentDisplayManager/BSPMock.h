#ifndef BSPMOCK_H
#define BSPMOCK_H

#include "Platform_Types.h"

void BSPMock_Reset(void);

uint8* BSPMock_GetSegments(uint8* count);
uint8 BSPMock_SetDisplaySegments_CallCount();
uint8 BSPMock_GetSegment(uint8 index);

#endif