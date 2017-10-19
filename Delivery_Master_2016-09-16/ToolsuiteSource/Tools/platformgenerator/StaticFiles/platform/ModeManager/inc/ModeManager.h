#ifndef MODEMANAGER_H
#define MODEMANAGER_H

/********************************* Includes **********************************/
#include "BSP_Types.h"

/**************************** Constants and Types ****************************/

/***************************** Exported Functions ****************************/
void ModeManager_Init(void);
void ModeManager_Run(void);

void APPL_WakeupNotify(WakeupSourceType wakeupSource);
void APPL_NotifyModeChange(BSP_ECUModes notifiedMode);
void ModeManager_CAN_Timeout(void);


#endif // MODEMANAGER_H
