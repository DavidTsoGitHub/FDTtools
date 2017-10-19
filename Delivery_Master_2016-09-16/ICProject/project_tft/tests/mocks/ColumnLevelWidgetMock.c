#include "ColumnLevelWidget.h"

const ui_widget_funcs ColumnLevelWidgetFuncs =
{
    .draw = ColumnLevelWidget_Draw,
    .refresh = ColumnLevelWidget_Refresh,
    .handleNext = ColumnLevelWidget_HandleNext,
    .handlePrevious = ColumnLevelWidget_HandlePrevious,
    .handleEnter = ColumnLevelWidget_HandleEnter,
    .handleLeave = ColumnLevelWidget_HandleLeave,
    .offerFocus = ColumnLevelWidget_OfferFocus,
    .forceFocusRelease = ColumnLevelWidget_ForceReleaseFocus,
    .focusFinished = ColumnLevelWidget_FocusFinished
};


/***************************** Exported Functions ****************************/
void ColumnLevelWidget_Draw(display_area_definition_t* area, const ui_widget_t* uiWidget)
{

}

boolean ColumnLevelWidget_Refresh(display_area_definition_t* area, const ui_widget_t* uiWidget)
{
    return TRUE;
}

void ColumnLevelWidget_HandleNext()
{

}

void ColumnLevelWidget_HandlePrevious()
{

}

void ColumnLevelWidget_HandleEnter()
{

}

void ColumnLevelWidget_HandleLeave()
{

}

boolean ColumnLevelWidget_OfferFocus(const ui_widget_t* uiWidget)
{
    return TRUE;
}

void ColumnLevelWidget_ForceReleaseFocus()
{

}

boolean ColumnLevelWidget_FocusFinished()
{
    return TRUE;
}
