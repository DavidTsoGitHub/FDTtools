/********************************* Includes **********************************/
#include "UIWidget.h"
#include "LayoutManager.h"
#include "Logger.h"

/**************************** Constants and Types ****************************/
/********************************* Variables *********************************/
/************************* Local Function Prototypes *************************/
/***************************** Exported Functions ****************************/
void UIWidget_Draw(const display_area_definition_t* area, const ui_widget_t* uiWidget)
{
    if ((uiWidget != NULL) && (uiWidget->functions != NULL))
    {
        if (uiWidget->functions->draw != NULL)
        {
            uiWidget->functions->draw(area, uiWidget);
        }
    }
    else
    {
        Logger_Error("UIWidget", "Widget type missing ui_widget_funcs (%s)", "UIWidget_Draw");
    }
}

boolean UIWidget_NeedsRedraw(const ui_widget_t* uiWidget)
{
    boolean changed = FALSE;
    if ((uiWidget != NULL) && (uiWidget->functions != NULL))
    {
        if (uiWidget->functions->needsRedraw != NULL)
        {
            changed = uiWidget->functions->needsRedraw(uiWidget);
        }
    }
    else
    {
        Logger_Error("UIWidget", "Widget type missing ui_widget_funcs (%s)", "UIWidget_NeedsRedraw");
    }

    return changed;
}

void UIWidget_Redraw(const display_area_definition_t* area, const ui_widget_t* uiWidget)
{
    if ((uiWidget != NULL) && (uiWidget->functions != NULL))
    {
        if (uiWidget->functions->redraw != NULL)
        {
            uiWidget->functions->redraw(area, uiWidget);
        }
    }
    else
    {
        Logger_Error("UIWidget", "Widget type missing ui_widget_funcs (%s)", "UIWidget_Redraw");
    }
}

void UIWidget_HandleNext(const ui_widget_t* uiWidget)
{
    if ((uiWidget != NULL) && (uiWidget->functions != NULL))
    {
        if (uiWidget->functions->handleNext != NULL)
        {
            uiWidget->functions->handleNext();
        }
    }
    else
    {
        Logger_Error("UIWidget", "Widget type missing ui_widget_funcs (%s)", "UIWidget_HandleNext");
    }
}

void UIWidget_HandlePrevious(const ui_widget_t* uiWidget)
{
    if ((uiWidget != NULL) && (uiWidget->functions != NULL))
    {
        if (uiWidget->functions->handlePrevious != NULL)
        {
            uiWidget->functions->handlePrevious();
        }
    }
    else
    {
        Logger_Error("UIWidget", "Widget type missing ui_widget_funcs (%s)", "UIWidget_HandlePrevious");
    }
}

void UIWidget_HandleEnter(const ui_widget_t* uiWidget)
{
    if ((uiWidget != NULL) && (uiWidget->functions != NULL))
    {
        if (uiWidget->functions->handleEnter != NULL)
        {
            uiWidget->functions->handleEnter();
        }
    }
    else
    {
        Logger_Error("UIWidget", "Widget type missing ui_widget_funcs (%s)", "UIWidget_HandleEnter");
    }
}

void UIWidget_HandleLeave(const ui_widget_t* uiWidget)
{
    if ((uiWidget != NULL) && (uiWidget->functions != NULL))
    {
        if (uiWidget->functions->handleLeave != NULL)
        {
            uiWidget->functions->handleLeave();
        }
    }
    else
    {
        Logger_Error("UIWidget", "Widget type missing ui_widget_funcs (%s)", "UIWidget_HandleLeave");
    }
}

boolean UIWidget_OfferFocus(sint8 layoutID, const ui_widget_t* uiWidget)
{
    boolean focusAccepted = FALSE;

    if ((uiWidget != NULL) && (uiWidget->functions != NULL))
    {
        if (uiWidget->functions->offerFocus != NULL)
        {
            focusAccepted = uiWidget->functions->offerFocus(uiWidget);
        }
    }
    else
    {
        Logger_Error("UIWidget", "Widget type missing ui_widget_funcs (%s)", "UIWidget_OfferFocus");
    }

    if (focusAccepted == TRUE)
    {
        LayoutManager_SetLayoutFocus(layoutID, FALSE);
    }

    return focusAccepted;
}

void UIWidget_ForceFocusRelease(const ui_widget_t* uiWidget)
{
    if ((uiWidget != NULL) && (uiWidget->functions != NULL))
    {
        if (uiWidget->functions->forceFocusRelease != NULL)
        {
            uiWidget->functions->forceFocusRelease();
        }
    }
    else
    {
        Logger_Error("UIWidget", "Widget type missing ui_widget_funcs (%s)", "UIWidget_ForceFocusRelease");
    }
}

boolean UIWidget_FocusFinished(const ui_widget_t* uiWidget)
{
    boolean focusFinished = TRUE;

    if ((uiWidget != NULL) && (uiWidget->functions != NULL))
    {
        if (uiWidget->functions->focusFinished != NULL)
        {
            focusFinished = uiWidget->functions->focusFinished();
        }
    }
    else
    {
        Logger_Error("UIWidget", "Widget type missing ui_widget_funcs (%s)", "UIWidget_FocusFinished");
    }

    return focusFinished;
}

/****************************** Local Functions ******************************/
