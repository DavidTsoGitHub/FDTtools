#ifndef COLUMNLEVELWIDGET_H
#define COLUMNLEVELWIDGET_H

/********************************* Includes **********************************/
#include "UIWidget.h"

/***************************** Exported Types ****************************/
extern const ui_widget_funcs ColumnLevelWidgetFuncs;

/********************************* Variables *********************************/

/***************************** Exported Functions ****************************/
void ColumnLevelWidget_Draw(const display_area_definition_t* area, const ui_widget_t* uiWidget);
boolean ColumnLevelWidget_NeedsRedraw(const ui_widget_t* uiWidget);

void ColumnLevelWidget_HandleNext(void);
void ColumnLevelWidget_HandlePrevious(void);
void ColumnLevelWidget_HandleEnter(void);
void ColumnLevelWidget_HandleLeave(void);
boolean ColumnLevelWidget_OfferFocus(const ui_widget_t* uiWidget);
void ColumnLevelWidget_ForceReleaseFocus(void);
boolean ColumnLevelWidget_FocusFinished(void);

#endif // COLUMNLEVELWIDGET_H
