#ifndef DYNAMICTEXTLABELWIDGET_H
#define DYNAMICTEXTLABELWIDGET_H

/********************************* Includes **********************************/
#include "UIWidget.h"

/***************************** Exported Types ****************************/
extern const ui_widget_funcs DynamicTextLabelWidgetFuncs;

/********************************* Variables *********************************/

/***************************** Exported Functions ****************************/
void DynamicTextLabel_Draw(const display_area_definition_t* area, const ui_widget_t* uiWidget);
boolean DynamicTextLabel_NeedsRedraw(const ui_widget_t* uiWidget);

/*****************************************************************************/

#endif //DYNAMICTEXTLABELWIDGET_H
