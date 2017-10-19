/********************************* Includes **********************************/
#include "ChargingTellTale.h"
#include "GCL_ChargingTellTale.h"

/**************************** Constants and Types ****************************/

/********************************* Variables *********************************/

/************************* Local Function Prototypes *************************/

/***************************** Exported Functions ****************************/
void ChargingTellTale_Init(void)
{
	m_lastValue = 0xFF;
}

void ChargingTellTale_Run(void)
{
    uint8 u8ChargingStatus, u8SignalStatus, u8PowerSignalStatus;
    PowerMode powerMode;
	
	GCL_Read_ChargingTellTale_SystemPowerMode(&powerMode, &u8PowerSignalStatus);
	
	if ((u8PowerSignalStatus == GCL_SIGNAL_OK) && (powerMode != POWER_IGNITION_ON))
	{
		if (m_lastValue != 0U)
		{
			GCL_Write_ChargingTellTale_ChargingTellTaleValue(0);
			m_lastValue = 0;
		}
	}
	else if (u8PowerSignalStatus == GCL_SIGNAL_OK)
	{
		GCL_Read_ChargingTellTale_ChargingIndicatorRequest(&u8ChargingStatus, &u8SignalStatus);
		
		if (u8ChargingStatus != m_lastValue)
		{
			GCL_Write_ChargingTellTale_ChargingTellTaleValue(u8ChargingStatus);
			m_lastValue = u8ChargingStatus;
		}
	}
	else
	{
		// Do nothing
	}
}

/****************************** Local Functions ******************************/
