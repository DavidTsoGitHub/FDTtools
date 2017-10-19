#ifndef SIGNALTRIGGERWIDGETCFG_H
#define SIGNALTRIGGERWIDGETCFG_H

/********************************* Includes **********************************/
#include "StandardTypes.h"

/***************************** Exported Types ****************************/
typedef struct
{
   uint32 widgetID;
   void (*writeSignal)(void);
} SignalTriggerCfg_t;

/********************************* Variables *********************************/
extern const uint32 SignalTriggerConfigCount;
extern const SignalTriggerCfg_t* const SignalTriggerConfigs;

/***************************** Exported Functions ****************************/

#endif // SIGNALTRIGGERWIDGETCFG_H
