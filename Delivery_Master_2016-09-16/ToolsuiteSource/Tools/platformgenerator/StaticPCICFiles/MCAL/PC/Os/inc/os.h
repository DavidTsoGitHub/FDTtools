#ifndef OS_H
#define OS_H


/********************************* Includes **********************************/
#include "os_types.h"
#include "os_cfg.h"
/**************************** Constants and Types ****************************/

/********************************* Functions *********************************/
void OS_Startup(void);
void OS_Shutdown(void);
void OS_SetEvent(OsEventType event);
void OS_StopTimer(void);
void OS_StartTimer(void);

#endif //OS_H


