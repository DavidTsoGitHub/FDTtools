#include "GCL_TurnIndication.h"

static uint8 RightTurnIndication = 0;
static uint8 RightTurnIndicationSignalStatus = GCL_SIGNAL_OK;
static uint8 LeftTurnIndication = 0;
static uint8 LeftTurnIndicationSignalStatus = GCL_SIGNAL_OK;

static uint8 RightTurnTelltaleValue = 0;
static uint8 LeftTurnTelltaleValue = 0;

void GCLStub_Reset()
{
    RightTurnIndication = 0;
    RightTurnIndicationSignalStatus = GCL_SIGNAL_OK;
    LeftTurnIndication = 0;
    LeftTurnIndicationSignalStatus = GCL_SIGNAL_OK;

    RightTurnTelltaleValue = 0;
    LeftTurnTelltaleValue = 0;
}

void GCLStub_SetRightTurnIndication(uint8 value, uint8 status)
{
    RightTurnIndication = value;
    RightTurnIndicationSignalStatus = status;
}

void GCLStub_SetLeftTurnIndication(uint8 value, uint8 status)
{
    LeftTurnIndication = value;
    LeftTurnIndicationSignalStatus = status;
}

uint8 GCLStub_GetRightTurnTelltaleValue(void)
{
    return RightTurnTelltaleValue;
}

uint8 GCLStub_GetLeftTurnTelltaleValue(void)
{
    return LeftTurnTelltaleValue;
}


void GCL_Read_TurnIndication_RightTurnIndication(uint8* value, uint8* status)
{
    *value = RightTurnIndication;
    *status = RightTurnIndicationSignalStatus;
}

void GCL_Read_TurnIndication_LeftTurnIndication(uint8* value, uint8* status)
{
    *value = LeftTurnIndication;
    *status = LeftTurnIndicationSignalStatus;
}

void GCL_Write_TurnIndication_TurnRightTelltaleValue(uint8 value)
{
    RightTurnTelltaleValue = value;
}

void GCL_Write_TurnIndication_TurnLeftTelltaleValue(uint8 value)
{
    LeftTurnTelltaleValue = value;
}
