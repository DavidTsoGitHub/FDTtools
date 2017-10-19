/********************************* Includes **********************************/
#include "ODOTrip.h"
#include "GCL_ODOTrip.h"
#include "StandardTypes.h"
#include "CAL.h"
#include "SysTime.h"
/**************************** Constants and Types ****************************/

#define MAX_ODO_VALUE   (9999999u)
#define MAX_TRIP_VALUE  (99999u)

static const uint32 u32ODOFingerPrint = 0xABBA;

/********************************* Variables *********************************/
// ODO
static uint32 u32CurrentOdoValue;    // Number of kilometers
static uint32 u32OdoRestPulseCount;  // Number of rest pulses for ODOmeter
static uint32 odo_pulses_per_km;     // Calibration

// TripA
static uint32 u32CurrentTripAValue;  // Number of one hundred meters. If set to 1 it means 0.1 Km
static uint32 u32TripARestPulseCount;// Number of rest pulses for TripA
static uint8  u8TripResetAIndication;

// TripB
static uint32 u32CurrentTripBValue;  // Number of one hundred meters. If set to 1 it means 0.1 Km
static uint32 u32TripBRestPulseCount;// Number of rest pulses for TripB
static uint8  u8TripResetBIndication;

static uint32 u32prevFreq;
static PowerMode oldPowerMode;
static uint32 u32LastMsTick;
static uint16 u16WriteIndex;
/************************* Local Function Prototypes *************************/
static tStandardReturnType ODOTrip_ReadOdoFromStorage(uint32 *u32OdoValueMax, uint16 *u16nextIndex);
static tStandardReturnType ODOTrip_WriteDefaultValuesToStorage(void);
static uint16 ODOTrip_GetNextIndex(uint16 u16Index);
static void ODOTrip_WriteOdo(void);

static void ODOTrip_CalcOdo(uint32 u32RestPulseCount);
static void ODOTrip_CalcTripA(uint32 u32RestPulseCount);
static void ODOTrip_CalcTripB(uint32 u32RestPulseCount);

/***************************** Exported Functions ****************************/
void ODOTrip_Init(void)
{
    //Init variables
    u32CurrentOdoValue =  0u;
    u32OdoRestPulseCount =  0u;
    odo_pulses_per_km = 0u;
    u32CurrentTripAValue = 0u;
    u32TripARestPulseCount = 0u;
    u32CurrentTripBValue = 0u;
    u32TripBRestPulseCount = 0u;
    u32prevFreq = 0u;
    oldPowerMode = POWER_INVALID;
    u32LastMsTick = 0u;
    u16WriteIndex = 0u;
    u8TripResetAIndication = 0u;
    u8TripResetBIndication = 0u;
    
    //Must be called after ODO init since the check of the memory is located in ODOTrip_ReadOdoFromStorage
    if (ODOTrip_ReadOdoFromStorage(&u32CurrentOdoValue, &u16WriteIndex) == STD_OK )
    {
        if ( CAL_NoCacheRead32(ODO_KM_REST_PULSES, &u32OdoRestPulseCount) != CAL_OK )
        {
            u32OdoRestPulseCount = 0u;
        }
        
        GCL_Write_ODOTrip_CurrentODOValue(u32CurrentOdoValue);
    }
    
    if ( CAL_Read32(ODO_HM_REST_PULSES_TRIP_A, &u32TripARestPulseCount) != CAL_OK )
    {
        u32TripARestPulseCount = 0u;
    }
    // Get current TripA
    if ( CAL_Read32(TRIP_A_VALUE, &u32CurrentTripAValue) != CAL_OK )
    {
        u32CurrentTripAValue = 0u;
    }
    
    if ( CAL_Read32(ODO_HM_REST_PULSES_TRIP_B, &u32TripBRestPulseCount) != CAL_OK )
    {
        u32TripBRestPulseCount = 0u; 
    }
    // Get current TripB
    if ( CAL_Read32(TRIP_B_VALUE, &u32CurrentTripBValue) != CAL_OK )
    {
        u32CurrentTripBValue =0u;
    }
}

