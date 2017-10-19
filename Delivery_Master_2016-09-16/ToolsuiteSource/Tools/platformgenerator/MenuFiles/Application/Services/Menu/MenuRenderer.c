/********************************* Includes **********************************/
#include "MenuRenderer.h"
#include "Menu_cfg.h"
#include "ContentsManager.h"
#include "ContentsManager_cfg.h"
#include "Logger.h"

/**************************** Constants and Types ****************************/

/********************************* Variables *********************************/

/************************* Local Function Prototypes *************************/

/***************************** Exported Functions ****************************/
void MenuRenderer_DrawMenuItem(const menuItem_t* menuItem)
{
    if (menuItem != NULLMENUITEM)
    {
        ContentsManager_ClearArea(ContentsManager_MenuArea);

        for (uint8 i = 0; i < menuItem->uiWidgetCount; ++i)
        {
            const ui_widget_t* uiWidget = &menuItem->uiWidgets[i];
            UIWidget_Draw(ContentsManager_MenuArea, uiWidget);
        }
        ContentsManager_SetLayerDirty(ContentsManager_MenuArea->layerID);
    }
    else
    {
        Logger_Info("MenuRenderer", "Attempted to draw a NULL menu item!");
    }
}

void MenuRenderer_RefreshMenuItem(const menuItem_t* menuItem)
{
    if (menuItem != NULLMENUITEM)
    {
        for (uint8 i = 0; i < menuItem->uiWidgetCount; ++i)
        {
            const ui_widget_t* uiWidget = &menuItem->uiWidgets[i];
            if (UIWidget_NeedsRedraw(uiWidget) == TRUE)
            {
                MenuRenderer_DrawMenuItem(menuItem);
                break;
            }
        }
    }
    else
    {
        Logger_Info("MenuRenderer", "Attempted to draw a NULL menu item!");
    }
}

/****************************** Local Functions  *****************************/
