/********************************* Includes **********************************/
#include "ModeManager.h"
#include "Logger.h"
#include "GCL_Platform.h"
#include "can_driver.h"
#include "systime.h"
#include "COM.h"
#include "BSP.h"

/**************************** Constants and Types ****************************/
typedef enum
{
    MM_STATE_INIT,
    MM_STATE_SLEEP_REQUESTED,
    MM_STATE_SLEEP,
    MM_STATE_LOWPOWER_REQUESTED,
    MM_STATE_LOWPOWER,
    MM_STATE_NORMAL_REQUESTED,
    MM_STATE_NORMAL
} t_ModeManagerStates;

/********************************* Variables *********************************/
static t_ModeManagerStates sCurrentState = MM_STATE_INIT;
static boolean bCANTimedOut;
static boolean bCANWakeup;
static boolean bIgnitionWakeup;

/************************* Local Function Prototypes *************************/
static boolean ignitionIsOn(void);
static boolean ignitionIsOff(void);
static void requestSleepMode(void);
static void requestLowPowerMode(void);
static void requestNormalMode(void);
static void stopCAN(void);
static void startCAN(void);

/***************************** Exported Functions ****************************/
void ModeManager_Init(void)
{
    Logger_Info("ModeManager", "ModeManager_Init");
    sCurrentState = MM_STATE_INIT;
    bCANTimedOut = FALSE;
    bCANWakeup = FALSE;
    bIgnitionWakeup = FALSE;
}

void ModeManager_Run(void)
{
    switch (sCurrentState)
    {
        case MM_STATE_INIT:
            startCAN();
            // In initialization phase we are already in low power mode
            // So if we post a formal request to the BSP for that mode
            // we will never receive the answer and end up waiting for
            // a mode that is already active.
            sCurrentState = MM_STATE_LOWPOWER;
        break;
        case MM_STATE_LOWPOWER:
            if (ignitionIsOn() == TRUE)
            {
                requestNormalMode();
            }
            else
            {
                if ((bCANTimedOut == FALSE) && (COM_Get_CANTimeout() == TRUE))
                {
                    stopCAN();
                    bCANTimedOut = TRUE;
                    requestSleepMode();
                }
            }
        break;
        case MM_STATE_NORMAL:
            if (ignitionIsOff() == TRUE)
            {
                requestLowPowerMode();
            }
        break;
        case MM_STATE_SLEEP:
            if (bCANWakeup == TRUE)
            {
                SysTime_Reset();
                COM_Reset();
                startCAN();
                bCANWakeup = FALSE;
                bIgnitionWakeup = FALSE;
                requestLowPowerMode();
            }
            else
            {
                if (bIgnitionWakeup == TRUE)
                {
                    SysTime_Reset();
                    COM_Reset();
                    startCAN();
                    bCANWakeup = FALSE;
                    bIgnitionWakeup = FALSE;
                    requestNormalMode();
                }
            }
        break;
        case MM_STATE_LOWPOWER_REQUESTED:
        case MM_STATE_NORMAL_REQUESTED:
        case MM_STATE_SLEEP_REQUESTED:
        default: // intended fall-through
        // We do nothing in-between modes
        break;
    }
}

void APPL_WakeupNotify(WakeupSourceType wakeupSource)
{
    if (sCurrentState == MM_STATE_SLEEP)
    {
        switch (wakeupSource)
        {
            case BSP_WAKEUP_CAN_COMM:
                Logger_Info("ModeManager", "WakeupNotify: CAN");
                bCANWakeup = TRUE;
            break;
            case BSP_WAKEUP_IGN:
                Logger_Info("ModeManager", "WakeupNotify: IGN");
                bIgnitionWakeup = TRUE;
            break;
            case BSP_WAKEUP_BUTTON:         // Intentional fall-through
            case BSP_WAKEUP_ANALOGUE_INP:   // Intentional fall-through
            case BSP_WAKEUP_DISCRETE_INP:   // Intentional fall-through
            case BSP_WAKEUP_TIMER:          // Intentional fall-through
            case BSP_WAKEUP_UNKNOWN:        // Intentional fall-through
            default:
            // No other wake up source relevant
            break;
        }
    }
}

void APPL_NotifyModeChange(BSP_ECUModes notifiedMode)
{
    switch (notifiedMode)
    {
        case BSP_MODE_LOWPOWER:
            sCurrentState = MM_STATE_LOWPOWER;
            Logger_Info("ModeManager", "State: MM_STATE_LOWPOWER");
            GCL_Write_Platform_PowerMode(POWER_LOW);
        break;
        case BSP_MODE_NORMAL:
            sCurrentState = MM_STATE_NORMAL;
            Logger_Info("ModeManager", "State: MM_STATE_NORMAL");
            GCL_Write_Platform_PowerMode(POWER_IGNITION_ON);
        break;
        case BSP_MODE_SLEEP:
        default:
            sCurrentState = MM_STATE_SLEEP;
            Logger_Info("ModeManager", "State: MM_STATE_SLEEP");
            GCL_Write_Platform_PowerMode(POWER_IGNITION_OFF);
        break;
    }
}

/****************************** Local Functions ******************************/
static boolean ignitionIsOn(void)
{
    BSP_DIStatusType ignitionStatus;
    boolean returnValue = FALSE;
    BSP_StdReturnType rc = BSP_Get_IgnitionStatus(&ignitionStatus);

    if ((rc == BSP_OK) && (ignitionStatus == BSP_DI_ON))
    {
        returnValue = TRUE;
    }

    return returnValue;
}

static boolean ignitionIsOff(void)
{
    return ((ignitionIsOn() == FALSE) ? TRUE : FALSE);
}

static void requestLowPowerMode(void)
{
    CAN_RxOnly();
    sCurrentState = MM_STATE_LOWPOWER_REQUESTED;
    Logger_Info("ModeManager", "State: MM_STATE_LOWPOWER_REQUESTED");
    BSP_RequestModeChange(BSP_MODE_LOWPOWER);
}

static void requestNormalMode(void)
{
    CAN_RxAndTx();
    sCurrentState = MM_STATE_NORMAL_REQUESTED;
    Logger_Info("ModeManager", "State: MM_STATE_NORMAL_REQUESTED");
    BSP_RequestModeChange(BSP_MODE_NORMAL);
}

static void requestSleepMode(void)
{
    sCurrentState = MM_STATE_SLEEP_REQUESTED;
    Logger_Info("ModeManager", "State: MM_STATE_SLEEP_REQUESTED");
    BSP_RequestModeChange(BSP_MODE_SLEEP);
}

static void stopCAN(void)
{
    CAN_Stop(0);
    BSP_Set_CanTrcvMode(0, BSP_CANTRCV_STANDBY);
}

static void startCAN(void)
{
    BSP_Set_CanTrcvMode(0, BSP_CANTRCV_NORMAL);
    CAN_Start(0);
    bCANTimedOut = FALSE;
}
