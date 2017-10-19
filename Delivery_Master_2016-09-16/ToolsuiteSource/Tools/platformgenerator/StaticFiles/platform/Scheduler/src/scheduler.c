/********************************* Includes **********************************/
#include "Scheduler.h"
#include "Scheduler_Cfg.h"
#include "SysTime.h"
#include "Logger.h"
#include "CAL.h"
#include "COM_Stack.h"

#ifdef __WIN32__
    #include <Windows.h>
#endif

#ifdef SCHEDULER_PROFILING
#include "can_driver.h"
#include "CAN_IF_Cfg.h"
#endif

/**************************** Constants and Types ****************************/
//Scheduler interrupt definition
#define SCHEDULER_PERIOD   5u      //milliseconds
#define COM_PERIOD         5u      //milliseconds

#ifdef SCHEDULER_PROFILING
typedef struct
{
    uint32 ExecutionStart;
    uint32 ExecutionStop;
} TaskProfilingData;

typedef struct
{
    uint32 SchedulerDelayedCount;
    uint32 ConcurrentTasksExecutedCount;
} SchedulerProfilingData;
#endif

/********************************* Variables *********************************/
static uint32 g_nTickms = 0u;
static uint32 u32msCounter = 0u;
#ifdef SCHEDULER_PROFILING
static boolean g_sendTaskProfilingData[NUMBER_OF_SCHEDULER_TASKS];
static TaskProfilingData g_taskProfilingData[NUMBER_OF_SCHEDULER_TASKS];
static SchedulerProfilingData g_schedulerProfilingData;
#endif

/************************* Local Function Prototypes *************************/
static void idleTask(void);

/***************************** Exported Functions ****************************/
/*****************************************************************************
*
*   Scheduler initialization.
*   Run all Init task and set next execution point
*
*******************************************************************************/
void APPL_Init()
{
    uint16 i;
    Logger_Info ("Scheduler","APPL_Init");
    g_nTickms = 0u;

    for (i=0u; i<NUMBER_OF_SCHEDULER_TASKS; ++i)
    {
        if ( TaskTable[i].fnpInit != NULL )
        {
            TaskTable[i].fnpInit();
        }
        TaskTable[i].u16TimeToActivation = TaskTable[i].u16InitialDelayms;
    }

    #ifdef SCHEDULER_PROFILING
    g_schedulerProfilingData.SchedulerDelayedCount = 0u;
    g_schedulerProfilingData.ConcurrentTasksExecutedCount = 0u;

    for (i=0u; i<NUMBER_OF_SCHEDULER_TASKS; ++i)
    {
        g_sendTaskProfilingData[i] = FALSE;
        g_taskProfilingData[i].ExecutionStart = 0u;
        g_taskProfilingData[i].ExecutionStop = 0u;
    }
    #endif
}

/*******************************************************************************
*
*   Shall be called by 1ms timer (interrupt context) to drive the scheduler.
*
*******************************************************************************/
void APPL_Run(void)
{
    //Increase the ms counter. Allow it to overflow.
    u32msCounter++;

    //Increment system timer
    SysTime_Tick1ms();

    if((u32msCounter % COM_PERIOD) == 0u)
    {
        COM_Stack_Run();
    }
}

/*******************************************************************************
*
*   APPL_Start is the main function of the scheduler. It will not return as it contains the
*   infinite loop of  the scheduler.
*
********************************************************************************/
void APPL_Start(void)
{
    static uint32 u32LastRunMs = 0u;
    static uint32 u32msSinceLastRun = 0u;

    for(;;)
    {
        //Calc time since last execution
        if(u32LastRunMs > u32msCounter) //Timer Wrap
        {
            u32msSinceLastRun = (0xFFFFFFFFu - u32LastRunMs) + u32msCounter + 1u;
        }
        else
        {
            u32msSinceLastRun = u32msCounter - u32LastRunMs;
        }

#ifdef SCHEDULER_PROFILING
        // Check for scheduler overrun
        if (u32msSinceLastRun > SCHEDULER_PERIOD )
        {
            ++g_schedulerProfilingData.SchedulerDelayedCount;
        }
#endif

        //Do Idle processing
        idleTask();

        {
            uint16 u16Task;
            if (u32msSinceLastRun >= SCHEDULER_PERIOD)
            {
#ifdef SCHEDULER_PROFILING
                uint8 tasksExecuted = 0u;
#endif
                u32LastRunMs = u32msCounter;
                for (u16Task = 0u; u16Task < NUMBER_OF_SCHEDULER_TASKS; u16Task ++)
                {
                    if (TaskTable[u16Task].fnpTask != NULL)
                    {
                        if (TaskTable[u16Task].u16TimeToActivation < SCHEDULER_PERIOD)
                        {
#ifdef SCHEDULER_PROFILING
                            ++tasksExecuted;
                            g_sendTaskProfilingData[u16Task] = TRUE;
                            g_taskProfilingData[u16Task].ExecutionStart = u32msCounter;
#endif
                            // Call the runnable
                            TaskTable[u16Task].fnpTask();

                            // Restore counter
                            TaskTable[u16Task].u16TimeToActivation
                            += TaskTable[u16Task].u16Periodms - SCHEDULER_PERIOD;

#ifdef SCHEDULER_PROFILING
                            g_taskProfilingData[u16Task].ExecutionStop = u32msCounter;
#endif // SCHEDULER_PROFILING

                        }
                        else
                        {
                            TaskTable[u16Task].u16TimeToActivation -= SCHEDULER_PERIOD;
                        }
                    }
                }
#ifdef SCHEDULER_PROFILING
                if (tasksExecuted > 1u)
                {
                    ++g_schedulerProfilingData.ConcurrentTasksExecutedCount;
                }
#endif
            }
        }
        //allow the task to switch context in windows
        #ifdef __WIN32__
        Sleep(0);
        #endif
    }
}

