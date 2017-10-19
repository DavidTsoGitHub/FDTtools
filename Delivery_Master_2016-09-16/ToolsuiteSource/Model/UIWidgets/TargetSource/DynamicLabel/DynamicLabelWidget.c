/********************************* Includes **********************************/
#include "DynamicLabelWidget.h"
#include "DynamicLabelWidgetCfg.h"
#include "ContentsManager.h"
#include "Logger.h"

/**************************** Constants and Types ****************************/
const ui_widget_funcs DynamicLabelWidgetFuncs =
{
    .draw = DynamicLabelWidget_Draw,
    .needsRedraw = DynamicLabelWidget_NeedRedraw,
    .redraw = DynamicLabelWidget_Draw,
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
static DynamicLabelCfg_t* getConfig(uint32 widgetID);
static void readCurrentValue(DynamicLabelCfg_t* config);
static void drawWidget(const display_area_definition_t* area, const ui_widget_t* uiWidget, const DynamicLabelCfg_t* config);

/***************************** Exported Functions ****************************/
void DynamicLabelWidget_Draw(const display_area_definition_t* area, const ui_widget_t* uiWidget)
{
    Logger_Info("DynamicLabelWidget", "DynamicLabelWidget_Draw");

    DynamicLabelCfg_t* config = getConfig(uiWidget->widgetID);
    if (config == NULL)
    {
        Logger_Error("DynamicLabelWidget", "Configuration for widget ID:%d not found!", uiWidget->widgetID);

    }
    else
    {
        readCurrentValue(config);
        drawWidget(area, uiWidget, config);
    }
}

boolean DynamicLabelWidget_NeedRedraw(const ui_widget_t* uiWidget)
{
    boolean changed = FALSE;
    uint32 oldLabelID;

    DynamicLabelCfg_t* config = getConfig(uiWidget->widgetID);
    if (config == NULL)
    {
        Logger_Error("DynamicLabelWidget", "Configuration for widget ID:%d not found!", uiWidget->widgetID);
    }
    else
    {
        oldLabelID = config->currentLabelID;
        readCurrentValue(config);

        if (oldLabelID != config->currentLabelID)
        {
            changed = TRUE;
        }
    }

    return changed;
}

/****************************** Local Functions  *****************************/

static DynamicLabelCfg_t* getConfig(uint32 widgetID)
{
    DynamicLabelCfg_t* returnValue = NULL;

    for (uint32 i=0; i<DynamicLabelConfigCount; ++i)
    {
        if (DynamicLabelConfigs[i].widgetID == widgetID)
        {
            returnValue = &DynamicLabelConfigs[i];
            break;
        }
    }

    return returnValue;
}

static void readCurrentValue(DynamicLabelCfg_t* config)
{
    uint8 status;
    uint32 labelID;
    config->readLabelID(&labelID, &status);

    config->currentLabelID = labelID;
}

static void drawWidget(const display_area_definition_t* area, const ui_widget_t* uiWidget, const DynamicLabelCfg_t* config)
{
    ContentsManager_ClearRectangle(area, uiWidget->x, uiWidget->y, uiWidget->width, uiWidget->height, uiWidget->bgColor);

    if (uiWidget->boxed == TRUE)
    {
        ContentsManager_DrawBoxWithRoundCorners(area, uiWidget->x, uiWidget->y, uiWidget->width, uiWidget->height, FALSE, FALSE, uiWidget->fgColor, uiWidget->bgColor);
    }

    if (config->currentLabelID > 0u)
    {
        if (config->alignment == TEXT_CENTER_ALIGNED)
        {
            TextRenderer_RenderLabelCenterAligned(area, config->currentLabelID, uiWidget->x, uiWidget->y, uiWidget->width, uiWidget->height, uiWidget->fgColor, uiWidget->bgColor);
        }
        else if (config->alignment == TEXT_RIGHT_ALIGNED)
        {
            TextRenderer_RenderLabelRightAligned(area, config->currentLabelID, uiWidget->x, uiWidget->y, uiWidget->width, uiWidget->height, uiWidget->fgColor, uiWidget->bgColor);
        }
        else
        {
            TextRenderer_RenderLabelLeftAligned(area, config->currentLabelID, uiWidget->x, uiWidget->y, uiWidget->width, uiWidget->height, uiWidget->fgColor, uiWidget->bgColor);
        }
    }
}
