#include "systime.h"

static uint32 currentTime = 0;

uint32 SysTime_GetTimeMs(void)
{
    return currentTime;
}

void SysTimeStub_SetTime(uint32 newTime)
{
    currentTime = newTime;
}

void SysTimeStub_FastForward(uint32 timeToFastForward)
{
    currentTime += timeToFastForward;
}
