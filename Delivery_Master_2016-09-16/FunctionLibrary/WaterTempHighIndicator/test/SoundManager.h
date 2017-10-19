#ifndef SOUNDMANAGER_H
#define SOUNDMANAGER_H

/********************************* Includes **********************************/
#include "StandardTypes.h"
typedef enum
{
    BSP_SOUND_ON,
    BSP_SOUND_1HZ
} BSP_BuzzerSound;

/**************************** Constants and Types ****************************/

/***************************** Exported Functions ****************************/
void SoundManager_Init(void);
void SoundManager_RequestActiveBuzzerSound(BSP_BuzzerSound sound);
void SoundManager_StopActiveBuzzerSound(void);

BSP_BuzzerSound SoundManagerStub_GetCurrentActiveBuzzerSound(void);
int SoundManagerStub_GetSoundRequestCallCount(void);

#endif // SOUNDMANAGER_H