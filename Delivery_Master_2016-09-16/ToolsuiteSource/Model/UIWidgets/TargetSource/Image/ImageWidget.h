#ifndef IMAGEWIDGET_H
#define IMAGEWIDGET_H

/********************************* Includes **********************************/
#include "UIWidget.h"

/***************************** Exported Types ****************************/
extern const ui_widget_funcs ImageWidgetFuncs;

/********************************* Variables *********************************/

/***************************** Exported Functions ****************************/
void Image_Draw(const display_area_definition_t* area, const ui_widget_t* uiWidget);
boolean Image_NeedsRedraw(const ui_widget_t* uiWidget);
void Image_Redraw(const display_area_definition_t* area, const ui_widget_t* uiWidget);

#endif // IMAGEWIDGET_H
