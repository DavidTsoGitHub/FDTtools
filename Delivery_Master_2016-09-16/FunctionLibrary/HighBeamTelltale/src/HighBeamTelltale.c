/********************************* Includes **********************************/
#include "HighBeamTelltale.h"
#include "GCL_HighBeamTelltale.h"
#include "Platform_Types.h"
#include "Logger.h"

/**************************** Constants and Types ****************************/

/********************************* Variables *********************************/
static uint8 lastValue;

/************************* Local Function Prototypes *************************/
static void SetHighBeamValue(uint8 value);

/***************************** Exported Functions ****************************/
void HighBeamTelltale_Run()
{
    uint8 u8HighBeamStatus, u8SignalStatus, u8PowerSignalStatus;
    PowerMode powerMode;
    GCL_Read_HighBeamTelltale_HighBeamIndicatorRequest(&u8HighBeamStatus, &u8SignalStatus);
    GCL_Read_HighBeamTelltale_SystemPowerMode(&powerMode, &u8PowerSignalStatus);

    if ((u8PowerSignalStatus == GCL_SIGNAL_OK) && (powerMode != POWER_IGNITION_ON))
    {
        if (lastValue != 0u)
        {
            Logger_Info("HighBeamTelltale", "HighBeam status changed %d -> 0", lastValue);
            SetHighBeamValue(0u);
        }
    }
    else if ((u8SignalStatus == GCL_SIGNAL_OK) && (u8HighBeamStatus != lastValue))
    {
        Logger_Info("HighBeamTelltale", "HighBeam status changed %d -> %d", lastValue, u8HighBeamStatus);
        SetHighBeamValue(u8HighBeamStatus);
    }
    else
    {
        // Do nothing
    }
}

void HighBeamTelltale_Init()
{
    Logger_Info("HighBeamTelltale", "HighBeamTelltale_Init");
    lastValue = 0xFF;
}

static void SetHighBeamValue(uint8 value)
{
    GCL_Write_HighBeamTelltale_HighBeamTellTaleValue(value);
    lastValue = value;
}
