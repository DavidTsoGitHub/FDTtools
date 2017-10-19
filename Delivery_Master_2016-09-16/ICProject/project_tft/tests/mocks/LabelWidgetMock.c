#include "LabelWidget.h"

const ui_widget_funcs LabelWidgetFuncs =
{
    .draw = Label_Draw,
    .refresh = NULL,
    .handleNext = NULL,
    .handlePrevious = NULL,
    .handleEnter = NULL,
    .handleLeave = NULL,
    .offerFocus = NULL,
    .forceFocusRelease = NULL,
    .focusFinished = NULL
};

void Label_Draw(display_area_definition_t* area, const ui_widget_t* uiWidget)
{

}
