/********************************* Includes **********************************/
#include "MenuRenderer.h"
#include "MenuRendererMock.h"
#include <check.h>

/********************************* Variables *********************************/
static int drawCallCount;
static int refreshCallCount;

/************************* Local Function Prototypes *************************/

/***************************** Exported Functions ****************************/
void MenuRendererMock_Reset(void)
{
    drawCallCount = 0;
    refreshCallCount = 0;
}

void MenuRenderer_DrawMenuItem(const menuItem_t* menuItem)
{
    ck_assert_msg(menuItem != NULL, "Received a DrawMenuItem call with a NULL menu item!");
    drawCallCount++;
}

void MenuRenderer_RefreshMenuItem(const menuItem_t* menuItem)
{
    ck_assert_msg(menuItem != NULL, "Received a RefreshMenuItem call with a NULL menu item!");
    refreshCallCount++;
}

int MenuRendererMock_DrawCallCount(void)
{
    return drawCallCount;
}

int MenuRendererMock_RefreshCallCount(void)
{
    return refreshCallCount;
}

/****************************** Local Functions  *****************************/