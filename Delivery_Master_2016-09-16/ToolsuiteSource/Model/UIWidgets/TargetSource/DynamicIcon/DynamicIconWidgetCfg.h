#ifndef DYNAMICICONWIDGETCFG_H
#define DYNAMICICONWIDGETCFG_H

/********************************* Includes **********************************/
#include "StandardTypes.h"

/**************************** Constants and Types ****************************/
typedef struct
{
   uint32 widgetID;
   uint32 currentIconID;
   void (*readIconID)(uint32* iconId, uint8* signalStatus);
} DynamicIconCfg_t;

/********************************* Variables *********************************/
extern const uint32 DynamicIconConfigCount;
extern DynamicIconCfg_t* const DynamicIconConfigs;



#endif // DYNAMICICONWIDGETCFG_H
