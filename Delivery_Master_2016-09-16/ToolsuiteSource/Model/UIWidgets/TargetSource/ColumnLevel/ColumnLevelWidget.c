/********************************* Includes **********************************/
#include "ColumnLevelWidget.h"
#include "ColumnLevelWidgetCfg.h"
#include "ContentsManager.h"
#include "Logger.h"
#include "LayoutManager.h"

/**************************** Constants and Types ****************************/
const ui_widget_funcs ColumnLevelWidgetFuncs =
{
    .draw = ColumnLevelWidget_Draw,
    .needsRedraw = ColumnLevelWidget_NeedsRedraw,
    .redraw = ColumnLevelWidget_Draw,
    .handleNext = ColumnLevelWidget_HandleNext,
    .handlePrevious = ColumnLevelWidget_HandlePrevious,
    .handleEnter = ColumnLevelWidget_HandleEnter,
    .handleLeave = ColumnLevelWidget_HandleLeave,
    .offerFocus = ColumnLevelWidget_OfferFocus,
    .forceFocusRelease = ColumnLevelWidget_ForceReleaseFocus,
    .focusFinished = ColumnLevelWidget_FocusFinished
};

/********************************* Variables *********************************/
static const ui_widget_t* m_focusedWidget;
static ColumnLevelWidgetCfg_t* m_focusedWidgetConfig;
static uint8 m_focusedWidgetOriginalValue;

/************************* Local Function Prototypes *************************/
static ColumnLevelWidgetCfg_t* getConfig(uint32 widgetID);
static boolean acceptFocus(const ui_widget_t* widgetToFocus);
static void releaseFocus(void);
static void readCurrentValue(ColumnLevelWidgetCfg_t* config);
static void drawWidget(const display_area_definition_t* area, const ui_widget_t* uiWidget, const ColumnLevelWidgetCfg_t* config);

/***************************** Exported Functions ****************************/
void ColumnLevelWidget_Draw(const display_area_definition_t* area, const ui_widget_t* uiWidget)
{
    if (m_focusedWidgetConfig == NULL)
    {
        ColumnLevelWidgetCfg_t* config = getConfig(uiWidget->widgetID);
        if (config == NULL)
        {
            Logger_Error("ColumnLevelWidget", "Configuration for widget ID:%d not found!", uiWidget->widgetID);
        }
        else
        {
            readCurrentValue(config);
            drawWidget(area, uiWidget, config);
        }
    }
    else
    {
        drawWidget(area, uiWidget, m_focusedWidgetConfig);
        m_focusedWidgetConfig->needsRedraw = FALSE;
    }
}

boolean ColumnLevelWidget_NeedsRedraw(const ui_widget_t* uiWidget)
{
    boolean changed = FALSE;
    uint8 oldValue;

    if ((m_focusedWidget != NULL) && (uiWidget->widgetID == m_focusedWidget->widgetID))
    {
        if (m_focusedWidgetConfig->needsRedraw == TRUE)
        {
            changed = TRUE;
        }
    }
    else
    {
        ColumnLevelWidgetCfg_t* config = getConfig(uiWidget->widgetID);
        if (config == NULL)
        {
            Logger_Error("ColumnLevelWidget", "Configuration for widget ID:%d not found!", uiWidget->widgetID);
        }
        else
        {
            oldValue = config->currentValue;
            readCurrentValue(config);

            if ((oldValue != config->currentValue) || (config->needsRedraw == TRUE))
            {
                config->needsRedraw = FALSE;
                changed = TRUE;
            }
        }
    }

    return changed;
}

void ColumnLevelWidget_HandleNext()
{
    if (m_focusedWidgetConfig == NULL)
    {
        Logger_Error("ColumnLevelWidget", "Received a HandleNext() call without an active focused widget!");
    }
    else
    {
        --m_focusedWidgetConfig->currentValue;
        if (m_focusedWidgetConfig->currentValue <= m_focusedWidgetConfig->min)
        {
            m_focusedWidgetConfig->currentValue = m_focusedWidgetConfig->min;
        }

        m_focusedWidgetConfig->writeCurrentColumnLevel(m_focusedWidgetConfig->currentValue);
        m_focusedWidgetConfig->needsRedraw = TRUE;
    }
}

