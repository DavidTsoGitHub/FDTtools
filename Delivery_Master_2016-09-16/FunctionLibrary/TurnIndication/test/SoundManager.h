#ifndef SOUNDMANAGER_H
#define SOUNDMANAGER_H

/********************************* Includes **********************************/
#include "Platform_Types.h"
typedef enum
{
    BSP_SOUND_TICK,
    BSP_SOUND_TACK,
    NO_SOUND = 0xFF
} BSP_Sound;

/**************************** Constants and Types ****************************/

/***************************** Exported Functions ****************************/
void SoundManager_Init(void);
void SoundManager_RequestSound(BSP_Sound sound);

BSP_Sound SoundManagerStub_GetCurrentSound(void);
int SoundManagerStub_GetSoundRequestCallCount(void);

#endif // SOUNDMANAGER_H