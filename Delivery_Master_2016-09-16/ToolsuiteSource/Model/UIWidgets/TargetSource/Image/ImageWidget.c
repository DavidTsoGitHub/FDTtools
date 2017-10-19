//lint -save
/********************************* Includes **********************************/
#include "ImageWidget.h"
#include "ImageWidgetCfg.h"
#include "ContentsManager.h"
#include "ImageManager.h"
#include "Logger.h"

/**************************** Constants and Types ****************************/
const ui_widget_funcs ImageWidgetFuncs =
{
    .draw = Image_Draw,
    .needsRedraw = Image_NeedsRedraw,
    .redraw = Image_Redraw,
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
static ImageCfg_t *getImageConfiguration(uint32 widgetID);
static boolean animationTriggered(const signal_transition_t *triggers, uint8 triggerCount);
static boolean compareConditions(boolean firstCondition, boolean secondCondition, transition_connective_t connective);
static boolean checkCondition(sint32 value, transition_condition_t compareCondition, sint32 compareValue);
static sint32 getSint32ValueFromDynamicWidgetValue(const DynamicWidgetValue* value);

/***************************** Exported Functions ****************************/
void Image_Draw(const display_area_definition_t* area, const ui_widget_t* uiWidget)
{
    (void) area;
    ImageCfg_t* imageConfig = getImageConfiguration(uiWidget->widgetID);

    if (imageConfig != NULL)
    {
        boolean hasAnimations = FALSE;
        if(uiWidget->animation1 != (animation_mapping_t*)0)
        {
            hasAnimations = TRUE;
            if (imageConfig->animation1Triggers != NULL)
            {
                imageConfig->layerEnabled = AnimationManager_StartAnimation(uiWidget->animation1, imageConfig->animation1TriggersProperties->onLoadBehavior, imageConfig->animation1TriggersProperties->onTriggerInactiveBehavior);
            }
            else
            {
                imageConfig->layerEnabled = AnimationManager_StartAnimation(uiWidget->animation1, RUN_ONE_CYCLE, STOP_AT_END_OF_CYCLE);
            }
        }

        if(uiWidget->animation2 != (animation_mapping_t*)0)
        {
            hasAnimations = TRUE;
            if (imageConfig->animation2Triggers != NULL)
            {
                imageConfig->layerEnabled |= AnimationManager_StartAnimation(uiWidget->animation2, imageConfig->animation2TriggersProperties->onLoadBehavior, imageConfig->animation2TriggersProperties->onTriggerInactiveBehavior);
            }
            else
            {
                imageConfig->layerEnabled |= AnimationManager_StartAnimation(uiWidget->animation2, RUN_ONE_CYCLE, STOP_AT_END_OF_CYCLE);
            }
        }

        const image_t* image = ImageManager_GetImage(imageConfig->imageID);

        if (image != NULL)
        {
            //lint --e(9005) [Reference of an const image to an adress pointer]
            ContentsManager_SetLayerFrameBuffer((uint8)uiWidget->layerID, (uint8*)image->bitmap);

            if (hasAnimations == TRUE)
            {
                ContentsManager_SetLayerEnabled((uint8)uiWidget->layerID, imageConfig->layerEnabled);
            }
            else
            {
                ContentsManager_SetLayerEnabled((uint8)uiWidget->layerID, TRUE);
            }
        }
        else
        {
            ContentsManager_SetLayerEnabled((uint8)uiWidget->layerID, FALSE);
        }
    }
}

boolean Image_NeedsRedraw(const ui_widget_t* uiWidget)
{
    boolean needsRedraw = FALSE;
    const ImageCfg_t* imageConfig = getImageConfiguration(uiWidget->widgetID);
    if (imageConfig == NULL)
    {
        Logger_Error("ImageWidget", "Configuration for widget ID:%d widget not found!", uiWidget->widgetID);
    }
    else
    {
        if ((uiWidget->animation1 != (animation_mapping_t*)0) || (uiWidget->animation2 != (animation_mapping_t*)0))
        {
            if (uiWidget->animation1 != (animation_mapping_t*)0)
            {
                if (imageConfig->animation1Triggers != NULL)
                {
                    if (animationTriggered(imageConfig->animation1Triggers, imageConfig->animation1TriggersCount) == TRUE)
                    {
                        uiWidget->animation1->triggerActive = TRUE;
                        uiWidget->animation1->animationUpdated = AnimationManager_AnimationNeedsRedraw(uiWidget->animation1, TRUE, imageConfig->animation1TriggersProperties->onTriggerActiveBehavior, imageConfig->animation1TriggersProperties->onTriggerInactiveBehavior);
                    }
                    else
                    {
                        uiWidget->animation1->triggerActive = FALSE;
                        uiWidget->animation1->animationUpdated = AnimationManager_AnimationNeedsRedraw(uiWidget->animation1, FALSE, imageConfig->animation1TriggersProperties->onTriggerActiveBehavior, imageConfig->animation1TriggersProperties->onTriggerInactiveBehavior);
                    }
                }
                else
                {
                    uiWidget->animation1->triggerActive = TRUE;
                    uiWidget->animation1->animationUpdated = AnimationManager_AnimationNeedsRedraw(uiWidget->animation1, TRUE, START_FROM_BEGINNING, STOP_AT_END_OF_CYCLE);
                }
                needsRedraw = uiWidget->animation1->animationUpdated;
            }

            if (uiWidget->animation2 != (animation_mapping_t*)0)
            {
                if (imageConfig->animation2Triggers != NULL)
                {
                    if (animationTriggered(imageConfig->animation2Triggers, imageConfig->animation2TriggersCount) == TRUE)
                    {
                        uiWidget->animation2->triggerActive = TRUE;
                        uiWidget->animation2->animationUpdated = AnimationManager_AnimationNeedsRedraw(uiWidget->animation2, TRUE, imageConfig->animation2TriggersProperties->onTriggerActiveBehavior, imageConfig->animation2TriggersProperties->onTriggerInactiveBehavior);
                    }
                    else
                    {
                        uiWidget->animation2->triggerActive = FALSE;
                        uiWidget->animation2->animationUpdated = AnimationManager_AnimationNeedsRedraw(uiWidget->animation2, FALSE, imageConfig->animation2TriggersProperties->onTriggerActiveBehavior, imageConfig->animation2TriggersProperties->onTriggerInactiveBehavior);
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
    }

    return needsRedraw;
}

void Image_Redraw(const display_area_definition_t* area, const ui_widget_t* uiWidget)
{
    (void)area;

    ImageCfg_t* imageConfig = getImageConfiguration(uiWidget->widgetID);
    if (imageConfig == NULL)
    {
        Logger_Error("ImageWidget", "Configuration for widget ID:%d widget not found!", uiWidget->widgetID);
    }
    else
    {
        if(uiWidget->animation1 != (animation_mapping_t*)0)
        {
            if (uiWidget->animation1->animationUpdated == TRUE)
            {
                if (imageConfig->animation1Triggers != NULL)
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
                if (imageConfig->animation2Triggers != NULL)
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

        if (imageConfig->layerEnabled == FALSE)
        {
            const image_t* image = ImageManager_GetImage(imageConfig->imageID);

            if ((image != NULL))
            {
                //lint --e(9005) [Reference of an const image to an adress pointer]
                ContentsManager_SetLayerFrameBuffer((uint8)uiWidget->layerID, (uint8*)image->bitmap);
                ContentsManager_SetLayerEnabled((uint8)uiWidget->layerID, TRUE);
                imageConfig->layerEnabled = TRUE;
            }
            else
            {
                ContentsManager_SetLayerEnabled((uint8)uiWidget->layerID, FALSE);
            }
        }
    }
}

/****************************** Local Functions  *****************************/
static ImageCfg_t* getImageConfiguration(uint32 widgetID)
{
    ImageCfg_t* returnValue = NULL;

    for (uint32 i = 0; i < ImageConfigCount; ++i)
    {
        if (ImageConfigs[i].widgetID == widgetID)
        {
            returnValue = &ImageConfigs[i];
            break;
        }
    }

    return returnValue;
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
