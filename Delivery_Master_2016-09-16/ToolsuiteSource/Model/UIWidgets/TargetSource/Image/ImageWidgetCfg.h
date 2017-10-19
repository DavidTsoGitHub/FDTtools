#ifndef IMAGEWIDGETCFG_H
#define IMAGEWIDGETCFG_H

/********************************* Includes **********************************/
#include "StandardTypes.h"
#include "LayerConfiguration.h"

/***************************** Exported Types ****************************/
typedef struct
{
   uint32 widgetID;
   uint32 imageID;
   boolean layerEnabled;
   uint8 animation1TriggersCount;
   animation_triggers_properties_t* animation1TriggersProperties;
   signal_transition_t* animation1Triggers;
   uint8 animation2TriggersCount;
   animation_triggers_properties_t* animation2TriggersProperties;
   signal_transition_t* animation2Triggers;
} ImageCfg_t;

/********************************* Variables *********************************/
extern const uint32 ImageConfigCount;
extern ImageCfg_t* ImageConfigs;

/***************************** Exported Functions ****************************/

#endif // IMAGEWIDGETCFG_H
