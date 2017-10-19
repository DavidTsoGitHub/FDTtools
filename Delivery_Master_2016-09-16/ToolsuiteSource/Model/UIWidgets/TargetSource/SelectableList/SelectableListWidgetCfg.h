#ifndef SELECTABLELISTWIDGETCFG_H
#define SELECTABLELISTWIDGETCFG_H

/********************************* Includes **********************************/
#include "StandardTypes.h"
#include "TextRenderer.h"

/***************************** Exported Types ****************************/
typedef struct
{
   uint32 widgetID;
   uint8 currentValue;
   boolean hasFocus;
   boolean isSelectable;
   boolean needsRedraw;
   boolean showNumbering;
   uint8 displayItemCount;
   uint8 selectedItemFgColor;
   uint8 selectedItemBgColor;
   TextAlignment textAlignment;
   LabelMappings* mappings;
   void (*readLabelMappings)(LabelMappings** mapping, uint8* signalStatus);
   void (*readCurrentValue)(uint8* value, uint8* signalStatus);
   void (*writeSelectedValue)(uint8 value);
} SelectableListCfg_t;

/********************************* Variables *********************************/
extern const uint32 SelectableListConfigCount;
extern SelectableListCfg_t* const SelectableListConfigs;

/***************************** Exported Functions ****************************/

#endif // SELECTABLELISTWIDGETCFG_H
