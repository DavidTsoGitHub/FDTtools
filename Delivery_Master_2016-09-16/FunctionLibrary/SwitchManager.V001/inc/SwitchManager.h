#ifndef SWITCH_MANAGER_H
#define SWITCH_MANAGER_H

/********************************* Includes **********************************/

#define LONG_PRESS_TIME           2000
#define EXTRA_LONG_PRESS_TIME     5000

/**************************** Constants and Types ****************************/
typedef enum
{
	INACTIVE,
    UP,
    DOWN,
    LEFT,
	RIGHT,
	SET,
	CLEAR
} inputState_t;

typedef enum
{
	BUTTON_RELEASE,
	BUTTON_PRESSED,
	BUTTON_LONG_PRESSED,
	BUTTON_EXTRALONG_PRESSED
} buttonState_t;

/***************************** Exported Functions ****************************/
void SwitchManager_Init(void);
void SwitchManager_Run(void);

#endif //SWITCH_MANAGER_H
