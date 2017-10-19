/********************************* Includes **********************************/
#include "BrakePressureLowTellTale.h"
#include "GCL_BrakePressureLowTellTale.h"
#include "StandardTypes.h"

#define RESERVOIR_CIRCUIT_LOW_LIMIT 590u
#define RESERVOIR_HIGH_LIMIT 1000u
#define CIRCUIT_LOW_LIMIT 700u

static uint8 u8warningShown;

/* Sample code is only generated for the function block named task functions
   If you have defined more tasks, they have to be added manually */
void BrakePressureLowTellTale_Init(void)
{
    u8warningShown = 0xFFu;
    
}

void BrakePressureLowTellTale_Run(void)
{
    /** Input signal reading samples **/
    PowerMode powerMode = POWER_IGNITION_OFF;
    uint8 powerModeSignalStatus;
    uint8 u8SignalStatus;
    uint8 AllSignalStatus = 0u;
    uint16 FrontReservoirPressure;
    uint16 RearReservoirPressure;
    uint16 ParkBrakeReservoirPressure;
    uint16 FrontCircuitPressure;
    uint16 RearCircuitPressure;

    GCL_Read_BrakePressureLowTellTale_SystemPowerMode(&powerMode, &powerModeSignalStatus);
    GCL_Read_BrakePressureLowTellTale_FrontCircuitPressure(&FrontCircuitPressure, &u8SignalStatus);
    AllSignalStatus |= u8SignalStatus;
    GCL_Read_BrakePressureLowTellTale_RearCircuitPressure(&RearCircuitPressure, &u8SignalStatus);
    AllSignalStatus |= u8SignalStatus;
    GCL_Read_BrakePressureLowTellTale_FrontReservoirPressure(&FrontReservoirPressure, &u8SignalStatus);
    AllSignalStatus |= u8SignalStatus;
    GCL_Read_BrakePressureLowTellTale_RearReservoirPressure(&RearReservoirPressure, &u8SignalStatus);
    AllSignalStatus |= u8SignalStatus;
    GCL_Read_BrakePressureLowTellTale_ParkBrakeReservoirPressure(&ParkBrakeReservoirPressure, &u8SignalStatus);
    AllSignalStatus |= u8SignalStatus;
    
    if ( (AllSignalStatus == GCL_SIGNAL_OK) && (powerModeSignalStatus == GCL_SIGNAL_OK) && (powerMode == POWER_IGNITION_ON) )
    {
        uint8 warningStatus = FALSE;
        if ((FrontCircuitPressure < RESERVOIR_CIRCUIT_LOW_LIMIT) ||
            (RearCircuitPressure < RESERVOIR_CIRCUIT_LOW_LIMIT) ||
            (FrontReservoirPressure < RESERVOIR_CIRCUIT_LOW_LIMIT) ||
            (RearReservoirPressure < RESERVOIR_CIRCUIT_LOW_LIMIT) ||
            (ParkBrakeReservoirPressure < RESERVOIR_CIRCUIT_LOW_LIMIT))
            {
                warningStatus = 1u;
            }
        else if (((FrontReservoirPressure > RESERVOIR_HIGH_LIMIT) || (RearReservoirPressure > RESERVOIR_HIGH_LIMIT)) &&
            ((FrontCircuitPressure <= CIRCUIT_LOW_LIMIT) || (RearCircuitPressure <= CIRCUIT_LOW_LIMIT) || (ParkBrakeReservoirPressure <= CIRCUIT_LOW_LIMIT)))
        {
            warningStatus = 1u;
        }
        else
        {
            warningStatus = 0u;
        }
        
        if (u8warningShown != warningStatus)
        {
            GCL_Write_BrakePressureLowTellTale_BrakePressureLowIndicatorValue(warningStatus);
            u8warningShown = warningStatus;
        }
    }
    else
    {
        if (u8warningShown != 0u)
        {
            GCL_Write_BrakePressureLowTellTale_BrakePressureLowIndicatorValue(0);
            u8warningShown = 0u;
        }
    }
}
