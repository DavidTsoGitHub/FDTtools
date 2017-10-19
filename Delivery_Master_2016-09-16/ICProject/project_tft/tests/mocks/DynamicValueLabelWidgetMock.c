#include "DynamicValueLabelWidget.h"

const ui_widget_funcs DynamicValueLabelWidgetFuncs =
{
    .draw = DynamicValueLabel_Draw,
    .refresh = DynamicValueLabel_Refresh,
    .handleNext = NULL,
    .handlePrevious = NULL,
    .handleEnter = NULL,
    .handleLeave = NULL,
    .offerFocus = NULL,
    .forceFocusRelease = NULL,
    .focusFinished = NULL
};

void DynamicValueLabel_Draw(display_area_definition_t* area, const ui_widget_t* uiWidget)
{

}

boolean DynamicValueLabel_Refresh(display_area_definition_t* area, const ui_widget_t* uiWidget)
{
    return TRUE;
}
