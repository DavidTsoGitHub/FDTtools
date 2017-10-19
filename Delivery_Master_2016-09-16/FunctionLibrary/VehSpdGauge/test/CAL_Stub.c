#include "CAL.h"

CAL_STATUS CAL_Read8(calibration_parameter_id_t u16ParameterId, uint8* value)
{
    switch (u16ParameterId)
    {
        case VEHSPD_MAXSPEEDVALUE:
            *value = 140;
            return CAL_OK;
        case VEHSPD_FILTERC:
        *value = 0xC8;
        return CAL_OK;
        default:
            *value = 0;
            return CAL_NOK;
    }
 }

CAL_STATUS CAL_Read16(calibration_parameter_id_t u16ParameterId, uint16* value)
{
    return CAL_NOK;
}

CAL_STATUS CAL_Read32(calibration_parameter_id_t u16ParameterId, uint32* value)
{
    switch (u16ParameterId)
    {
        case VEHSPD_MAXINPUTFREQUENCY:
            *value = 79616;
            return CAL_OK;
        default:
            *value = 0;
            return CAL_NOK;
    }
}