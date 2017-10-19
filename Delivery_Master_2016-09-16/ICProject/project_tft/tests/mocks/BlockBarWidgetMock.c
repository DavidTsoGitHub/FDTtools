#include "BlockBarWidget.h"
/*#include "BlockBarWidgetCfg.h"
#include "ContentsManager.h"
#include "UIWidget.h"
#include "LabelManager.h"
#include "TextRenderer.h"
#include "GCL_Types.h"
#include "Logger.h"
*/

const ui_widget_funcs BlockBarWidgetFuncs =
{
    .draw = BlockBar_Draw,
    .refresh = BlockBar_Refresh,
    .handleNext = NULL,
    .handlePrevious = NULL,
    .handleEnter = NULL,
    .handleLeave = NULL,
    .offerFocus = NULL,
    .forceFocusRelease = NULL,
    .focusFinished = NULL
};

void BlockBar_Draw(display_area_definition_t* area, const ui_widget_t* uiWidget)
{

}

boolean BlockBar_Refresh(display_area_definition_t* area, const ui_widget_t* uiWidget)
{
    return TRUE;
}
