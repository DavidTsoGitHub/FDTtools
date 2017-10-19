#ifndef CONTENTSMANAGERMOCK_H
#define CONTENTSMANAGERMOCK_H

#include "ContentsManager.h"
#include <time.h>

void ContentsManagerMock_Reset(void);

void ContentsManagerMock_SetState(dm_state_t state);

int ContentsManagerMock_ClearArea_CallCount(void);
clock_t ContentsManagerMock_ClearArea_FirstCallTimeStamp(void);
clock_t ContentsManagerMock_ClearArea_LastCallTimeStamp(void);
uint8 ContentsManagerMock_ClearArea_LastAreaID(void);

int ContentsManagerMock_GetDisplayAreaDefinition_CallCount(void);
clock_t ContentsManagerMock_GetDisplayAreaDefinition_FirstCallTimeStamp(void);
clock_t ContentsManagerMock_GetDisplayAreaDefinition_LastCallTimeStamp(void);
uint8 ContentsManagerMock_GetDisplayAreaDefinition_LastAreaID(void);


#endif