void ODOTrip_Run(void)
{
    uint32 u32CurrentFreq = 0;
    uint8 sigStatus = 0;
    PowerMode currentPowerMode =  POWER_INVALID;
    uint32 u32averageFreq = 0 ;
    uint32 u32CurrentMsTick = 0;
    uint32 u32TimeDiff = 0 ;
    uint8 powerModeStatus;
    uint32 u32RestPulseCount = 0;
    
    GCL_Read_ODOTrip_SystemPowerMode(&currentPowerMode, &powerModeStatus);
	
    if (powerModeStatus != GCL_SIGNAL_OK)
    {
        return;
    }

    //Save the current ODO and rest pulses if we have left IGN ON.
    if ( (oldPowerMode != currentPowerMode)  && (currentPowerMode != POWER_IGNITION_ON) )
    {
        //Save current ODO/Trip and rest pulses
        //TODO by DETC.CHeck customer requirements and determine is DTC shall be set if we cannot store current ODO
        (void)CAL_NoCacheWrite32(ODO_KM_REST_PULSES, &u32OdoRestPulseCount);
        (void)CAL_Write(ODO_HM_REST_PULSES_TRIP_A, (uint8*)&u32TripARestPulseCount, 4);
        (void)CAL_Write(TRIP_A_VALUE, (uint8*)&u32CurrentTripAValue, 4);
        (void)CAL_Write(ODO_HM_REST_PULSES_TRIP_B, (uint8*)&u32TripBRestPulseCount, 4);
        (void)CAL_Write(TRIP_B_VALUE, (uint8*)&u32CurrentTripBValue, 4);
    }
   
    if(currentPowerMode == POWER_IGNITION_ON)
    {
        if ( CAL_Read32(ODO_PULSES_PER_KM, &odo_pulses_per_km) != CAL_OK)
        {
            //Failed to read. Set to default
            odo_pulses_per_km = 4096u;
        }
        
        GCL_Read_ODOTrip_VehSpdFrequency(&u32CurrentFreq, &sigStatus );

        u32CurrentMsTick = SysTime_GetTimeMs();
        
        //Average freq calc
        u32averageFreq = (u32CurrentFreq + u32prevFreq) /2u;
        u32prevFreq = u32CurrentFreq;
        
        //Manage overflow of counter
        if(u32CurrentMsTick >= u32LastMsTick)
        {
            u32TimeDiff = u32CurrentMsTick - u32LastMsTick;
        }
        else    // overflow
        {
            u32TimeDiff = (0xFFFFFFFFu - u32LastMsTick) + u32CurrentMsTick;
        }
        u32LastMsTick = u32CurrentMsTick;
        
        //calculate pulses / millisecond 
        // Note: Frequency is in FPF24.8. So resulting value is 256 times to large.
        // We keep this to reduce loss of accuracy.        
        u32RestPulseCount = u32averageFreq * u32TimeDiff;
        
        if (oldPowerMode != POWER_IGNITION_ON) // To "get rid of" timestamps and frequency since we powered off
        {
            ODOTrip_CalcOdo(0);
            ODOTrip_CalcTripA(0);
            ODOTrip_CalcTripB(0);
            GCL_Write_ODOTrip_CurrentTripValueA(u32CurrentTripAValue);
            GCL_Write_ODOTrip_CurrentTripValueB(u32CurrentTripAValue);            
        }
        else
        {
            ODOTrip_CalcOdo(u32RestPulseCount);
            ODOTrip_CalcTripA(u32RestPulseCount);
            ODOTrip_CalcTripB(u32RestPulseCount);
        }
    }
    oldPowerMode = currentPowerMode;
}

/****************************** Local Functions ******************************/

