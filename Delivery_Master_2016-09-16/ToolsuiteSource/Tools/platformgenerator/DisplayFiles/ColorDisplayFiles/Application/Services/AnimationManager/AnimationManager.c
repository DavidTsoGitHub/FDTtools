#include "AnimationManager.h"
#include "ContentsManager.h"

static boolean updateAnimation(boolean activeTrigger, animation_mapping_t* mapping, animation_on_trigger_inactive_behavior_t onTriggerInactiveBehavior, animation_on_trigger_active_behavior_t startBehavior);
static void updateStartIndex(const animation_t* animation, animation_mapping_t* mapping, animation_on_trigger_active_behavior_t startBehavior);
static uint8 checkIfRedrawNeeded(const animation_t *animation, animation_mapping_t* mapping);
static boolean animateLayer(const animation_t* animation, const animation_mapping_t *mapping);
static const animation_t* getAnimation(uint32 animationID);

boolean AnimationManager_StartAnimation(animation_mapping_t* mapping, animation_on_load_behavior_t onLoadBehavior, animation_on_trigger_inactive_behavior_t onTriggerInactiveBehavior)
{
    boolean layerActive = FALSE;

    if(mapping != NULL)
    {
        mapping->allreadyTriggered = FALSE;
        const animation_t* animation = getAnimation(mapping->animationID);

        switch (onTriggerInactiveBehavior)
        {
            case STOP_AT_DEFAULT_FRAME:
                mapping->endIndex = animation->defaultStep;
                break;
            case STOP:
            case STOP_AT_END_OF_CYCLE:
            case CONTINUE:
            default:
                mapping->endIndex = (sint16)animation->stepCount - 1;
                break;
        }

        switch (onLoadBehavior)
        {
            case RUN_ONE_CYCLE:
            {
                mapping->index = mapping->startIndex;
                mapping->step = animation->steps[mapping->index].step;
                uint8 returnValue = animateLayer(animation, mapping);
                UNUSED(returnValue);
                mapping->step += 1;

                mapping->animationRunning = TRUE;
                mapping->onLoadRunOneCycleRunning = TRUE;
                mapping->lastIndication = TRUE;
                layerActive = TRUE;
                break;
            }

            case SHOW_DEFAULT:
            {
                if ((mapping->startIndex == 1) && (animation->type != FRAMEBYFRAME_ANIMATION))
                {
                    // If 1 than the default index is set to the last index in the animation
                    mapping->startIndex = animation->stepCount - 1;
                }
                mapping->index = mapping->startIndex;
                mapping->step = animation->steps[mapping->index].step;
                uint8 returnValue = animateLayer(animation, mapping);

                UNUSED(returnValue);
                layerActive = TRUE;
                break;
            }
            case DO_NOTHING:
            default:
                // Do nothing
                break;
        }
    }

    return layerActive;
}

boolean AnimationManager_AnimationNeedsRedraw(animation_mapping_t* mapping, boolean activeTrigger, animation_on_trigger_active_behavior_t onTriggerActiveBehavior, animation_on_trigger_inactive_behavior_t onTriggerInactiveBehavior)
{
    boolean needsRedraw = FALSE;

    if (updateAnimation(activeTrigger, mapping, onTriggerInactiveBehavior, onTriggerActiveBehavior) == TRUE)
    {
        mapping->animationRunning = TRUE;
        const animation_t* animation = getAnimation(mapping->animationID);

        uint8 returnValue = checkIfRedrawNeeded(animation, mapping);

        if (returnValue != ANIM_NOP)
        {
            needsRedraw = TRUE;
        }
        else
        {
            if (mapping->incrementStep == TRUE)
            {
                mapping->step += 1;
            }
            else
            {
                mapping->step -= 1;
            }
        }
    }
    return needsRedraw;
}

