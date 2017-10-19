#ifndef GCL_ChargingTellTale_H
#define GCL_ChargingTellTale_H

#include "StandardTypes.h"

/**************************** Constants and Types ****************************/
#define GCL_SIGNAL_OK         (0U)
#define GCL_SIGNAL_TIMEOUT    (1 << 0)
#define GCL_SIGNAL_ERROR      (1 << 1)

void GCL_Read_ChargingTellTale_SystemPowerMode(PowerMode* value, uint8* status);
void GCL_Read_ChargingTellTale_ChargingIndicatorRequest(uint8* value, uint8* status);

void GCL_Write_ChargingTellTale_ChargingTellTaleValue(uint8 value);

#endif // GCL_ChargingTellTale_H
