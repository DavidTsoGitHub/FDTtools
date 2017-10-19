#include "BSPMock.h"
#include "ModeManager.h"
#include "../tools/check.h"

#define SEGMENT_COUNT   6

static BSP_ECUModes currentMode = BSP_MODE_LOWPOWER;
static BSP_CanTrcvMdType canTransceiverMode = BSP_CANTRCV_STANDBY;
static BSP_DIStatusType ignitionStatus = BSP_DI_OFF;
static uint32 requestModeChangeCallCount = 0;
static uint32 setCanTrcvModeCallCount = 0;
static uint8 segmentArray[SEGMENT_COUNT];

void BSPMock_Reset()
{
    currentMode = BSP_MODE_LOWPOWER;
    canTransceiverMode = BSP_CANTRCV_STANDBY;
    ignitionStatus = BSP_DI_OFF;
    requestModeChangeCallCount = 0;
    setCanTrcvModeCallCount = 0;
    
    memset(segmentArray, 0xFF, sizeof(uint8)*SEGMENT_COUNT);
}


/** Mocked BSP functions **/
void BSP_RequestModeChange(BSP_ECUModes mode)
{
    ++requestModeChangeCallCount;
    currentMode = mode;
    APPL_NotifyModeChange(mode);
}

BSP_StdReturnType BSP_Get_IgnitionStatus(BSP_DIStatusType* status)
{
    *status = ignitionStatus;
    return BSP_OK;
}

void BSP_Set_CanTrcvMode(uint8 chan, BSP_CanTrcvMdType mode)
{
    canTransceiverMode = mode;
    ++setCanTrcvModeCallCount;
}

void BSP_Set_DisplaySegments(uint8* Segments, uint8 ArrayLength)
{
    if (ArrayLength > SEGMENT_COUNT) 
    {
        ck_abort_msg("Received a %s call with array length %d when max segments is set to %d!", ArrayLength, SEGMENT_COUNT);
    }
    
    memcpy(segmentArray, Segments, ArrayLength);
}



/** Set and assert functions **/
void BSPMock_TurnOnIgnition(void)
{
    ignitionStatus = BSP_DI_ON;
}

void BSPMock_TurnOffIgnition(void)
{
    ignitionStatus = BSP_DI_OFF;
}

BSP_ECUModes BSPMock_GetCurrentMode(void)
{
    return currentMode;
}

BSP_CanTrcvMdType BSPMock_GetCANTransceiverMode(void)
{
    return canTransceiverMode;
}

uint32 BSPMock_GetRequestModeChange_CallCount(void)
{
    return requestModeChangeCallCount;
}

uint32 BSPMock_GetSetCanTrcvMode_CallCount(void)
{
    return setCanTrcvModeCallCount;
}

uint8* BSPMock_GetSegments(uint8* segmentLength)
{
    *segmentLength = SEGMENT_COUNT;
    return segmentArray;
}


