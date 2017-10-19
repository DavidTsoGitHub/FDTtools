#ifndef GCL_WaterTempHighIndicator_H
#define GCL_WaterTempHighIndicator_H

/********************************* Includes **********************************/
#include "StandardTypes.h"

/**************************** Constants and Types ****************************/
#define GCL_SIGNAL_OK         (0U)
#define GCL_SIGNAL_TIMEOUT    (1 << 0)
#define GCL_SIGNAL_ERROR      (1 << 1)

/***************************** Exported Functions ****************************/

void GCL_Read_WaterTempHighIndicator_WaterTempHighIndication(uint8* value, uint8* status);

void GCL_Write_WaterTempHighIndicator_WaterTempHighTelltaleValue(uint8 value);

void GCL_Read_WaterTempHighIndicator_SystemPowerMode(PowerMode* value, uint8* status);

#endif // GCL_WaterTempHighIndicator_H
