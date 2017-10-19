#ifndef GCL_ReleaseParkBrakeTrigger_H
#define GCL_ReleaseParkBrakeTrigger_H

#include "StandardTypes.h"

/**************************** Constants and Types ****************************/
#define GCL_SIGNAL_OK         (0U)
#define GCL_SIGNAL_TIMEOUT    (1 << 0)
#define GCL_SIGNAL_ERROR      (1 << 1)

void GCL_Read_ParkBrakeIndicator_VehSpd(uint16* value, uint8* status);
void GCL_Read_ParkBrakeIndicator_ParkBrakeSwitch(uint8* value, uint8* status);
void GCL_Read_ParkBrakeIndicator_SystemPowerMode(PowerMode* value, uint8* status);

void GCL_Write_ParkBrakeIndicator_ParkBrakeIndicatorCheckMessageActive(uint8 value);
void GCL_Write_ParkBrakeIndicator_ParkBrakeIndicatorValue(uint8 value);

#endif // GCL_ReleaseParkBrakeTrigger_H
