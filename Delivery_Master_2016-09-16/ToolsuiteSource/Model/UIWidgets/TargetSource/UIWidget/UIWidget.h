#ifndef _UIWIDGET_H_
#define _UIWIDGET_H_

/********************************* Includes **********************************/
#include "StandardTypes.h"
#include "LayerConfiguration.h"

/**************************** Constants and Types ****************************/

/***************************** Exported Functions ****************************/
void UIWidget_Draw(const display_area_definition_t* area, const ui_widget_t* uiWidget);
boolean UIWidget_NeedsRedraw(const ui_widget_t* uiWidget);
void UIWidget_Redraw(const display_area_definition_t* area, const ui_widget_t* uiWidget);
void UIWidget_HandleNext(const ui_widget_t* uiWidget);
void UIWidget_HandlePrevious(const ui_widget_t* uiWidget);
void UIWidget_HandleEnter(const ui_widget_t* uiWidget);
void UIWidget_HandleLeave(const ui_widget_t* uiWidget);
boolean UIWidget_OfferFocus(sint8 layoutID, const ui_widget_t* uiWidget);
void UIWidget_ForceFocusRelease(const ui_widget_t* uiWidget);
boolean UIWidget_FocusFinished(const ui_widget_t* uiWidget);

#endif // _UIWIDGET_H_
