#include "DisplayManagerMock.h"

static dm_state_t m_state = DM_STATE_OFF;
static int clearAreaCallCount = 0;
static clock_t clearAreaFirstCallTimeStamp = -1;
static clock_t clearAreaLastCallTimeStamp = -1;
static uint8 clearAreaLastAreaId = 0;

static int getDisplayAreaDefinitionCallCount = 0;
static clock_t getDisplayAreaDefinitionFirstCallTimeStamp = -1;
static clock_t getDisplayAreaDefinitionLastCallTimeStamp = -1;
static uint8 getDisplayAreaDefinitionRequestedAreaId = 255;
static writable_area_definition_t area =
{
    .x = 0,
    .y = 0,
    .width = 240,
    .height = 320
};

void DisplayManagerMock_Reset()
{
    clearAreaCallCount = 0;
    clearAreaFirstCallTimeStamp = -1;
    clearAreaLastCallTimeStamp = -1;
    clearAreaLastAreaId = 255;

    getDisplayAreaDefinitionCallCount = 0;
    getDisplayAreaDefinitionFirstCallTimeStamp = -1;
    getDisplayAreaDefinitionLastCallTimeStamp = -1;
    getDisplayAreaDefinitionRequestedAreaId = 255;
    
    m_state = DM_STATE_OFF;
}

/**** ClearArea ****/

void DisplayManager_ClearArea(uint8 areaId)
{
    if (clearAreaFirstCallTimeStamp < 0)
    {
        clearAreaFirstCallTimeStamp = clock();
    }
    clearAreaLastCallTimeStamp = clock();

    clearAreaLastAreaId = areaId;
    ++clearAreaCallCount;
}

int DisplayManagerMock_ClearArea_CallCount()
{
    return clearAreaCallCount;
}

uint8 DisplayManagerMock_ClearArea_LastAreaID()
{
    return clearAreaLastAreaId;
}

clock_t DisplayManagerMock_ClearArea_FirstCallTimeStamp(void)
{
    return clearAreaFirstCallTimeStamp;
}

clock_t DisplayManagerMock_ClearArea_LastCallTimeStamp(void)
{
    return clearAreaLastCallTimeStamp;
}

/**** GetDisplayAreaDefinition ****/
writable_area_definition_t* DisplayManager_GetDisplayAreaDefinition(uint8 areaId)
{
    if (getDisplayAreaDefinitionFirstCallTimeStamp < 0)
    {
        getDisplayAreaDefinitionFirstCallTimeStamp = clock();
    }
    getDisplayAreaDefinitionLastCallTimeStamp = clock();
    getDisplayAreaDefinitionRequestedAreaId = areaId;

    return &area;
}

int DisplayManagerMock_GetDisplayAreaDefinition_CallCount(void)
{
    return getDisplayAreaDefinitionCallCount;
}

clock_t DisplayManagerMock_GetDisplayAreaDefinition_FirstCallTimeStamp(void)
{
    return getDisplayAreaDefinitionFirstCallTimeStamp;
}

clock_t DisplayManagerMock_GetDisplayAreaDefinition_LastCallTimeStamp(void)
{
    return getDisplayAreaDefinitionLastCallTimeStamp;
}

uint8 DisplayManagerMock_GetDisplayAreaDefinition_LastAreaID(void)
{
    return getDisplayAreaDefinitionRequestedAreaId;
}

dm_state_t DisplayManager_GetState(void)
{
    return m_state;
}

void DisplayManagerMock_SetState(dm_state_t state)
{
    m_state = state;
}