/********************************* Includes **********************************/
#include "EngineOilPressureTellTale.h"
#include "GCL_EngineOilPressureTellTale.h"
#include "StandardTypes.h"
#include "Logger.h"

/**************************** Constants and Types ****************************/
#define COMPONENTNAME   "EngineOilPressureTellTale"
#define TELLTALE_OFF    0U
#define TELLTALE_ON     1U

typedef enum
{
    STATE_INIT,
    STATE_WARNING_OFF,
    STATE_WARNING_ON
} EngineOilPressureTellTale_State_e;

/********************************* Variables *********************************/
static EngineOilPressureTellTale_State_e m_state;

/************************* Local Function Prototypes *************************/
static void StateInit(void);
static void StateWarningOn(void);
static void StateWarningOff(void);

/***************************** Exported Functions ****************************/

void EngineOilPressureTellTale_Init(void)
{
    StateInit();
}

void EngineOilPressureTellTale_Run(void)
{
    uint8 u8SignalStatus;
    PowerMode SystemPowerMode;
    uint8 OilPressureWarning;

    GCL_Read_EngineOilPressureTellTale_SystemPowerMode(&SystemPowerMode, &u8SignalStatus);
    
    switch (m_state)
    {
        case STATE_INIT:
            if (SystemPowerMode == POWER_IGNITION_ON)
            {
                GCL_Read_EngineOilPressureTellTale_OilPressureWarningRequest(&OilPressureWarning, &u8SignalStatus);
                
                if (OilPressureWarning == 0x01U)
                {
                    StateWarningOn();
                }
                else
                {
                    StateWarningOff();
                }
            }
        break;
        case STATE_WARNING_OFF:
            if (SystemPowerMode != POWER_IGNITION_ON)
            {
                StateInit();
            }
            else
            {
                GCL_Read_EngineOilPressureTellTale_OilPressureWarningRequest(&OilPressureWarning, &u8SignalStatus);
                if (OilPressureWarning == 0x01U)
                {
                    StateWarningOn();
                }
            }
        break;
        case STATE_WARNING_ON:
            if (SystemPowerMode != POWER_IGNITION_ON)
            {
                StateInit();
            }
            else
            {
                GCL_Read_EngineOilPressureTellTale_OilPressureWarningRequest(&OilPressureWarning, &u8SignalStatus);
                if (OilPressureWarning == 0x00U)
                {
                    StateWarningOff();
                }
            }
        break;
        default:
            StateInit();
        break;
    }
}

/****************************** Local Functions ******************************/

static void StateInit(void)
{
    Logger_Info(COMPONENTNAME, "State -> INIT");
    GCL_Write_EngineOilPressureTellTale_OilPressureWarningIndication(TELLTALE_OFF);
    m_state = STATE_INIT;
}

static void StateWarningOff(void)
{
    Logger_Info(COMPONENTNAME, "State -> WARNING_OFF");
    GCL_Write_EngineOilPressureTellTale_OilPressureWarningIndication(TELLTALE_OFF);
    m_state = STATE_WARNING_OFF;
}

static void StateWarningOn(void)
{
    Logger_Info(COMPONENTNAME, "State -> WARNING_ON");
    GCL_Write_EngineOilPressureTellTale_OilPressureWarningIndication(TELLTALE_ON);
    m_state = STATE_WARNING_ON;
}
