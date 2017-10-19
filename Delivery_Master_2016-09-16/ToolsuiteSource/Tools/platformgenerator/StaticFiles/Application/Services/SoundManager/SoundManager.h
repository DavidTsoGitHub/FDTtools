#ifndef SOUNDMANAGER_H
#define SOUNDMANAGER_H

/********************************* Includes **********************************/
#include "BSP_Types.h"

/**************************** Constants and Types ****************************/


/***************************** Exported Functions ****************************/
void SoundManager_Init(void);
void SoundManager_RequestActiveBuzzerSound(BSP_BuzzerSound sound);
void SoundManager_StopActiveBuzzerSound(void);
void SoundManager_RequestSound(BSP_Sound sound);

#endif // SOUNDMANAGER_H
