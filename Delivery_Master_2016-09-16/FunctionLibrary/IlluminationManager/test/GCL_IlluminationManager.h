#ifndef GCL_IlluminationManager_H
#define GCL_IlluminationManager_H

/********************************* Includes **********************************/
#include "StandardTypes.h"

/**************************** Constants and Types ****************************/
#define GCL_SIGNAL_OK         (0U)
#define GCL_SIGNAL_TIMEOUT    (1 << 0)
#define GCL_SIGNAL_ERROR      (1 << 1)

/***************************** Exported Functions ****************************/
void GCL_Read_IlluminationManager_SystemPowerMode(PowerMode* value, uint8* status);
void GCL_Read_IlluminationManager_IlluminationMode(uint8* mode, uint8* status);
void GCL_Read_IlluminationManager_IlluminationLevel(uint8* level, uint8* status);
void GCL_Write_IlluminationManager_DisplayIlluminationLevel(uint8 level);
void GCL_Write_IlluminationManager_GaugeIlluminationLevel(uint8 level);
void GCL_Write_IlluminationManager_CurrentIlluminationLevel(uint8 level);

#endif // GCL_IlluminationManager_H
