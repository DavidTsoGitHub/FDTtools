#ifndef DYNAMICVALUELABELWIDGETCFG_H
#define DYNAMICVALUELABELWIDGETCFG_H

/********************************* Includes **********************************/
#include "StandardTypes.h"
#include "Font.h"

/***************************** Exported Types ****************************/
#define DYNAMIC_VALUE_LABEL_MAX_LENGTH    12U

typedef struct
{
   uint32 widgetID;
   void (*gclSink)(DynamicWidgetValue* value, uint8* signalStatus);
   wchar currentString[DYNAMIC_VALUE_LABEL_MAX_LENGTH];
   DynamicWidgetValue currentValue;
   const font_t* font;
   sint8 scaling;
   sint8 decimals;
   sint8 maximumNbrOfDigits;
} DynamicValueLabelCfg_t;

/********************************* Variables *********************************/
extern const uint32 DynamicValueLabelConfigCount;
extern DynamicValueLabelCfg_t* const DynamicValueLabelConfigs;

/***************************** Exported Functions ****************************/

/*****************************************************************************/

#endif //DYNAMICVALUELABELWIDGETCFG_H
