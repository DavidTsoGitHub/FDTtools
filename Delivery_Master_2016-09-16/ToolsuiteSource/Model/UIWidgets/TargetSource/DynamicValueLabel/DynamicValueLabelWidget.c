/********************************* Includes **********************************/
#include "DynamicValueLabelWidget.h"
#include "DynamicValueLabelWidgetCfg.h"
#include "LabelManager.h"
#include "ContentsManager.h"
#include "TextRenderer.h"
#include "GCL_Types.h"
#include "systime.h"
#include "Logger.h"
#include "WCharDefines.h"

/**************************** Constants and Types ****************************/
const ui_widget_funcs DynamicValueLabelWidgetFuncs =
{
    .draw = DynamicValueLabel_Draw,
    .needsRedraw = DynamicValueLabel_NeedsRedraw,
    .redraw = DynamicValueLabel_Draw,
    .handleNext = NULL,
    .handlePrevious = NULL,
    .handleEnter = NULL,
    .handleLeave = NULL,
    .offerFocus = NULL,
    .forceFocusRelease = NULL,
    .focusFinished = NULL
};

/********************************* Variables *********************************/
static boolean stringUpdated = FALSE;

/************************* Local Function Prototypes *************************/
static DynamicValueLabelCfg_t* getDynamicValueLabelConfig(uint32 widgetID);
static void renderString(const display_area_definition_t* area, const ui_widget_t* uiWidget, const DynamicValueLabelCfg_t* config);
static void readCurrentValue(DynamicValueLabelCfg_t* config);
static boolean valueUpdated(const wchar* a, const wchar* b);
static void updateString(DynamicValueLabelCfg_t* labelConfig);
static void setInvalidString(wchar* currentString, sint8 maximumNbrOfDigits, sint8 decimals, wchar errorChar);
static uint32 getU32ValueFromDynamicWidgetValue(const DynamicWidgetValue* value);

static sint8 calculateDisplayDigitCount(sint8 digitCount, sint8 scaling, sint8 decimals);

/***************************** Exported Functions ****************************/
void DynamicValueLabel_Draw(const display_area_definition_t* area, const ui_widget_t* uiWidget)
{
    DynamicValueLabelCfg_t* labelConfig = getDynamicValueLabelConfig(uiWidget->widgetID);
    if (labelConfig != NULL)
    {
        if (labelConfig->font == NULL)
        {
            labelConfig->font = LabelManager_GetDefaultFont();
        }

        if (stringUpdated == FALSE)
        {
            readCurrentValue(labelConfig);
            updateString(labelConfig);
        }
        else
        {
            stringUpdated = FALSE;
        }
        renderString(area, uiWidget, labelConfig);
    }
}

boolean DynamicValueLabel_NeedsRedraw(const ui_widget_t* uiWidget)
{
    boolean changed = FALSE;

    DynamicValueLabelCfg_t* labelConfig = getDynamicValueLabelConfig(uiWidget->widgetID);
    if (labelConfig != NULL)
    {
        wchar oldString[DYNAMIC_VALUE_LABEL_MAX_LENGTH];
        for (uint8 i = 0u; i < DYNAMIC_VALUE_LABEL_MAX_LENGTH; ++i)
        {
            oldString[i] = labelConfig->currentString[i];
        }

        readCurrentValue(labelConfig);
        updateString(labelConfig);
        if (valueUpdated(oldString, labelConfig->currentString) == TRUE)
        {
            stringUpdated = TRUE;
            changed = TRUE;
        }
    }

    return changed;
}

/****************************** Local Functions  *****************************/
static DynamicValueLabelCfg_t* getDynamicValueLabelConfig(uint32 widgetID)
{
    DynamicValueLabelCfg_t* returnValue = NULL;
    for (uint32 i = 0u; i < DynamicValueLabelConfigCount; ++i)
    {
        if (DynamicValueLabelConfigs[i].widgetID == widgetID)
        {
            returnValue = &DynamicValueLabelConfigs[i];
            break;
        }
    }

    return returnValue;
}

