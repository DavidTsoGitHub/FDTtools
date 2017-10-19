#ifndef DYNAMICLABELWIDGETCFG_H
#define DYNAMICLABELWIDGETCFG_H

/********************************* Includes **********************************/
#include "StandardTypes.h"
#include "TextRenderer.h"

/**************************** Constants and Types ****************************/
typedef struct
{
   uint32 widgetID;
   uint32 currentLabelID;
   TextAlignment alignment;
   void (*readLabelID)(uint32* labelId, uint8* signalStatus);
} DynamicLabelCfg_t;

/********************************* Variables *********************************/
extern const uint32 DynamicLabelConfigCount;
extern DynamicLabelCfg_t* const DynamicLabelConfigs;

#endif // DYNAMICLABELWIDGETCFG_H
