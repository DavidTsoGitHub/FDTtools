#ifndef GCL_WarningIconFields_H
#define GCL_WarningIconFields_H

/********************************* Includes **********************************/
#include "StandardTypes.h"

/**************************** Constants and Types ****************************/
#define GCL_SIGNAL_OK         (0U)
#define GCL_SIGNAL_TIMEOUT    (1 << 0)
#define GCL_SIGNAL_ERROR      (1 << 1)

/***************************** Exported Functions ****************************/
void GCL_Read_WarningIconFields_ChargingIndication(uint8* value, uint8* status);
void GCL_Read_WarningIconFields_ParkBrakeIndication(uint8* value, uint8* status);
void GCL_Read_WarningIconFields_BrakePressureLowIndication(uint8* value, uint8* status);
void GCL_Read_WarningIconFields_EngineOilPressureIndication(uint8* value, uint8* status);
void GCL_Write_WarningIconFields_Icon1ID(uint32 iconID);
void GCL_Write_WarningIconFields_Icon2ID(uint32 iconID);
void GCL_Write_WarningIconFields_Icon3ID(uint32 iconID);
void GCL_Write_WarningIconFields_Icon4ID(uint32 iconID);

#endif // GCL_WarningIconFields_H
