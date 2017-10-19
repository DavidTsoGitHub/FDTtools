#ifndef GCL_STUB_H
#define GCL_STUB_H

#include "StandardTypes.h"

#define GCL_SIGNAL_OK         (0U)
#define GCL_SIGNAL_TIMEOUT    (1 << 0)
#define GCL_SIGNAL_ERROR      (1 << 1)

void GCLStub_Reset(void);

void GCLStub_Set_ShowNext(uint8 value, uint8 signalStatus);
void GCLStub_Indicate_ShowNext(void);
void GCLStub_Set_OdoValue(uint32 value, uint8 signalStatus);
void GCLStub_Set_TripValue(uint32 value, uint8 signalStatus);
void GCLStub_Set_InstantaneousFuelConsumption(uint16 value, uint8 signalStatus);
void GCLStub_Set_TripFuelConsumption(uint32 value, uint8 signalStatus);

wchar* GCLStub_Get_TripFieldText(void);
wchar* GCLStub_Get_OdoAndInstFuelFieldText(void);


#endif //GCL_STUB_H
