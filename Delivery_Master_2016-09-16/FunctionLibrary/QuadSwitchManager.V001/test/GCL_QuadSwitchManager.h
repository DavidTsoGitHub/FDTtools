#ifndef GCLQUADSWITCHMANAGER_H
#define GCLQUADSWITCHMANAGER_H

#include "StandardTypes.h"

/**************************** Constants and Types ****************************/
#define GCL_SIGNAL_OK         (0U)
#define GCL_SIGNAL_TIMEOUT    (1 << 0)
#define GCL_SIGNAL_ERROR      (1 << 1)

void GCL_Read_QuadSwitchManager_Button1State(uint8* value, uint8* status);
void GCL_Read_QuadSwitchManager_Button2State(uint8* value, uint8* status);
void GCL_Read_QuadSwitchManager_Button3State(uint8* value, uint8* status);
void GCL_Read_QuadSwitchManager_Button4State(uint8* value, uint8* status);

void GCL_Write_QuadSwitchManager_Button1Event(uint8 value);
void GCL_Write_QuadSwitchManager_Button2Event(uint8 value);
void GCL_Write_QuadSwitchManager_Button3Event(uint8 value);
void GCL_Write_QuadSwitchManager_Button4Event(uint8 value);

#endif // GCLQUADSWITCHMANAGER_H
