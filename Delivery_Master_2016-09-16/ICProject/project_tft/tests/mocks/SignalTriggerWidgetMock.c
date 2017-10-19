
#include "SignalTriggerWidget.h"

const ui_widget_funcs SignalTriggerWidgetFuncs =
{
    .draw = NULL,
    .refresh = NULL,
    .handleNext = NULL,
    .handlePrevious = NULL,
    .handleEnter = NULL,
    .handleLeave = NULL,
    .offerFocus = SignalTriggerWidget_OfferFocus,
    .forceFocusRelease = NULL,
    .focusFinished = NULL
};

boolean SignalTriggerWidget_OfferFocus(const ui_widget_t* uiWidget)
{
    return FALSE; // Always return FALSE as the SignalTriggerWidget don't need to keep the focus
}
