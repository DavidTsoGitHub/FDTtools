#ifndef ICONWIDGET_H
#define ICONWIDGET_H

/********************************* Includes **********************************/
#include "UIWidget.h"

/***************************** Exported Types ****************************/
extern const ui_widget_funcs IconWidgetFuncs;

/********************************* Variables *********************************/

/***************************** Exported Functions ****************************/
void Icon_Draw(const display_area_definition_t* area, const ui_widget_t* uiWidget);

#endif // ICONWIDGET_H
