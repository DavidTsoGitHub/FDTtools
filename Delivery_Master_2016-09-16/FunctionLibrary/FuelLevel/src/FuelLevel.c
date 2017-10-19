/********************************* Includes **********************************/
#include "FuelLevel.h"
#include "FuelLevel_cfg.h"
#include "GCL_FuelLevel.h"
#include "StandardTypes.h"
#include "CAL.h"
#include "Logger.h"

/**************************** Constants and Types ****************************/
#define COMPONENTNAME_FL            "FuelLevel"
#define WARNING_OFF                 0
#define WARNING_ON                  1

#define HISTORIC_VALUE_COUNT_FL     10U
#define COEFFICIENT_SCALING_FL      10000U

typedef enum
{
    STATE_OFF,
    STATE_FUEL_OK,
    STATE_FUEL_LOW
} states_e;

/********************************* Variables *********************************/
static states_e m_state;
static uint16 valueHistory[HISTORIC_VALUE_COUNT_FL];
static uint16 currentGaugeValue;

/************************* Local Function Prototypes *************************/
static void StateOff(void);
static void StateFuelOK(void);
static void StateFuelLow(void);

static void setGaugeValueFiltered(uint16 fuelLevel);
static uint16 calculateFilteredOutputValue(uint16 fuelLevel);
static void pushValueOnHistory(uint16 value);
static uint16 getHistoricAverage(void);
static void resetHistory(void);

/***************************** Exported Functions ****************************/
void FuelLevel_Init(void)
{
    Logger_Info(COMPONENTNAME_FL, "FuelLevel_Init");
    StateOff();
}

void FuelLevel_Run(void)
{
    uint8 u8SignalStatus, u8WarningThreshold, u8WarningTolerance;
    uint16 u16FuelLevel;
	PowerMode powerMode;
    
    if(CAL_Read8(FUEL_WARNINGTHRESHOLD, &u8WarningThreshold) == CAL_NOK)
    {
        Logger_Info(COMPONENTNAME_FL, "Can't read CAL PARAMETER: FUEL_WARNINGTHRESHOLD");
    }
    if(CAL_Read8(FUEL_WARNINGTOLERANCE, &u8WarningTolerance) == CAL_NOK)
    {
        Logger_Info(COMPONENTNAME_FL, "Can't read CAL PARAMETER: FUEL_WARNINGTOLERANCE");
    }    
       
    GCL_Read_FuelLevel_PowerMode(&powerMode, &u8SignalStatus);
    
    switch (m_state)
    {
        case STATE_OFF:
            if (powerMode == POWER_IGNITION_ON)
            {
                GCL_Read_FuelLevel_FuelLevel(&u16FuelLevel, &u8SignalStatus);
                if (u16FuelLevel < u8WarningThreshold)
                {
                    StateFuelLow();
                }
                else
                {
                    StateFuelOK();
                }
            }
        break;
        case STATE_FUEL_LOW:
            if (powerMode != POWER_IGNITION_ON)
            {
                StateOff();
            }
            else
            {
                GCL_Read_FuelLevel_FuelLevel(&u16FuelLevel, &u8SignalStatus);
                setGaugeValueFiltered(u16FuelLevel);

                if (currentGaugeValue >= ((uint16)u8WarningThreshold + (uint16)u8WarningTolerance))
                {
                    StateFuelOK();
                }
            }
        break;
        case STATE_FUEL_OK:
            if (powerMode != POWER_IGNITION_ON)
            {
                StateOff();
            }
            else
            {
                GCL_Read_FuelLevel_FuelLevel(&u16FuelLevel, &u8SignalStatus);
                setGaugeValueFiltered(u16FuelLevel);
                
                if (currentGaugeValue < u8WarningThreshold)
                {
                    StateFuelLow();
                }
            }
        break;
        default:
            // Do nothing
        break;
    }
}

/****************************** Local Functions ******************************/
static void StateOff(void)
{
    Logger_Info(COMPONENTNAME_FL, "State -> OFF");
    GCL_Write_FuelLevel_FuelLevelGaugeValue(0);
    GCL_Write_FuelLevel_FuelLevelWarningIndication(WARNING_OFF);
    currentGaugeValue = 0xFFFF;
    resetHistory();
    
    m_state = STATE_OFF;
}

static void StateFuelOK(void)
{
    Logger_Info(COMPONENTNAME_FL, "State -> FUEL_OK");
    GCL_Write_FuelLevel_FuelLevelWarningIndication(WARNING_OFF);
    m_state = STATE_FUEL_OK;
}

static void StateFuelLow(void)
{
    Logger_Info(COMPONENTNAME_FL, "State -> FUEL_LOW");
    GCL_Write_FuelLevel_FuelLevelWarningIndication(WARNING_ON);
    m_state = STATE_FUEL_LOW;
}

static void setGaugeValueFiltered(uint16 fuelLevel)
{
    uint16 output;
    output = calculateFilteredOutputValue(fuelLevel);
    if (output != currentGaugeValue)
    {
        currentGaugeValue = output;
        GCL_Write_FuelLevel_FuelLevelGaugeValue(output);
    }
    
    pushValueOnHistory(fuelLevel);
}

/*
This function calculates a filtered output value using the latest reading and the last 
HISTORIC_VALUE_COUNT_FL average values.


The scaling parameters are used to increase accuracy since we do not work with floating point data types.
The coefficient, given between zero and COEFFICIENT_SCALING_FL, denotes the importance of the historic average value.
A higher coefficient means that the historic average will be given higher consideration and vice versa.
*/
static uint16 calculateFilteredOutputValue(uint16 fuelLevel)
{
    uint16 historicAverage;
    historicAverage = getHistoricAverage();
    uint16 filteredValue;
    uint32 tmpValue;    
    
    tmpValue = ((((uint32)COEFFICIENT_SCALING_FL-(uint32)u16Coefficient) * fuelLevel) + ((uint32)u16Coefficient * (uint32)historicAverage)) / (uint32)COEFFICIENT_SCALING_FL;
    
    if(tmpValue < 0xffffU)
    {
        filteredValue = (uint16)tmpValue;
    }
    else
    {
        filteredValue = 0xffff;
    }    
    return filteredValue;
}

static void pushValueOnHistory(uint16 value)
{
    uint8 i;
    
    
    for (i=((uint8)HISTORIC_VALUE_COUNT_FL-(uint8)1U); i>0U; --i)
    {
        valueHistory[i] = valueHistory[i-1U];
    }
    
    valueHistory[0] = value;    
}

static uint16 getHistoricAverage()
{
    uint32 sum = 0;
    uint8 i;
    for (i=0; i<HISTORIC_VALUE_COUNT_FL; ++i)
    {
        sum += valueHistory[i];
    }
    
    return (uint16)(sum/HISTORIC_VALUE_COUNT_FL);
}

static void resetHistory(void)
{
    uint8 i;
    for (i=0; i<HISTORIC_VALUE_COUNT_FL; ++i)
    {
        valueHistory[i] = 0x00;
    }
}


