#ifndef GCL_BrakePressureLowTellTale_H
#define GCL_BrakePressureLowTellTale_H

/********************************* Includes **********************************/
#include "StandardTypes.h"

/**************************** Constants and Types ****************************/
#define GCL_SIGNAL_OK         (0U)
#define GCL_SIGNAL_TIMEOUT    (1 << 0)
#define GCL_SIGNAL_ERROR      (1 << 1)

/***************************** Exported Functions ****************************/
// Define the GCL_BrakePressureLowTellTale_(Read|Write)_<SignalName>(...) and indication callback methods that will be called by the function block
//
// Read function template:

void GCL_Read_BrakePressureLowTellTale_FrontCircuitPressure(uint16* FrontCircuitPressure, uint8* u8SignalStatus);
void GCL_Read_BrakePressureLowTellTale_RearCircuitPressure(uint16* RearCircuitPressure, uint8* u8SignalStatus);
void GCL_Read_BrakePressureLowTellTale_FrontReservoirPressure(uint16* value, uint8* u8SignalStatus);
void GCL_Read_BrakePressureLowTellTale_RearReservoirPressure(uint16* value, uint8* u8SignalStatus);
void GCL_Read_BrakePressureLowTellTale_ParkBrakeReservoirPressure(uint16* value, uint8* u8SignalStatus);
void GCL_Read_BrakePressureLowTellTale_SystemPowerMode(PowerMode* powerMode, uint8* u8SignalStatus);
//
// Write function template:
void GCL_Write_BrakePressureLowTellTale_BrakePressureLowIndicatorValue(uint8 value);
//
// Indication callback template
// void BrakePressureLowTellTale_<SignalName>_Indication(void);

#endif // GCL_BrakePressureLowTellTale_H
