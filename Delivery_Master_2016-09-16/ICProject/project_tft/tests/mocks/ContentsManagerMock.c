#include "ContentsManagerMock.h"

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

static display_area_definition_t displayArea =
{
    .writableArea = 0,
    .layerID = 1,
    .x = 0,
    .y = 0,
    .width = 240,
    .height = 320,
    .border = 0,
    .widgetCount = 0,
    .bufferOffset = 0,
    .bufferSize = 0,
    .buffer = 0,
    .widgets = 0
};

display_area_definition_t* ContentsManager_MenuArea = &displayArea;

void ContentsManagerMock_Reset()
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

void ContentsManager_SetLayerDirty(uint8 layerID)
{}

/**** ClearArea ****/

void ContentsManager_ClearArea(display_area_definition_t* area)
{
    if (clearAreaFirstCallTimeStamp < 0)
    {
        clearAreaFirstCallTimeStamp = clock();
    }
    clearAreaLastCallTimeStamp = clock();

    clearAreaLastAreaId = area->layerID;
    ++clearAreaCallCount;
}

int ContentsManagerMock_ClearArea_CallCount()
{
    return clearAreaCallCount;
}

uint8 ContentsManagerMock_ClearArea_LastAreaID()
{
    return clearAreaLastAreaId;
}

clock_t ContentsManagerMock_ClearArea_FirstCallTimeStamp(void)
{
    return clearAreaFirstCallTimeStamp;
}

clock_t ContentsManagerMock_ClearArea_LastCallTimeStamp(void)
{
    return clearAreaLastCallTimeStamp;
}

/**** GetDisplayAreaDefinition ****/
writable_area_definition_t* ContentsManager_GetDisplayAreaDefinition(uint8 areaId)
{
    if (getDisplayAreaDefinitionFirstCallTimeStamp < 0)
    {
        getDisplayAreaDefinitionFirstCallTimeStamp = clock();
    }
    getDisplayAreaDefinitionLastCallTimeStamp = clock();
    getDisplayAreaDefinitionRequestedAreaId = areaId;

    return &area;
}

int ContentsManagerMock_GetDisplayAreaDefinition_CallCount(void)
{
    return getDisplayAreaDefinitionCallCount;
}

clock_t ContentsManagerMock_GetDisplayAreaDefinition_FirstCallTimeStamp(void)
{
    return getDisplayAreaDefinitionFirstCallTimeStamp;
}

clock_t ContentsManagerMock_GetDisplayAreaDefinition_LastCallTimeStamp(void)
{
    return getDisplayAreaDefinitionLastCallTimeStamp;
}

uint8 ContentsManagerMock_GetDisplayAreaDefinition_LastAreaID(void)
{
    return getDisplayAreaDefinitionRequestedAreaId;
}

dm_state_t ContentsManager_GetState(void)
{
    return m_state;
}

void ContentsManagerMock_SetState(dm_state_t state)
{
    m_state = state;
}