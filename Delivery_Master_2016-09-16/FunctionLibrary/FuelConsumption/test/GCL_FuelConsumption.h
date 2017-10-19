#ifndef GCL_FuelConsumption_H
#define GCL_FuelConsumption_H

/********************************* Includes **********************************/
#include "StandardTypes.h"

/**************************** Constants and Types ****************************/
#define GCL_SIGNAL_OK         (0U)
#define GCL_SIGNAL_TIMEOUT    (1 << 0)
#define GCL_SIGNAL_ERROR      (1 << 1)

/***************************** Exported Functions ****************************/
// Define the GCL_(Read|Write)_FuelConsumption_<SignalName>(...) and indication callback methods that will be called by the function block
//
// Read function template:
// void GCL_Read_FuelConsumption_<SignalName>(<ValueType>* value, uint8* status);
//
// Write function template:
// void GCL_Write_FuelConsumption_<SignalName>(<ValueType> value);
//
void GCL_Write_FuelConsumption_FilteredInstantaniousFuelConsumption(uint16 fuelConsumption);
void GCL_Read_FuelConsumption_InstantaneousFuelConsumption(uint16* fuelConsumption, uint8* signalStatus);
void GCL_Read_FuelConsumption_SystemPowerMode(PowerMode* systemPowerMode, uint8* u8SignalStatus);

void GCL_Read_FuelConsumption_FuelRate(uint16* fuelRate, uint8* u8SignalStatus);
void GCL_Read_FuelConsumption_TripDistance(uint32* tripDistance, uint8* u8SignalStatus);

void GCL_Write_FuelConsumption_TotalFuelConsumption(uint32 totalFuelConsumption);
void GCL_Write_FuelConsumption_TripFuelConsumption(uint32 value);

// Indication callback template
void FuelConsumption_ResetSignal_Indication(void);

#endif // GCL_FuelConsumption_H
