#ifndef GCL_TurnIndication_H
#define GCL_TurnIndication_H

/********************************* Includes **********************************/
#include "Platform_Types.h"

/**************************** Constants and Types ****************************/
#define GCL_SIGNAL_OK         (0U)
#define GCL_SIGNAL_TIMEOUT    (1 << 0)
#define GCL_SIGNAL_ERROR      (1 << 1)

/***************************** Exported Functions ****************************/
void GCL_Read_TurnIndication_RightTurnIndication(uint8* value, uint8* status);
void GCL_Read_TurnIndication_LeftTurnIndication(uint8* value, uint8* status);

void GCL_Write_TurnIndication_TurnRightTelltaleValue(uint8 value);
void GCL_Write_TurnIndication_TurnLeftTelltaleValue(uint8 value);

#endif // GCL_TurnIndication_H
