#ifndef DYNAMICLABELWIDGET_H
#define DYNAMICLABELWIDGET_H

/********************************* Includes **********************************/
#include "UIWidget.h"

/***************************** Exported Types ****************************/
extern const ui_widget_funcs DynamicLabelWidgetFuncs;

/***************************** Exported Functions ****************************/
void DynamicLabelWidget_Draw(const display_area_definition_t* area, const ui_widget_t* uiWidget);
boolean DynamicLabelWidget_NeedRedraw(const ui_widget_t* uiWidget);

#endif // DYNAMICLABELWIDGET_H
