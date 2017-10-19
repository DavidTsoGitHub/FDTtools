#ifndef LABELWIDGETCFG_H
#define LABELWIDGETCFG_H

/********************************* Includes **********************************/
#include "StandardTypes.h"

/***************************** Exported Types ****************************/
typedef struct
{
   uint32 widgetID;
   uint32 labelID;
} LabelCfg_t;

/********************************* Variables *********************************/
extern const uint32 LabelConfigCount;
extern const LabelCfg_t* const LabelConfigs;

/***************************** Exported Functions ****************************/

#endif // LABELWIDGETCFG_H
