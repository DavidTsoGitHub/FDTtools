#ifndef GCL_STUB_H
#define GCL_STUB_H

#include "StandardTypes.h"

#define GCL_SIGNAL_OK         (0U)
#define GCL_SIGNAL_TIMEOUT    (1 << 0)
#define GCL_SIGNAL_ERROR      (1 << 1)

void GCLStub_Reset(void);

void GCLStub_Set_OdoValue(uint32 value, uint8 signalStatus);
void GCLStub_Set_RuntimeValue(uint16 value, uint8 signalStatus);
void GCLStub_Set_ShowNext(boolean value, uint8 signalStatus);
void GCLStub_Indicate_ShowNext(void);

uint32 GCLStub_Get_Value(void);
boolean GCLStub_Get_ShowDot(void);
boolean GCLStub_Get_ShowHourGlass(void);


#endif //GCL_STUB_H
