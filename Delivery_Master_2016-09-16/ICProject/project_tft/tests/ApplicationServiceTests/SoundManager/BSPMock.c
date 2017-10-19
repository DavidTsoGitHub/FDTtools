/********************************* Includes **********************************/
#include "BSPMock.h"
//#include "BSP.h"

/**************************** Constants and Types ****************************/

/********************************* Variables *********************************/
static BSP_BuzzerSound m_currentActiveBuzzerSound;
static BSP_Sound m_currentSound;
static BSP_SoundGenStatus m_currentSoundGenStatus;
static BSP_SoundGenStatus m_currentActiveSoundGenStatus;
static uint32 m_stopSoundCalls;
static uint32 m_stopActiveSoundCalls;
/************************* Local Function Prototypes *************************/

/***************************** Exported Functions ****************************/
void BSPMock_Init(void)
{
    m_currentActiveBuzzerSound = 0xFF;
	m_currentSound = 0xFF;
    m_stopSoundCalls = 0;
    m_currentSoundGenStatus = BSP_SOUND_GEN_INACTIVE;
}

BSP_BuzzerSound BSPMock_GetCurrentActiveBuzzerSound(void)
{
	return m_currentActiveBuzzerSound;
}

BSP_Sound BSPMock_GetCurrentSound(void)
{
    return m_currentSound;
}

void BSPMock_SetSoundGeneratorStatus(BSP_SoundGenStatus status)
{
    m_currentSoundGenStatus = status;
}

void BSPMock_SetActiveSoundGeneratorStatus(BSP_SoundGenStatus status)
{
    m_currentActiveSoundGenStatus = status;
}

uint32 BSPMock_GetStopSoundCalls()
{
    return m_stopSoundCalls;
}

uint32 BSPMock_GetStopActiveSoundCalls()
{
    return m_stopActiveSoundCalls;
}

/* BSP functions Active Buzzer */
void BSP_Sound_Buzzer(BSP_BuzzerSound sound)
{
    m_currentActiveBuzzerSound = sound;
    m_currentActiveSoundGenStatus = BSP_SOUND_GEN_ACTIVE;
}

void BSP_StopActiveSoundGeneration(void)
{
    m_currentActiveSoundGenStatus = BSP_SOUND_GEN_INACTIVE;
    ++m_stopActiveSoundCalls;
}

BSP_SoundGenStatus BSP_Get_ActiveSoundGenerationStatus(void)
{
    return m_currentActiveSoundGenStatus;
}

/* BSP functions Passive Buzzer */
BSP_StdReturnType BSP_Service_SoundGen(BSP_Sound Sound, uint8 Volume)
{
    m_currentSound = Sound;
    m_currentSoundGenStatus = BSP_SOUND_GEN_ACTIVE;
    
    return BSP_OK;
}

void BSP_Service_StopSoundGeneration(void)
{
    m_currentSoundGenStatus = BSP_SOUND_GEN_INACTIVE;
    ++m_stopSoundCalls;
}

BSP_SoundGenStatus BSP_Get_SoundGeneratorStatus(void)
{
    return m_currentSoundGenStatus;
}



/****************************** Local Functions ******************************/