void ColumnLevelWidget_HandlePrevious()
{
    if (m_focusedWidgetConfig == NULL)
    {
        Logger_Error("ColumnLevelWidget", "Received a HandlePrevious() call without an active focused widget!");
    }
    else
    {
        ++m_focusedWidgetConfig->currentValue;
        if (m_focusedWidgetConfig->currentValue >= m_focusedWidgetConfig->max)
        {
            m_focusedWidgetConfig->currentValue = m_focusedWidgetConfig->max;
        }

        m_focusedWidgetConfig->writeCurrentColumnLevel(m_focusedWidgetConfig->currentValue);
        m_focusedWidgetConfig->needsRedraw = TRUE;
    }
}

void ColumnLevelWidget_HandleEnter()
{
    if (m_focusedWidgetConfig == NULL)
    {
        Logger_Error("ColumnLevelWidget", "Received a HandleEnter() call without an active focused widget!");
    }
    else
    {
        m_focusedWidgetConfig->writeCurrentColumnLevel(m_focusedWidgetConfig->currentValue);

        releaseFocus();
    }
}

void ColumnLevelWidget_HandleLeave()
{
    if (m_focusedWidgetConfig == NULL)
    {
        Logger_Error("ColumnLevelWidget", "Received a HandleLeave() call without an active focused widget!");
    }
    else
    {
        m_focusedWidgetConfig->currentValue = m_focusedWidgetOriginalValue;
        m_focusedWidgetConfig->writeCurrentColumnLevel(m_focusedWidgetConfig->currentValue);

        releaseFocus();
    }
}

boolean ColumnLevelWidget_OfferFocus(const ui_widget_t* uiWidget)
{
    boolean returnValue;
    if (m_focusedWidget != NULL)
    {
        Logger_Error("ColumnLevelWidget", "Received another focus offer while already having a focused widget!");
        returnValue = FALSE;
    }
    else
    {
        returnValue = acceptFocus(uiWidget);
    }

    return returnValue;
}

void ColumnLevelWidget_ForceReleaseFocus()
{
    releaseFocus();
}

boolean ColumnLevelWidget_FocusFinished()
{
    return (m_focusedWidget == NULL) ? TRUE : FALSE;
}

/****************************** Local Functions  *****************************/

static ColumnLevelWidgetCfg_t* getConfig(uint32 widgetID)
{
    ColumnLevelWidgetCfg_t* returnValue = (ColumnLevelWidgetCfg_t*)0;

    for (uint32 i = 0u; i < ColumnLevelWidgetConfigCount; ++i)
    {
        if (widgetID == ColumnLevelWidgetConfigs[i].widgetID)
        {
            returnValue = &ColumnLevelWidgetConfigs[i];
        }
    }

    return returnValue;
}

static boolean acceptFocus(const ui_widget_t* widgetToFocus)
{
    boolean returnValue;
    ColumnLevelWidgetCfg_t* config = getConfig(widgetToFocus->widgetID);
    if (config == NULL)
    {
        Logger_Error("ColumnLevelWidget", "Configuration for widget ID:%d not found when giving focus!", widgetToFocus->widgetID);
        returnValue = FALSE;
    }
    else
    {
        m_focusedWidgetOriginalValue = config->currentValue;
        config->hasFocus = TRUE;
        config->needsRedraw = TRUE;
        m_focusedWidget = widgetToFocus;
        m_focusedWidgetConfig = config;
        returnValue = TRUE;
    }

    return returnValue;
}

