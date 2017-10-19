/********************************* Includes **********************************/
#include "IconWidget.h"
#include "IconWidgetCfg.h"
#include "IconRenderer.h"
#include "IconManager.h"
#include "ContentsManager.h"

/**************************** Constants and Types ****************************/
const ui_widget_funcs IconWidgetFuncs =
{
    .draw = Icon_Draw,
    .needsRedraw = NULL,
    .redraw = Icon_Draw,
    .handleNext = NULL,
    .handlePrevious = NULL,
    .handleEnter = NULL,
    .handleLeave = NULL,
    .offerFocus = NULL,
    .forceFocusRelease = NULL,
    .focusFinished = NULL
};

/********************************* Variables *********************************/

/************************* Local Function Prototypes *************************/
static const IconCfg_t* getIconConfiguration(uint32 widgetID);

/***************************** Exported Functions ****************************/
void Icon_Draw(const display_area_definition_t* area, const ui_widget_t* uiWidget)
{
    const IconCfg_t* config = getIconConfiguration(uiWidget->widgetID);
    if (config != NULL)
    {
        const icon_t* icon = IconManager_GetIcon(config->iconID);

        if(icon != NULL)
        {
            if (uiWidget->boxed == TRUE)
            {
                ContentsManager_DrawBoxWithRoundCorners(area, uiWidget->x, uiWidget->y, uiWidget->width, uiWidget->height, FALSE, FALSE, uiWidget->fgColor, uiWidget->bgColor);
            }

            IconRenderer_RenderIcon(area, icon, uiWidget->x, uiWidget->y, uiWidget->width, uiWidget->height, FALSE, uiWidget->fgColor, uiWidget->bgColor);
        }
    }
}

/****************************** Local Functions  *****************************/
static const IconCfg_t* getIconConfiguration(uint32 widgetID)
{
    const IconCfg_t* returnValue = NULL;

    for (uint32 i=0; i<IconConfigCount; ++i)
    {
        if (IconConfigs[i].widgetID == widgetID)
        {
            returnValue = &IconConfigs[i];
            break;
        }
    }

    return returnValue;
}
