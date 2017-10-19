/********************************* Includes **********************************/
#include "ListNavigationWidget.h"
#include "ListNavigationWidgetCfg.h"
#include "Menu_cfg.h"
#include "ContentsManager.h"
#include "TextRenderer.h"
#include "MenuControllerInformation.h"
#include "Logger.h"
#include "LayoutManager.h"

/**************************** Constants and Types ****************************/
const ui_widget_funcs ListNavigationWidgetFuncs =
{
    .draw = ListNavigation_Draw,
    .needsRedraw = ListNavigation_NeedsRedraw,
    .redraw = ListNavigation_Draw,
    .handleNext = NULL,
    .handlePrevious = NULL,
    .handleEnter = NULL,
    .handleLeave = NULL,
    .offerFocus = NULL,
    .forceFocusRelease = NULL,
    .focusFinished = NULL
};


/********************************* Variables *********************************/
static const ui_widget_t* m_lastFocusedWidget;

/************************* Local Function Prototypes *************************/
static ListNavigationCfg_t* getListNavigationConfiguration(uint32 widgetID);
static void clearMenu(const display_area_definition_t* area, const ui_widget_t* uiWidget);
static void redrawMenu(const display_area_definition_t* area, const ui_widget_t* uiWidget, ListNavigationCfg_t* config);
static uint8 getMenuItemCount(const menuItem_t* const currentMenuItem);
static boolean areSiblings(const menuItem_t* const a, const menuItem_t* const b);

/***************************** Exported Functions ****************************/
void ListNavigation_Draw(const display_area_definition_t* area, const ui_widget_t* uiWidget)
{
    ListNavigationCfg_t* config = getListNavigationConfiguration(uiWidget->widgetID);
    if (config != NULL)
    {
        if (CurrentMenuItem != NULL)
        {
            Logger_Info("ListNavigation", "%s CurrentMenuItem %p", "ListNavigation_Draw", CurrentMenuItem);
            redrawMenu(area, uiWidget, config);
        }
        else
        {
            Logger_Info("ListNavigation", "%s CurrentMenuItem is NULL", "ListNavigation_Draw", CurrentMenuItem);
            clearMenu(area, uiWidget);
        }

        config->lastDrawnMenuItem = CurrentMenuItem;
    }
    m_lastFocusedWidget = FocusedWidget;
}

boolean ListNavigation_NeedsRedraw(const ui_widget_t* uiWidget)
{
    boolean changed = FALSE;
    const ListNavigationCfg_t* config = getListNavigationConfiguration(uiWidget->widgetID);
    if (config != NULL)
    {
        if ((config->lastDrawnMenuItem != CurrentMenuItem) || (m_lastFocusedWidget != FocusedWidget))
        {
            changed = TRUE;
        }
    }

    return changed;
}

/****************************** Local Functions  *****************************/
static ListNavigationCfg_t* getListNavigationConfiguration(uint32 widgetID)
{
    ListNavigationCfg_t* returnValue = NULL;
    for (uint32 i=0; i<ListNavigationConfigCount; ++i)
    {
        if (ListNavigationConfigs[i].widgetID == widgetID)
        {
            returnValue = &ListNavigationConfigs[i];
            break;
        }
    }

    return returnValue;
}

static void clearMenu(const display_area_definition_t* area, const ui_widget_t* uiWidget)
{
    ContentsManager_ClearRectangle(area, uiWidget->x, uiWidget->y, uiWidget->width, uiWidget->height, uiWidget->bgColor);
}

