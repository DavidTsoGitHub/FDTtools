/********************************* Includes **********************************/
#include "QuadSwitchManager.h"
#include "GCL_QuadSwitchManager.h"
#include "StandardTypes.h"
#include "Logger.h"
#include "systime.h"

/**************************** Constants and Types ****************************/
#define BUTTON_COUNT (4U)

typedef struct
{
	uint32 eventTime;
	void (*writeFunc)(uint8 value);
	void (*readFunc)(uint8* value, uint8* status);
	buttonState_t state;
	uint8 inputState;
} button_t;

/********************************* Variables *********************************/
static button_t buttons[BUTTON_COUNT] =
{
	{ .state = BUTTON_RELEASE, .inputState = BUTTON_INPUT_OFF, .eventTime = 0, .readFunc = GCL_Read_QuadSwitchManager_Button1State, .writeFunc = GCL_Write_QuadSwitchManager_Button1Event },
	{ .state = BUTTON_RELEASE, .inputState = BUTTON_INPUT_OFF, .eventTime = 0, .readFunc = GCL_Read_QuadSwitchManager_Button2State, .writeFunc = GCL_Write_QuadSwitchManager_Button2Event },
	{ .state = BUTTON_RELEASE, .inputState = BUTTON_INPUT_OFF, .eventTime = 0, .readFunc = GCL_Read_QuadSwitchManager_Button3State, .writeFunc = GCL_Write_QuadSwitchManager_Button3Event },
	{ .state = BUTTON_RELEASE, .inputState = BUTTON_INPUT_OFF, .eventTime = 0, .readFunc = GCL_Read_QuadSwitchManager_Button4State, .writeFunc = GCL_Write_QuadSwitchManager_Button4Event }
};

static uint8 activeButtonIndex = 0xFFu;


/************************* Local Function Prototypes *************************/
static void handleButtonInputOn(uint8 buttonIndex);
static void handleButtonInputOff(uint8 buttonIndex);
static void sendButtonEvent(uint8 buttonIndex);

/***************************** Exported Functions ****************************/
void QuadSwitchManager_Init()
{
    Logger_Info("QuadSwitchManager", "QuadSwitchManager_Init");

    activeButtonIndex = 0xFF;
    for (uint8 i=0u; i<BUTTON_COUNT; ++i)
    {
    	buttons[i].state = BUTTON_RELEASE;
    	buttons[i].eventTime = 0u;
    }
}

void QuadSwitchManager_Run()
{
	for (uint8 i=0u; i<BUTTON_COUNT; ++i)
	{
		if ((activeButtonIndex != 0xFFu) && (activeButtonIndex != i))
		{
			continue;
		}

		uint8 signalStatus;
		buttons[i].readFunc(&buttons[i].inputState, &signalStatus);
		if (signalStatus == GCL_SIGNAL_OK)
		{
			if (buttons[i].inputState == BUTTON_INPUT_ON)
			{
				handleButtonInputOn(i);
			}
			else
			{
				handleButtonInputOff(i);
			}
		}
	}
}

/****************************** Local Functions  *****************************/
static void handleButtonInputOff(uint8 buttonIndex)
{
	switch (buttons[buttonIndex].state)
	{
		case BUTTON_RELEASE:
		break;
		case BUTTON_PRESSED:
		case BUTTON_LONG_PRESSED:
		case BUTTON_EXTRALONG_PRESSED:
		default:
			activeButtonIndex = 0xFF;
			buttons[buttonIndex].state = BUTTON_RELEASE;
			buttons[buttonIndex].eventTime = 0u;
			sendButtonEvent(buttonIndex);
		break;
	}
}

static void handleButtonInputOn(uint8 buttonIndex)
{
	switch (buttons[buttonIndex].state)
	{
		case BUTTON_RELEASE:
			activeButtonIndex = buttonIndex;
			buttons[buttonIndex].eventTime = SysTime_GetTimeMs();
			buttons[buttonIndex].state = BUTTON_PRESSED;
			sendButtonEvent(buttonIndex);
		break;
		case BUTTON_PRESSED:
			if ((SysTime_GetTimeMs() - buttons[buttonIndex].eventTime) >= LONG_PRESS_TIME)
			{
				buttons[buttonIndex].state = BUTTON_LONG_PRESSED;
				sendButtonEvent(buttonIndex);
			}
		break;
		case BUTTON_LONG_PRESSED:
			if ((SysTime_GetTimeMs() - buttons[buttonIndex].eventTime) >= EXTRA_LONG_PRESS_TIME)
			{
				buttons[buttonIndex].state = BUTTON_EXTRALONG_PRESSED;
				sendButtonEvent(buttonIndex);
			}
		break;
		case BUTTON_EXTRALONG_PRESSED:
        default:
            // Do nothing at the moment
		break;
	}
}

static void sendButtonEvent(uint8 buttonIndex)
{
    Logger_Info("QuadSwitchManager", "Button event! Index: %d State:%d", buttonIndex, buttons[buttonIndex].state);

    buttons[buttonIndex].writeFunc((uint8)buttons[buttonIndex].state);
}
