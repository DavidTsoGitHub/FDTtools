#include "CAL.h"
#include "check.h"

static uint16 pulsesPerRotation;
static uint8 nominator;
static uint8 denominator;

void CAL_Init()
{
    pulsesPerRotation = 0;
    nominator = 0;
    denominator = 0;
}

CAL_STATUS CAL_Read8(calibration_parameter_id_t u16ParameterId, uint8* value)
{
    switch (u16ParameterId)
    {
        case RATIO_NUMERATOR:
            *value = nominator;
            return CAL_OK;
        case RATIO_DENOMINATOR:
            *value = denominator;
            return CAL_OK;
        default:
            *value = 0xFF;
            ck_abort_msg("unknown calibration parameter ID (%d) in CAL_Read8 call!", u16ParameterId);
            return CAL_NOK;
    }
}


CAL_STATUS CAL_Read16(calibration_parameter_id_t u16ParameterId, uint16* value)
{
    switch (u16ParameterId)
    {
        case PULSES_PER_ROTATION:
            *value = pulsesPerRotation;
            return CAL_OK;
        default:
            *value = 0xFF;
            ck_abort_msg("unknown calibration parameter ID (%d) in CAL_Read16 call!", u16ParameterId);
            return CAL_NOK;
    }
}




void CALStub_Write8(calibration_parameter_id_t CalId, uint8 pData)
{
    switch (CalId)
    {
        case RATIO_NUMERATOR:
            nominator = pData;
            break;        
        case RATIO_DENOMINATOR:
            denominator = pData;
            break;        
        default:
            ck_abort_msg("unknown calibration parameter ID (%d) in CAL_Write8 call!", CalId);
            break;        
    }
}


void CALStub_Write16(calibration_parameter_id_t CalId, uint16 pData)
{
    switch (CalId)
    {
        case PULSES_PER_ROTATION:
            pulsesPerRotation = pData;
            break;        

        default:
            ck_abort_msg("unknown calibration parameter ID (%d) in CAL_Write8 call!", CalId);
            break;        
    }
}



