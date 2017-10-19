#ifndef ANIMATIONMANAGER_H
#define ANIMATIONMANAGER_H

#include "StandardTypes.h"
#include "LayerConfiguration.h"
#include "Animations_cfg.h"

#define ANIM_NOP 1u
#define ANIM_UPDATED 2u
#define ANIM_END 3u

boolean AnimationManager_StartAnimation(animation_mapping_t* mapping, animation_on_load_behavior_t onLoadBehavior, animation_on_trigger_inactive_behavior_t onTriggerInactiveBehavior);
boolean AnimationManager_AnimationNeedsRedraw(animation_mapping_t* mapping, boolean activeTrigger, animation_on_trigger_active_behavior_t onTriggerActiveBehavior, animation_on_trigger_inactive_behavior_t onTriggerInactiveBehavior);
void AnimationManager_RefreshAnimation(animation_mapping_t* mapping, boolean activeTrigger);
//uint8 AnimationManager_AnimateLayer(const animation_t* animation, animation_mapping_t* mapping);

#endif // ANIMATIONMANAGER_H
