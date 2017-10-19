/********************************* Includes **********************************/
#include "FuelConsumption.h"
#include "FuelConsumption_cfg.h"
#include "GCL_FuelConsumption.h"
#include "StandardTypes.h"
#include "CAL.h"

/**************************** Constants and Types ****************************/
#define HISTORIC_VALUE_COUNT                            5u
#define COEFFICIENT_SCALING                             10000U
#define INSTANTANIOUS_FUEL_CONSUMPTION_GAIN             1953U
#define INSTANTANIOUS_FUEL_CONSUMPTION_GAIN_DIVIDER     100000UL

/********************************* Variables *********************************/
static uint16 valueHistory[HISTORIC_VALUE_COUNT];
static uint32 u32TotalFuel;
static uint32 u32TickTotalFuel;
static uint32 TickTripFuel;
static uint32 TripFuel;
static PowerMode PreviousPowerMode;
static boolean ResetSignalActive;

/************************* Local Function Prototypes *************************/
static uint16 calculateFilteredOutputValue(uint16 fuelRate);
static void pushValueOnHistory(uint16 value);
static uint16 getHistoricAverage(void);
static void resetHistory(void);

void FuelConsumption_Init(void)
{
    u32TickTotalFuel = 0u;
    TickTripFuel = 0u;
    TripFuel = 0u;
    PreviousPowerMode = POWER_IGNITION_OFF;
    ResetSignalActive = FALSE;
    
    uint32 TotalFuelConsumptionInCAL = 0u;
    (void)CAL_Read32(TOTAL_FUEL_CONSUMPTION, &TotalFuelConsumptionInCAL);
    u32TotalFuel = TotalFuelConsumptionInCAL;
    
    uint32 TotalFuelConsumptionTickInCAL = 0u;
    (void)CAL_Read32(TOTAL_FUEL_CONSUMPTION_TICKS, &TotalFuelConsumptionTickInCAL);
    u32TickTotalFuel = TotalFuelConsumptionTickInCAL;
}