static void redrawMenu(const display_area_definition_t* area, const ui_widget_t* uiWidget, ListNavigationCfg_t* config)
{
    clearMenu(area, uiWidget);
    const menuItem_t* currentMenuItem = CurrentMenuItem;
    boolean menuNavigationActive = TRUE;

    if ((LayoutManager_GetCurrentLayoutID() != MENU_LAYOUT_INDEX) || (FocusedWidget != NULL))
    {
        menuNavigationActive = FALSE;
    }

    // Calculate dimensions
    static const uint16 navigationTriangleWidth = 9;
    static const uint16 sliderWidth = 5;
    static const uint16 scrollbarWidth = 9;
    static const uint16 scrollbarGap = 2;
    static const uint16 scrollbarTriangleWidth = 5;
    static const uint16 menuItemGap = 1;

    const uint16 itemWidth = uiWidget->width - (uint16)(scrollbarWidth + scrollbarGap);
    const uint16 itemHeight = (uint16)(uiWidget->height / config->displayItemCount);
    const uint16 itemOffset = itemHeight + menuItemGap;
    const uint16 navigationTriangleX = (uiWidget->x + itemWidth) - navigationTriangleWidth;
    const uint16 navigationTriangleYOffset = (itemHeight >> 1) - (navigationTriangleWidth >> 1);
    const uint16 scrollbarHeight = (uint16)(itemOffset * config->displayItemCount) - 1u;
    const uint16 scrollbarContainerX = (uiWidget->x + uiWidget->width) - scrollbarWidth;
    const uint16 scrollbarContainerY = uiWidget->y;
    const uint16 scrollbarTriangleX = (uint16)(uiWidget->x + uiWidget->width) - (uint16)(scrollbarTriangleWidth + 2u);
    const uint16 scrollbarTriangleTopY = uiWidget->y + 2u;
    const uint16 scrollbarTriangleBottomY = uiWidget->y + scrollbarHeight - 5u;
    const uint16 sliderRegionStartY = scrollbarTriangleTopY + scrollbarTriangleWidth - 1u;
    const uint16 sliderRegionEndY = scrollbarTriangleBottomY - 1u;
    const uint16 sliderRegionHeight = sliderRegionEndY - sliderRegionStartY;
    const uint16 sliderRegionX = scrollbarContainerX + 2u;

    // Determine if the current menu is a sibling to the last drawn menu
    if (areSiblings(currentMenuItem, config->lastDrawnMenuItem) == FALSE)
    {
        config->firstIndexInView = 0;
    }

    // Get current menu view index boundaries
    uint8 firstIndexInView = config->firstIndexInView;
    uint8 lastIndexInView = config->firstIndexInView + (config->displayItemCount - 1u);

    // Update menu view index boundaries if necessary
    if (currentMenuItem->menuItemIndex < firstIndexInView)
    {
        // We need to slide the menu view window up
        firstIndexInView = currentMenuItem->menuItemIndex;
        lastIndexInView = firstIndexInView + (config->displayItemCount - 1u);
        config->firstIndexInView = firstIndexInView;
    }
    else if (currentMenuItem->menuItemIndex > lastIndexInView)
    {
        // We need to slide the menu view window down
        lastIndexInView = currentMenuItem->menuItemIndex;
        firstIndexInView = lastIndexInView - (config->displayItemCount - 1u);
        config->firstIndexInView = firstIndexInView;
    }
    else
    {
        // The menu view window can remain unchanged
    }

    // Draw menu item boxes
    uint8 selectedItemViewIndex = currentMenuItem->menuItemIndex - firstIndexInView;
    uint16 y = uiWidget->y;
    for (uint8 i=0; i<config->displayItemCount; ++i)
    {
        if ((i == selectedItemViewIndex) && (menuNavigationActive == TRUE))
        {
            ContentsManager_DrawBoxWithRoundCorners(area, uiWidget->x, y, itemWidth, itemHeight, TRUE, TRUE, config->selectedItemBgColor, uiWidget->fgColor);
        }
        else
        {
            ContentsManager_DrawBoxWithRoundCorners(area, uiWidget->x, y, itemWidth, itemHeight, FALSE, FALSE, uiWidget->fgColor, uiWidget->bgColor);
        }
        y += itemOffset;
    }

    // Draw current selected menu item label
    y = (uint16)(itemOffset * selectedItemViewIndex) + uiWidget->y;
    if (menuNavigationActive == TRUE)
    {
        TextRenderer_RenderLabel(area, currentMenuItem->labelID, uiWidget->x, y, itemWidth, itemHeight, config->selectedItemFgColor, config->selectedItemBgColor);
    }
    else
    {
        TextRenderer_RenderLabel(area, currentMenuItem->labelID, uiWidget->x, y, itemWidth, itemHeight, uiWidget->fgColor, uiWidget->bgColor);
    }
    if (currentMenuItem->actionEnter != NULL)
    {
        ContentsManager_DrawTriangle(area, DM_TRIANGLE_DIRECTION_RIGHT, navigationTriangleX, y + navigationTriangleYOffset, navigationTriangleWidth, FALSE, TRUE, uiWidget->fgColor, config->selectedItemFgColor);
    }

    // Draw items before current selection
    const menuItem_t* previousMenuItem = currentMenuItem->actionPrevious;
    while ((previousMenuItem != NULL) && (previousMenuItem->menuItemIndex >= firstIndexInView))
    {
        if (previousMenuItem->menuItemIndex >= currentMenuItem->menuItemIndex)
        {
            break;
        }

        uint8 itemViewIndex = previousMenuItem->menuItemIndex - firstIndexInView;
        y = (uint16)(itemOffset * itemViewIndex) + uiWidget->y;
        TextRenderer_RenderLabel(area, previousMenuItem->labelID, uiWidget->x, y, itemWidth, itemHeight, uiWidget->fgColor, uiWidget->bgColor);
        if (previousMenuItem->actionEnter != NULL)
        {
            ContentsManager_DrawTriangle(area, DM_TRIANGLE_DIRECTION_RIGHT, navigationTriangleX, y + navigationTriangleYOffset, navigationTriangleWidth, TRUE, FALSE, uiWidget->fgColor, uiWidget->bgColor);
        }
        previousMenuItem = previousMenuItem->actionPrevious;
    }

    // Draw items after current selection
    const menuItem_t* nextMenuItem = currentMenuItem->actionNext;
    while ((nextMenuItem != NULL) && (nextMenuItem->menuItemIndex <= lastIndexInView))
    {
        if (nextMenuItem->menuItemIndex <= currentMenuItem->menuItemIndex)
        {
            break;
        }

        uint8 itemViewIndex = nextMenuItem->menuItemIndex - firstIndexInView;
        y = (uint16)(itemOffset * itemViewIndex) + uiWidget->y;
        TextRenderer_RenderLabel(area, nextMenuItem->labelID, uiWidget->x, y, itemWidth, itemHeight, uiWidget->fgColor, uiWidget->bgColor);
        if (nextMenuItem->actionEnter != NULL)
        {
            ContentsManager_DrawTriangle(area, DM_TRIANGLE_DIRECTION_RIGHT, navigationTriangleX, y + navigationTriangleYOffset, navigationTriangleWidth, TRUE, FALSE, uiWidget->fgColor, uiWidget->bgColor);
        }
        nextMenuItem = nextMenuItem->actionNext;
    }

    // Draw scrollbar
    const uint8 menuItemCount = getMenuItemCount(currentMenuItem);
    ContentsManager_DrawBoxWithRoundCorners(area, scrollbarContainerX, scrollbarContainerY, scrollbarWidth, scrollbarHeight, FALSE, FALSE, uiWidget->fgColor, uiWidget->bgColor);
    ContentsManager_DrawTriangle(area, DM_TRIANGLE_DIRECTION_UP, scrollbarTriangleX, scrollbarTriangleTopY, scrollbarTriangleWidth, TRUE, FALSE, uiWidget->fgColor, uiWidget->bgColor);
    ContentsManager_DrawTriangle(area, DM_TRIANGLE_DIRECTION_DOWN, scrollbarTriangleX, scrollbarTriangleBottomY, scrollbarTriangleWidth, TRUE, FALSE, uiWidget->fgColor, uiWidget->bgColor);

    // Draw slider
    if (menuNavigationActive == FALSE)
    {
        ContentsManager_DrawBoxWithRoundCorners(area, sliderRegionX, sliderRegionStartY, sliderWidth, sliderRegionHeight, FALSE, FALSE, uiWidget->fgColor, uiWidget->bgColor);
    }
    else if (menuItemCount <= config->displayItemCount)
    {
        ContentsManager_DrawBoxWithRoundCorners(area, sliderRegionX, sliderRegionStartY, sliderWidth, sliderRegionHeight, TRUE, FALSE, uiWidget->fgColor, uiWidget->bgColor);
    }
    else
    {
        static boolean bottomMenuScreen = FALSE;
        const uint8 heightPerItemInScrollbar = (uint8)(sliderRegionHeight / menuItemCount);
        const uint16 sliderYOffset = (uint16)((uint16)firstIndexInView * heightPerItemInScrollbar);
        const uint16 sliderStartY = sliderRegionStartY + sliderYOffset;

        if ((bottomMenuScreen == TRUE) && (currentMenuItem->menuItemIndex < (menuItemCount - config->displayItemCount)))
        {
            bottomMenuScreen = FALSE;
        }

        if ((currentMenuItem->menuItemIndex == (menuItemCount - 1u)) || (bottomMenuScreen == TRUE))
        {
            // The slider's height should be the distance left of the slider area when reaching the bottom item in the menu
            const uint16 sliderHeight = (scrollbarTriangleBottomY - 1u) - sliderStartY;
            ContentsManager_DrawBoxWithRoundCorners(area, sliderRegionX, sliderStartY, sliderWidth, sliderHeight, TRUE, FALSE, uiWidget->fgColor, uiWidget->bgColor);
            bottomMenuScreen = TRUE;
        }
        else
        {
            const uint16 sliderHeight = (uint16)((uint16)config->displayItemCount * heightPerItemInScrollbar);
            ContentsManager_DrawBoxWithRoundCorners(area, sliderRegionX, sliderStartY, sliderWidth, sliderHeight, TRUE, FALSE, uiWidget->fgColor, uiWidget->bgColor);
        }
    }
}

