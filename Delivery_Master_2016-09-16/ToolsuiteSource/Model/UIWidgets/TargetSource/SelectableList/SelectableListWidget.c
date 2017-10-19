/********************************* Includes **********************************/
#include "SelectableListWidget.h"
#include "SelectableListWidgetCfg.h"
#include "ContentsManager.h"
#include "TextRenderer.h"
#include "Logger.h"
#include "LabelManager.h"

/**************************** Constants and Types ****************************/
const ui_widget_funcs SelectableListWidgetFuncs =
{
    .draw = SelectableListWidget_Draw,
    .needsRedraw = SelectableListWidget_NeedsRedraw,
    .redraw = SelectableListWidget_Draw,
    .handleNext = SelectableListWidget_HandleNext,
    .handlePrevious = SelectableListWidget_HandlePrevious,
    .handleEnter = SelectableListWidget_HandleEnter,
    .handleLeave = SelectableListWidget_HandleLeave,
    .offerFocus = SelectableListWidget_OfferFocus,
    .forceFocusRelease = SelectableListWidget_ForceReleaseFocus,
    .focusFinished = SelectableListWidget_FocusFinished
};

/********************************* Variables *********************************/
static const ui_widget_t* m_focusedWidget;
static SelectableListCfg_t* m_focusedWidgetConfig;
static uint8 m_focusedWidgetOriginalValue;
static boolean m_twoDigitsCount;
static boolean m_selectedValueChanged = FALSE;
static uint8 m_newSelectedValue = 0;

/************************* Local Function Prototypes *************************/
static SelectableListCfg_t* getConfig(uint32 widgetID);
static boolean refreshFocusedWidget(void);
static boolean acceptFocus(const ui_widget_t* widgetToFocus);
static void releaseFocus(void);
static void readCurrentValue(SelectableListCfg_t* config);
static void drawWidget(const display_area_definition_t* area, const ui_widget_t* uiWidget, const SelectableListCfg_t* config);
static void drawArrows(const display_area_definition_t* area, const ui_widget_t* uiWidget, const SelectableListCfg_t* config);
static void drawItems(const display_area_definition_t* area, const ui_widget_t* uiWidget, const SelectableListCfg_t* config);
static void drawIndexNumbers(const display_area_definition_t* area, const ui_widget_t* uiWidget, const SelectableListCfg_t* config);

/***************************** Exported Functions ****************************/
void SelectableListWidget_Draw(const display_area_definition_t* area, const ui_widget_t* uiWidget)
{
    Logger_Info("SelectableListWidget", "SelectableListWidget_Draw");

    if (m_focusedWidgetConfig == NULL)
    {
        SelectableListCfg_t* config = getConfig(uiWidget->widgetID);
        if (config == NULL)
        {
            Logger_Error("SelectableListWidget", "Configuration for widget ID:%d not found!", uiWidget->widgetID);
        }
        else
        {
            uint8 status;
            config->readLabelMappings(&config->mappings, &status);

            if (m_selectedValueChanged == TRUE)
            {
                m_selectedValueChanged = FALSE;
            }
            else
            {
                readCurrentValue(config);
            }
            config->needsRedraw = FALSE;
            drawWidget(area, uiWidget, config);
        }
    }
    else
    {
        if (m_selectedValueChanged == TRUE)
        {
            m_focusedWidgetConfig->currentValue = m_newSelectedValue;
            m_selectedValueChanged = FALSE;
        }
        drawWidget(area, uiWidget, m_focusedWidgetConfig);
        m_focusedWidgetConfig->needsRedraw = FALSE;

    }
}

