#ifndef DYNAMICIMAGEWIDGET_H
#define DYNAMICIMAGEWIDGET_H

/********************************* Includes **********************************/
#include "UIWidget.h"

/***************************** Exported Types ****************************/
extern const ui_widget_funcs DynamicImageWidgetFuncs;

/***************************** Exported Functions ****************************/
void DynamicImageWidget_Draw(const display_area_definition_t* area, const ui_widget_t* uiWidget);
boolean DynamicImageWidget_NeedsRedraw(const ui_widget_t* uiWidget);
void DynamicImageWidget_Redraw(const display_area_definition_t* area, const ui_widget_t* uiWidget);

#endif //DYNAMICIMAGEWIDGET_H