/*******************************************************************************
*
* Use to retreive the current ms tick count of the scheduler
*
*******************************************************************************/
uint32 Scheduler_GetMsCount(void)
{
   return g_nTickms;
}

uint32 Scheduler_GetMsCountDelta(uint32 lastMsCount)
{
   if(lastMsCount < g_nTickms)
   {
       return g_nTickms - lastMsCount;
   }
   else
   {
       return (0xFFFFFFFFu - lastMsCount) + g_nTickms + 1u;
   }
}

/****************************** Local Functions ******************************/
/*******************************************************************************
*
*   the function that will be called during the IDLE phase of the scheduler.
*
*******************************************************************************/
static void idleTask(void)
{
#ifdef SCHEDULER_PROFILING
    static uint8 profilingMailboxOffset = 1u;

    if ((u32msCounter % 500u) == 0u)
    {
        // Transmit scheduler debug info
#ifdef __WIN32__
        uint8 data[8];
        data[0] = (g_schedulerProfilingData.SchedulerDelayedCount & 0xFF000000u) >> 24u;
        data[1] = (g_schedulerProfilingData.SchedulerDelayedCount & 0xFF0000u) >> 16u;
        data[2] = (g_schedulerProfilingData.SchedulerDelayedCount & 0xFF00u) >> 8u;
        data[3] = (g_schedulerProfilingData.SchedulerDelayedCount & 0xFFu);
        data[4] = (g_schedulerProfilingData.ConcurrentTasksExecutedCount & 0xFF000000u) >> 24u;
        data[5] = (g_schedulerProfilingData.ConcurrentTasksExecutedCount & 0xFF0000u) >> 16u;
        data[6] = (g_schedulerProfilingData.ConcurrentTasksExecutedCount & 0xFF00u) >> 8u;
        data[7] = (g_schedulerProfilingData.ConcurrentTasksExecutedCount & 0xFFu);
        CAN_Write(USED_CAN_CONTROLLER_INDEX, (g_CanMbCount-profilingMailboxOffset), 0x1FFF1000u, data, 8u);
#else
        CAN_Write(USED_CAN_CONTROLLER_INDEX, (g_CanMbCount-profilingMailboxOffset), 0x1FFF1000u, (uint8*)&g_schedulerProfilingData, 8u);
#endif // __WIN32__

        ++profilingMailboxOffset;
        if (profilingMailboxOffset > 8u)
        {
            profilingMailboxOffset = 1u;
        }
    }

    for (uint32 i=0; i<NUMBER_OF_SCHEDULER_TASKS; ++i)
    {
        if (g_sendTaskProfilingData[i] == TRUE)
        {
#ifdef __WIN32__
            uint8 data[8];
            data[0] = (g_taskProfilingData[i].ExecutionStart & 0xFF000000u) >> 24u;
            data[1] = (g_taskProfilingData[i].ExecutionStart & 0xFF0000u) >> 16u;
            data[2] = (g_taskProfilingData[i].ExecutionStart & 0xFF00u) >> 8u;
            data[3] = (g_taskProfilingData[i].ExecutionStart & 0xFFu);
            data[4] = (g_taskProfilingData[i].ExecutionStop & 0xFF000000u) >> 24u;
            data[5] = (g_taskProfilingData[i].ExecutionStop & 0xFF0000u) >> 16u;
            data[6] = (g_taskProfilingData[i].ExecutionStop & 0xFF00u) >> 8u;
            data[7] = (g_taskProfilingData[i].ExecutionStop & 0xFFu);
            CAN_Write(USED_CAN_CONTROLLER_INDEX, (g_CanMbCount-profilingMailboxOffset), (0x1FFF0000 + i), data, 8u);
#else
            CAN_Write(USED_CAN_CONTROLLER_INDEX, (g_CanMbCount-profilingMailboxOffset), (0x1FFF0000 + i), (uint8*)&g_taskProfilingData[i], 8u);
#endif // __WIN32__
            g_sendTaskProfilingData[i] = FALSE;

            ++profilingMailboxOffset;
            if (profilingMailboxOffset > 8) profilingMailboxOffset = 1;
        }
    }
#endif
}
