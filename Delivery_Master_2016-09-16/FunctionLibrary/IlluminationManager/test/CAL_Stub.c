#include "CAL_Stub.h"
#include "check.h"

#define ILLUMINATION_LEVEL_COUNT     5


const uint8 DisplayIlluminationLevel1 = 1u;
const uint8 DisplayIlluminationLevel2 = 7u;
const uint8 DisplayIlluminationLevel3 = 14u;
const uint8 DisplayIlluminationLevel4 = 20u;
const uint8 DisplayIlluminationLevel5 = 30u;

const uint8 GaugeIlluminationLevel1 = 7u;
const uint8 GaugeIlluminationLevel2 = 14u;
const uint8 GaugeIlluminationLevel3 = 20u;
const uint8 GaugeIlluminationLevel4 = 40u;
const uint8 GaugeIlluminationLevel5 = 52u;

static uint8 DispIllumLevel[ILLUMINATION_LEVEL_COUNT];
static uint8 GaugeIllumLevel[ILLUMINATION_LEVEL_COUNT];
static uint8 IlluminationLevel;
static uint8 calWriteCounter;

void CALStub_Init()
{
    DispIllumLevel[0] = DisplayIlluminationLevel1;
    DispIllumLevel[1] = DisplayIlluminationLevel2;
    DispIllumLevel[2] = DisplayIlluminationLevel3;
    DispIllumLevel[3] = DisplayIlluminationLevel4;
    DispIllumLevel[4] = DisplayIlluminationLevel5;
    
    GaugeIllumLevel[0] = GaugeIlluminationLevel1;
    GaugeIllumLevel[1] = GaugeIlluminationLevel2;
    GaugeIllumLevel[2] = GaugeIlluminationLevel3;
    GaugeIllumLevel[3] = GaugeIlluminationLevel4;
    GaugeIllumLevel[4] = GaugeIlluminationLevel5;
    
    IlluminationLevel = 0;
    calWriteCounter = 0;
}

CAL_STATUS CAL_Read(calibration_parameter_id_t u16ParameterId, uint8* value, uint16 expectedParameterLength)
{
    switch (u16ParameterId)
    {
        case DISP_ILLUM:
            for (int i=0; i<expectedParameterLength; ++i)
            {
                value[i] = DispIllumLevel[i];
            }
            return CAL_OK;
        case GAUGE_ILLUM:
            for (int i=0; i<expectedParameterLength; ++i)
            {
                value[i] = GaugeIllumLevel[i];
            }
        
            return CAL_OK;
            
        case ILLUMINATION_LEVEL:
            *value = IlluminationLevel;
            return CAL_OK;
        default:
            ck_abort_msg("unknown calibration parameter ID (%d) in CAL_Read call!", u16ParameterId);
            return CAL_NOK;
    }
}

CAL_STATUS CAL_Write(calibration_parameter_id_t u16ParameterId, uint8* value, uint16 u16Length)
{
    ++calWriteCounter;
    switch (u16ParameterId)
    {
        case ILLUMINATION_LEVEL:
            IlluminationLevel = *value;
            return CAL_OK;
        default:
            ck_abort_msg("unknown calibration parameter ID (%d) in CAL_Write call!", u16ParameterId);
            return CAL_NOK;
    }
}

void CALStub_SetDisplayIlluminationLevels(uint8 level1, uint8 level2, uint8 level3, uint8 level4, uint8 level5)
{
    DispIllumLevel[0] = level1;
    DispIllumLevel[1] = level2;
    DispIllumLevel[2] = level3;
    DispIllumLevel[3] = level4;
    DispIllumLevel[4] = level5;
}

void CALStub_SetGaugeIlluminationLevels(uint8 level1, uint8 level2, uint8 level3, uint8 level4, uint8 level5)
{
    GaugeIllumLevel[0] = level1;
    GaugeIllumLevel[1] = level1;
    GaugeIllumLevel[2] = level1;
    GaugeIllumLevel[3] = level1;
    GaugeIllumLevel[4] = level1;
}

void CALStub_SetIlluminationLevel(uint8 level)
{
    IlluminationLevel = level;
}

uint8 CALStub_GetIlluminationLevel(void)
{
    return IlluminationLevel;
}

uint8 CALStub_GetCalWriteCount()
{
    return calWriteCounter;
}