static void renderString(const display_area_definition_t* area, const ui_widget_t* uiWidget, const DynamicValueLabelCfg_t* config)
{
    ContentsManager_ClearRectangle(area, uiWidget->x, uiWidget->y, uiWidget->width, uiWidget->height, uiWidget->bgColor);

    if (uiWidget->boxed == TRUE)
    {
        ContentsManager_DrawBoxWithRoundCorners(area, uiWidget->x, uiWidget->y, uiWidget->width, uiWidget->height, FALSE, FALSE, uiWidget->fgColor, uiWidget->bgColor);
    }

    TextRenderer_RenderTextRightAligned(area, config->currentString, config->font, uiWidget->x, uiWidget->y, uiWidget->width, uiWidget->height, uiWidget->fgColor, uiWidget->bgColor);
}

static void readCurrentValue(DynamicValueLabelCfg_t* config)
{
    uint8 status;
    config->gclSink(&config->currentValue, &status);

    if (status != GCL_SIGNAL_OK)
    {
        config->currentValue.Valid = FALSE;
        Logger_Info("DynamicValueLabelWidget", "GCL signal not OK.");
    }
}

static sint8 getDigitsInValue(uint32 value)
{
    uint32 divider = 10;
    sint8 digitCount = 1;

    while ((value / ((divider == 0u) ? 1u : divider)) > 0u)
    {
        ++digitCount;
        divider *= 10u;
    }

    return digitCount;
}

static sint8 calculateDisplayDigitCount(sint8 digitCount, sint8 scaling, sint8 decimals)
{
    sint8 returnValue = 0;
    // If we scale down more digits than we have, we need to display only the decimal count + the initial zero.
    // Given value 10000, scaling -6 and 3 decimals.
    // 10000 * 10^-6 would be 0.100, i.e. 4 digits.
    if ((digitCount + scaling) < 0)
    {
        returnValue = decimals + 1;
    }
    else
    {
        returnValue = digitCount + decimals + scaling;
    }

    return returnValue;
}

static void setCharacter(wchar value, wchar* currentString, uint8 stringIndex)
{
    if (stringIndex < (DYNAMIC_VALUE_LABEL_MAX_LENGTH-1u))
    {
        currentString[stringIndex] = value;
    }
}

static void setInvalidString(wchar* currentString, sint8 maximumNbrOfDigits, sint8 decimals, wchar errorChar)
{
    sint8 maximumNbrOfChars = maximumNbrOfDigits;
    sint8 nbrOfIntegerDigits = (maximumNbrOfChars - decimals);
    if (decimals > 0) // Account for decimal point
    {
        ++maximumNbrOfChars;
    }

    for (sint8 i = 0; i < maximumNbrOfChars; ++i)
    {
        if (i == nbrOfIntegerDigits)
        {
            currentString[i] = WCHAR_DECIMAL_POINT;
        }
        else
        {
            currentString[i] = errorChar;
        }
    }

    currentString[maximumNbrOfChars] = WCHAR_EOL;
}