static void releaseFocus(void)
{
    if ((m_focusedWidget != NULL) && (m_focusedWidgetConfig != NULL))
    {
        m_focusedWidgetConfig->hasFocus = FALSE;
        m_focusedWidgetConfig->needsRedraw = TRUE;
        m_focusedWidget = NULL;
        m_focusedWidgetConfig = NULL;
    }
}

static void readCurrentValue(ColumnLevelWidgetCfg_t* config)
{
    uint8 status, value;
    config->readCurrentColumnLevel(&value, &status);
    config->currentValue = value;
}

static void drawWidget(const display_area_definition_t* area, const ui_widget_t* uiWidget, const ColumnLevelWidgetCfg_t* config)
{
    ContentsManager_ClearRectangle(area, uiWidget->x, uiWidget->y, uiWidget->width, uiWidget->height, uiWidget->bgColor);

    if (uiWidget->boxed == TRUE)
    {
        if (config->hasFocus == TRUE)
        {
            ContentsManager_DrawBoxWithRoundCorners(area, uiWidget->x, uiWidget->y, uiWidget->width, uiWidget->height, TRUE, FALSE, config->selectedItemBGColor, config->selectedItemFGColor);
        }
        else
        {
            ContentsManager_DrawBoxWithRoundCorners(area, uiWidget->x, uiWidget->y, uiWidget->width, uiWidget->height, FALSE, FALSE, uiWidget->fgColor, uiWidget->bgColor);
        }
    }
    else
    {
        if (config->hasFocus == TRUE)
        {
            ContentsManager_DrawBoxWithRoundCorners(area, uiWidget->x, uiWidget->y, uiWidget->width, uiWidget->height, TRUE, FALSE, config->selectedItemBGColor, config->selectedItemFGColor);
        }
    }

    uint8 currentValue = (config->currentValue < config->min) ? config->max : config->currentValue;

    uint16 widthPerBlock = (uint16)((uiWidget->width - 4u) / config->numberOfBlocks);

    widthPerBlock -= config->spacing;
    if (widthPerBlock == 0u)
    {
        widthPerBlock += 1u;
    }

    uint16 currentBlockHeight = config->minBlockHeight;
    uint8 blockDelta = config->maxBlockHeight - config->minBlockHeight;
    uint8 heightDiff = blockDelta / (config->numberOfBlocks - 1u);

    uint16 startXPos = uiWidget->x + 2u;

    for (uint8 i = 1u; i <= config->numberOfBlocks; ++i)
    {
        if (i > 1u)
        {
            startXPos += widthPerBlock + config->spacing;
            currentBlockHeight += (uint16)heightDiff;
        }

        if (i <= currentValue)
        {
            if (config->hasFocus == TRUE)
            {
                ContentsManager_DrawBoxWithHardCorners(area, startXPos, ((uiWidget->y + uiWidget->height) - (currentBlockHeight + 2u)), widthPerBlock, currentBlockHeight, FALSE, TRUE, config->selectedItemBGColor, config->selectedItemFGColor);
            }
            else
            {
                ContentsManager_DrawBoxWithHardCorners(area, startXPos, ((uiWidget->y + uiWidget->height) - (currentBlockHeight + 2u)), widthPerBlock, currentBlockHeight, TRUE, FALSE, uiWidget->fgColor, uiWidget->bgColor);
            }
        }
        else
        {
            if (config->hasFocus == TRUE)
            {
                ContentsManager_DrawBoxWithHardCorners(area, startXPos, ((uiWidget->y + uiWidget->height) - (currentBlockHeight + 2u)), widthPerBlock, currentBlockHeight, TRUE, TRUE, config->selectedItemBGColor, config->selectedItemFGColor);
            }
            else
            {
                ContentsManager_DrawBoxWithHardCorners(area, startXPos, ((uiWidget->y + uiWidget->height) - (currentBlockHeight + 2u)), widthPerBlock, currentBlockHeight, FALSE, FALSE, uiWidget->fgColor, uiWidget->bgColor);
            }
        }
    }
}
