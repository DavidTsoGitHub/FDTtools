/********************************* Includes **********************************/
#include "IlluminationManager.h"
#include "GCL_IlluminationManager.h"
#include "StandardTypes.h"
#include "CAL.h"
#include "Logger.h"

/**************************** Constants and Types ****************************/
#define COMPONENTNAME_IM "IlluminationManager"
#define ILLUMINATION_LEVEL_COUNT    5U

typedef enum
{
    STATE_OFF_IM,
    STATE_NIGHT,
    STATE_DAY
} states_im_e;

/********************************* Variables *********************************/
static states_im_e currentState;
static uint8 m_DisplayIlluminationLevels[ILLUMINATION_LEVEL_COUNT];
static uint8 m_GaugeIlluminationLevels[ILLUMINATION_LEVEL_COUNT];
static uint8 m_GCLIlluminationLevel;
static uint8 m_CALIlluminationLevel;
static boolean m_IlluminationLevelUpdatedInGCL;
static boolean m_IlluminationLevelUpdatedInCAL;

/************************* Local Function Prototypes *************************/
static void pushIlluminationLevelsToGCL(void);
static void readCALValues(void);
static void stateOff(void);
static void stateNight(void);
static void stateDay(void);

/***************************** Exported Functions ****************************/
void IlluminationManager_Init(void)
{
    Logger_Info(COMPONENTNAME_IM, "IlluminationManager_Init");
    readCALValues();
    m_GCLIlluminationLevel = m_CALIlluminationLevel;
    stateOff();
    m_IlluminationLevelUpdatedInGCL = FALSE;
    m_IlluminationLevelUpdatedInCAL = FALSE;
}

void IlluminationManager_Run(void)
{
    uint8 u8SignalStatus;
    uint8 IlluminationMode;
    PowerMode powerMode;
    
    GCL_Read_IlluminationManager_SystemPowerMode(&powerMode, &u8SignalStatus);
    GCL_Read_IlluminationManager_IlluminationMode(&IlluminationMode, &u8SignalStatus);
    
    switch (currentState)
    {
        case STATE_OFF_IM:
            if (powerMode == POWER_IGNITION_ON)
            {
                if (IlluminationMode == (uint8)NIGHT_TIME)
                {
                    stateNight();
                }
                else
                {
                    stateDay();
                }
            }
        break;
        case STATE_NIGHT:
            if (powerMode == POWER_IGNITION_ON)
            {
                if (IlluminationMode != (uint8)NIGHT_TIME)
                {
                    stateDay();
                }
                else if (m_IlluminationLevelUpdatedInGCL == TRUE)
                {
                    GCL_Read_IlluminationManager_IlluminationLevel(&m_GCLIlluminationLevel, &u8SignalStatus);
                    Logger_Info(COMPONENTNAME_IM, "NightMode: New IlluminationLevel from GCL:%d", m_GCLIlluminationLevel); 
                    m_IlluminationLevelUpdatedInGCL = FALSE;
                    pushIlluminationLevelsToGCL();
                }
                else
                {
                    readCALValues();
                    if (m_IlluminationLevelUpdatedInCAL == TRUE)
                    {
                        Logger_Info(COMPONENTNAME_IM, "NightMode: New IlluminationLevel from CAL:%d", m_CALIlluminationLevel); 
                        m_GCLIlluminationLevel = m_CALIlluminationLevel;
                        pushIlluminationLevelsToGCL();
                    }
                }
            }
            else
            {
                stateOff();
            }
        break;
        case STATE_DAY:
            if (powerMode == POWER_IGNITION_ON)
            {
                if (IlluminationMode != (uint8)DAY_TIME)
                {
                    stateNight();
                }
                else if (m_IlluminationLevelUpdatedInGCL == TRUE)
                {
                    GCL_Read_IlluminationManager_IlluminationLevel(&m_GCLIlluminationLevel, &u8SignalStatus);
                    Logger_Info(COMPONENTNAME_IM, "DayMode: New IlluminationLevel from GCL:%d", m_GCLIlluminationLevel); 
                    // We don't push new gauge/display values to GCL here because we are in day mode
                    // But the current illumination level should still be able to be manipulated in the UI
                    GCL_Write_IlluminationManager_CurrentIlluminationLevel(m_GCLIlluminationLevel);
                    m_IlluminationLevelUpdatedInGCL = FALSE;
                }
                else
                {
                    readCALValues();
                    if (m_IlluminationLevelUpdatedInCAL == TRUE)
                    {
                        Logger_Info(COMPONENTNAME_IM, "DayMode: New IlluminationLevel from CAL:%d", m_CALIlluminationLevel); 
                        // We don't send new gauge/display values here because we are in day mode
                        GCL_Write_IlluminationManager_CurrentIlluminationLevel(m_CALIlluminationLevel);
                        m_GCLIlluminationLevel = m_CALIlluminationLevel;
                    }
                }
            }
            else
            {
                stateOff();
            }
        break;
        default: // Should never end up here
        break;
    }
}

