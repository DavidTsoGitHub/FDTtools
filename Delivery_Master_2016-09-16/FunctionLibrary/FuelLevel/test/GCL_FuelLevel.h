#ifndef GCL_FuelLevel_H
#define GCL_FuelLevel_H

/********************************* Includes **********************************/
#include "Platform_Types.h"
#include "StandardTypes.h"

/**************************** Constants and Types ****************************/
#define GCL_SIGNAL_OK         (0U)
#define GCL_SIGNAL_TIMEOUT    (1 << 0)
#define GCL_SIGNAL_ERROR      (1 << 1)

/***************************** Exported Functions ****************************/
void GCL_Read_FuelLevel_PowerMode(PowerMode* value, uint8* status);
void GCL_Read_FuelLevel_FuelLevel(uint16* value, uint8* status);
void GCL_Write_FuelLevel_FuelLevelGaugeValue(uint16 value);
void GCL_Write_FuelLevel_FuelLevelWarningIndication(uint8 value);

#endif // GCL_FuelLevel_H
