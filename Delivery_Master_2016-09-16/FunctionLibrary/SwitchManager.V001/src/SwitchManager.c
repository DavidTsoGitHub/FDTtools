/********************************* Includes **********************************/
#ifdef _WIN32
#ifndef CI_BUILD
#define ENABLE_PRINTOUT
#endif // CI_BUILD
#endif // _WIN32

#include "../inc/SwitchManager.h"
#include "systime.h"
#include "GCL_SwitchManager.h"
#include "Platform_Types.h"
#include "Logger.h"

#define COMPONENTNAME "SwitchManager"

/********************************* Variables *********************************/
static inputState_t pressedButton;
static buttonState_t pressedButtonState;
static uint32 eventTime;

/************************* Local Function Prototypes *************************/
void sendButtonEvent(void);

/***************************** Exported Functions ****************************/
void SwitchManager_Init()
{
#ifdef ENABLE_PRINTOUT
    Logger_Info(COMPONENTNAME, "SwitchManager_Init");
#endif
	pressedButton = INACTIVE;
	pressedButtonState = BUTTON_RELEASE;
	eventTime = 0;
}

void SwitchManager_Run()
{
	static uint8 rawButtonValue, rawButtonStatus;
	GCL_Read_SwitchManager_ButtonRaw(&rawButtonValue, &rawButtonStatus);

	//If read state is not INACTIVE and previous state was released
	if((rawButtonValue > ((uint8) INACTIVE)) && (pressedButtonState == BUTTON_RELEASE))
	{
		pressedButton = (inputState_t)rawButtonValue;
		eventTime = SysTime_GetTime();
		pressedButtonState = BUTTON_PRESSED;
		sendButtonEvent();
	}
	//Or if previous state was something else than INACTIVE, release it
	else if((rawButtonValue == ((uint8) INACTIVE)) && (pressedButton != INACTIVE))
	{
		pressedButtonState = BUTTON_RELEASE;
		sendButtonEvent();
		pressedButton = INACTIVE;
	}

	//If current state is else than RELEASED, check time to see if it is LONG_PRESS etc.
	if(pressedButtonState != BUTTON_RELEASE)
	{
		uint32 timeDelta = SysTime_GetTime() - eventTime;
		if((pressedButtonState == BUTTON_PRESSED) && (timeDelta >= ((uint32) LONG_PRESS_TIME)))
		{
			pressedButtonState = BUTTON_LONG_PRESSED;
			sendButtonEvent();
		}
		else if(pressedButtonState == BUTTON_LONG_PRESSED && timeDelta >= ((uint32) EXTRA_LONG_PRESS_TIME))
		{
			pressedButtonState = BUTTON_EXTRALONG_PRESSED;
			sendButtonEvent();
		}
	}
}

/****************************** Local Functions  *****************************/
void sendButtonEvent()
{
#ifdef ENABLE_PRINTOUT
    Logger_Info(COMPONENTNAME, "Button event! Button:%d State:%d\n", pressedButton, pressedButtonState);
#endif
	switch(pressedButton)
	{
		case UP:
			GCL_Write_SwitchManager_ButtonEvent_Up(pressedButtonState);
			break;
		case DOWN:
			GCL_Write_SwitchManager_ButtonEvent_Down(pressedButtonState);
			break;
		case LEFT:
			GCL_Write_SwitchManager_ButtonEvent_Left(pressedButtonState);
			break;
		case RIGHT:
			GCL_Write_SwitchManager_ButtonEvent_Right(pressedButtonState);
			break;
		case SET:
			GCL_Write_SwitchManager_ButtonEvent_Set(pressedButtonState);
			break;
		case CLEAR:
			GCL_Write_SwitchManager_ButtonEvent_Clear(pressedButtonState);
			break;
		default:
			break;
	}
}
