/********************************* Includes **********************************/
#include "SoundManager.h"
#include "BSP.h"
#include "Logger.h"

/**************************** Constants and Types ****************************/
#define DEFAULT_VOLUME 128u

/********************************* Variables *********************************/
static BSP_BuzzerSound m_lastBuzzerSoundPlayed;
static BSP_Sound m_lastSoundPlayed;

/************************* Local Function Prototypes *************************/
static boolean soundPlaying(void);

/***************************** Exported Functions ****************************/
void SoundManager_Init(void)
{
    Logger_Info("SoundManager", "SoundManager_Init");
    m_lastSoundPlayed = BSP_SOUND_INVALID;
    m_lastBuzzerSoundPlayed = BSP_BUZZER_SOUND_INVALID;
}

void SoundManager_RequestActiveBuzzerSound(BSP_BuzzerSound sound)
{
    Logger_Info("SoundManager", "Active buzzer sound %d requested", sound);

    if (m_lastBuzzerSoundPlayed != sound)
    {
        Logger_Info("SoundManager", "Playing active buzzer sound %d", sound);
        BSP_Sound_Buzzer(sound);
        m_lastBuzzerSoundPlayed = sound;
    }
}

void SoundManager_StopActiveBuzzerSound(void)
{
    Logger_Info("SoundManager", "Stopping active buzzer");
    m_lastBuzzerSoundPlayed = BSP_BUZZER_SOUND_INVALID;
    BSP_StopActiveSoundGeneration();
}

void SoundManager_RequestSound(BSP_Sound sound)
{
    Logger_Info("SoundManager", "Passive buzzer sound %d requested", sound);
    if (soundPlaying() == TRUE)
    {
        Logger_Info("SoundManager", "Currently playing passive buzzer sound %d", m_lastSoundPlayed);
        if (sound >= m_lastSoundPlayed)
        {
            Logger_Info("SoundManager", "Changing passive buzzer sound to %d", sound);
            BSP_Service_StopSoundGeneration();
            (void)BSP_Service_SoundGen(sound, DEFAULT_VOLUME);
        }
    }
    else
    {
        Logger_Info("SoundManager", "Playing passive buzzer sound %d", sound);
        (void)BSP_Service_SoundGen(sound, DEFAULT_VOLUME);
    }

    m_lastSoundPlayed = sound;
}

/****************************** Local Functions ******************************/
static boolean soundPlaying()
{
    return ((BSP_Get_SoundGeneratorStatus() == BSP_SOUND_GEN_ACTIVE) ? TRUE : FALSE);
}
