/********************************* Includes **********************************/
#include "LabelWidget.h"
#include "LabelWidgetCfg.h"
#include "TextRenderer.h"
#include "ContentsManager.h"

/**************************** Constants and Types ****************************/
const ui_widget_funcs LabelWidgetFuncs =
{
    .draw = Label_Draw,
    .needsRedraw = NULL,
    .redraw = Label_Draw,
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
static const LabelCfg_t* getLabelConfiguration(uint32 widgetID);

/***************************** Exported Functions ****************************/
void Label_Draw(const display_area_definition_t* area, const ui_widget_t* uiWidget)
{
    const LabelCfg_t* config = getLabelConfiguration(uiWidget->widgetID);
    if (config != NULL)
    {
        ContentsManager_ClearRectangle(area, uiWidget->x, uiWidget->y, uiWidget->width, uiWidget->height, uiWidget->bgColor);
        if (uiWidget->boxed == TRUE)
        {
            ContentsManager_DrawBoxWithRoundCorners(area, uiWidget->x, uiWidget->y, uiWidget->width, uiWidget->height, FALSE, FALSE, uiWidget->fgColor, uiWidget->bgColor);
        }

        TextRenderer_RenderLabel(area, config->labelID, uiWidget->x, uiWidget->y, uiWidget->width, uiWidget->height, uiWidget->fgColor, uiWidget->bgColor);
    }
}

/****************************** Local Functions  *****************************/
static const LabelCfg_t* getLabelConfiguration(uint32 widgetID)
{
    const LabelCfg_t* returnValue = NULL;
    for (uint32 i=0; i<LabelConfigCount; ++i)
    {
        if (LabelConfigs[i].widgetID == widgetID)
        {
            returnValue = &LabelConfigs[i];
            break;
        }
    }

    return returnValue;
}
