#ifndef LISTNAVIGATIONWIDGETCFG_H
#define LISTNAVIGATIONWIDGETCFG_H

/********************************* Includes **********************************/
#include "StandardTypes.h"
#include "MenuTypes.h"

/***************************** Exported Types ****************************/
typedef struct
{
    uint32 widgetID;
    const menuItem_t* lastDrawnMenuItem;
    uint8 firstIndexInView;
    uint8 displayItemCount;
    uint8 selectedItemFgColor;
    uint8 selectedItemBgColor;
} ListNavigationCfg_t;

/********************************* Variables *********************************/
extern const uint32 ListNavigationConfigCount;
extern ListNavigationCfg_t* const ListNavigationConfigs;

/***************************** Exported Functions ****************************/

#endif // LISTNAVIGATIONWIDGETCFG_H
