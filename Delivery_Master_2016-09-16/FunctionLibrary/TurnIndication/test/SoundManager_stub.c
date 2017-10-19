#include "SoundManager.h"

static BSP_Sound currentSound;
static int requestSoundCallCount;

void SoundManager_Init(void)
{
    currentSound = NO_SOUND;
    requestSoundCallCount = 0;
}

void SoundManager_RequestSound(BSP_Sound sound)
{
    currentSound = sound;
    ++requestSoundCallCount;
}

BSP_Sound SoundManagerStub_GetCurrentSound(void)
{
    return currentSound;
}

int SoundManagerStub_GetSoundRequestCallCount()
{
    return requestSoundCallCount;
}
