/********************************* Includes **********************************/
#include "WaterTempHighIndicator.h"
#include "GCL_WaterTempHighIndicator.h"
#include "StandardTypes.h"
#include "SoundManager.h"

/**************************** Constants and Types ****************************/

/********************************* Variables *********************************/
static uint8 lastValue;

/************************* Local Function Prototypes *************************/

/***************************** Exported Functions ****************************/
void WaterTempHighIndicator_Init(void)
{
    lastValue = 0x00;
}

void WaterTempHighIndicator_Run(void)
{
    uint8 u8SignalStatus;
    uint8 WaterTempHighIndication;
    PowerMode systemPowerMode;
    uint8 u8SystemPowerModeStatus;

    GCL_Read_WaterTempHighIndicator_WaterTempHighIndication(&WaterTempHighIndication, &u8SignalStatus);
    GCL_Read_WaterTempHighIndicator_SystemPowerMode(&systemPowerMode, &u8SystemPowerModeStatus);

    if ((u8SignalStatus == GCL_SIGNAL_OK) && (u8SystemPowerModeStatus == GCL_SIGNAL_OK) && (systemPowerMode == POWER_IGNITION_ON))
    {
        if (lastValue != WaterTempHighIndication)
        {
            GCL_Write_WaterTempHighIndicator_WaterTempHighTelltaleValue(WaterTempHighIndication);
            if (WaterTempHighIndication == 0U)
            {
                SoundManager_StopActiveBuzzerSound();
            }
            else
            {
                SoundManager_RequestActiveBuzzerSound(BSP_SOUND_ON);
            }

            lastValue = WaterTempHighIndication;
        }
    }
    else
    {
        if (lastValue != 0U)
        {
            GCL_Write_WaterTempHighIndicator_WaterTempHighTelltaleValue(0);
            SoundManager_StopActiveBuzzerSound();
            lastValue = 0;
        }
    }
}

/****************************** Local Functions ******************************/