/* Read ODOmeter value and get next index there next write shall be done */
static tStandardReturnType ODOTrip_ReadOdoFromStorage(uint32 *u32OdoValueMax, uint16 *u16nextIndex)
{
    tStandardReturnType ret = STD_OK;
    uint16 u16i;
    uint32 u32odoValue[5];
    uint32 u32greatestOdoValue = 0;
    uint16 u16IndexValue = 0;
    uint32 u32ODOInitStatus = 0;
    
    if ( CAL_NoCacheRead32(ODO_STATUS, &u32ODOInitStatus) != CAL_OK )
    {
        //Failed to read ODO Status parameter. Assume the EEPROM is corrupted. Show Max values
        ret = STD_NOK;
    }
    else
    {	
        if ( u32ODOInitStatus != u32ODOFingerPrint )
        {
            //ODO has not been initialized. Write the default values
            if ( ODOTrip_WriteDefaultValuesToStorage() != STD_OK )
            {
                ret = STD_NOK;
            }
        }
        
        if (ret != STD_NOK)
        {
            //TODO DETC: Check customer requirements if DTC shall be set is data cannot be retreived
            if ( CAL_NoCacheRead32(ODO_VALUE1, &u32odoValue[0]) != CAL_OK )
            {
                u32odoValue[0] = 0u;
            }
            if ( CAL_NoCacheRead32(ODO_VALUE2, &u32odoValue[1]) != CAL_OK )
            {
                u32odoValue[1] = 0u;
            }
            if ( CAL_NoCacheRead32(ODO_VALUE3, &u32odoValue[2]) != CAL_OK )
            {
                u32odoValue[2] = 0u;
            }
            if ( CAL_NoCacheRead32(ODO_VALUE4, &u32odoValue[3]) != CAL_OK )
            {
                u32odoValue[3] = 0u;
            }
            if ( CAL_NoCacheRead32(ODO_VALUE5, &u32odoValue[4]) != CAL_OK )
            {
                u32odoValue[4] = 0u;
            }
            
            for(u16i=0u; u16i < 5u; u16i++)
            {
                if(u32odoValue[u16i] > u32greatestOdoValue)
                {
                    u32greatestOdoValue = u32odoValue[u16i];
                    u16IndexValue = u16i;
                }
            }
            *u32OdoValueMax = u32greatestOdoValue;
            *u16nextIndex = ODOTrip_GetNextIndex(u16IndexValue);
        }
    }
    return ret;
}

/* Calculate next index */
static uint16 ODOTrip_GetNextIndex(uint16 u16Index)
{
    uint16 u16NextIndex = u16Index;
    if ( u16Index >= 4u )
    {
        u16NextIndex = 0u;
    }
    else
    {
        u16NextIndex = u16Index +1u;
    }
    return u16NextIndex;
    
}

/* Write current ODO. Step write index so EEPROM don't get corrupt */
static void ODOTrip_WriteOdo()
{
    switch(u16WriteIndex)
    {
        case 0:
			(void)CAL_NoCacheWrite32(ODO_VALUE1, &u32CurrentOdoValue);
        break;
        case 1:
			(void)CAL_NoCacheWrite32(ODO_VALUE2, &u32CurrentOdoValue);
        break;
        case 2:
			(void)CAL_NoCacheWrite32(ODO_VALUE3, &u32CurrentOdoValue);
        break;
        case 3:
			(void)CAL_NoCacheWrite32(ODO_VALUE4, &u32CurrentOdoValue);
        break;
        case 4:
			(void)CAL_NoCacheWrite32(ODO_VALUE5, &u32CurrentOdoValue);
        break;		
        default:
            //Should not be here only 5 slots defined.
        break;
    }
    u16WriteIndex = ODOTrip_GetNextIndex(u16WriteIndex);
}



//This function shall initialize the ODO storage. When encryption is added the default values (ODO =0 shall be encrypted as well)
static tStandardReturnType ODOTrip_WriteDefaultValuesToStorage(void)
{
    tStandardReturnType ret = STD_NOK;
    uint32 u32InitData = 0;
    
    if ( CAL_NoCacheWrite32(ODO_VALUE1, &u32InitData) != CAL_OK )
    {   
        ret = STD_NOK;
    }
    if ( CAL_NoCacheWrite32(ODO_VALUE2, &u32InitData) != CAL_OK )
    {
        ret = STD_NOK;
    }
    if ( CAL_NoCacheWrite32(ODO_VALUE3, &u32InitData) != CAL_OK )
    {
        ret = STD_NOK;
    }
    if ( CAL_NoCacheWrite32(ODO_VALUE4, &u32InitData) != CAL_OK )
    {
        ret = STD_NOK;
    }
    if ( CAL_NoCacheWrite32(ODO_VALUE5, &u32InitData) != CAL_OK )
    {
        ret = STD_NOK;
    }
    if ( CAL_NoCacheWrite32(ODO_KM_REST_PULSES, &u32InitData) != CAL_OK )
    {
        ret = STD_NOK;
    }
    if ( CAL_NoCacheWrite32(ODO_STATUS, &u32ODOFingerPrint) != CAL_OK )
    {
        ret = STD_NOK;
    }
    return ret;
}


