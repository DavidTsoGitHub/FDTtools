
#include "SelectableListWidget.h"

const ui_widget_funcs SelectableListWidgetFuncs =
{
    .draw = SelectableListWidget_Draw,
    .refresh = SelectableListWidget_Refresh,
    .handleNext = SelectableListWidget_HandleNext,
    .handlePrevious = SelectableListWidget_HandlePrevious,
    .handleEnter = SelectableListWidget_HandleEnter,
    .handleLeave = SelectableListWidget_HandleLeave,
    .offerFocus = SelectableListWidget_OfferFocus,
    .forceFocusRelease = SelectableListWidget_ForceReleaseFocus,
    .focusFinished = SelectableListWidget_FocusFinished
};

void SelectableListWidget_Draw(display_area_definition_t* area, const ui_widget_t* uiWidget)
{

}

boolean SelectableListWidget_Refresh(display_area_definition_t* area, const ui_widget_t* uiWidget)
{
    return TRUE;
}

void SelectableListWidget_HandleNext()
{

}

void SelectableListWidget_HandlePrevious()
{

}

void SelectableListWidget_HandleEnter()
{

}

void SelectableListWidget_HandleLeave()
{

}

boolean SelectableListWidget_OfferFocus(const ui_widget_t* uiWidget)
{
    return TRUE;
}

void SelectableListWidget_ForceReleaseFocus()
{

}

boolean SelectableListWidget_FocusFinished()
{
    return TRUE;
}