void AnimationManager_RefreshAnimation(animation_mapping_t* mapping, boolean activeTrigger)
{
    //boolean changed = FALSE;
    uint8 returnValue = ANIM_NOP;

    const animation_t* animation = getAnimation(mapping->animationID);
    boolean noop = animateLayer(animation, mapping);

    if (noop == FALSE)
    {
        if (mapping->incrementStep == TRUE)
        {
            if(mapping->index == mapping->endIndex)
            {
                returnValue = ANIM_UPDATED;
                if ((animation->repeat != BOUNCE_ANIMATION) && (animation->repeat != BOUNCE_LOOP_ANIMATION))
                {
                    returnValue = ANIM_END;
                    if (mapping->startIndex == mapping->endIndex)
                    {
                        mapping->index = mapping->startIndex + 1;
                    }
                    else
                    {
                        mapping->index = mapping->startIndex;
                    }
                }
                else
                {
                    mapping->index = mapping->endIndex - 1;
                    mapping->incrementStep = FALSE;
                }
            }
            else
            {
                mapping->index += 1;

                if(mapping->index > (animation->stepCount - 1))
                {
                    if ((animation->repeat != BOUNCE_ANIMATION) && (animation->repeat != BOUNCE_LOOP_ANIMATION))
                    {
                        mapping->index = 0;
                        mapping->step = 0 - (animation->steps[1].step - animation->steps[0].step);
                    }
                    else
                    {
                        mapping->index = mapping->endIndex;
                        mapping->incrementStep = FALSE;
                    }
                }
            }
        }
        else
        {
            if(mapping->index <= 0)
            {
                returnValue = ANIM_END;
                mapping->incrementStep = TRUE;
                mapping->index += 1;
                mapping->step = animation->steps[0].step + 1;
            }
            else
            {
                returnValue = ANIM_UPDATED;
                mapping->index -= 1;
            }
        }

        if(returnValue == ANIM_UPDATED)
        {
            if (mapping->incrementStep == TRUE)
            {
                mapping->step += 1;
            }
            else
            {
                mapping->step -= 1;
            }
        }
        else if(returnValue == ANIM_END)
        {
            if(animation->repeat == LOOP_ANIMATION)
            {
                if (mapping->startIndex > 0)
                {
                    mapping->step = animation->steps[mapping->index-1].step + 1;
                }
                else
                {
                    if (animation->stepCount > 1)
                    {
                        mapping->step = 0 - ((animation->steps[1].step - animation->steps[0].step) - 1);
                    }
                }

                if (activeTrigger == FALSE)
                {
                    mapping->animationRunning = FALSE;
                }
            }
            else if (((animation->repeat == ONESHOT_ANIMATION) || (animation->repeat == BOUNCE_ANIMATION)) && (mapping->onLoadRunOneCycleRunning == FALSE))
            {
                mapping->allreadyTriggered = TRUE;
                mapping->animationRunning = FALSE;
            }
            else if (mapping->onLoadRunOneCycleRunning == TRUE)
            {
                mapping->allreadyTriggered = TRUE;
                mapping->onLoadRunOneCycleRunning = FALSE;
                mapping->animationRunning = FALSE;
            }
            else if (animation->repeat == BOUNCE_LOOP_ANIMATION)
            {
                if (activeTrigger == FALSE)
                {
                    mapping->animationRunning = FALSE;
                }
            }
            else
            {
                // Do nothing
            }
        }
        else
        {
            if (mapping->incrementStep == TRUE)
            {
                mapping->step += 1;
            }
            else
            {
                mapping->step -= 1;
            }
        }
    }
}

static const animation_t* getAnimation(uint32 animationID)
{
    const animation_t* returnValue = NULL;

    for (uint32 i = 0u; i < NUMBER_OF_ANIMATIONS; ++i)
    {
        if (animations[i].id == animationID)
        {
            returnValue = &animations[i];
            break;
        }
    }

    return returnValue;
}

