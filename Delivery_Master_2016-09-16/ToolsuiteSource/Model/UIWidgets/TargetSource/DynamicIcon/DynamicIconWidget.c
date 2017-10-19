/********************************* Includes **********************************/
#include "DynamicIconWidget.h"
#include "DynamicIconWidgetCfg.h"
#include "ContentsManager.h"
#include "IconRenderer.h"
#include "IconManager.h"
#include "Logger.h"

/**************************** Constants and Types ****************************/
const ui_widget_funcs DynamicIconWidgetFuncs =
{
    .draw = DynamicIconWidget_Draw,
    .needsRedraw = DynamicIconWidget_NeedsRedraw,
    .redraw = DynamicIconWidget_Draw,
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
static DynamicIconCfg_t* getConfig(uint32 widgetID);
static void readCurrentValue(DynamicIconCfg_t* config);
static void drawWidget(const display_area_definition_t* area, const ui_widget_t* uiWidget, const DynamicIconCfg_t* config);

/***************************** Exported Functions ****************************/
void DynamicIconWidget_Draw(const display_area_definition_t* area, const ui_widget_t* uiWidget)
{
    Logger_Info("DynamicIconWidget", "DynamicIconWidget_Draw");

    DynamicIconCfg_t* config = getConfig(uiWidget->widgetID);
    if (config == NULL)
    {
        Logger_Error("DynamicIconWidget", "Configuration for widget ID:%d not found!", uiWidget->widgetID);

    }
    else
    {
        readCurrentValue(config);
        drawWidget(area, uiWidget, config);
    }
}

boolean DynamicIconWidget_NeedsRedraw(const ui_widget_t* uiWidget)
{
    boolean changed = FALSE;
    uint32 oldIconID;

    DynamicIconCfg_t* config = getConfig(uiWidget->widgetID);
    if (config == NULL)
    {
        Logger_Error("DynamicIconWidget", "Configuration for widget ID:%d not found!", uiWidget->widgetID);
    }
    else
    {
        oldIconID = config->currentIconID;
        readCurrentValue(config);

        if (oldIconID != config->currentIconID)
        {
            changed = TRUE;
        }
    }

    return changed;
}

/****************************** Local Functions  *****************************/

static DynamicIconCfg_t* getConfig(uint32 widgetID)
{
    DynamicIconCfg_t* returnValue = NULL;

    for (uint32 i=0; i<DynamicIconConfigCount; ++i)
    {
        if (DynamicIconConfigs[i].widgetID == widgetID)
        {
            returnValue = &DynamicIconConfigs[i];
            break;
        }
    }

    return returnValue;
}

static void readCurrentValue(DynamicIconCfg_t* config)
{
    uint8 status;
    uint32 iconID;
    config->readIconID(&iconID, &status);

    config->currentIconID = iconID;
}

static void drawWidget(const display_area_definition_t* area, const ui_widget_t* uiWidget, const DynamicIconCfg_t* config)
{
    ContentsManager_ClearRectangle(area, uiWidget->x, uiWidget->y, uiWidget->width, uiWidget->height, uiWidget->bgColor);

    if (uiWidget->boxed == TRUE)
    {
        ContentsManager_DrawBoxWithRoundCorners(area, uiWidget->x, uiWidget->y, uiWidget->width, uiWidget->height, FALSE, FALSE, uiWidget->fgColor, uiWidget->bgColor);
    }

    if (config->currentIconID > 0u)
    {
        const icon_t* icon = IconManager_GetIcon(config->currentIconID);
        if(icon != NULL)
        {
            IconRenderer_RenderIcon(area, icon, uiWidget->x, uiWidget->y, uiWidget->width, uiWidget->height, FALSE, uiWidget->fgColor, uiWidget->bgColor);
        }
    }
}
