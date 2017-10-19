#ifndef DISPLAYMANAGERMOCK_H
#define DISPLAYMANAGERMOCK_H

#include "DisplayManager.h"
#include <time.h>

void DisplayManagerMock_Reset(void);

void DisplayManagerMock_SetState(dm_state_t state);

int DisplayManagerMock_ClearArea_CallCount(void);
clock_t DisplayManagerMock_ClearArea_FirstCallTimeStamp(void);
clock_t DisplayManagerMock_ClearArea_LastCallTimeStamp(void);
uint8 DisplayManagerMock_ClearArea_LastAreaID(void);

int DisplayManagerMock_GetDisplayAreaDefinition_CallCount(void);
clock_t DisplayManagerMock_GetDisplayAreaDefinition_FirstCallTimeStamp(void);
clock_t DisplayManagerMock_GetDisplayAreaDefinition_LastCallTimeStamp(void);
uint8 DisplayManagerMock_GetDisplayAreaDefinition_LastAreaID(void);


#endif
