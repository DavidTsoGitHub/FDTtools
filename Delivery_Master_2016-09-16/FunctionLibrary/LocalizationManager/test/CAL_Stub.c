#include "CAL.h"
#include "check.h"

static uint8 languageValue;
static uint8 calWriteCounter = 0;

void CALStub_Reset()
{
    languageValue = 0;
    calWriteCounter = 0;
}

CAL_STATUS CAL_Read8(calibration_parameter_id_t u16ParameterId, uint8* value)
{
    switch (u16ParameterId)
    {
        case INITIAL_LANGUAGE_VALUE:
            *value = languageValue;
            return CAL_OK;
        default:
            ck_abort_msg("unknown calibration parameter ID (%d) in CAL_Read8 call!", u16ParameterId);
            return CAL_NOK;
    }
}

CAL_STATUS CAL_Write(calibration_parameter_id_t u16ParameterId, uint8* value, uint16 length)
{
    ++calWriteCounter;
    switch (u16ParameterId)
    {
        case INITIAL_LANGUAGE_VALUE:
            languageValue = *value;
            return CAL_OK;
        default:
            ck_abort_msg("unknown calibration parameter ID (%d) in CAL_Write call!", u16ParameterId);
            return CAL_NOK;
    }
}

uint8 CALStub_GetLanguageValue(void)
{
    return languageValue;
}

void CALStub_SetLanguageValue(uint8 value)
{
    languageValue = value;
}

uint8 CALStub_GetCalWriteCounter(void)
{
    return calWriteCounter;
}