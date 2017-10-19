#include "BSPMock.h"
#include "check.h"
#include "BSP.h"

#define DIGIT_COUNT   6

static uint8 digitArray[DIGIT_COUNT];
static uint8 setDisplaySegmentCalls;

void BSPMock_Reset()
{
    for (int i=0; i<DIGIT_COUNT; ++i)
    {
        digitArray[i] = 0xFF;
    }
    
    setDisplaySegmentCalls = 0;
}

/** Mocked BSP functions **/
void BSP_Set_DisplaySegments(const uint8* Segments, uint8 ArrayLength)
{
    ++setDisplaySegmentCalls;
    if (ArrayLength > DIGIT_COUNT) 
    {
        ck_abort_msg("Received a %s call with array length %d when max segments is set to %d!", ArrayLength, DIGIT_COUNT);
    }
    
    for (int i=0; i<ArrayLength; ++i)
    {
        digitArray[i] = Segments[i];
    }
}

/** Set and assert functions **/
uint8* BSPMock_GetSegments(uint8* segmentLength)
{
    *segmentLength = DIGIT_COUNT;
    return digitArray;
}

uint8 BSPMock_GetSegment(uint8 index)
{
    return digitArray[index];
}

uint8 BSPMock_SetDisplaySegments_CallCount()
{
    return setDisplaySegmentCalls;
}
