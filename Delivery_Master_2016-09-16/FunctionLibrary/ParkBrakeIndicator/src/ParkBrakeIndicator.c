/********************************* Includes **********************************/
#ifdef _WIN32
#ifndef CI_BUILD
#define ENABLE_PRINTOUT
#endif // UNITTEST
#endif // _WIN32

#include "ParkBrakeIndicator.h"
#include "GCL_ParkBrakeIndicator.h"
#include "StandardTypes.h"
#include "SoundManager.h"
#include "Logger.h"

#define COMPONENTNAME_PBI "ParkBrakeIndicator"
#define TELLTALE_ON             1U
#define TELLTALE_OFF            0U
#define CHECKMESSAGE_ACTIVE     1U
#define CHECKMESSAGE_INACTIVE   0U
#define SOUND_ON                BSP_SOUND_ON

typedef enum
{
    STATE_OFF_PBI,
    STATE_RUNNING,
    STATE_INDICATOR_ON,
    STATE_INDICATOR_AND_WARNING
} ParkBrakeIndicator_State_e;

/********************************* Variables *********************************/
static ParkBrakeIndicator_State_e m_state;
static void StateIndicatorAndWarning(void);
static void StateIndicatorOn(void);
static void StateOff(void);
static void StateRunning(void);

/***************************** Exported Functions ****************************/
void ParkBrakeIndicator_Init()
{
#ifdef ENABLE_PRINTOUT
    Logger_Info(COMPONENTNAME_PBI, "ParkBrakeIndicator_Init");
#endif
    m_state = STATE_OFF_PBI;
}

void ParkBrakeIndicator_Run()
{
	uint8 u8ParkBrakeStatus, u8PowerModeSignalStatus,u8ParkBrakeSignalStatus, u8VehSpdSignalStatus;
	uint16 u16VehSpd;
    PowerMode powerMode;

    GCL_Read_ParkBrakeIndicator_SystemPowerMode(&powerMode, &u8PowerModeSignalStatus);
    
    switch (m_state)
    {
        case STATE_OFF_PBI:
            if (powerMode == POWER_IGNITION_ON)
            {
                StateRunning();
            }
        break;
        case STATE_RUNNING:
            if (powerMode != POWER_IGNITION_ON)
            {
                StateOff();
            }
            else
            {
                GCL_Read_ParkBrakeIndicator_ParkBrakeSwitch(&u8ParkBrakeStatus, &u8ParkBrakeSignalStatus);
                GCL_Read_ParkBrakeIndicator_VehSpd(&u16VehSpd, &u8VehSpdSignalStatus);            
                
                if((u8ParkBrakeStatus > 0U) && (u16VehSpd > 0U))
                {
                    StateIndicatorAndWarning();
                }
                else if (u8ParkBrakeStatus > 0U)
                {
                    StateIndicatorOn();
                }
                else
                {
                    // Do nothing
                }
            }
        break;
        case STATE_INDICATOR_ON:
            if (powerMode != POWER_IGNITION_ON)
            {
                StateOff();
            }
            else
            {
                GCL_Read_ParkBrakeIndicator_ParkBrakeSwitch(&u8ParkBrakeStatus, &u8ParkBrakeSignalStatus);
                GCL_Read_ParkBrakeIndicator_VehSpd(&u16VehSpd, &u8VehSpdSignalStatus);
                
                if ((u16VehSpd > 0U) && (u8ParkBrakeStatus > 0U))
                {
                    StateIndicatorAndWarning();
                }
                else if (u8ParkBrakeStatus == 0U)
                {
                    StateRunning();
                }
                else
                {
                    // Do nothing
                }
            }
        break;
        case STATE_INDICATOR_AND_WARNING:
            if (powerMode != POWER_IGNITION_ON)
            {
                StateOff();
            }
            else
            {
                GCL_Read_ParkBrakeIndicator_ParkBrakeSwitch(&u8ParkBrakeStatus, &u8ParkBrakeSignalStatus);
                GCL_Read_ParkBrakeIndicator_VehSpd(&u16VehSpd, &u8VehSpdSignalStatus);
                
                if (u8ParkBrakeStatus == 0U)
                {
                    StateRunning();
                }
                else if (u16VehSpd == 0U)
                {
                    StateIndicatorOn();
                }
                else
                {
                    // Do nothing
                }
                
            }
        break;
        default:
            // We should not end up here, return to OFF
            StateOff();
        break;
    }	
}


static void StateIndicatorAndWarning()
{
    Logger_Info(COMPONENTNAME_PBI, "State -> INDICATOR_AND_WARNING");
    GCL_Write_ParkBrakeIndicator_ParkBrakeIndicatorCheckMessageActive(CHECKMESSAGE_ACTIVE);
    GCL_Write_ParkBrakeIndicator_ParkBrakeIndicatorValue(TELLTALE_ON);
    SoundManager_RequestActiveBuzzerSound(SOUND_ON);
    m_state = STATE_INDICATOR_AND_WARNING;
}

static void StateIndicatorOn()
{
    Logger_Info(COMPONENTNAME_PBI, "State -> INDICATOR_ON");
    GCL_Write_ParkBrakeIndicator_ParkBrakeIndicatorCheckMessageActive(CHECKMESSAGE_INACTIVE);
    GCL_Write_ParkBrakeIndicator_ParkBrakeIndicatorValue(TELLTALE_ON);
    SoundManager_StopActiveBuzzerSound();
    m_state = STATE_INDICATOR_ON;
}

static void StateOff()
{
    Logger_Info(COMPONENTNAME_PBI, "State -> OFF");
    GCL_Write_ParkBrakeIndicator_ParkBrakeIndicatorCheckMessageActive(CHECKMESSAGE_INACTIVE);
    GCL_Write_ParkBrakeIndicator_ParkBrakeIndicatorValue(TELLTALE_OFF);
    SoundManager_StopActiveBuzzerSound();
    m_state = STATE_OFF_PBI;
}

static void StateRunning()
{
    Logger_Info(COMPONENTNAME_PBI, "State -> RUNNING");
    GCL_Write_ParkBrakeIndicator_ParkBrakeIndicatorCheckMessageActive(CHECKMESSAGE_INACTIVE);
    GCL_Write_ParkBrakeIndicator_ParkBrakeIndicatorValue(TELLTALE_OFF);
    SoundManager_StopActiveBuzzerSound();
    m_state = STATE_RUNNING;
}
