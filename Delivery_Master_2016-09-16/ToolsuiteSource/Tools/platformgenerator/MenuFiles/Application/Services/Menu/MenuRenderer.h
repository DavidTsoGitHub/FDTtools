#ifndef MENU_RENDERER_H
#define MENU_RENDERER_H

/********************************* Includes **********************************/
#include "MenuTypes.h"

/**************************** Constants and Types ****************************/

/***************************** Exported Functions ****************************/
void MenuRenderer_DrawMenuItem(const menuItem_t* menuItem);
void MenuRenderer_RefreshMenuItem(const menuItem_t* menuItem);

#endif