void FuelConsumption_Run(void)
{
    uint8 u8SPMSignalStatus;
    uint8 u8IFCSignalStatus;
    uint8 u8FRSignalStatus;
    uint16 instantaneousFuelConsumption;
    uint16 filteredInstantaniuosFuelConsumption;
    uint16 fuelRate;
    PowerMode systemPowerMode;

    GCL_Read_FuelConsumption_InstantaneousFuelConsumption(&instantaneousFuelConsumption, &u8IFCSignalStatus);
    GCL_Read_FuelConsumption_FuelRate(&fuelRate, &u8FRSignalStatus);
    GCL_Read_FuelConsumption_SystemPowerMode(&systemPowerMode, &u8SPMSignalStatus);

    if( (u8IFCSignalStatus == GCL_SIGNAL_OK)  &&
        (u8SPMSignalStatus == GCL_SIGNAL_OK) && 
        (u8FRSignalStatus == GCL_SIGNAL_OK) &&
        (systemPowerMode == POWER_IGNITION_ON))
    {
        if (PreviousPowerMode != POWER_IGNITION_ON)
        {
            PreviousPowerMode = POWER_IGNITION_ON;
            TickTripFuel = 0u;
            TripFuel = 0u;
        }
        
        if (ResetSignalActive == TRUE)
        {
            TickTripFuel = 0u;
            TripFuel = 0u;
            ResetSignalActive = FALSE;
        }
        else
        {
            if (ResetSignalActive == TRUE)
            {
                ResetSignalActive = FALSE;
            }
        }
        
        // Filtered FuelConsumption
        filteredInstantaniuosFuelConsumption = calculateFilteredOutputValue(instantaneousFuelConsumption);
        pushValueOnHistory(instantaneousFuelConsumption);
        
        uint16 convertedInstantaniousFuelConsumption = (uint16)((((uint32)filteredInstantaniuosFuelConsumption * INSTANTANIOUS_FUEL_CONSUMPTION_GAIN) + (INSTANTANIOUS_FUEL_CONSUMPTION_GAIN_DIVIDER / 2u)) / INSTANTANIOUS_FUEL_CONSUMPTION_GAIN_DIVIDER);
        
        // Tick Total Fuel Consumption -> ((fuelRate * gain (0.05L/h) * 1000) + 18) / 36000 ==> used fuel since last run in microliter
        uint32 fr = (uint32)(((uint32)fuelRate * 5u * 10u) + 18u);
        u32TickTotalFuel += (uint32)(fr / 36u);
        GCL_Write_FuelConsumption_FilteredInstantaniousFuelConsumption(convertedInstantaniousFuelConsumption);
        
        //Overflow each litre
        if (u32TickTotalFuel >= 1000000u) // u32TotalFuel is increased every litre
        {
            u32TotalFuel += 1u;
            u32TickTotalFuel -= 1000000u;
        }

        if (u32TotalFuel > 9999999u)
        {
            GCL_Write_FuelConsumption_TotalFuelConsumption(9999999u);
        }
        else
        {
            GCL_Write_FuelConsumption_TotalFuelConsumption(u32TotalFuel);
        }
        
        // Trip Fuel Consumption
        fr = (uint32)(((uint32)fuelRate * 5u * 10u) + 18u);
        TickTripFuel += (uint32)(fr / 36u);

        
        // Overflow each 0,1 litre
        if (TickTripFuel >= 100000u)
        {
            TickTripFuel -= 100000u; 
            TripFuel += 1u;
        }
        GCL_Write_FuelConsumption_TripFuelConsumption(TripFuel);
    }
    else if( (u8SPMSignalStatus == GCL_SIGNAL_OK) && (systemPowerMode != POWER_IGNITION_ON) )
    {
        if (PreviousPowerMode == POWER_IGNITION_ON)
        {
            PreviousPowerMode = POWER_LOW;
            (void)CAL_Write(TOTAL_FUEL_CONSUMPTION, (uint8*)&u32TotalFuel, 4u);
            (void)CAL_Write(TOTAL_FUEL_CONSUMPTION_TICKS, (uint8*)&u32TickTotalFuel, 4u);
            GCL_Write_FuelConsumption_FilteredInstantaniousFuelConsumption(0u);
            resetHistory();
        }
    }
    else
    {
        //Oh MISRA....
    }
}

void FuelConsumption_ResetSignal_Indication()
{
    ResetSignalActive = TRUE;
}

static uint16 calculateFilteredOutputValue(uint16 fuelRate)
{
    uint16 historicAverage = getHistoricAverage();

    //return ((COEFFICIENT_SCALING-u16FuelConsumptionCoefficient) * fuelRate) + (u16FuelConsumptionCoefficient * historicAverage))/ COEFFICIENT_SCALING;

    uint32 rate = (uint32)( (uint32)( (uint16)COEFFICIENT_SCALING - u16FuelConsumptionCoefficient ) * fuelRate);
    uint32 hist = (uint32)((uint32)u16FuelConsumptionCoefficient * historicAverage);
    return (uint16)( (rate + hist)/ (uint16)COEFFICIENT_SCALING);

}

static void pushValueOnHistory(uint16 value)
{
    uint8 i;
    for (i=(uint8)(HISTORIC_VALUE_COUNT-1u); i>0u; --i)
    {
        valueHistory[i] = valueHistory[i-1u];
    }
    
    valueHistory[0u] = value;    
}

static uint16 getHistoricAverage()
{
    uint32 sum = 0u;
    uint8 i;
    for (i=0u; i<HISTORIC_VALUE_COUNT; ++i)
    {
        sum += valueHistory[i];
    }
    
    return (uint16)(sum/HISTORIC_VALUE_COUNT);
}

static void resetHistory(void)
{
    uint8 i;
    for (i=0u; i<HISTORIC_VALUE_COUNT; ++i)
    {
        valueHistory[i] = 0x00u;
    }
}
