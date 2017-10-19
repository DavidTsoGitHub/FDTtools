#ifndef GCL_STUB_H
#define GCL_STUB_H

#include "StandardTypes.h"

void GCLStub_Reset(void);

void GCLStub_SetPowerMode(PowerMode powerMode);
void GCLStub_SetPowerModeSignalStatus(uint8 signalStatus);
void GCLStub_SetChargingSignal(uint8 chargingSignal);
void GCLStub_SetChargingSignalStatus(uint8 signalStatus);

void GCL_Read_ChargingTellTale_SystemPowerMode(PowerMode* value, uint8* status);
void GCL_Read_ChargingTellTale_ChargingIndicatorRequest(uint8* value, uint8* status);
void GCL_Write_ChargingTellTale_CharingTellTaleValue(uint8 value);

// Define getters to inspect the signals that are written by the application block
 uint8 GCLStub_GetChargingTellTaleValue(void);

#endif //GCL_STUB_H
