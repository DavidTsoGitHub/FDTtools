#include "CAL.h"

static uint32 odoStatus;
static uint32 odoKmRestPulses;
static uint32 odoHmRestPulsesTripA;
static uint32 odoHmRestPulsesTripB;
static uint32 tripAValue;
static uint32 tripBValue;
static uint32 odoPulsesPerKm;
static uint32 odoValue1;
static uint32 odoValue2;
static uint32 odoValue3;
static uint32 odoValue4;
static uint32 odoValue5;

void CAL_Init(void)
{
    odoStatus = 0;
    odoKmRestPulses = 0;
    odoHmRestPulsesTripA = 0;
    odoHmRestPulsesTripB = 0;
    tripAValue = 0;
    tripBValue = 0;    
    odoPulsesPerKm = 7997;
    odoValue1 = 0;
    odoValue2 = 0;
    odoValue3 = 0;
    odoValue4 = 0;
    odoValue5 = 0;
}

void CAL_Refresh(void)
{}

CAL_STATUS CAL_Read8(calibration_parameter_id_t u16ParameterId, uint8* value)
{
    return CAL_NOK;
}

CAL_STATUS CAL_Read16(calibration_parameter_id_t u16ParameterId, uint16* value)
{
    return CAL_NOK;
}

CAL_STATUS CAL_Read32(calibration_parameter_id_t u16ParameterId, uint32* value)
{
    switch (u16ParameterId)
    {
        case TRIP_A_VALUE:
        {
            *value = tripAValue;
        }
        break;
        case TRIP_B_VALUE:
        {
            *value = tripBValue;
        }
        break;
        case ODO_HM_REST_PULSES_TRIP_A:
        {
            *value = odoHmRestPulsesTripA;
        }
        break;
        case ODO_HM_REST_PULSES_TRIP_B:
        {
            *value = odoHmRestPulsesTripB;
        }
        break;
        case ODO_PULSES_PER_KM:
        {
            *value = odoPulsesPerKm;
        }
        break;
        default:
        {
            *value = 0;
            return CAL_NOK;
        }
    }
    
    return CAL_OK;
}

//Arbitrary length intefaces
CAL_STATUS CAL_Write(calibration_parameter_id_t CalId, uint8* pData, uint16 u16Length)
{
    switch (CalId)
    {
        case TRIP_A_VALUE:
        {
            tripAValue = *(uint32*)pData;
        }
        break;
        case TRIP_B_VALUE:
        {
            tripBValue = *(uint32*)pData;
        }
        break;
        case ODO_HM_REST_PULSES_TRIP_A:
        {
            odoHmRestPulsesTripA = *(uint32*)pData;
        }
        break;
        case ODO_HM_REST_PULSES_TRIP_B:
        {
            odoHmRestPulsesTripB = *(uint32*)pData;
        }
        break;
        default:
        {
            return CAL_NOK;
        }
    }
    
    return CAL_OK;
}

CAL_STATUS CAL_Read(calibration_parameter_id_t CalId, uint8* pData, uint16 u16Length)
{
    return CAL_NOK;
}

//ODO storage interfaces
CAL_STATUS CAL_NoCacheWrite32(calibration_parameter_id_t u16ParameterId,const uint32* value)
{
    switch (u16ParameterId)
    {
        case ODO_STATUS:
        {
            odoStatus = *value;
        }
        break;
        case ODO_VALUE1:
        {
            odoValue1 = *value;
        }
        break;
        case ODO_VALUE2:
        {
            odoValue2 = *value;
        }
        break;
        case ODO_VALUE3:
        {
            odoValue3 = *value;
        }
        break;
        case ODO_VALUE4:
        {
            odoValue4 = *value;
        }
        break;
        case ODO_VALUE5:
        {
            odoValue5 = *value;
        }
        break;
        case ODO_KM_REST_PULSES:
        {
            odoKmRestPulses = *value;
        }
        break;
        default:
        {
            return CAL_NOK;
        }
    }
    
    return CAL_OK;
}

CAL_STATUS CAL_NoCacheRead32(calibration_parameter_id_t u16ParameterId, uint32* value)
{
    switch (u16ParameterId)
    {
        case ODO_STATUS:
        {
            *value = odoStatus;
        }
        break;        
        case ODO_VALUE1:
        {
            *value = odoValue1;
        }
        break;
        case ODO_VALUE2:
        {
            *value = odoValue2;
        }
        break;
        case ODO_VALUE3:
        {
            *value = odoValue3;
        }
        break;
        case ODO_VALUE4:
        {
            *value = odoValue4;
        }
        break;
        case ODO_VALUE5:
        {
            *value = odoValue5;
        }
        break;
        case ODO_KM_REST_PULSES:
        {
            *value = odoKmRestPulses;
        }
        break;        
        default:
        {
            *value = 0;
            return CAL_NOK;
        }
    }
    
    return CAL_OK;
}

uint32 CALStub_GetPulsesPerKm(void)
{
    return odoPulsesPerKm;
}

void CALStub_SetCurrentOdo(uint16 parameterId, uint32 value)
{
    CAL_NoCacheWrite32(parameterId, &value);
}

void CALStub_GetCurrentOdo(uint16 parameterId, uint32* value)
{
    CAL_NoCacheRead32(parameterId, value);
}

void CALStub_SetTripValue(uint16 parameterId, uint32 value)
{
    CAL_Write(parameterId, (uint8*)&value, 4);
}

uint32 CALStub_GetODORestPulseValue()
{
    return odoKmRestPulses;
}

uint32 CALStub_GetTripARestPulseValue()
{
    return odoHmRestPulsesTripA;
}

uint32 CALStub_GetTripBRestPulseValue()
{
    return odoHmRestPulsesTripB;
}