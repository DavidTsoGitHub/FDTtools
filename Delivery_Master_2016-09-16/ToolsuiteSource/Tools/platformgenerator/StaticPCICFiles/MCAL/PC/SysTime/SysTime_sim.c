/********************************* Includes **********************************/
#include <stdio.h>
#if defined(_WIN32) || defined(__CYGWIN__)
#include <Windows.h>
#else
#include <sys/time.h>
#include <time.h>
#include <stdint.h>
#endif
#include "SysTime.h"
#include "Logger.h"

/**************************** Constants and Types ****************************/
#if defined(_WIN32) || defined(__CYGWIN__)
#define SKIP_ITERATIONS  100
#define MAX_ITERATIONS 1000000
#define TIMER_RESOLUTION_1MS 10000U //1ms
#define TIMER_TIMEOUT_MS 10U

typedef LONG(CALLBACK* NTSETTIMERRESOLUTION)
(
   IN ULONG DesiredTime,
   IN BOOLEAN SetResolution,
   OUT PULONG ActualTime
);

typedef LONG(CALLBACK* NTQUERYTIMERRESOLUTION)
(
   OUT PULONG MaximumTime,
   OUT PULONG MinimumTime,
   OUT PULONG CurrentTime
);
#else
static struct timespec m_sleepTime = { 0, 1000000*10};
static struct timespec m_beginTime;
static struct timespec m_nextTime;
#endif
/********************************* Variables *********************************/
#if defined(_WIN32) || defined(__CYGWIN__)
NTSETTIMERRESOLUTION NtSetTimerResolution;
NTQUERYTIMERRESOLUTION NtQueryTimerResolution;
LARGE_INTEGER g_freq;
static HANDLE g_hTimer = INVALID_HANDLE_VALUE;
LARGE_INTEGER g_ref;
#endif
static uint32 g_u32SystimeTick;
/************************* Local Function Prototypes *************************/
#if !defined(_WIN32) && !defined(__CYGWIN__)
static struct timespec timespec_add(struct  timespec  time1,struct  timespec  time2);
static struct timespec timespec_subtract(struct  timespec  time1,struct  timespec  time2);
#endif
/***************************** Exported Functions ****************************/

int SysTime_Init(uint32 tickMs)
{
    Logger_Info("PCIC", "%s tickMs=%d", __FUNCTION__, tickMs);
#if defined(_WIN32) || defined(__CYGWIN__)
   //Windows
   ULONG min, max, cur;
   HMODULE hNtDll;
   LARGE_INTEGER dt;
   BOOL b;
   int i, n = 0;
   QueryPerformanceFrequency(&g_freq);
   QueryPerformanceCounter(&g_ref);
   hNtDll = LoadLibrary("NtDll.dll");
   if (!hNtDll)
   {
      return -2;
   }
   
   NtQueryTimerResolution = (NTQUERYTIMERRESOLUTION)GetProcAddress(hNtDll, "NtQueryTimerResolution");
   NtSetTimerResolution = (NTSETTIMERRESOLUTION)GetProcAddress(hNtDll, "NtSetTimerResolution");
   
   NtQueryTimerResolution(&max, &min, &cur);
   NtSetTimerResolution(TIMER_RESOLUTION_1MS, TRUE, &cur);
   FreeLibrary(hNtDll);
   
   
   dt.QuadPart = TIMER_RESOLUTION_1MS * tickMs;
   g_hTimer = CreateWaitableTimer(NULL, FALSE, NULL);
   if (g_hTimer == INVALID_HANDLE_VALUE)
   {
      fprintf(stderr, "CreateWaitableTimer failed\n");
      return -1;
   }
   b = SetWaitableTimer(g_hTimer, &dt, tickMs, NULL, NULL, FALSE);
   if (!b)
   {
      fprintf(stderr, "SetWaitableTimer failed\n");
      return -1;
   }
   
   //run timer for some time to get rid of initial jitter
   for (i = 0; i < MAX_ITERATIONS; ++i)
   {
      if (WaitForSingleObject(g_hTimer, 0) == WAIT_OBJECT_0)
      {
         n++;
         if (n >= SKIP_ITERATIONS)
         {
            break;
         }
      }
   }
   SysTime_Reset();
   return 0;
#else
	//Linux
   if( (tickMs>0) && (tickMs<=1000) ){
      m_sleepTime.tv_sec = 0;
      m_sleepTime.tv_nsec = tickMs*1000000;
      SysTime_Reset();
      return 0;
   }
   return -1;
#endif
}

