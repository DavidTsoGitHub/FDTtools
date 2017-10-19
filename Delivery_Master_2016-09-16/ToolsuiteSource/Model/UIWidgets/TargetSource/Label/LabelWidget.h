#ifndef LABELWIDGET_H
#define LABELWIDGET_H

/********************************* Includes **********************************/
#include "UIWidget.h"

/***************************** Exported Types ****************************/
extern const ui_widget_funcs LabelWidgetFuncs;

/********************************* Variables *********************************/

/***************************** Exported Functions ****************************/
void Label_Draw(const display_area_definition_t* area, const ui_widget_t* uiWidget);

#endif // LABELWIDGET_H
