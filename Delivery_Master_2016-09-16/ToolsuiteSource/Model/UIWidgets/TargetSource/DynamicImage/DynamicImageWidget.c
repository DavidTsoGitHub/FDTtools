//lint -save
/********************************* Includes **********************************/
#include "DynamicImageWidget.h"
#include "DynamicImageWidgetCfg.h"
#include "ContentsManager.h"
#include "ImageManager.h"
#include "AnimationManager.h"
#include "Logger.h"

/**************************** Constants and Types ****************************/
const ui_widget_funcs DynamicImageWidgetFuncs =
{
    .draw = DynamicImageWidget_Draw,
    .needsRedraw = DynamicImageWidget_NeedsRedraw,
    .redraw = DynamicImageWidget_Redraw,
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
static DynamicImageCfg_t* getConfig(uint32 widgetID);
static uint32 readCurrentValue(const DynamicImageCfg_t *config);
static void displayImage(const ui_widget_t* uiWidget, const DynamicImageCfg_t* config);
static boolean animationTriggered(const signal_transition_t *triggers, uint8 triggerCount);
static boolean compareConditions(boolean firstCondition, boolean secondCondition, transition_connective_t connective);
static boolean checkCondition(sint32 value, transition_condition_t compareCondition, sint32 compareValue);
static sint32 getSint32ValueFromDynamicWidgetValue(const DynamicWidgetValue* value);

/***************************** Exported Functions ****************************/
void DynamicImageWidget_Draw(const display_area_definition_t* area, const ui_widget_t* uiWidget)
{
    (void)area;
    Logger_Info("DynamicImageWidget", "DynamicImageWidget_Draw");

    DynamicImageCfg_t* config = getConfig(uiWidget->widgetID);
    if (config == NULL)
    {
        Logger_Error("DynamicImageWidget", "Configuration for widget ID:%d widget not found!", uiWidget->widgetID);
    }
    else
    {
        config->currentImageID = readCurrentValue(config);
        displayImage(uiWidget, config);

        if(uiWidget->animation1 != (animation_mapping_t*)0)
        {
            if (config->animation1Triggers != NULL)
            {
                boolean returnValue = AnimationManager_StartAnimation(uiWidget->animation1, config->animation1TriggersProperties->onLoadBehavior, config->animation1TriggersProperties->onTriggerInactiveBehavior);
                UNUSED(returnValue);
            }
            else
            {
                boolean returnValue = AnimationManager_StartAnimation(uiWidget->animation1, RUN_ONE_CYCLE, STOP_AT_END_OF_CYCLE);
                UNUSED(returnValue);
            }
        }

        if(uiWidget->animation2 != (animation_mapping_t*)0)
        {
            if (config->animation2Triggers != NULL)
            {
                boolean returnValue = AnimationManager_StartAnimation(uiWidget->animation2, config->animation2TriggersProperties->onLoadBehavior, config->animation2TriggersProperties->onTriggerInactiveBehavior);
                UNUSED(returnValue);
            }
            else
            {
                boolean returnValue = AnimationManager_StartAnimation(uiWidget->animation2, RUN_ONE_CYCLE, STOP_AT_END_OF_CYCLE);
                UNUSED(returnValue);
            }
        }
    }
}

boolean DynamicImageWidget_NeedsRedraw(const ui_widget_t* uiWidget)
{
    boolean needsRedraw = FALSE;
    const DynamicImageCfg_t* dynamicImageConfig = getConfig(uiWidget->widgetID);
    if (dynamicImageConfig == NULL)
    {
        Logger_Error("DynamicImageWidget", "Configuration for widget ID:%d widget not found!", uiWidget->widgetID);
    }
    else
    {
        if ((uiWidget->animation1 != (animation_mapping_t*)0) || (uiWidget->animation2 != (animation_mapping_t*)0))
        {
            if (uiWidget->animation1 != (animation_mapping_t*)0)
            {
                if (dynamicImageConfig->animation1Triggers != NULL)
                {
                    if (animationTriggered(dynamicImageConfig->animation1Triggers, dynamicImageConfig->animation1TriggersCount) == TRUE)
                    {
                        uiWidget->animation1->triggerActive = TRUE;
                        uiWidget->animation1->animationUpdated = AnimationManager_AnimationNeedsRedraw(uiWidget->animation1, TRUE, dynamicImageConfig->animation1TriggersProperties->onTriggerActiveBehavior, dynamicImageConfig->animation1TriggersProperties->onTriggerInactiveBehavior);
                    }
                    else
                    {
                        uiWidget->animation1->triggerActive = FALSE;
                        uiWidget->animation1->animationUpdated = AnimationManager_AnimationNeedsRedraw(uiWidget->animation1, FALSE, dynamicImageConfig->animation1TriggersProperties->onTriggerActiveBehavior, dynamicImageConfig->animation1TriggersProperties->onTriggerInactiveBehavior);
                    }
                }
                else
                {
                    uiWidget->animation1->triggerActive = TRUE;
                    uiWidget->animation1->animationUpdated = AnimationManager_AnimationNeedsRedraw(uiWidget->animation1, TRUE, START_FROM_BEGINNING, STOP_AT_END_OF_CYCLE);
                }
                needsRedraw |= uiWidget->animation1->animationUpdated;
            }

            if (uiWidget->animation2 != (animation_mapping_t*)0)
            {
                if (dynamicImageConfig->animation2Triggers != NULL)
                {
                    if (animationTriggered(dynamicImageConfig->animation2Triggers, dynamicImageConfig->animation2TriggersCount) == TRUE)
                    {
                        uiWidget->animation2->triggerActive = TRUE;
                        uiWidget->animation2->animationUpdated = AnimationManager_AnimationNeedsRedraw(uiWidget->animation2, TRUE, dynamicImageConfig->animation2TriggersProperties->onTriggerActiveBehavior, dynamicImageConfig->animation2TriggersProperties->onTriggerInactiveBehavior);
                    }
                    else
                    {
                        uiWidget->animation2->triggerActive = FALSE;
                        uiWidget->animation2->animationUpdated = AnimationManager_AnimationNeedsRedraw(uiWidget->animation2, FALSE, dynamicImageConfig->animation2TriggersProperties->onTriggerActiveBehavior, dynamicImageConfig->animation2TriggersProperties->onTriggerInactiveBehavior);
                    }
                }
                else
                {
                    uiWidget->animation2->triggerActive = TRUE;
                    uiWidget->animation2->animationUpdated = AnimationManager_AnimationNeedsRedraw(uiWidget->animation2, TRUE, START_FROM_BEGINNING, STOP_AT_END_OF_CYCLE);
                }
                needsRedraw |= uiWidget->animation2->animationUpdated;
            }
        }

        uint32 oldImageID = dynamicImageConfig->currentImageID;
        uint32 currentImageID = readCurrentValue(dynamicImageConfig);

        if (oldImageID != currentImageID)
        {
            needsRedraw = TRUE;
        }
    }

    return needsRedraw;
}

void DynamicImageWidget_Redraw(const display_area_definition_t* area, const ui_widget_t* uiWidget)
{
    (void)area;
    Logger_Info("DynamicImageWidget", "DynamicImageWidget_Redraw");

    DynamicImageCfg_t* config = getConfig(uiWidget->widgetID);
    if (config == NULL)
    {
        Logger_Error("DynamicImageWidget", "Configuration for widget ID:%d widget not found!", uiWidget->widgetID);
    }
    else
    {
        if((uiWidget->animation1 != (animation_mapping_t*)0) || (uiWidget->animation2 != (animation_mapping_t*)0))
        {
            if(uiWidget->animation1 != (animation_mapping_t*)0)
            {
                if (uiWidget->animation1->animationUpdated == TRUE)
                {
                    if (config->animation1Triggers != NULL)
                    {
                        AnimationManager_RefreshAnimation(uiWidget->animation1, uiWidget->animation1->triggerActive);
                    }
                    else
                    {
                        AnimationManager_RefreshAnimation(uiWidget->animation1, uiWidget->animation1->triggerActive);
                    }
                    uiWidget->animation1->animationUpdated = FALSE;
                }
            }

            if(uiWidget->animation2 != (animation_mapping_t*)0)
            {
                if (uiWidget->animation2->animationUpdated == TRUE)
                {
                    if (config->animation2Triggers != NULL)
                    {
                        AnimationManager_RefreshAnimation(uiWidget->animation2, uiWidget->animation2->triggerActive);
                    }
                    else
                    {
                        AnimationManager_RefreshAnimation(uiWidget->animation2, uiWidget->animation2->triggerActive);
                    }
                    uiWidget->animation2->animationUpdated = FALSE;
                }
            }
        }

        uint32 oldImageID = config->currentImageID;
        config->currentImageID = readCurrentValue(config);

        if (oldImageID != config->currentImageID)
        {
            Logger_Info("DynamicImageWidget", "Displaying Image: %d", config->currentImageID);
            displayImage(uiWidget, config);
        }
    }
}


/****************************** Local Functions  *****************************/

static DynamicImageCfg_t* getConfig(uint32 widgetID)
{
    DynamicImageCfg_t* returnValue = NULL;

    for (uint32 i=0; i<DynamicImageConfigCount; ++i)
    {
        if (DynamicImageConfigs[i].widgetID == widgetID)
        {
            returnValue = &DynamicImageConfigs[i];
            break;
        }
    }

    return returnValue;
}

static uint32 readCurrentValue(const DynamicImageCfg_t* config)
{
    uint8 status;
    uint32 imageID;
    config->readImageID(&imageID, &status);

    return imageID;
}

static void displayImage(const ui_widget_t* uiWidget, const DynamicImageCfg_t* config)
{
    const image_t* image = ImageManager_GetImage(config->currentImageID);

    if ((image != NULL) && (ContentsManager_ValidImageSize((uint8)uiWidget->layerID, image->width, image->height) == TRUE))
    {
        //lint --e(9005) [Reference of an const image to an adress pointer]
        ContentsManager_SetLayerFrameBuffer((uint8)uiWidget->layerID, (uint8*)image->bitmap);
        ContentsManager_SetLayerEnabled((uint8)uiWidget->layerID, TRUE);
    }
    else
    {
        ContentsManager_SetLayerEnabled((uint8)uiWidget->layerID, FALSE);
    }
}

static boolean animationTriggered(const signal_transition_t *triggers, uint8 triggerCount)
{
    boolean conditionIsTrue = FALSE;
    for (uint8 i = 0; i < triggerCount; ++i)
    {
        DynamicWidgetValue firstValue;
        uint8 signalStatus = 0;
        triggers[i].firstConditionGCLSink(&firstValue, &signalStatus);
        sint32 firstSint32Value = getSint32ValueFromDynamicWidgetValue(&firstValue);
        conditionIsTrue = checkCondition(firstSint32Value, triggers[i].firstConditionCompareOperator, triggers[i].firstConditionCompareValue);

        if (triggers[i].connective != NONE)
        {
            boolean firstConditionIsTrue = conditionIsTrue;
            DynamicWidgetValue secondValue;
            triggers[i].secondConditionGCLSink(&secondValue, &signalStatus);
            sint32 secondSint32Value = getSint32ValueFromDynamicWidgetValue(&secondValue);
            boolean secondConditionIsTrue = checkCondition(secondSint32Value, triggers[i].secondConditionCompareOperator, triggers[i].secondConditionCompareValue);

            conditionIsTrue = compareConditions(firstConditionIsTrue, secondConditionIsTrue, triggers[i].connective);
        }

        if (conditionIsTrue == TRUE)
        {
            break;
        }
    }
    return conditionIsTrue;
}

static boolean compareConditions(boolean firstCondition, boolean secondCondition, transition_connective_t connective)
{
    boolean retVal = FALSE;
    switch (connective)
    {
    case AND:
        retVal = ((firstCondition == TRUE) && (secondCondition == TRUE)) ? TRUE : FALSE;
        break;
    case XOR:
        retVal = (firstCondition != secondCondition) ? TRUE : FALSE;
        break;
    default:
        retVal = FALSE;
        break;
    }
    return retVal;
}

static boolean checkCondition(sint32 value, transition_condition_t compareCondition, sint32 compareValue)
{
    boolean retVal = FALSE;
    switch (compareCondition)
    {
        case EQUALS_TO:
            retVal = (value == compareValue) ? TRUE : FALSE;
            break;
        case GREATER_THAN:
            retVal = (value > compareValue) ? TRUE : FALSE;
            break;
        case EQUALS_OR_GREATER_THAN:
            retVal = (value >= compareValue) ? TRUE : FALSE;
            break;
        case LESS_THAN:
            retVal = (value < compareValue) ? TRUE : FALSE;
            break;
        case EQUALS_OR_LESS_THAN:
            retVal = (value <= compareValue) ? TRUE : FALSE;
            break;
        default:
            retVal = FALSE;
            break;
    }

    return retVal;
}

static sint32 getSint32ValueFromDynamicWidgetValue(const DynamicWidgetValue* value)
{
    sint32 returnValue = 0;

    switch (value->DataType)
    {
        case STD_DATATYPE_UINT8:
            returnValue = (sint32)value->Value.U8;
            break;
        case STD_DATATYPE_UINT16:
            returnValue = (sint32)value->Value.U16;
            break;
        case STD_DATATYPE_UINT32:
            returnValue = (sint32)value->Value.U32;
            break;
        case STD_DATATYPE_INT8:
            returnValue = (sint32)value->Value.S8;
            break;
        case STD_DATATYPE_INT16:
            returnValue = (sint32)value->Value.S16;
            break;
        case STD_DATATYPE_INT32:
            returnValue = value->Value.S32;
            break;
        default:
            // Do nothing
            break;
    }

    return returnValue;
}

//lint -restore
