#include "SoundManager.h"

static BSP_BuzzerSound currentSound;
static int requestSoundCallCount;

void SoundManager_Init(void)
{
    currentSound = BSP_SOUND_1HZ;
    requestSoundCallCount = 0;
}

void SoundManager_RequestActiveBuzzerSound(BSP_BuzzerSound sound)
{
    currentSound = sound;
    ++requestSoundCallCount;
}

void SoundManager_StopActiveBuzzerSound()
{
    
}

BSP_BuzzerSound SoundManagerStub_GetCurrentActiveBuzzerSound(void)
{
    return currentSound;
}

int SoundManagerStub_GetSoundRequestCallCount()
{
    return requestSoundCallCount;
}
