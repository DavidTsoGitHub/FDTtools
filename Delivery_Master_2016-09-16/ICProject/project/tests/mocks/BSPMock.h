#ifndef BSPMOCK_H
#define BSPMOCK_H

#include "Platform_Types.h"
#include "BSP_Types.h"


void BSPMock_TurnOnIgnition(void);
void BSPMock_TurnOffIgnition(void);

BSP_ECUModes BSPMock_GetCurrentMode(void);
BSP_CanTrcvMdType BSPMock_GetCANTransceiverMode(void);

uint32 BSPMock_GetRequestModeChange_CallCount(void);
uint32 BSPMock_GetSetCanTrcvMode_CallCount(void);

uint8* BSPMock_GetSegments(uint8* segmentLength);


void BSPMock_Reset(void);

#endif