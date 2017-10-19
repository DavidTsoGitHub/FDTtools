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

BSP_BuzzerSound SoundManagerStub_GetCurrentActiveBuzzerSound(void)
{
    return currentSound;
}

void SoundManager_StopActiveBuzzerSound(void)
{
}


int SoundManagerStub_GetSoundRequestCallCount()
{
    return requestSoundCallCount;
}
