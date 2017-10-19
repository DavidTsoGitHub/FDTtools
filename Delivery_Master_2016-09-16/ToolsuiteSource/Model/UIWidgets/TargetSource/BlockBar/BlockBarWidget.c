/********************************* Includes **********************************/
#include "BlockBarWidget.h"
#include "BlockBarWidgetCfg.h"
#include "ContentsManager.h"
#include "LabelManager.h"
#include "TextRenderer.h"
#include "GCL_Types.h"
#include "Logger.h"

/**************************** Constants and Types ****************************/
const ui_widget_funcs BlockBarWidgetFuncs =
{
    .draw = BlockBar_Draw,
    .needsRedraw = BlockBar_NeedsRedraw,
    .redraw = BlockBar_Draw,
    .handleNext = NULL,
    .handlePrevious = NULL,
    .handleEnter = NULL,
    .handleLeave = NULL,
    .offerFocus = NULL,
    .forceFocusRelease = NULL,
    .focusFinished = NULL
};

/************************* Member variables **********************************/

/************************* Local Function Prototypes *************************/
static BlockBarWidgetCfg_t* getBlockBarWidgetConfigForID(uint32 widgetID);
static void readCurrentValue(BlockBarWidgetCfg_t* config);
static uint32 getU32ValueFromDynamicWidgetValue(const DynamicWidgetValue* value);
static boolean needsRedraw(BlockBarWidgetCfg_t* config);
static void renderBlockBar(const display_area_definition_t* area, const ui_widget_t* uiWidget, BlockBarWidgetCfg_t* config);

/***************************** Exported Functions ****************************/
void BlockBar_Draw(const display_area_definition_t* area, const ui_widget_t* uiWidget)
{
    BlockBarWidgetCfg_t* blockBarWidgetConfig = getBlockBarWidgetConfigForID(uiWidget->widgetID);

    if (blockBarWidgetConfig != NULL)
    {
        readCurrentValue(blockBarWidgetConfig);
        renderBlockBar(area, uiWidget, blockBarWidgetConfig);
    }
}

boolean BlockBar_NeedsRedraw(const ui_widget_t* uiWidget)
{
    boolean changed = FALSE;
    BlockBarWidgetCfg_t* blockBarWidgetConfig = getBlockBarWidgetConfigForID(uiWidget->widgetID);

    if (blockBarWidgetConfig != NULL)
    {
        readCurrentValue(blockBarWidgetConfig);
        if (needsRedraw(blockBarWidgetConfig) == TRUE)
        {
            changed = TRUE;
        }
    }

    return changed;
}

static boolean needsRedraw(BlockBarWidgetCfg_t* config)
{
    boolean returnValue = FALSE;
    uint32 precision = 1000u;
    uint32 barMinValue = config->min * precision;
    uint32 barMaxValue = config->max * precision;

    uint32 currentValue = getU32ValueFromDynamicWidgetValue(&config->currentValue) * precision;

    if ((barMinValue <= currentValue) && (currentValue <= barMaxValue))
    {
        uint32 valuePerBlock = ((config->max - config->min) * precision) / config->numberOfBlocks;

        uint32 blockMinValue;
        uint32 blockMaxValue;

        if (config->lastRenderedValue < barMinValue)
        {
            config->lastRenderedValue = barMinValue;
        }

        // Block min
        if (config->lastRenderedValue <= barMinValue)
        {
            blockMinValue = barMinValue;
        }
        else
        {
            blockMinValue = config->lastRenderedValue;
        }

        // Block max
        if ((config->lastRenderedValue + valuePerBlock) >= barMaxValue)
        {
            blockMaxValue = barMaxValue;
        }
        else
        {
            blockMaxValue = config->lastRenderedValue + valuePerBlock;
        }

        if ((blockMinValue > currentValue) || (currentValue > blockMaxValue))
        {
            returnValue = TRUE;
        }
    }

    return returnValue;
}

static void renderBlockBar(const display_area_definition_t* area, const ui_widget_t* uiWidget, BlockBarWidgetCfg_t* config)
{
    ContentsManager_ClearRectangle(area, uiWidget->x, uiWidget->y, uiWidget->width, uiWidget->height, uiWidget->bgColor);

    if (uiWidget->boxed == TRUE)
    {
        ContentsManager_DrawBoxWithRoundCorners(area, uiWidget->x, uiWidget->y, uiWidget->width, uiWidget->height, FALSE, FALSE, uiWidget->fgColor, uiWidget->bgColor);
    }

    uint16 widthPerBlock = (uint16)((uiWidget->width - 4u) / config->numberOfBlocks);

    widthPerBlock -= config->spacing;
    if (widthPerBlock == 0u)
    {
        widthPerBlock += 1u;
    }

    uint16 startXPos = uiWidget->x + 2u;

    uint32 precision = 1000u;
    uint32 accumulatedValue = config->min * precision;
    uint32 valuePerBlock = ((config->max - config->min) * precision) / config->numberOfBlocks;
    uint32 currentValue = getU32ValueFromDynamicWidgetValue(&config->currentValue) * precision;

    for (uint8 i = 0u; i < config->numberOfBlocks; ++i)
    {
        if (i > 0u)
        {
            startXPos += widthPerBlock + config->spacing;
        }

        if ((accumulatedValue + valuePerBlock) < currentValue)
        {
            ContentsManager_DrawBoxWithHardCorners(area, startXPos, uiWidget->y + 2u, widthPerBlock, uiWidget->height - 4u, TRUE, FALSE, uiWidget->fgColor, uiWidget->bgColor);
            accumulatedValue += valuePerBlock;
        }
        else
        {
            ContentsManager_DrawBoxWithHardCorners(area, startXPos, uiWidget->y + 2u, widthPerBlock, uiWidget->height - 4u, FALSE, FALSE, uiWidget->fgColor, uiWidget->bgColor);
        }
    }
    config->lastRenderedValue = accumulatedValue;
}

static void readCurrentValue(BlockBarWidgetCfg_t* config)
{
    uint8 status;
    config->gclSink(&config->currentValue, &status);

    if (status != GCL_SIGNAL_OK)
    {
        config->currentValue.Valid = FALSE;
        Logger_Info("BlockBarWidget", "GCL signal not OK.");
    }
}

static uint32 getU32ValueFromDynamicWidgetValue(const DynamicWidgetValue* value)
{
    uint32 returnValue = 0xFFFFFFFFu;

    switch (value->DataType)
    {
        case STD_DATATYPE_UINT8:
            returnValue = (uint32)value->Value.U8;
            break;
        case STD_DATATYPE_UINT16:
            returnValue = (uint32)value->Value.U16;
            break;
        case STD_DATATYPE_UINT32:
            returnValue = value->Value.U32;
            break;
        case STD_DATATYPE_INT8: // Intentional fall-through, we don't support signed values here yet
        case STD_DATATYPE_INT16:
        case STD_DATATYPE_INT32:
        default:
            // Do nothing
            break;
    }

    return returnValue;
}

static BlockBarWidgetCfg_t* getBlockBarWidgetConfigForID(uint32 widgetID)
{
    BlockBarWidgetCfg_t* returnValue = (BlockBarWidgetCfg_t*)0;

    for (uint32 i = 0u; i < BlockBarWidgetConfigCount; ++i)
    {
        if (widgetID == BlockBarWidgetConfigs[i].widgetID)
        {
            returnValue = &BlockBarWidgetConfigs[i];
            break;
        }
    }

    return returnValue;
}