void SysTime_Destroy(void)
{
#if defined(_WIN32) || defined(__CYGWIN__)
   if (g_hTimer != INVALID_HANDLE_VALUE)
   {
      CloseHandle(g_hTimer);
   }
#endif   
}

uint8 SysTime_Wait(int isBlocking)
{
#if defined(_WIN32) || defined(__CYGWIN__)
   if (g_hTimer != INVALID_HANDLE_VALUE)
   {
      DWORD timeout = isBlocking ? INFINITE : 0;
      if (WaitForSingleObject(g_hTimer, timeout) == WAIT_OBJECT_0)
      {
         g_u32SystimeTick++;
         return 1;
      }
   }
   return 0;
#else
   if(isBlocking){
      clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME,&m_nextTime, NULL);
      m_nextTime=timespec_add(m_nextTime,m_sleepTime);
   }
   else{
      return 0; //nonblocking is not supported in Linux version
   }
   g_u32SystimeTick++;
   return 1;
#endif   

}

uint32 SysTime_GetTimeMs(void){
   return g_u32SystimeTick;
}

uint32 SysTime_GetTimeMsDelta(uint32 lastMsCount)
{
   if(lastMsCount < g_u32SystimeTick)
   {
       return g_u32SystimeTick - lastMsCount;
   }
   else
   {
       return (0xFFFFFFFFFF - lastMsCount) + g_u32SystimeTick;
   }
}

void SysTime_Reset(void)
{
#if defined(_WIN32) || defined(__CYGWIN__)
   SysTime_Wait(1);
   QueryPerformanceCounter(&g_ref);
#else
   clock_gettime(CLOCK_MONOTONIC, &m_beginTime);
   m_nextTime = timespec_add(m_beginTime,m_sleepTime);
#endif   
   g_u32SystimeTick = 0;
}

/****************************** Local Functions ******************************/
#if !defined(_WIN32) && !defined(__CYGWIN__)
struct  timespec  timespec_add (struct  timespec  time1,struct  timespec  time2){
    struct  timespec  result ;
    result.tv_sec = time1.tv_sec + time2.tv_sec ;
    result.tv_nsec = time1.tv_nsec + time2.tv_nsec ;
    if (result.tv_nsec >= 1000000000L) {     /* Carry? */
        result.tv_sec++ ;  result.tv_nsec = result.tv_nsec - 1000000000L ;
    }
    return (result) ;
}

struct  timespec  timespec_subtract (struct  timespec  time1,struct  timespec  time2){
    struct  timespec  result ;
    /* Subtract the second time from the first. */
    if ((time1.tv_sec < time2.tv_sec) ||
        ((time1.tv_sec == time2.tv_sec) &&
         (time1.tv_nsec <= time2.tv_nsec))) {      /* TIME1 <= TIME2? */
        result.tv_sec = result.tv_nsec = 0 ;
    } else {                  /* TIME1 > TIME2 */
        result.tv_sec = time1.tv_sec - time2.tv_sec ;
        if (time1.tv_nsec < time2.tv_nsec) {
            result.tv_nsec = time1.tv_nsec + 1000000000L - time2.tv_nsec ;
            result.tv_sec-- ;          /* Borrow a second. */
        } else {
            result.tv_nsec = time1.tv_nsec - time2.tv_nsec ;
        }
    }

    return (result) ;

}

void SysTime_Tick1ms(void)
{

}
#endif

void SysTime_Tick1ms(void)
{

}
