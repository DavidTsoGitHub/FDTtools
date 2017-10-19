#ifndef GCL_UreaLevelMeter_H
#define GCL_UreaLevelMeter_H

#include "StandardTypes.h"

#define GCL_SIGNAL_OK      (0U)
#define GCL_SIGNAL_TIMEOUT (1 << 0)


/**************************** Constants and Types ****************************/

void GCL_Read_UreaLevelMeter_CatalystTankLevel(uint8* value, uint8* status);
void GCL_Read_UreaLevelMeter_PowerMode(PowerMode* value, uint8* status);
void GCL_Write_UreaLevelMeter_Segment0(uint8 value);
void GCL_Write_UreaLevelMeter_Segment1(uint8 value);
void GCL_Write_UreaLevelMeter_Segment2(uint8 value);
void GCL_Write_UreaLevelMeter_Segment3(uint8 value);

#endif // GCL_UreaLevelMeter_H
