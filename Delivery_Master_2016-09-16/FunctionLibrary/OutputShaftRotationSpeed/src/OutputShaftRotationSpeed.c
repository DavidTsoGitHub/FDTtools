/********************************* Includes **********************************/
#include "OutputShaftRotationSpeed.h"
#include "GCL_OutputShaftRotationSpeed.h"
#include "StandardTypes.h"
#include "CAL.h"
#include "Logger.h"

#define COMPONENTNAME_OSRS "OutputShaftRotationSpeed"

/* Sample code is only generated for the function block named task functions
   If you have defined more tasks, they have to be added manually */
void OutputShaftRotationSpeed_Init(void)
{
    // Add initialization routines here
}

void OutputShaftRotationSpeed_Run(void)
{
    /** Calibration parameter reading samples **/
    uint16 pulses_per_rotation;
    if(CAL_Read16(PULSES_PER_ROTATION, &pulses_per_rotation) == CAL_NOK)
    {
        Logger_Info(COMPONENTNAME_OSRS, "Can't read CAL PARAMETER: PULSES_PER_ROTATION");
    }

    uint8 ratio_numerator;
    if(CAL_Read8(RATIO_NUMERATOR, &ratio_numerator) == CAL_NOK)
    {
        Logger_Info(COMPONENTNAME_OSRS, "Can't read CAL PARAMETER: RATIO_NUMERATOR");
    }

    uint8 ratio_denominator;
    if(CAL_Read8(RATIO_DENOMINATOR, &ratio_denominator) == CAL_NOK)
    {
        Logger_Info(COMPONENTNAME_OSRS, "Can't read CAL PARAMETER: RATIO_DENOMINATOR");
    }
    
    
    if(ratio_denominator == 0u)
    {
        ratio_denominator = 1;
    }
    if(pulses_per_rotation == 0u)
    {
        pulses_per_rotation = 1;
    }
    

        
    /** Input signal reading samples **/
    uint8 u8SignalStatus;
    uint32 VehSpdFreqInput;
    uint32 outputShaftRotationSpeed;

    GCL_Read_OutputShaftRotationSpeed_VehSpdFreqInput(&VehSpdFreqInput, &u8SignalStatus);
    
    outputShaftRotationSpeed = (uint32)(((uint32)ratio_numerator*VehSpdFreqInput)/((uint32)ratio_denominator*(uint32)pulses_per_rotation));

    /** Output signal writing samples **/
    GCL_Write_OutputShaftRotationSpeed_OutputShaftRotationSpeed((uint16)(outputShaftRotationSpeed >> 8));
}
