#ifndef GCL_VehSpdGauge_H
#define GCL_VehSpdGauge_H

#include "StandardTypes.h"

/**************************** Constants and Types ****************************/
#define GCL_SIGNAL_OK         (0U)
#define GCL_SIGNAL_TIMEOUT    (1 << 0)
#define GCL_SIGNAL_ERROR      (1 << 1)

void GCL_Read_VehSpdGauge_InputFrequency(uint32* value, uint8* status);
void GCL_Read_VehSpdGauge_SystemPowerMode(PowerMode *value, uint8* status);

void GCL_Write_VehSpdGauge_VehSpdGaugeValue(uint16 value);

#endif // GCL_VehSpdGauge_H
