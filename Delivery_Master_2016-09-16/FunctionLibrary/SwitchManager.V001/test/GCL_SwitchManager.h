#ifndef GCL_SwitchManager_H
#define GCL_SwitchManager_H

#include "Platform_Types.h"

/**************************** Constants and Types ****************************/
#define GCL_SIGNAL_OK         (0U)
#define GCL_SIGNAL_TIMEOUT    (1 << 0)
#define GCL_SIGNAL_ERROR      (1 << 1)

void GCL_Read_SwitchManager_ButtonRaw(uint8* value, uint8* status);

void GCL_Write_SwitchManager_ButtonEvent_Up(uint8 value);
void GCL_Write_SwitchManager_ButtonEvent_Down(uint8 value);
void GCL_Write_SwitchManager_ButtonEvent_Left(uint8 value);
void GCL_Write_SwitchManager_ButtonEvent_Right(uint8 value);
void GCL_Write_SwitchManager_ButtonEvent_Set(uint8 value);
void GCL_Write_SwitchManager_ButtonEvent_Clear(uint8 value);

#endif // GCL_SwitchManager_H
