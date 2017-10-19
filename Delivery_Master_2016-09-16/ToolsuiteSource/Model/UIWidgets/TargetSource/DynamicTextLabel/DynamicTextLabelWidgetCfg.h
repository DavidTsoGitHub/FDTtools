#ifndef DYNAMICTEXTLABELWIDGETCFG_H
#define DYNAMICTEXTLABELWIDGETCFG_H

/********************************* Includes **********************************/
#include "StandardTypes.h"
#include "Font.h"

/***************************** Exported Types ****************************/
#define DYNAMIC_TEXT_LABEL_MAX_LENGTH    32U

typedef struct
{
   uint32 widgetID;
   void (*gclSink)(wchar** text, uint8* signalStatus);
   wchar* currentString;
   wchar lastString[DYNAMIC_TEXT_LABEL_MAX_LENGTH];
   const font_t* font;
   boolean rightAligned;
} DynamicTextLabelCfg_t;

/********************************* Variables *********************************/
extern const uint32 DynamicTextLabelConfigCount;
extern DynamicTextLabelCfg_t* const DynamicTextLabelConfigs;

/***************************** Exported Functions ****************************/

/*****************************************************************************/

#endif //DYNAMICTEXTLABELWIDGETCFG_H
