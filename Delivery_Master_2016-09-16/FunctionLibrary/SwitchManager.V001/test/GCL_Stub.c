#include "GCL_SwitchManager.h"

// Define variables to hold stubbed GCL signals
static uint8 ButtonRaw = 0;

static uint8 ButtonEvent_Up = 0;
static uint8 ButtonEvent_Down = 0;
static uint8 ButtonEvent_Left = 0;
static uint8 ButtonEvent_Right = 0;
static uint8 ButtonEvent_Set = 0;
static uint8 ButtonEvent_Clear = 0;

void GCLStub_Reset()
{
    ButtonRaw = 0;
    ButtonEvent_Up = 0;
    ButtonEvent_Down = 0;
    ButtonEvent_Left = 0;
    ButtonEvent_Right = 0;
    ButtonEvent_Set = 0;
    ButtonEvent_Clear = 0;
}

void GCLStub_SetButtonRaw(uint8 buttonRaw)
{
    ButtonRaw = buttonRaw;
}

uint8 GCLStub_GetButtonEventUp(void)
{
    return ButtonEvent_Up;
}

uint8 GCLStub_GetButtonEventDown(void)
{
    return ButtonEvent_Down;
}

uint8 GCLStub_GetButtonEventLeft(void)
{
    return ButtonEvent_Left;
}

uint8 GCLStub_GetButtonEventRight(void)
{
    return ButtonEvent_Right;
}

uint8 GCLStub_GetButtonEventSet(void)
{
    return ButtonEvent_Set;
}

uint8 GCLStub_GetButtonEventClear(void)
{
    return ButtonEvent_Clear;
}

void GCL_Read_SwitchManager_ButtonRaw(uint8* value, uint8* status)
{
    *value = ButtonRaw;
    *status = GCL_SIGNAL_OK;
}

void GCL_Write_SwitchManager_ButtonEvent_Up(uint8 value)
{
    ButtonEvent_Up = value;
}

void GCL_Write_SwitchManager_ButtonEvent_Down(uint8 value)
{
    ButtonEvent_Down = value;
}

void GCL_Write_SwitchManager_ButtonEvent_Left(uint8 value)
{
    ButtonEvent_Left = value;
}

void GCL_Write_SwitchManager_ButtonEvent_Right(uint8 value)
{
    ButtonEvent_Right = value;
}

void GCL_Write_SwitchManager_ButtonEvent_Set(uint8 value)
{
    ButtonEvent_Set = value;
}

void GCL_Write_SwitchManager_ButtonEvent_Clear(uint8 value)
{
    ButtonEvent_Clear = value;
}


