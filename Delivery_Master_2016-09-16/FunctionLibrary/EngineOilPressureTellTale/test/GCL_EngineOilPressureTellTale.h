#ifndef GCL_EngineOilPressureTellTale_H
#define GCL_EngineOilPressureTellTale_H

/********************************* Includes **********************************/
#include "StandardTypes.h"

/**************************** Constants and Types ****************************/
#define GCL_SIGNAL_OK         (0U)
#define GCL_SIGNAL_TIMEOUT    (1 << 0)
#define GCL_SIGNAL_ERROR      (1 << 1)

/***************************** Exported Functions ****************************/

void GCL_Read_EngineOilPressureTellTale_SystemPowerMode(PowerMode* value, uint8* status);
void GCL_Read_EngineOilPressureTellTale_OilPressureWarningRequest(uint8* value, uint8* status);

void GCL_Write_EngineOilPressureTellTale_OilPressureWarningIndication(uint8 value);

#endif // GCL_EngineOilPressureTellTale_H
