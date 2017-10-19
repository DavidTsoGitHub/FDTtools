#include "CAL.h"
#include "check.h"

static uint8 threshold;
static uint8 tolerance;

void CAL_Init()
{
    threshold = 12;
    tolerance = 5;
}

CAL_STATUS CAL_Read8(calibration_parameter_id_t u16ParameterId, uint8* value)
{
    switch (u16ParameterId)
    {
        case FUEL_WARNINGTHRESHOLD:
            *value = threshold;
            return CAL_OK;
        case FUEL_WARNINGTOLERANCE:
            *value = tolerance;
            return CAL_OK;
        default:
            *value = 0xFF;
            ck_abort_msg("unknown calibration parameter ID (%d) in CAL_Read8 call!", u16ParameterId);
            return CAL_NOK;
    }
}

void CALStub_SetThreshold(uint8 value)
{
    threshold = value;
}

void CALStub_SetTolerance(uint8 value)
{
    tolerance = value;
}