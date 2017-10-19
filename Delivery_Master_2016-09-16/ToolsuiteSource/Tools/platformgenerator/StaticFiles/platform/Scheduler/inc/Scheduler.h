#ifndef _SCHEDULER_H_
#define _SCHEDULER_H_

/********************************* Includes **********************************/
#include "Platform_Types.h"

/**************************** Constants and Types ****************************/
// TASK Interface definitions
typedef void (*TASKCALLBACK)(void);

// Array of tasks
typedef struct
{
    TASKCALLBACK fnpInit;
    TASKCALLBACK fnpTask;
    uint16 u16InitialDelayms;
    uint16 u16Periodms;
    uint16 u16TimeToActivation;
} tTimers;

/********************************* Functions *********************************/

// External function prototypes
void APPL_Init(void);
void APPL_Start(void);
void APPL_Run(void);

uint32 Scheduler_GetMsCount(void);
uint32 Scheduler_GetMsCountDelta(uint32 lastMsCount);

#endif //_SCHEDULER_H_

