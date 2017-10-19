//lint -save
#ifdef _WIN32
#ifndef UNITTEST
#define ENABLE_PRINTOUT
#endif // UNITTEST
#endif // _WIN32

#include "VehSpdGauge.h"
#include "VehSpdGauge_cfg.h"
#include "FilterLib.h"
#include "GCL_VehSpdGauge.h"
#include "CAL.h"
#include "Platform_Types.h"
#include "Logger.h"

#define COMPONENTNAME_VSG "VehSpdGauge"

typedef enum
{
    Gauge_Off,
    Gauge_On
} gauge_state_t;

static FirstOrderFilterInpuPosNegLoRes16 filterData;
static gauge_state_t gaugeState;
static uint16 previousGaugeValue;

static void ResetFilter(void);
static void InitializeFilter(void);
static void SetNeedleRestPos(void);
static uint16 ConvertSpeed(uint32 u32SpeedInput);
static void SetVehicleSpeed(uint16 u16Spd);
static void SetVehicleSpeedFiltered(uint16 u16Spd);
static void updateFilterParameter(void);

void VehSpdGauge_Run(void)
{
    uint32 u32InputFrequency;
    uint16 u16VehSpd;
    uint8 u8Status;
    PowerMode powerMode;
    
    GCL_Read_VehSpdGauge_SystemPowerMode(&powerMode, &u8Status);
    
    switch (gaugeState)
    {
        case Gauge_Off:
            if (powerMode == POWER_IGNITION_ON)
            {
                Logger_Info(COMPONENTNAME_VSG, "State --> On");
                gaugeState = Gauge_On;
            }
        break;
        case Gauge_On:
            if (powerMode != POWER_IGNITION_ON)
            {
                Logger_Info(COMPONENTNAME_VSG, "State --> Off");
                SetVehicleSpeed(0);
                ResetFilter();
                gaugeState = Gauge_Off;
            }
            else
            {
                GCL_Read_VehSpdGauge_InputFrequency(&u32InputFrequency, &u8Status);
                u16VehSpd = ConvertSpeed(u32InputFrequency);
                SetVehicleSpeedFiltered(u16VehSpd);
            }
        break;
        default:
            Logger_Info(COMPONENTNAME_VSG, "Unknown state %d", gaugeState);
        break;
    }
}

void VehSpdGauge_Init(void)
{

#ifdef ENABLE_PRINTOUT
    Logger_Info(COMPONENTNAME_VSG, "VehSpdGauge_Init");
#endif

    previousGaugeValue = 0xFFFF;
    gaugeState = Gauge_Off;
    InitializeFilter();
    SetNeedleRestPos();

}

static void SetNeedleRestPos(void)
{
   GCL_Write_VehSpdGauge_VehSpdGaugeValue(u16NeedleRestPos);
}

static uint16 ConvertSpeed(uint32 u32SpeedInput)
{
    uint32 u32MaxInputFrequency;
    uint8 u8MaxSpeedValue;
    uint16 ret;
        
        
    if(CAL_Read32(VEHSPD_MAXINPUTFREQUENCY, &u32MaxInputFrequency) == CAL_NOK)
    {
        Logger_Info(COMPONENTNAME_VSG, "Can't read CAL PARAMETER: VEHSPD_MAXINPUTFREQUENCY");
    }        
    
    if(CAL_Read8(VEHSPD_MAXSPEEDVALUE, &u8MaxSpeedValue) == CAL_NOK)
    {
        Logger_Info(COMPONENTNAME_VSG, "Can't read CAL PARAMETER: VEHSPD_MAXSPEEDVALUE");
    }    
    
    if (u32MaxInputFrequency != 0U )
    {
        ret = (uint16)((u32SpeedInput*u8MaxSpeedValue)/u32MaxInputFrequency);
    }
    else
    {
        ret = 0U;
    }
    return ret;
}

static void SetVehicleSpeed(uint16 u16Spd)
{
    if (u16Spd > u16NeedleMaxPos)
    {
        GCL_Write_VehSpdGauge_VehSpdGaugeValue(u16NeedleMaxPos);
    }
    else
    {
        GCL_Write_VehSpdGauge_VehSpdGaugeValue(u16Spd);
    }
}

static void SetVehicleSpeedFiltered(uint16 u16Spd)
{
    //lint --e(845) [Value is a calibration parameter]
    uint16 u16needlePos = u16Spd + u16VehSpdBias;
    
    if (u16needlePos > u16NeedleMaxPos)
    {
        GCL_Write_VehSpdGauge_VehSpdGaugeValue(u16NeedleMaxPos);
    }
    else
    {
        updateFilterParameter();
        filterData.Xnf = u16needlePos;
        uint16 filterValue = FirstOrderLagfilterPosNegLoRes16(&filterData);
        UNUSED(filterValue);

        if (filterData.u16RetValue != previousGaugeValue)
        {
            previousGaugeValue = filterData.u16RetValue;
            GCL_Write_VehSpdGauge_VehSpdGaugeValue(filterData.u16RetValue);
        }
    }
}

static void ResetFilter(void)
{
    filterData.Xnf = 0U;
    filterData.Ynf = 0U;
}

static void updateFilterParameter(void)
{
    uint8 data =0u;
    
    if(CAL_Read8(VEHSPD_FILTERC, &data) == CAL_NOK)
    {
        Logger_Info(COMPONENTNAME_VSG, "Can't read CAL PARAMETER: VEHSPD_FILTERC");
    }    
    
    //use same values for both positive and negative movement
    filterData.cPos =  data;
    filterData.cNeg =  data;
    
}

static void InitializeFilter(void)
{
    updateFilterParameter();
    ResetFilter();
}

//lint -restore
