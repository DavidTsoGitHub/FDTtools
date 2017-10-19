#ifndef BSPMOCK_H
#define BSPMOCK_H

/********************************* Includes **********************************/
#include "BSP_Types.h"

/**************************** Constants and Types ****************************/

/***************************** Exported Functions ****************************/
void BSPMock_Init(void);

BSP_BuzzerSound BSPMock_GetCurrentActiveBuzzerSound(void);

BSP_Sound BSPMock_GetCurrentSound(void);
void BSPMock_SetSoundGeneratorStatus(BSP_SoundGenStatus status);
void BSPMock_SetActiveSoundGeneratorStatus(BSP_SoundGenStatus status);
uint32 BSPMock_GetStopSoundCalls(void);
uint32 BSPMock_GetStopActiveSoundCalls(void);

#endif //BSPMOCK_H