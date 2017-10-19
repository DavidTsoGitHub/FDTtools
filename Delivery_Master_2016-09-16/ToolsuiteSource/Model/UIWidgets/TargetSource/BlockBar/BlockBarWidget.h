#ifndef BLOCKBARWIDGET_H
#define BLOCKBARWIDGET_H

/********************************* Includes **********************************/
#include "UIWidget.h"

/***************************** Exported Types ****************************/
extern const ui_widget_funcs BlockBarWidgetFuncs;

/********************************* Variables *********************************/

/***************************** Exported Functions ****************************/
void BlockBar_Draw(const display_area_definition_t* area, const ui_widget_t* uiWidget);
boolean BlockBar_NeedsRedraw(const ui_widget_t* uiWidget);

#endif //BLOCKBARWIDGET_H
