#ifndef DYNAMICIMAGEWIDGETCFG_H
#define DYNAMICIMAGEWIDGETCFG_H

/********************************* Includes **********************************/
#include "StandardTypes.h"
#include "LayerConfiguration.h"

/**************************** Constants and Types ****************************/
typedef struct
{
   uint32 widgetID;
   uint32 currentImageID;
   void (*readImageID)(uint32* imageId, uint8* signalStatus);
   uint8 animation1TriggersCount;
   animation_triggers_properties_t* animation1TriggersProperties;
   signal_transition_t* animation1Triggers;
   uint8 animation2TriggersCount;
   animation_triggers_properties_t* animation2TriggersProperties;
   signal_transition_t* animation2Triggers;
} DynamicImageCfg_t;

/********************************* Variables *********************************/
extern const uint32 DynamicImageConfigCount;
extern DynamicImageCfg_t* const DynamicImageConfigs;

#endif //DYNAMICIMAGEWIDGETCFG_H

