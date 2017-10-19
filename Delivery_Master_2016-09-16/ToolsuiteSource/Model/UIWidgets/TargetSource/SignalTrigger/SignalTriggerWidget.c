/********************************* Includes **********************************/
#include "SignalTriggerWidget.h"
#include "SignalTriggerWidgetCfg.h"
#include "Logger.h"

/**************************** Constants and Types ****************************/
const ui_widget_funcs SignalTriggerWidgetFuncs =
{
    .draw = NULL,
    .needsRedraw = NULL,
    .redraw = NULL,
    .handleNext = NULL,
    .handlePrevious = NULL,
    .handleEnter = NULL,
    .handleLeave = NULL,
    .offerFocus = SignalTriggerWidget_OfferFocus,
    .forceFocusRelease = NULL,
    .focusFinished = NULL
};

/********************************* Variables *********************************/

/************************* Local Function Prototypes *************************/
static const SignalTriggerCfg_t* getConfig(uint32 widgetID);

/***************************** Exported Functions ****************************/

boolean SignalTriggerWidget_OfferFocus(const ui_widget_t* uiWidget)
{
    const SignalTriggerCfg_t* config = getConfig(uiWidget->widgetID);
    if (config == NULL)
    {
        Logger_Error("SignalTriggerWidget", "Configuration for widget ID:%d not found when giving focus!", uiWidget->widgetID);
    }
    else
    {
        config->writeSignal();
    }

    return FALSE; // Always return FALSE as the SignalTriggerWidget don't need to keep the focus
}

/****************************** Local Functions  *****************************/
static const SignalTriggerCfg_t* getConfig(uint32 widgetID)
{
    const SignalTriggerCfg_t* returnValue = NULL;

    for (uint32 i=0; i<SignalTriggerConfigCount; ++i)
    {
        if (SignalTriggerConfigs[i].widgetID == widgetID)
        {
            returnValue = &SignalTriggerConfigs[i];
        }
    }

    return returnValue;
}