static void updateString(DynamicValueLabelCfg_t* labelConfig)
{
    static boolean flashingValueShown = FALSE;
    uint32 value;
    wchar* currentString;
    sint8 scaling, nbrOfDecimals, maximumNbrOfDigits;
    uint8 stringIndex = 0;

    value = getU32ValueFromDynamicWidgetValue(&labelConfig->currentValue);
    currentString = labelConfig->currentString;
    scaling = labelConfig->scaling;
    nbrOfDecimals = labelConfig->decimals;
    maximumNbrOfDigits = labelConfig->maximumNbrOfDigits;

    if (labelConfig->currentValue.Valid == FALSE)
    {
        setInvalidString(currentString, maximumNbrOfDigits, nbrOfDecimals, WCHAR_DASH);
    }
    else
    {
        uint32 currentMs = SysTime_GetTimeMs();
        flashingValueShown = ((currentMs % 2000u) >= 1000u) ? TRUE : FALSE;
        if ((labelConfig->currentValue.Flashing == TRUE) && (flashingValueShown == FALSE))
        {
            labelConfig->currentString[0] = WCHAR_EOL;
        }
        else
        {
            for (uint8 i = 0u; i < DYNAMIC_VALUE_LABEL_MAX_LENGTH; ++i)
            {
                currentString[i] = WCHAR_EOL;
            }


            sint8 valueDigitCount, displayDigitCount;
            valueDigitCount = getDigitsInValue(value);
            displayDigitCount = calculateDisplayDigitCount(valueDigitCount, scaling, nbrOfDecimals);

            boolean isValidLength = TRUE;
            if (displayDigitCount > maximumNbrOfDigits)
            {
                setInvalidString(currentString, maximumNbrOfDigits, nbrOfDecimals, WCHAR_HASH);
                isValidLength = FALSE;
            }

            if (isValidLength == TRUE)
            {
                uint32 divider = 1;
                for (sint8 i = 1; i < valueDigitCount; ++i)
                {
                    divider *= 10u;
                }

                if (scaling >= 0)
                {
                    uint32 rest = value;
                    for (sint8 i = valueDigitCount; i >= 1; --i)
                    {
                        setCharacter(DigitToWchar((uint8)(rest / divider)), currentString, stringIndex);
                        stringIndex++;
                        rest %= divider;
                        divider /= 10u;
                    }

                    for (sint8 i = 0; i < ((displayDigitCount - valueDigitCount) - nbrOfDecimals); ++i)
                    {
                        setCharacter(WCHAR_ZERO, currentString, stringIndex++);
                    }

                    if (nbrOfDecimals > 0)
                    {
                        setCharacter(WCHAR_DECIMAL_POINT, currentString, stringIndex++);
                        for (sint8 i = 0; i < nbrOfDecimals; ++i)
                        {
                            setCharacter(WCHAR_ZERO, currentString, stringIndex++);
                        }
                    }
                }
                else if ((valueDigitCount + scaling) <= 0)
                {
                    sint8 writtenDecimals = 0;
                    setCharacter(WCHAR_ZERO, currentString, stringIndex++);
                    setCharacter(WCHAR_DECIMAL_POINT, currentString, stringIndex++);

                    for (sint8 i = 0; i < (-scaling - valueDigitCount); ++i)
                    {
                        setCharacter(WCHAR_ZERO, currentString, stringIndex++);
                        ++writtenDecimals;
                        if (writtenDecimals == nbrOfDecimals)
                        {
                            break;
                        }
                    }

                    uint32 rest = value;
                    for (sint8 i = valueDigitCount; i >= 1; --i)
                    {
                        if (writtenDecimals == nbrOfDecimals)
                        {
                            break;
                        }
                        setCharacter(DigitToWchar((uint8)(rest / divider)), currentString, stringIndex);
                        stringIndex++;
                        rest %= divider;
                        divider /= 10u;
                        ++writtenDecimals;
                    }

                    while (writtenDecimals < nbrOfDecimals)
                    {
                        setCharacter(WCHAR_ZERO, currentString, stringIndex++);
                        ++writtenDecimals;
                    }
                }
                else
                {
                    uint32 rest = value;
                    sint8 writtenDigits = 0;
                    sint8 writtenDecimals = 0;
                    boolean countDecimals = FALSE;
                    for (sint8 i = valueDigitCount; (i >= 1) && (writtenDecimals < nbrOfDecimals); --i)
                    {
                        if (writtenDigits == (displayDigitCount - nbrOfDecimals))
                        {
                            setCharacter(WCHAR_DECIMAL_POINT, currentString, stringIndex++);
                            countDecimals = TRUE;
                        }
                        setCharacter(DigitToWchar((uint8)(rest / divider)), currentString, stringIndex);
                        stringIndex++;
                        rest %= divider;
                        divider /= 10u;

                        if (countDecimals == TRUE)
                        {
                            ++writtenDecimals;
                        }
                        ++writtenDigits;
                    }

                    while (writtenDecimals < nbrOfDecimals)
                    {
                        setCharacter(WCHAR_ZERO, currentString, stringIndex++);
                        ++writtenDecimals;
                    }
                }
            }
        }
    }
}

static uint32 getU32ValueFromDynamicWidgetValue(const DynamicWidgetValue* value)
{
    uint32 returnValue;
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
        // Intentional fall-through, we don't handle signed widget values
        case STD_DATATYPE_INT8:
        case STD_DATATYPE_INT16:
        case STD_DATATYPE_INT32:
        default:
            returnValue = 0xFFFFFFFFu;
            break;
    }

    return returnValue;
}

static boolean valueUpdated(const wchar* a, const wchar* b)
{
    uint8 i;
    boolean returnValue = FALSE;
    for (i = 0; i < DYNAMIC_VALUE_LABEL_MAX_LENGTH; ++i)
    {
        if (a[i] != b[i])
        {
            returnValue = TRUE;
            break;
        }
    }

    return returnValue;
}
