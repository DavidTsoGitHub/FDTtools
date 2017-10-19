#ifndef COLUMNLEVELWIDGETCFG_H
#define COLUMNLEVELWIDGETCFG_H

/********************************* Includes **********************************/
#include "StandardTypes.h"

/***************************** Exported Types ****************************/
typedef struct
{
    uint32  widgetID;
    uint8   currentValue;
    boolean hasFocus;
    boolean needsRedraw;
    uint16  spacing;
    uint8   numberOfBlocks;
    uint8   min;
    uint8   max;
    uint8   minBlockHeight;
    uint8   maxBlockHeight;
    uint8   selectedItemFGColor;
    uint8   selectedItemBGColor;
    void (*readCurrentColumnLevel)(uint8* value, uint8* signalStatus);
    void (*writeCurrentColumnLevel)(uint8 value);
} ColumnLevelWidgetCfg_t;

/********************************* Variables *********************************/
extern const uint32 ColumnLevelWidgetConfigCount;
extern ColumnLevelWidgetCfg_t* const ColumnLevelWidgetConfigs;

/***************************** Exported Functions ****************************/

#endif // COLUMNLEVELWIDGETCFG_H
