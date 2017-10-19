#ifndef QUADSWITCHMANAGER_H
#define QUADSWITCHMANAGER_H

/********************************* Includes **********************************/
#include "StandardTypes.h"

/**************************** Constants and Types ****************************/
#define BUTTON_INPUT_ON             1U
#define BUTTON_INPUT_OFF            0U
#define LONG_PRESS_TIME             2000U
#define EXTRA_LONG_PRESS_TIME       5000U

typedef enum
{
	BUTTON_RELEASE,
	BUTTON_PRESSED,
	BUTTON_LONG_PRESSED,
	BUTTON_EXTRALONG_PRESSED
} buttonState_t;

/***************************** Exported Functions ****************************/
void QuadSwitchManager_Init(void);
void QuadSwitchManager_Run(void);

#endif //QUADSWITCHMANAGER_H
