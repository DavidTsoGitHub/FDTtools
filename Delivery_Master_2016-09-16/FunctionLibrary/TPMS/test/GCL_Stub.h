#ifndef GCL_STUB_H
#define GCL_STUB_H

#include "StandardTypes.h"
#include "Platform_Types.h"

#define GCL_SIGNAL_OK         (0U)
#define GCL_SIGNAL_TIMEOUT    (1 << 0)
#define GCL_SIGNAL_ERROR      (1 << 1)

#define PRESSURE_WARNING_MASK (0x07)

void GCLStub_Reset(void);

void GCLStub_SetTirePressure(uint8 id, uint8 pressure);
void GCLStub_SetTirePressureWarning(uint8 id, uint8 warningValue);
DynamicWidgetValue GCLStub_GetTPMSValue(uint8 id);
uint8 GCLStub_GetTPMSIndicatorValue();

void GCLStub_SetPowerMode(uint8 powerMode);
void GCLStub_SetPowerModeSignalStatus(uint8 highBeamSignal);

#endif //GCL_STUB_H
