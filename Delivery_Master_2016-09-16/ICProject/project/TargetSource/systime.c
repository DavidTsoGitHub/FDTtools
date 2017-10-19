#include "SysTime.h"

static uint32 u32Timer;

int SysTime_Init(uint32 tickMs)
{
    u32Timer = tickMs;
    return 0;
}

uint32 SysTime_GetTimeMs()
{
    return u32Timer;
}

uint32 SysTime_GetTimeMsDelta(uint32 lastMsCount)
{
   if(lastMsCount < u32Timer)
   {
       return u32Timer - lastMsCount;
   }
   else
   {
       return (0xFFFFFFFFFF - lastMsCount) + u32Timer;
   }
}

void SysTime_Destroy(void)
{
    return;
}
uint8 SysTime_Wait(int isBlocking)
{
    isBlocking = isBlocking; //Prevent warnings
    return 0u;
}

void SysTime_Reset(void)
{
    u32Timer = 0u;
}

void SysTime_Tick1ms(void)
{

    u32Timer++;
}