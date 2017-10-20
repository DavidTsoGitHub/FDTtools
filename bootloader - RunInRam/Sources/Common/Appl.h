#ifndef APPL_H
#define APPL_H

/*=======[I N C L U D E S]====================================================*/

#include "Std_Types.h"
#include "FlashLoader.h"
#include "FlashLoader_Cfg.h"

/*=======[T Y P E   D E F I N I T I O N S]====================================*/


/*=======[E X T E R N A L   F U N C T I O N   D E C L A R A T I O N S]========*/
FL_ResultType Appl_EcuStartup (void);

FL_ResultType Appl_FlStartup (void);

void Appl_InitNvmInfo(FL_NvmInfoType* NvmInfo);

#if (FL_SLEEP_TIMER > 0)
void Appl_EcuShutdown (void);
#endif

void Appl_EcuReset (void);

FL_ResultType Appl_CheckConsistency (void);

void Appl_UpdateTriggerCondition (void);

void Appl_Memcpy(uint8 * dest, const uint8 *source, uint32 length);

void Appl_Memset(uint8 * dest, const uint8 source, uint32 length);
void Tsw(void);

#endif/* endof APPL_H */


