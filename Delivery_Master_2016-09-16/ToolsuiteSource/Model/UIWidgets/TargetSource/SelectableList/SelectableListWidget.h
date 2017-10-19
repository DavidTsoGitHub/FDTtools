#ifndef SELECTABLELISTWIDGET_H
#define SELECTABLELISTWIDGET_H

/********************************* Includes **********************************/
#include "UIWidget.h"

/***************************** Exported Types ****************************/
extern const ui_widget_funcs SelectableListWidgetFuncs;

/********************************* Variables *********************************/

/***************************** Exported Functions ****************************/
void SelectableListWidget_Draw(const display_area_definition_t* area, const ui_widget_t* uiWidget);
boolean SelectableListWidget_NeedsRedraw(const ui_widget_t* uiWidget);

void SelectableListWidget_HandleNext(void);
void SelectableListWidget_HandlePrevious(void);
void SelectableListWidget_HandleEnter(void);
void SelectableListWidget_HandleLeave(void);
boolean SelectableListWidget_OfferFocus(const ui_widget_t* uiWidget);
void SelectableListWidget_ForceReleaseFocus(void);
boolean SelectableListWidget_FocusFinished(void);

#endif // SELECTABLELISTWIDGET_H
