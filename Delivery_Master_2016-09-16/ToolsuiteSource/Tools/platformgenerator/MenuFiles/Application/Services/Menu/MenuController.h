#ifndef MENU_CONTROLLER_H
#define MENU_CONTROLLER_H

#include "StandardTypes.h"

/********************************* Includes **********************************/

/**************************** Constants and Types ****************************/

/***************************** Exported Functions ****************************/
void MenuController_Init(void);
void MenuController_Run(void);
void MenuController_ForceWidgetFocusRelease(void);
boolean MenuController_AtStartMenuItem(void);

#endif
