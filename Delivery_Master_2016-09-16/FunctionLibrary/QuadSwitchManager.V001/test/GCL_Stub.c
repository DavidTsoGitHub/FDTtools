#include "GCL_QuadSwitchManager.h"
#include "../inc/QuadSwitchManager.h"

// Define variables to hold stubbed GCL signals
static uint8 Button1State = BUTTON_INPUT_OFF;
static uint8 Button2State = BUTTON_INPUT_OFF;
static uint8 Button3State = BUTTON_INPUT_OFF;
static uint8 Button4State = BUTTON_INPUT_OFF;

static uint8 Button1Event = BUTTON_RELEASE;
static uint8 Button2Event = BUTTON_RELEASE;
static uint8 Button3Event = BUTTON_RELEASE;
static uint8 Button4Event = BUTTON_RELEASE;

void GCLStub_Reset()
{
    Button1State = BUTTON_INPUT_OFF;
    Button2State = BUTTON_INPUT_OFF;
    Button3State = BUTTON_INPUT_OFF;
    Button4State = BUTTON_INPUT_OFF;
    Button1Event = BUTTON_RELEASE;
    Button2Event = BUTTON_RELEASE;
    Button3Event = BUTTON_RELEASE;
    Button4Event = BUTTON_RELEASE;
}

void GCLStub_SetButton1State(uint8 buttonState)
{
    Button1State = buttonState;
}

void GCLStub_SetButton2State(uint8 buttonState)
{
    Button2State = buttonState;
}

void GCLStub_SetButton3State(uint8 buttonState)
{
    Button3State = buttonState;
}

void GCLStub_SetButton4State(uint8 buttonState)
{
    Button4State = buttonState;
}

uint8 GCLStub_GetButton1Event(void)
{
    return Button1Event;
}

uint8 GCLStub_GetButton2Event(void)
{
    return Button2Event;
}

uint8 GCLStub_GetButton3Event(void)
{
    return Button3Event;
}

uint8 GCLStub_GetButton4Event(void)
{
    return Button4Event;
}

void GCL_Read_QuadSwitchManager_Button1State(uint8* value, uint8* status)
{
    *value = Button1State;
    *status = GCL_SIGNAL_OK;
}

void GCL_Read_QuadSwitchManager_Button2State(uint8* value, uint8* status)
{
    *value = Button2State;
    *status = GCL_SIGNAL_OK;
}

void GCL_Read_QuadSwitchManager_Button3State(uint8* value, uint8* status)
{
    *value = Button3State;
    *status = GCL_SIGNAL_OK;
}

void GCL_Read_QuadSwitchManager_Button4State(uint8* value, uint8* status)
{
    *value = Button4State;
    *status = GCL_SIGNAL_OK;
}

void GCL_Write_QuadSwitchManager_Button1Event(uint8 value)
{
    Button1Event = value;
}

void GCL_Write_QuadSwitchManager_Button2Event(uint8 value)
{
    Button2Event = value;
}

void GCL_Write_QuadSwitchManager_Button3Event(uint8 value)
{
    Button3Event = value;
}

void GCL_Write_QuadSwitchManager_Button4Event(uint8 value)
{
    Button4Event = value;
}
