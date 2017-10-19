#include "CAL.h"
#include "check.h"

static uint32 TotalFuel;
static uint32 TicksTotalFuel;

void CAL_Init()
{
    TotalFuel = 0;
    TicksTotalFuel = 0;
}

CAL_STATUS CAL_Read32(calibration_parameter_id_t u16ParameterId, uint32* value)
{
    switch (u16ParameterId)
    {
        case TOTAL_FUEL_CONSUMPTION:
            *value = TotalFuel;
            return CAL_OK;
        case TOTAL_FUEL_CONSUMPTION_TICKS:
            *value = TicksTotalFuel;
            return CAL_OK;
        default:
            *value = 0xFF;
            ck_abort_msg("unknown calibration parameter ID (%d) in CAL_Read32 call!", u16ParameterId);
            return CAL_NOK;
    }
}

CAL_STATUS CAL_Write(calibration_parameter_id_t u16ParameterId, uint8* value, uint16 u16Length)
{
    //printf("value == %d\n", *value);
    switch (u16ParameterId)
    {
        case TOTAL_FUEL_CONSUMPTION:
        {
            TotalFuel = *(value++);
            for (int i = 1; i < 4; ++i)
            {
                TotalFuel += *(value++) << (i * 8);
            }
        }
        case TOTAL_FUEL_CONSUMPTION_TICKS:
        {
            TicksTotalFuel = *(value++);
            for (int i = 1; i < 4; ++i)
            {
                TicksTotalFuel += *(value++) << (i * 8);
            }
        }
            return CAL_OK;
        default:
            ck_abort_msg("unknown calibration parameter ID (%d) in CAL_Write call!", u16ParameterId);
            return CAL_NOK;
    }
}

void CALStub_SetTotalFuel(uint32 value)
{
    TotalFuel = value;
}

uint32 CALStub_GetTotalFuel()
{
    return TotalFuel;
}

void CALStub_SetTickValue(uint32 value)
{
    TicksTotalFuel = value;
}

uint32 CALStub_GetTickValue()
{
    return TicksTotalFuel;
}
