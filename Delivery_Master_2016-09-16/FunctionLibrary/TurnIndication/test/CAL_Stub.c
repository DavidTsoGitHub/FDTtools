#include "CAL.h"
#include "check.h"

static uint8 soundDelay;

void CAL_Init()
{
    soundDelay = 0;
}

CAL_STATUS CAL_Read8(calibration_parameter_id_t u16ParameterId, uint8* value)
{
    switch (u16ParameterId)
    {
        case TURNINDICATOR_TELLTALEDELAY:
            *value = soundDelay;
            return CAL_OK;
        default:
            *value = 0xFF;
            ck_abort_msg("unknown calibration parameter ID (%d) in CAL_Read8 call!", u16ParameterId);
            return CAL_NOK;
    }
}

void CALStub_SetTelltaleDelay(uint8 delay)
{
    soundDelay = delay;
}