void IlluminationManager_IlluminationLevel_Indication(void)
{
    m_IlluminationLevelUpdatedInGCL = TRUE;
}

/****************************** Local Functions ******************************/
static void pushIlluminationLevelsToGCL(void)
{
    GCL_Write_IlluminationManager_CurrentIlluminationLevel(m_GCLIlluminationLevel);
    GCL_Write_IlluminationManager_GaugeIlluminationLevel(m_GaugeIlluminationLevels[m_GCLIlluminationLevel - 1U]);
    GCL_Write_IlluminationManager_DisplayIlluminationLevel(m_DisplayIlluminationLevels[m_GCLIlluminationLevel - 1U]);
}

static void readCALValues(void)
{
    static uint8 u8previousCALIlluminationLevel;
    u8previousCALIlluminationLevel = m_CALIlluminationLevel;
    
    
    if(CAL_Read(ILLUMINATION_LEVEL, &m_CALIlluminationLevel, 1U) == CAL_NOK)
    {
        Logger_Info(COMPONENTNAME_IM, "Can't read CAL PARAMETER: ILLUMINATION_LEVEL");
    }    
        
    if (m_CALIlluminationLevel == 0U)
    {
        m_CALIlluminationLevel = 1U;
    }
    
    if (u8previousCALIlluminationLevel != m_CALIlluminationLevel)
    {
        m_IlluminationLevelUpdatedInCAL = TRUE;
    }
    else
    {
        m_IlluminationLevelUpdatedInCAL = FALSE;
    }
    
    if(CAL_Read(DISP_ILLUM, m_DisplayIlluminationLevels, ILLUMINATION_LEVEL_COUNT) == CAL_NOK)
    {
        Logger_Info(COMPONENTNAME_IM, "Can't read CAL PARAMETER: DISP_ILLUM");
    }    
    
    if(CAL_Read(GAUGE_ILLUM, m_GaugeIlluminationLevels, ILLUMINATION_LEVEL_COUNT) == CAL_NOK)
    {
        Logger_Info(COMPONENTNAME_IM, "Can't read CAL PARAMETER: GAUGE_ILLUM");
    }    
}

static void stateDay(void)
{
    Logger_Info(COMPONENTNAME_IM, "State --> Day");
    GCL_Write_IlluminationManager_GaugeIlluminationLevel(0);
    GCL_Write_IlluminationManager_DisplayIlluminationLevel(100);
    currentState = STATE_DAY;
}

static void stateNight(void)
{
    Logger_Info(COMPONENTNAME_IM, "State --> Night");
    GCL_Write_IlluminationManager_GaugeIlluminationLevel(m_GaugeIlluminationLevels[m_GCLIlluminationLevel - 1U]);
    GCL_Write_IlluminationManager_DisplayIlluminationLevel(m_DisplayIlluminationLevels[m_GCLIlluminationLevel - 1U]);
    
    currentState = STATE_NIGHT;
}

static void stateOff(void)
{
    Logger_Info(COMPONENTNAME_IM, "State --> Off");
    if (m_CALIlluminationLevel != m_GCLIlluminationLevel)
    {
        if(CAL_Write(ILLUMINATION_LEVEL, &m_GCLIlluminationLevel, 1U) == CAL_NOK)
        {
            Logger_Info(COMPONENTNAME_IM, "Can't read CAL PARAMETER: ILLUMINATION_LEVEL");
        }
        m_CALIlluminationLevel = m_GCLIlluminationLevel;
    }
    
    GCL_Write_IlluminationManager_CurrentIlluminationLevel(m_GCLIlluminationLevel);
    GCL_Write_IlluminationManager_GaugeIlluminationLevel(0);
    GCL_Write_IlluminationManager_DisplayIlluminationLevel(0);
    currentState = STATE_OFF_IM;
}
