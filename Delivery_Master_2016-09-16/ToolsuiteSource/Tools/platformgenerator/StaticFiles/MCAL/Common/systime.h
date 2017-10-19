#ifndef SYSTIME_H
#define SYSTIME_H
//lint -save

/********************************* Includes **********************************/
#include "Platform_Types.h"

/**************************** Constants and Types ****************************/

/********************************* Functions *********************************/
//lint -e970 [Headerfile used by both platform and simulator context, hence the ambiguity of variable type]
int SysTime_Init(uint32 tickMs);
void SysTime_Destroy(void);
uint8 SysTime_Wait(int isBlocking);
uint32 SysTime_GetTimeMs(void);
uint32 SysTime_GetTimeMsDelta(uint32 lastMsCount);
void SysTime_Reset(void);
void SysTime_Tick1ms(void);


#endif //SYSTIME_H