boolean SelectableListWidget_NeedsRedraw(const ui_widget_t* uiWidget)
{
    boolean changed = FALSE;
    uint8 oldValue;
    static uint8 oldCount = 0;

    if ((m_focusedWidget != NULL) && (uiWidget->widgetID == m_focusedWidget->widgetID))
    {
        changed = refreshFocusedWidget();
    }
    else
    {
        SelectableListCfg_t* config = getConfig(uiWidget->widgetID);
        if (config == NULL)
        {
            Logger_Error("SelectableListWidget", "Configuration for widget ID:%d not found!", uiWidget->widgetID);
        }
        else if (config->mappings == NULL)
        {
            Logger_Error("SelectableListWidget", "Configuration for widget ID:%d did not have a label/value mapping!", uiWidget->widgetID);
        }
        else
        {
            uint8 mappingCount = config->mappings->Count;
            oldValue = config->currentValue;
            readCurrentValue(config);

            if (m_selectedValueChanged == TRUE)
            {
                if (oldValue != m_newSelectedValue)
                {
                    return FALSE;
                }
                else
                {
                    config->currentValue = m_newSelectedValue;
                    return TRUE;
                }
            }

            if ((oldValue != config->currentValue) || (oldCount != mappingCount) || (config->needsRedraw == TRUE))
            {
                oldCount = mappingCount;
                changed = TRUE;
            }
        }
    }

    return changed;
}

void SelectableListWidget_HandleNext()
{
    if (m_focusedWidgetConfig == NULL)
    {
        Logger_Error("SelectableListWidget", "Received a HandleNext() call without an active focused widget!");
    }
    else
    {
        ++m_focusedWidgetConfig->currentValue;
        if (m_focusedWidgetConfig->currentValue >= m_focusedWidgetConfig->mappings->Count)
        {
            m_focusedWidgetConfig->currentValue = 0;
        }

        m_focusedWidgetConfig->needsRedraw = TRUE;
    }
}

void SelectableListWidget_HandlePrevious()
{
    if (m_focusedWidgetConfig == NULL)
    {
        Logger_Error("SelectableListWidget", "Received a HandlePrevious() call without an active focused widget!");
    }
    else
    {
        if (m_focusedWidgetConfig->currentValue == 0u)
        {
            m_focusedWidgetConfig->currentValue = m_focusedWidgetConfig->mappings->Count - 1u;
        }
        else
        {
            --m_focusedWidgetConfig->currentValue;
        }

        m_focusedWidgetConfig->needsRedraw = TRUE;
    }
}

void SelectableListWidget_HandleEnter()
{
    if (m_focusedWidgetConfig == NULL)
    {
        Logger_Error("SelectableListWidget", "Received a HandleEnter() call without an active focused widget!");
    }
    else
    {
        if (m_focusedWidgetConfig->isSelectable == FALSE)
        {
            releaseFocus();
        }
        else
        {
            m_newSelectedValue = m_focusedWidgetConfig->currentValue;
            m_selectedValueChanged = TRUE;
            ContentsManager_SetLanguageAwaitingUpdate(m_newSelectedValue);
            m_focusedWidgetConfig->writeSelectedValue(m_focusedWidgetConfig->currentValue);

            releaseFocus();
        }
    }
}

void SelectableListWidget_HandleLeave()
{
    if (m_focusedWidgetConfig == NULL)
    {
        Logger_Error("SelectableListWidget", "Received a HandleLeave() call without an active focused widget!");
    }
    else
    {
        m_focusedWidgetConfig->currentValue = m_focusedWidgetOriginalValue;

        releaseFocus();
    }
}

boolean SelectableListWidget_OfferFocus(const ui_widget_t* uiWidget)
{
    boolean returnValue;
    if (m_focusedWidget != NULL)
    {
        Logger_Error("SelectableListWidget", "Received another focus offer while already having a focused widget!");
        returnValue = FALSE;
    }
    else
    {
        returnValue = acceptFocus(uiWidget);
    }

    return returnValue;
}

void SelectableListWidget_ForceReleaseFocus()
{
    releaseFocus();
}

boolean SelectableListWidget_FocusFinished()
{
    return (m_focusedWidget == NULL) ? TRUE : FALSE;
}

/****************************** Local Functions  *****************************/
static SelectableListCfg_t* getConfig(uint32 widgetID)
{
    SelectableListCfg_t* returnValue = NULL;

    for (uint32 i=0; i<SelectableListConfigCount; ++i)
    {
        if (SelectableListConfigs[i].widgetID == widgetID)
        {
            returnValue = &SelectableListConfigs[i];
        }
    }

    return returnValue;
}

