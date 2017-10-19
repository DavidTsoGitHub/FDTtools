#include "UIWidgetMock.h"

static uint8 drawCount = 0u;
static uint8 refreshCount = 0u;

static clock_t lastDrawTime = 0;
static clock_t lastRefreshTime = 0;

void UIWidgetMock_Reset()
{
    drawCount = 0;
    refreshCount = 0;
    lastDrawTime = 0;
    lastRefreshTime = 0;
}

void UIWidget_Draw(const ui_widget_t* uiWidget)
{
    lastDrawTime = clock();
    drawCount++;
}

void UIWidget_Refresh(const ui_widget_t* uiWidget)
{
    lastRefreshTime = clock();
    refreshCount++;
}

void UIWidget_HandleNext(const ui_widget_t* uiWidget)
{
}

void UIWidget_HandlePrevious(const ui_widget_t* uiWidget)
{
}

void UIWidget_HandleEnter(const ui_widget_t* uiWidget)
{
}

void UIWidget_HandleLeave(const ui_widget_t* uiWidget)
{
}

boolean UIWidget_OfferFocus(const ui_widget_t* uiWidget)
{
    return TRUE;
}

void UIWidget_ForceReleaseFocus(const ui_widget_t* uiWidget)
{
}

boolean UIWidget_FocusFinished(const ui_widget_t* uiWidget)
{
    return TRUE;
}

uint8 UIWidgetMock_Draw_CallCount(void)
{
    return drawCount;
}

clock_t UIWidgetMock_GetLastDrawCallTime(void)
{
    return lastDrawTime;
}

uint8 UIWidgetMock_Refresh_CallCount(void)
{
    return refreshCount;
}

clock_t UIWidgetMock_GetLastRefreshCallTime(void)
{
    return lastRefreshTime;
}