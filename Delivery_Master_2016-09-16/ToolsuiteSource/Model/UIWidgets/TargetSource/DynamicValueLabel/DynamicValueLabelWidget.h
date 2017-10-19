#ifndef DYNAMICVALUELABELWIDGET_H
#define DYNAMICVALUELABELWIDGET_H

/********************************* Includes **********************************/
#include "UIWidget.h"

/***************************** Exported Types ****************************/
extern const ui_widget_funcs DynamicValueLabelWidgetFuncs;

/********************************* Variables *********************************/

/***************************** Exported Functions ****************************/
void DynamicValueLabel_Draw(const display_area_definition_t* area, const ui_widget_t* uiWidget);
boolean DynamicValueLabel_NeedsRedraw(const ui_widget_t* uiWidget);

/*****************************************************************************/

#endif //DYNAMICVALUELABELWIDGET_H
