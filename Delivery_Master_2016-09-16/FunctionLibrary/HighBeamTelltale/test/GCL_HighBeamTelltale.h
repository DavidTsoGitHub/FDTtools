#ifndef GCL_HighBeamTelltale_H
#define GCL_HighBeamTelltale_H

/********************************* Includes **********************************/
#include "StandardTypes.h"

/**************************** Constants and Types ****************************/
#define GCL_SIGNAL_OK         (0U)
#define GCL_SIGNAL_TIMEOUT    (1 << 0)
#define GCL_SIGNAL_ERROR      (1 << 1)


/*** Input signals ***/
void GCL_Read_HighBeamTelltale_HighBeamIndicatorRequest(uint8* value, uint8* status);
void GCL_Read_HighBeamTelltale_SystemPowerMode(PowerMode* value, uint8* status);

/*** Output signals ***/
void GCL_Write_HighBeamTelltale_HighBeamTellTaleValue(uint8 value);

/*** Signal indication callbacks ***/

#endif // GCL_HighBeamTelltale_H