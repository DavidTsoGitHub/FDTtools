#ifndef LISTNAVIGATIONWIDGET_H
#define LISTNAVIGATIONWIDGET_H

/********************************* Includes **********************************/
#include "UIWidget.h"

/***************************** Exported Types ****************************/
extern const ui_widget_funcs ListNavigationWidgetFuncs;

/********************************* Variables *********************************/

/***************************** Exported Functions ****************************/
void ListNavigation_Draw(const display_area_definition_t* area, const ui_widget_t* uiWidget);
boolean ListNavigation_NeedsRedraw(const ui_widget_t* uiWidget);

#endif // LISTNAVIGATIONWIDGET_H
