#ifndef BLOCKBARWIDGETCFG_H
#define BLOCKBARWIDGETCFG_H

/********************************* Includes **********************************/
#include "StandardTypes.h"

/***************************** Exported Types ****************************/
typedef struct
{
    uint32 widgetID;
    DynamicWidgetValue currentValue;
    uint32 lastRenderedValue;
    uint16 spacing;
    uint8 numberOfBlocks;
    uint32 min;
    uint32 max;
    void (*gclSink)(DynamicWidgetValue* value, uint8* signalStatus);
} BlockBarWidgetCfg_t;

/********************************* Variables *********************************/
extern const uint32 BlockBarWidgetConfigCount;
extern BlockBarWidgetCfg_t* const BlockBarWidgetConfigs;

/***************************** Exported Functions ****************************/
#endif //BLOCKBARWIDGETCFG_H