static boolean refreshFocusedWidget()
{
    static uint8 m_focusedWidgetOriginalMappingCount = 255;
    boolean changed = FALSE;
    if ((m_focusedWidgetConfig->needsRedraw == TRUE) || (m_focusedWidgetOriginalMappingCount != m_focusedWidgetConfig->mappings->Count))
    {
        m_focusedWidgetOriginalMappingCount = m_focusedWidgetConfig->mappings->Count;
        if (m_focusedWidgetConfig->currentValue >= m_focusedWidgetConfig->mappings->Count)
        {
            m_focusedWidgetConfig->currentValue = m_focusedWidgetConfig->mappings->Count - 1u;
        }
        changed = TRUE;
    }
    return changed;
}

static boolean acceptFocus(const ui_widget_t* widgetToFocus)
{
    boolean returnValue;

    SelectableListCfg_t* config = getConfig(widgetToFocus->widgetID);
    if (config == NULL)
    {
        Logger_Error("SelectableListWidget", "Configuration for widget ID:%d not found when giving focus!", widgetToFocus->widgetID);
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

static void readCurrentValue(SelectableListCfg_t* config)
{
    if (config->isSelectable == TRUE)
    {
        uint8 status, value;
        config->readCurrentValue(&value, &status);

        config->currentValue = value;
    }
}

static void drawWidget(const display_area_definition_t* area, const ui_widget_t* uiWidget, const SelectableListCfg_t* config)
{
    ContentsManager_ClearRectangle(area, uiWidget->x, uiWidget->y, uiWidget->width, uiWidget->height, uiWidget->bgColor);

    if (config->showNumbering == TRUE)
    {
        drawIndexNumbers(area, uiWidget, config);
        drawArrows(area, uiWidget, config);
    }
    drawItems(area, uiWidget, config);
}

static void drawArrows(const display_area_definition_t* area, const ui_widget_t* uiWidget, const SelectableListCfg_t* config)
{
    uint8 arrowWidth;
    uint16 arrowX, upArrowY, downArrowY;

    arrowWidth = 10u;
    arrowX = uiWidget->x + (uint16)(uiWidget->width - (arrowWidth + (uint16)7u + ((m_twoDigitsCount == TRUE) ? (uint16)7u : (uint16)0u)));
    upArrowY = uiWidget->y+2u;
    downArrowY = uiWidget->y+uiWidget->height-6u;

    if ((config->hasFocus == TRUE) && (config->isSelectable == TRUE))
    {
        ContentsManager_DrawTriangle(area, DM_TRIANGLE_DIRECTION_UP, arrowX, upArrowY, arrowWidth, TRUE, FALSE, uiWidget->fgColor, uiWidget->bgColor);
        ContentsManager_DrawTriangle(area, DM_TRIANGLE_DIRECTION_DOWN, arrowX, downArrowY, arrowWidth, TRUE, FALSE, uiWidget->fgColor, uiWidget->bgColor);
    }
    else
    {
        ContentsManager_DrawTriangle(area, DM_TRIANGLE_DIRECTION_UP, arrowX, upArrowY, arrowWidth, TRUE, FALSE, uiWidget->fgColor, uiWidget->bgColor);
        ContentsManager_DrawTriangle(area, DM_TRIANGLE_DIRECTION_DOWN, arrowX, downArrowY, arrowWidth, TRUE, FALSE, uiWidget->fgColor, uiWidget->bgColor);
    }
}

static void drawIndexNumbers(const display_area_definition_t* area, const ui_widget_t* uiWidget, const SelectableListCfg_t* config)
{
    uint8 currentIndex = config->currentValue + 1u;
    uint8 indexCount = config->mappings->Count;
    wchar indexString[6];

    if ((currentIndex / 10u) > 0u)
    {
        indexString[0] = (0x0030u + ((wchar)currentIndex / 10u));
        indexString[1] = (0x0030u + ((wchar)currentIndex % 10u));
    }
    else
    {
        indexString[0] = 0x0020u; // Space
        indexString[1] = (0x30u + (wchar)currentIndex);
    }

    indexString[2] = (wchar)0x2Fu; // '/'

    if ((indexCount / 10u) > 0u)
    {
        indexString[3] = (0x0030u + ((wchar)indexCount / 10u));
        indexString[4] = (0x0030u + ((wchar)indexCount % 10u));
        m_twoDigitsCount = TRUE;
    }
    else
    {
        indexString[3] = (0x0030u + (wchar)indexCount);
        indexString[4] = (wchar)'\0';
        m_twoDigitsCount = FALSE;
    }

    indexString[5] = (wchar)'\0';

    uint16 indexX = uiWidget->x + (uint16)(uiWidget->width-52u);
    uint16 indexY = uiWidget->y + (uiWidget->height / 2u) - 10u;
    const font_t* font = LabelManager_GetDefaultFont();

    if ((config->hasFocus == TRUE) && (config->isSelectable == TRUE))
    {
        TextRenderer_RenderTextRightAligned(area, indexString, font, indexX, indexY, 50, 20, uiWidget->fgColor, uiWidget->bgColor);
    }
    else
    {
        TextRenderer_RenderTextRightAligned(area, indexString, font, indexX, indexY, 50, 20, uiWidget->fgColor, uiWidget->bgColor);
    }

}

static void drawItems(const display_area_definition_t* area, const ui_widget_t* uiWidget, const SelectableListCfg_t* config)
{
    static const uint16 itemGap = 2u;

    uint16 itemWidth = uiWidget->width - (uint16)(itemGap * 2u);

    if (config->showNumbering == TRUE)
    {
        if (config->mappings->Count > 10u)
        {
            itemWidth = itemWidth - 50u;
        }
        else
        {
            itemWidth = itemWidth - 50u;
        }
    }

    const uint16 itemHeight = (uint16)(uiWidget->height / config->displayItemCount) - (uint16)(itemGap * 2u);
    const uint16 itemOffset = itemHeight + itemGap;

    if (config->currentValue <= config->mappings->Count)
    {
        uint8 currentIndex = config->currentValue;
        uint8 selectedListPosition = 0u;

        if ((currentIndex + 1u) >= config->displayItemCount)
        {
            selectedListPosition = config->displayItemCount - 1u;
        }
        else
        {
            selectedListPosition = currentIndex;
        }

        uint8 startIndex = currentIndex - selectedListPosition;

        uint16 y = uiWidget->y;
        uint16 x = (uiWidget->x + itemGap);
        for (uint8 i = 0; i < config->displayItemCount; ++i)
        {
            if ((startIndex + i) == currentIndex)
            {
                if ((config->hasFocus == TRUE) && (config->isSelectable == TRUE))
                {
                    ContentsManager_DrawBoxWithHardCorners(area, x, y, itemWidth, itemHeight, TRUE, FALSE, config->selectedItemBgColor, config->selectedItemBgColor);
                    TextRenderer_RenderAlignedLabel(area, config->mappings->Mappings[startIndex + i].LabelID, x , y, itemWidth, itemHeight, config->selectedItemFgColor, config->selectedItemBgColor, config->textAlignment);
                }
                else
                {
                    //ContentsManager_DrawBoxWithHardCorners(area, x, y, itemWidth, itemHeight, FALSE, FALSE, uiWidget->fgColor, uiWidget->bgColor);
                    TextRenderer_RenderAlignedLabel(area, config->mappings->Mappings[startIndex + i].LabelID, x , y, itemWidth, itemHeight, uiWidget->fgColor, uiWidget->bgColor, config->textAlignment);
                }
            }
            else
            {
                TextRenderer_RenderAlignedLabel(area, config->mappings->Mappings[startIndex + i].LabelID, x, y, itemWidth, itemHeight, uiWidget->fgColor, uiWidget->bgColor, config->textAlignment);
            }
            y += itemOffset;
        }
    }
}
