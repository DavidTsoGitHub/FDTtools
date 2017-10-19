#ifndef MENUMANAGER_TYPES_H
#define MENUMANAGER_TYPES_H

/********************************* Includes **********************************/
#include "StandardTypes.h"
#include "UIWidget.h"

/**************************** Constants and Types ****************************/
#define NULLMENUITEM (menuItem_t*)0

struct menuItem_t;

typedef struct menuItem_t
{
    const uint32 labelID;
    const uint32 imageID;
    const struct menuItem_t* const actionNext;
    const struct menuItem_t* const actionPrevious;
    const struct menuItem_t* const actionEnter;
    const struct menuItem_t* const actionLeave;
    const ui_widget_t* uiWidgets;
    const uint8 menuItemIndex;
    const uint8 uiWidgetCount;
} menuItem_t;

/***************************** Exported Functions ****************************/

#endif // MENUMANAGER_TYPES_H
