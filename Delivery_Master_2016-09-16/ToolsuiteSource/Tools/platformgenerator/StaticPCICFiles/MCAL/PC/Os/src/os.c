/********************************* Includes **********************************/
#include "os.h"
#ifdef _WIN32
#include <Windows.h>
#include <Process.h>
#else
#include <pthread.h>
#include <semaphore.h>
#include <sys/time.h>
#include <time.h>
#include <limits.h>
#endif
#include <errno.h>
#include <assert.h>
#include <stdio.h>
#include "osmacro.h"
#include "os_cfg.h"
#include "Scheduler.h"
#include "Logger.h"
#include "CAL.h"
#include "PC_BSP.h"

/**************************** Constants and Types ****************************/

THREAD_PROTO(SchedulerTask, arg);
THREAD_PROTO(TimerTask, arg);


static unsigned int SchedulerThreadId = 0;
static unsigned int TimerThreadId = 0 ;
THREAD_T m_thread_Scheduler ;
THREAD_T m_thread_Timer ;

SPINLOCK_T m_spin;
volatile uint8 m_running;

void OS_StartTimer(void)
{
    if ( TimerThreadId == 0 )
    {
        m_running = 1;
        THREAD_CREATE(m_thread_Timer, TimerTask, 0, TimerThreadId);
    }
    else
    {
        Logger_Info("PCIC", "Timer thread already started");
    }
}

void OS_StopTimer(void)
{
    m_running = 0;
    THREAD_JOIN(m_thread_Timer);
    TimerThreadId = 0;
}

void OS_Startup(void)
{
    Logger_Info("PCIC", __FUNCTION__);
    SysTime_Init(1);
    SPINLOCK_INIT(m_spin);

    if (SchedulerThreadId == 0)
    {
        THREAD_CREATE(m_thread_Scheduler, SchedulerTask, 0, SchedulerThreadId);
        Logger_Info("PCIC", "Scheduler thread started with ID: %d", SchedulerThreadId);
    }
}

void OS_Shutdown(void)
{
    m_running = 0;

    THREAD_JOIN(m_thread_Timer);
    if (THREAD_DESTROY(m_thread_Scheduler) == 0)
    {
        Logger_Info("PCIC", "Failed to kill scheduler thread!!! (ID: %d)", SchedulerThreadId);
    }

    TimerThreadId = 0;
    SchedulerThreadId = 0;
}


THREAD_PROTO(SchedulerTask,arg)
{
    Logger_Info("PCIC", "Starting SchedulerTask");
    CAL_Init();
    APPL_Init();
    APPL_Start();
    Logger_Info("PCIC", "Stopping scheduler!");
    THREAD_RETURN(0);
}

THREAD_PROTO(TimerTask,arg)
{
    Logger_Info("PCIC", "Starting TimerTask");
    uint8 DCUPeriodicity = 50;
    uint32 runtime = 0;
    for(;;)
    {
        uint8 running;
        SPINLOCK_ENTER(m_spin);
        running = m_running;
        SPINLOCK_LEAVE(m_spin);
        if(running == 0)
        {
            break;
        }
        SysTime_Wait(1);
        ++runtime;

        if (runtime % DCUPeriodicity == 0)
        {
            PCIC_DCUUpdateAllowed();
            PCIC_FramebufferUpdateAllowed();
        }

        APPL_Run();
    }
    Logger_Info("PCIC", "Stopping timer!");
    //Scheduler_Destroy();
    THREAD_RETURN(0);
}
