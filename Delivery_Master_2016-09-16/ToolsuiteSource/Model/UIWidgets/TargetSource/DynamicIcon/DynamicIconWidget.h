#ifndef DYNAMICICONWIDGET_H
#define DYNAMICICONWIDGET_H

/********************************* Includes **********************************/
#include "UIWidget.h"

/***************************** Exported Types ****************************/
extern const ui_widget_funcs DynamicIconWidgetFuncs;

/***************************** Exported Functions ****************************/
void DynamicIconWidget_Draw(const display_area_definition_t* area, const ui_widget_t* uiWidget);
boolean DynamicIconWidget_NeedsRedraw(const ui_widget_t* uiWidget);

#endif // DYNAMICICONWIDGET_H