static uint8 checkIfRedrawNeeded(const animation_t* animation,animation_mapping_t* mapping)
{
    uint8 returnValue = ANIM_NOP;

    if (mapping->incrementStep == TRUE)
    {
        //Fault check
        if(mapping->index > (animation->stepCount - 1))
        {
            if ((animation->repeat != BOUNCE_ANIMATION) && (animation->repeat != BOUNCE_LOOP_ANIMATION))
            {
                mapping->index = 0;
                mapping->step = 0;
            }
            else
            {
                mapping->index = mapping->endIndex;
                mapping->incrementStep = FALSE;
            }
        }
        else if (mapping->step >= animation->steps[mapping->index].step)
        {
            returnValue = ANIM_UPDATED;
        }
        else
        {
            // Do nothing
        }
    }
    else
    {
        //Fault check
        if(mapping->index < 0)
        {
            returnValue = ANIM_END;
            mapping->incrementStep = TRUE;
            mapping->index += 1 ;
        }
        else if (mapping->step <= animation->steps[mapping->index].step)
        {
            returnValue = ANIM_UPDATED;
        }
        else
        {
            // Do nothing
        }
    }

    return returnValue;
}

static boolean updateAnimation(boolean activeTrigger, animation_mapping_t* mapping, animation_on_trigger_inactive_behavior_t onTriggerInactiveBehavior, animation_on_trigger_active_behavior_t startBehavior)
{
    boolean update = TRUE;
    if ((mapping != NULL) && ((activeTrigger == TRUE) || (mapping->animationRunning == TRUE)))
    {
        if ((mapping->animationRunning == TRUE) && (activeTrigger == FALSE) && (onTriggerInactiveBehavior == STOP) && (mapping->onLoadRunOneCycleRunning != TRUE))
        {
            mapping->animationRunning = FALSE;
            update = FALSE;
        }
        else
        {
            const animation_t* animation = getAnimation(mapping->animationID);

            if (animation != NULL)
            {
                if ((animation->repeat == ONESHOT_ANIMATION) || (animation->repeat == BOUNCE_ANIMATION))
                {
                    if ((activeTrigger == TRUE) && (mapping->lastIndication == FALSE))
                    {
                        updateStartIndex(animation, mapping, startBehavior);
                        mapping->lastIndication = TRUE;
                    }

                    if (mapping->allreadyTriggered == FALSE)
                    {
                        update = TRUE;
                    }
                    else
                    {
                        update = FALSE;
                    }

                }
                else if (mapping->animationRunning == FALSE)
                {
                    mapping->animationRunning = TRUE;
                    updateStartIndex(animation, mapping, startBehavior);
                    update = TRUE;
                }
                else
                {
                    // Do Nothing;
                }
            }
        }
    }
    else
    {
        if (mapping != NULL)
        {
            if ((activeTrigger == FALSE) && (mapping->lastIndication == TRUE))
            {
                mapping->lastIndication = FALSE;
                mapping->allreadyTriggered = FALSE;
            }
        }
        update = FALSE;
    }
    return update;
}

static void updateStartIndex(const animation_t* animation, animation_mapping_t* mapping, animation_on_trigger_active_behavior_t startBehavior)
{
    switch (startBehavior)
    {
        case START_FROM_BEGINNING:
        case START_FROM_DEFAULT_FRAME:
            if ((animation->repeat == BOUNCE_ANIMATION) || (animation->repeat == BOUNCE_LOOP_ANIMATION))
            {
                mapping->index = 0;
                mapping->step = 0;
                mapping->incrementStep = TRUE;
            }
            else
            {
                mapping->index = mapping->startIndex;
                mapping->step = animation->steps[mapping->index].step;
            }
            break;
        case CONTINUE_FROM_CURRENT_FRAME:
        default:
            // Do nothing
            break;
    }
}

static boolean animateLayer(const animation_t* animation, const animation_mapping_t* mapping)
{
    boolean noop = FALSE;
    switch (animation->type)
    {
        case FRAMEBYFRAME_ANIMATION:
            animation->AnimationSourceFunction(animation->steps[mapping->index].value.imageID);
            break;
        case OPACITY_ANIMATION:
            ContentsManager_SetLayerOpacity(mapping->layerID, animation->steps[mapping->index].value.opacity);
            break;
        case TRANSLATION_ANIMATION:
            ContentsManager_SetLayerRelativePosition(mapping->layerID, animation->steps[mapping->index].value.position.x, animation->steps[mapping->index].value.position.y);
            break;
        default:
            noop = TRUE;
            break;
    }
    return noop;
}
