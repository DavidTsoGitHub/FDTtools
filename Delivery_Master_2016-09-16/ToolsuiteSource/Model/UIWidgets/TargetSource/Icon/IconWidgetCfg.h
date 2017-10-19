#ifndef ICONWIDGETCFG_H
#define ICONWIDGETCFG_H

/********************************* Includes **********************************/
#include "StandardTypes.h"

/***************************** Exported Types ****************************/
typedef struct
{
   uint32 widgetID;
   uint32 iconID;
} IconCfg_t;

/********************************* Variables *********************************/
extern const uint32 IconConfigCount;
extern const IconCfg_t* const IconConfigs;

/***************************** Exported Functions ****************************/

#endif // ICONWIDGETCFG_H
