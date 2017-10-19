#ifndef UIWIDGETMOCK_H
#define UIWIDGETMOCK_H

#include "UIWidget.h"
#include "time.h"

void UIWidgetMock_Reset(void);

/** UIWidget_Draw(uint8 areaID, ui_widget_t* uiWidget) **/
uint8 UIWidgetMock_Draw_CallCount(void);
clock_t UIWidgetMock_GetLastDrawCallTime(void);

/** UIWidget_Refresh(uint8 areaID, ui_widget_t* uiWidget) **/
uint8 UIWidgetMock_Refresh_CallCount(void);
clock_t UIWidgetMock_GetLastRefreshCallTime(void);



#endif // UIWIDGETMOCK_H