/* Calculate Odo value */
static void ODOTrip_CalcOdo(uint32 u32RestPulseCount)
{
    if (u32CurrentOdoValue >= MAX_ODO_VALUE)
    {
        // do nothing
    }
    else
    {
        // Save pulses
        u32OdoRestPulseCount += u32RestPulseCount;
            
        //Compare the current number of pulses to the calibration value.
        //Remove the decimals and scale to correct format (from milliseconds)
        if ( (((u32OdoRestPulseCount>>8u)/1000u)) > odo_pulses_per_km )
        { 
            u32CurrentOdoValue++;
            GCL_Write_ODOTrip_CurrentODOValue(u32CurrentOdoValue);
            ODOTrip_WriteOdo();

            //Remove the counted pulses (scaled to FPF24.8*1000)
            u32OdoRestPulseCount -= ((odo_pulses_per_km<<8u)*1000u);
        }
    }
}


/* Calculate Trip A value */
static void ODOTrip_CalcTripA(uint32 u32RestPulseCount)
{
    if(u8TripResetAIndication == 1u)
    {
        u8TripResetAIndication = 0u;
        u32CurrentTripAValue = 0u;
        u32TripARestPulseCount = 0u;
        
        GCL_Write_ODOTrip_CurrentTripValueA(u32CurrentTripAValue);
    }
    else if (u32CurrentTripAValue >= MAX_TRIP_VALUE)
    {
        // do nothing
    }
    else
    {
        // Save pulses
        u32TripARestPulseCount += u32RestPulseCount;
        
        //Compare the current number of pulses to the calibration value.
        //Remove the decimals and scale to correct format (from milliseconds)
        // Count hundred meters
        if ( (((u32TripARestPulseCount>>8u)/1000u)) > (odo_pulses_per_km/10u) )
        { 
            u32CurrentTripAValue++;
            GCL_Write_ODOTrip_CurrentTripValueA(u32CurrentTripAValue);

            //Remove the counted pulses for 100m (scaled to (FPF24.8*1000)/10)
            u32TripARestPulseCount -= ((odo_pulses_per_km<<8u)*100u);
        }
    }
}

/* Calculate Trip B value */
static void ODOTrip_CalcTripB(uint32 u32RestPulseCount)
{
    if(u8TripResetBIndication == 1u)
    {
        u8TripResetBIndication = 0u;
        u32CurrentTripBValue = 0u;
        u32TripBRestPulseCount = 0u;
        
        GCL_Write_ODOTrip_CurrentTripValueB(u32CurrentTripBValue);
    }
    else if (u32CurrentTripBValue >= MAX_TRIP_VALUE)
    {
        // do nothing
    }
    else
    {
        // Save pulses
        u32TripBRestPulseCount += u32RestPulseCount;
        
        //Compare the current number of pulses to the calibration value.
        //Remove the decimals and scale to correct format (from milliseconds)
        // Count hundred meters
        if ( (((u32TripBRestPulseCount>>8u)/1000u)) > (odo_pulses_per_km/10u) )
        { 
            u32CurrentTripBValue++;
            GCL_Write_ODOTrip_CurrentTripValueB(u32CurrentTripBValue);

            //Remove the counted pulses for 100m (scaled to (FPF24.8*1000)/10)
            u32TripBRestPulseCount -= ((odo_pulses_per_km<<8u)*100u);
        }
    }
}

void ODOTrip_TripResetA_Indication()
{
    u8TripResetAIndication = 1u;
}

void ODOTrip_TripResetB_Indication()
{
    u8TripResetBIndication = 1u;
}