static uint8 getMenuItemCount(const menuItem_t* const currentMenuItem)
{
    uint8 returnValue = currentMenuItem->menuItemIndex + 1u;
    const menuItem_t* nextMenuItem = currentMenuItem->actionNext;
    while ((nextMenuItem != NULL) && (nextMenuItem->menuItemIndex > currentMenuItem->menuItemIndex))
    {
        ++returnValue;
        nextMenuItem = nextMenuItem->actionNext;
    }

    return returnValue;
}

static boolean areSiblings(const menuItem_t* const a, const menuItem_t* const b)
{
    boolean siblingFound = FALSE;
    if (a == b)
    {
        siblingFound = TRUE;
    }
    else
    {
        const menuItem_t* previousMenuItem = a->actionPrevious;
        while ((previousMenuItem != NULL) && (previousMenuItem->menuItemIndex < a->menuItemIndex))
        {
            if (previousMenuItem == b)
            {
                siblingFound = TRUE;
                break;
            }

            previousMenuItem = previousMenuItem->actionPrevious;
        }

        if (siblingFound != TRUE)
        {
            const menuItem_t* nextMenuItem = a->actionNext;
            while ((nextMenuItem != NULL) && (nextMenuItem->menuItemIndex > a->menuItemIndex))
            {
                if (nextMenuItem == b)
                {
                    siblingFound = TRUE;
                    break;
                }
                nextMenuItem = nextMenuItem->actionNext;
            }
        }
    }

    return siblingFound;
}
