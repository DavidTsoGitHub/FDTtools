/********************************* Includes **********************************/
#include "DynamicTextLabelWidget.h"
#include "DynamicTextLabelWidgetCfg.h"
#include "UIWidget.h"
#include "LabelManager.h"
#include "TextRenderer.h"
#include "ContentsManager.h"

/**************************** Constants and Types ****************************/
const ui_widget_funcs DynamicTextLabelWidgetFuncs =
{
    .draw = DynamicTextLabel_Draw,
    .needsRedraw = DynamicTextLabel_NeedsRedraw,
    .redraw = DynamicTextLabel_Draw,
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
static DynamicTextLabelCfg_t* getDynamicTextLabelConfig(uint32 widgetID);
static void renderString(const display_area_definition_t* area, const ui_widget_t* uiWidget, const DynamicTextLabelCfg_t* config);
static void readCurrentValue(DynamicTextLabelCfg_t* config);
static boolean valueUpdated(const wchar* a, const wchar* b);
static void saveLastStringToConfig(DynamicTextLabelCfg_t *config);

/***************************** Exported Functions ****************************/
void DynamicTextLabel_Draw(const display_area_definition_t* area, const ui_widget_t* uiWidget)
{
    DynamicTextLabelCfg_t* labelConfig = getDynamicTextLabelConfig(uiWidget->widgetID);
    if (labelConfig != NULL)
    {
        if (labelConfig->font == NULL)
        {
            labelConfig->font = LabelManager_GetDefaultFont();
        }

        readCurrentValue(labelConfig);
        renderString(area, uiWidget, labelConfig);
        saveLastStringToConfig(labelConfig);
    }
}

boolean DynamicTextLabel_NeedsRedraw(const ui_widget_t* uiWidget)
{
    boolean changed = FALSE;
    DynamicTextLabelCfg_t* labelConfig = getDynamicTextLabelConfig(uiWidget->widgetID);
    if (labelConfig != NULL)
    {
        if (valueUpdated(labelConfig->lastString, labelConfig->currentString) == TRUE)
        {
            saveLastStringToConfig(labelConfig);
            changed = TRUE;
        }
    }

    return changed;
}

/****************************** Local Functions  *****************************/
static DynamicTextLabelCfg_t* getDynamicTextLabelConfig(uint32 widgetID)
{
    DynamicTextLabelCfg_t* returnValue = NULL;
    for (uint32 i=0; i<DynamicTextLabelConfigCount; ++i)
    {
        if (DynamicTextLabelConfigs[i].widgetID == widgetID)
        {
            returnValue = &DynamicTextLabelConfigs[i];
        }
    }

    return returnValue;
}

static void renderString(const display_area_definition_t* area, const ui_widget_t* uiWidget, const DynamicTextLabelCfg_t* config)
{
    ContentsManager_ClearRectangle(area, uiWidget->x, uiWidget->y, uiWidget->width, uiWidget->height, uiWidget->bgColor);

    if (uiWidget->boxed == TRUE)
    {
        ContentsManager_DrawBoxWithRoundCorners(area, uiWidget->x, uiWidget->y, uiWidget->width, uiWidget->height, FALSE, FALSE, uiWidget->fgColor, uiWidget->bgColor);
    }

    if (config->rightAligned == TRUE)
    {
        TextRenderer_RenderTextRightAligned(area, config->currentString, config->font, uiWidget->x, uiWidget->y, uiWidget->width, uiWidget->height, uiWidget->fgColor, uiWidget->bgColor);
    }
    else
    {
        TextRenderer_RenderText(area, config->currentString, config->font, uiWidget->x, uiWidget->y, uiWidget->width, uiWidget->height, uiWidget->fgColor, uiWidget->bgColor);
    }
}

static void readCurrentValue(DynamicTextLabelCfg_t* config)
{
    uint8 status;
    config->gclSink(&config->currentString, &status);
}

static boolean valueUpdated(const wchar* a, const wchar* b)
{
    uint8 i;
    boolean returnValue = FALSE;
    for (i = 0u; i<DYNAMIC_TEXT_LABEL_MAX_LENGTH; ++i)
    {
        if (a[i] != b[i])
        {
            returnValue = TRUE;
        }

        if ((a[i] == 0x0000u) || (b[i] == 0x0000u))
        {
            break;
        }
    }

    return returnValue;
}

static void saveLastStringToConfig(DynamicTextLabelCfg_t* config)
{
    for (uint8 i=0; i<DYNAMIC_TEXT_LABEL_MAX_LENGTH; ++i)
    {
        config->lastString[i] = config->currentString[i];
        if (config->currentString[i] == 0x0000u)
        {
            break;
        }
    }
}
