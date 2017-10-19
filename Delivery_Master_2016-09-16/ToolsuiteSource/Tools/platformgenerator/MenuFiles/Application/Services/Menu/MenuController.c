//lint -save
//lint -esym(9003, stateTable) [Supressed as we need to separate generated and static code.]
//lint -esym(9003, stateTableSize) [ -''- ]
/********************************* Includes **********************************/
#include "MenuController.h"
#include "Menu_cfg.h"
#include "ContentsManager_cfg.h"
#include "MenuControllerInformation.h" // Provides the current menu item to other components
#include "MenuRenderer.h"
#include "ResourceManager.h"
#include "GCL_MenuController.h"
#include "Logger.h"
#include "LayoutManager.h" // We get the current state from the layout manager

/**************************** Constants and Types ****************************/
#define MENU_CONTROLLER_LOGNAME  "MenuController"
#define ACTION_COUNT_MAX    2U
#define GUARD_COUNT_MAX     5U

typedef enum
{
    DISPLAY_DENIED,
    DISPLAY_GRANTED
} displayAccessState;

typedef enum
{
    MENU_INIT,
    MENU_WAITING_FOR_RESOURCE,
    MENU_NAVIGATING,
    MENU_WIDGETHASFOCUS
} menuState;

#ifdef __WIN32__
static const char* const stateNames[] =
{
    "Init",
    "WaitingForResource",
    "Navigating",
    "WidgetHasFocus"
};
#endif

typedef struct
{
    boolean (*predicate)(void);
    void (*actions[ACTION_COUNT_MAX])(void);
    menuState nextState;
} guard;

typedef struct
{
  menuState currentState;
  guard guards[GUARD_COUNT_MAX];
} stateTableElement;

static const uint8 MM_BUTTON_PRESSED = 1;

/********************************* Variables *********************************/
const menuItem_t* CurrentMenuItem = NULL;
const ui_widget_t* FocusedWidget = NULL;
static uint8 m_Up_Indicated = 0;
static uint8 m_Down_Indicated = 0;
static uint8 m_Set_Indicated = 0;
static uint8 m_Clear_Indicated = 0;
static displayAccessState m_displayAccessState;
static const stateTableElement* m_currentState = NULL;
static uint16 m_currentVehicleSpeed = 0;
static boolean m_hasResource = FALSE;
static uint8 m_nextItemEvent = 0;
static uint8 m_previousItemEvent = 0;
static uint8 m_enterItemEvent = 0;
static uint8 m_leaveItemEvent = 0;
static uint8 m_awaitingItemRedraw = 0;
static uint8 m_awaitingItemRefresh = 0;

/************************* Local Function Prototypes *************************/
static void driveStateMachine(void);
static const stateTableElement* getStateTableElement(menuState state);
static void resourceGrantedCallback(void);
static void resourceDeniedCallback(void);
static void updateMenuGraphicsInformation(void);

/**** Actions ****/
static void requestResource(void);
static void releaseResource(void);
static void updateCurrentVehicleSpeed(void);
static void offerWidgetFocus(void);
static void releaseWidgetFocus(void);
static void reset(void);
static void redrawCurrentMenuItem(void);
static void refreshCurrentMenuItem(void);
static void clearButtonIndications(void);
static void handleNavigation(void);
static void handleFocusedWidgetNavigation(void);

/**** Predicates ****/
static boolean always(void);
static boolean startingUp(void);
static boolean shuttingDown(void);
static boolean focusGivenToWidget(void);
static boolean focusFinished(void);
static boolean buttonPressed(void);
static boolean resourceGranted(void);
static boolean resourceDenied(void);

//Dont' forget to update ACTION_COUNT_MAX and GUARD_COUNT_MAX if you change the maximum number of actions or guards
static const stateTableElement stateTable[] =
{
    { .currentState = MENU_INIT,
      .guards =
      {
          { .predicate = startingUp, .nextState = MENU_WAITING_FOR_RESOURCE, .actions = { requestResource, NULL } },
          { NULL },
          { NULL },
          { NULL },
          { NULL }
      }
    },

    { .currentState = MENU_WAITING_FOR_RESOURCE,
      .guards =
      {
          { .predicate = shuttingDown, .nextState = MENU_INIT, .actions = { releaseResource, NULL } },
          { .predicate = resourceGranted, .nextState = MENU_NAVIGATING, .actions = { redrawCurrentMenuItem, NULL }},
          { NULL },
          { NULL },
          { NULL }
      }
    },

    { .currentState = MENU_NAVIGATING,
      .guards =
      {
          { .predicate = shuttingDown, .nextState = MENU_INIT, .actions = { reset, NULL } },
          { .predicate = resourceDenied, .nextState = MENU_WAITING_FOR_RESOURCE, .actions = { NULL, NULL } },
          { .predicate = focusGivenToWidget, .nextState = MENU_WIDGETHASFOCUS, .actions = { refreshCurrentMenuItem, clearButtonIndications } },
          { .predicate = buttonPressed, .nextState = MENU_NAVIGATING, .actions = { handleNavigation, NULL } },
          { .predicate = always, .nextState = MENU_NAVIGATING, .actions = { refreshCurrentMenuItem, NULL } }
      }
    },

    { .currentState = MENU_WIDGETHASFOCUS,
      .guards =
      {
          { .predicate = shuttingDown, .nextState = MENU_INIT, .actions =  { reset, NULL } },
          { .predicate = resourceDenied, .nextState = MENU_WAITING_FOR_RESOURCE, .actions = { NULL, NULL } },
          { .predicate = buttonPressed, .nextState = MENU_WIDGETHASFOCUS, .actions = { handleFocusedWidgetNavigation, NULL } },
          { .predicate = focusFinished, .nextState = MENU_NAVIGATING, .actions = { refreshCurrentMenuItem, NULL } },
          { .predicate = always, .nextState = MENU_WIDGETHASFOCUS, .actions = { refreshCurrentMenuItem, NULL } }
      }
    }
};

static const uint32 stateTableSize = sizeof(stateTable)/sizeof(stateTable[0]);

/***************************** Exported Functions ****************************/
void MenuController_Init()
{
    Logger_Info(MENU_CONTROLLER_LOGNAME, "MenuController_Init");
    CurrentMenuItem = StartMenuItem;
    m_currentState = getStateTableElement(MENU_INIT);
    RM_RegisterResourceClient(RM_CLIENT_MENUCONTROLLER, resourceGrantedCallback, resourceDeniedCallback);

    m_displayAccessState = DISPLAY_DENIED;
}

void MenuController_Run()
{
    dm_state_t dmState = LayoutManager_GetState();

    if (m_nextItemEvent != 0u)
    {
        m_nextItemEvent = 0u;
        GCL_Write_MenuController_NextItemEvent(m_nextItemEvent);
    }
    else if (m_previousItemEvent != 0u)
    {
        m_previousItemEvent = 0u;
        GCL_Write_MenuController_PreviousItemEvent(m_previousItemEvent);
    }
    else if (m_enterItemEvent != 0u)
    {
        m_enterItemEvent = 0u;
        GCL_Write_MenuController_EnterItemEvent(m_enterItemEvent);
    }
    else if (m_leaveItemEvent != 0u)
    {
        m_leaveItemEvent = 0u;
        GCL_Write_MenuController_LeaveItemEvent(m_leaveItemEvent);
    }
    else if (m_awaitingItemRedraw != 0u)
    {
        if (ContentsManager_GetLanguageAwaitingUpdate() != TRUE)
        {
            m_awaitingItemRedraw = 0u;
            MenuRenderer_DrawMenuItem(CurrentMenuItem);
        }
    }
    else if (m_awaitingItemRefresh != 0u)
    {
        if (ContentsManager_GetLanguageAwaitingUpdate() != TRUE)
        {
            m_awaitingItemRefresh = 0u;
            MenuRenderer_RefreshMenuItem(CurrentMenuItem);
        }
    }
    else
    {
        // Do Nothing
    }

    switch (m_displayAccessState)
    {
        case DISPLAY_DENIED:
            if ((ContentsManager_MenuLayoutActive() == TRUE) && (dmState == DM_STATE_RUN))
            {
                Logger_Info(MENU_CONTROLLER_LOGNAME, "ContentsManager in Running state. Display access granted!");
                m_displayAccessState = DISPLAY_GRANTED;
                m_currentVehicleSpeed = 0;
                updateMenuGraphicsInformation();
            }
        break;
        case DISPLAY_GRANTED:
            if ((ContentsManager_MenuLayoutActive() == FALSE) || (dmState != DM_STATE_RUN))
            {
                Logger_Info(MENU_CONTROLLER_LOGNAME, "ContentsManager not in Running state. Display access denied!");
                m_displayAccessState = DISPLAY_DENIED;
                m_currentState = getStateTableElement(MENU_WAITING_FOR_RESOURCE);
            }
            else
            {
                updateCurrentVehicleSpeed();
            }
        break;
        default:
            m_displayAccessState = DISPLAY_DENIED;
            m_currentState = getStateTableElement(MENU_WAITING_FOR_RESOURCE);
        break;
    }

    driveStateMachine();
}

void MenuController_ForceWidgetFocusRelease(void)
{
    UIWidget_HandleLeave(FocusedWidget);
}

boolean MenuController_AtStartMenuItem(void)
{
    boolean atStartMenuItem = TRUE;

    if (CurrentMenuItem != StartMenuItem)
    {
        atStartMenuItem = FALSE;
    }

    return atStartMenuItem;
}

void MenuController_Up_Indication()
{
    if ((ContentsManager_MenuLayoutActive() == TRUE) && (CurrentMenuItem != NULLMENUITEM))
    {
        m_Up_Indicated = TRUE;
    }
    LayoutManager_SetLayoutFocus(MENU_LAYOUT_INDEX, FALSE);
}

void MenuController_Down_Indication()
{
    if ((ContentsManager_MenuLayoutActive() == TRUE) && (CurrentMenuItem != NULLMENUITEM))
    {
        m_Down_Indicated = TRUE;
    }
    LayoutManager_SetLayoutFocus(MENU_LAYOUT_INDEX, FALSE);
}

void MenuController_Set_Indication()
{
    if ((ContentsManager_MenuLayoutActive() == TRUE) && (CurrentMenuItem != NULLMENUITEM))
    {
        m_Set_Indicated = TRUE;
    }
    LayoutManager_SetLayoutFocus(MENU_LAYOUT_INDEX, FALSE);
}

void MenuController_Clear_Indication()
{
    if ((ContentsManager_MenuLayoutActive() == TRUE) && (CurrentMenuItem != NULLMENUITEM))
    {
        if (CurrentMenuItem->actionLeave != NULLMENUITEM)
        {
            m_Clear_Indicated = TRUE;
        }
        else
        {
            LayoutManager_SetLayoutFocus(MENU_LAYOUT_INDEX, TRUE);
        }
    }
}

/****************************** Local Functions  *****************************/
static const stateTableElement* getStateTableElement(menuState state)
{
    const stateTableElement* element = NULL;
    uint32 i;

    for (i = 0U; i < stateTableSize; ++i)
    {
        if (stateTable[i].currentState == state)
        {
            element = &stateTable[i];
        }
    }

    return element;
}

static void driveStateMachine(void)
{
    if (m_displayAccessState == DISPLAY_GRANTED)
    {
        uint32 guardIndex = 0, actionIndex = 0;
        if (m_currentState != NULL)
        {
            for (guardIndex = 0U; guardIndex < GUARD_COUNT_MAX; ++guardIndex)
            {
                if (m_currentState->guards[guardIndex].predicate != NULL)
                {
                    if (m_currentState->guards[guardIndex].predicate() == TRUE)
                    {
                        for (actionIndex = 0U; actionIndex < ACTION_COUNT_MAX; ++actionIndex)
                        {
                            if (m_currentState->guards[guardIndex].actions[actionIndex] == NULL)
                            {
                                break;
                            }

                            m_currentState->guards[guardIndex].actions[actionIndex]();
                        }

    #ifdef __WIN32__
                        if (m_currentState->currentState != m_currentState->guards[guardIndex].nextState)
                        {

                            Logger_Info(MENU_CONTROLLER_LOGNAME, "State transition: %s --> %s", stateNames[m_currentState->currentState], stateNames[m_currentState->guards[guardIndex].nextState]);
                        }
    #endif

                        m_currentState = getStateTableElement(m_currentState->guards[guardIndex].nextState);
                        break;
                    }
                }
            }
        }
        else
        {
            refreshCurrentMenuItem();
        }
    }
}

static void updateCurrentVehicleSpeed(void)
{
    uint8 u8SignalStatus = 0U;
    GCL_Read_MenuController_VehicleSpeed(&m_currentVehicleSpeed, &u8SignalStatus);
}

static void resourceGrantedCallback(void)
{
    Logger_Info(MENU_CONTROLLER_LOGNAME, "Resource granted");
    m_hasResource = TRUE;
}

static void resourceDeniedCallback(void)
{
    Logger_Info(MENU_CONTROLLER_LOGNAME, "Resource denied");
    m_hasResource = FALSE;
}

/**** Actions ****/
static void requestResource(void)
{
    Logger_Info(MENU_CONTROLLER_LOGNAME, "Requesting resource");
    RM_RequestResource(RM_CLIENT_MENUCONTROLLER, RM_PRIO_MENUCONTROLLER);
}

static void releaseResource(void)
{
    Logger_Info(MENU_CONTROLLER_LOGNAME, "Releasing resource");
    RM_ReleaseResource(RM_CLIENT_MENUCONTROLLER);
}

static void offerWidgetFocus(void)
{
    uint8 i;
    for (i=0; i<CurrentMenuItem->uiWidgetCount; ++i)
    {
        const ui_widget_t* widget = &CurrentMenuItem->uiWidgets[i];
        if (UIWidget_OfferFocus(MENU_LAYOUT_INDEX, widget) == TRUE)
        {
            Logger_Info(MENU_CONTROLLER_LOGNAME, "Widget accepted focus offer.");
            FocusedWidget = widget;
            break;
        }
    }

    if (FocusedWidget == NULL)
    {
        Logger_Info(MENU_CONTROLLER_LOGNAME, "No widget accepted focus offer.");
    }
}

static void releaseWidgetFocus(void)
{
    if (FocusedWidget != NULL)
    {
        Logger_Info(MENU_CONTROLLER_LOGNAME, "Forcing widget to release focus");
        UIWidget_ForceFocusRelease(FocusedWidget);
        FocusedWidget = NULL;
    }
}

static void reset(void)
{
    CurrentMenuItem = StartMenuItem;
    releaseWidgetFocus();
}

static void redrawCurrentMenuItem(void)
{
    if (ContentsManager_GetLanguageAwaitingUpdate() != TRUE)
    {
        MenuRenderer_DrawMenuItem(CurrentMenuItem);
    }
    else
    {
        m_awaitingItemRedraw = 1;
    }
}

static void refreshCurrentMenuItem(void)
{
    if (ContentsManager_GetLanguageAwaitingUpdate() != TRUE)
    {
        MenuRenderer_RefreshMenuItem(CurrentMenuItem);
    }
    else
    {
        m_awaitingItemRefresh = 1;
    }
}

static void handleNavigation(void)
{
    uint8 u8ButtonStatus = 0U;
    uint8 u8SignalStatus = 0U;

    if (m_Up_Indicated == TRUE)
    {
        GCL_Read_MenuController_Up(&u8ButtonStatus, &u8SignalStatus);
        if ((u8ButtonStatus == MM_BUTTON_PRESSED) && (CurrentMenuItem->actionPrevious != NULLMENUITEM))
        {
            CurrentMenuItem = CurrentMenuItem->actionPrevious;
            m_previousItemEvent = 1u;
            updateMenuGraphicsInformation();
            redrawCurrentMenuItem();
        }
        m_Up_Indicated = FALSE;
    }
    else if (m_Down_Indicated == TRUE)
    {
        GCL_Read_MenuController_Down(&u8ButtonStatus, &u8SignalStatus);
        if ((u8ButtonStatus == MM_BUTTON_PRESSED) && (CurrentMenuItem->actionNext != NULLMENUITEM))
        {
            CurrentMenuItem = CurrentMenuItem->actionNext;
            m_nextItemEvent = 1u;
            updateMenuGraphicsInformation();
            redrawCurrentMenuItem();
        }
        m_Down_Indicated = FALSE;
    }
    else if (m_Set_Indicated == TRUE)
    {
        GCL_Read_MenuController_Set(&u8ButtonStatus, &u8SignalStatus);
        if (u8ButtonStatus == MM_BUTTON_PRESSED)
        {
            if (CurrentMenuItem->actionEnter != NULLMENUITEM)
            {
                CurrentMenuItem = CurrentMenuItem->actionEnter;
                m_enterItemEvent = 1u;
                updateMenuGraphicsInformation();
                redrawCurrentMenuItem();
            }
        }
        m_Set_Indicated = FALSE;
    }
    else if (m_Clear_Indicated == TRUE)
    {
        GCL_Read_MenuController_Clear(&u8ButtonStatus, &u8SignalStatus);
        if (u8ButtonStatus == MM_BUTTON_PRESSED)
        {
            if (CurrentMenuItem->actionLeave != NULLMENUITEM)
            {
                CurrentMenuItem = CurrentMenuItem->actionLeave;
                m_leaveItemEvent = 1u;
                updateMenuGraphicsInformation();
                redrawCurrentMenuItem();
            }
            else
            {
                // Only the top menu will return NULLMENUITEM for Clear
                LayoutManager_SetLayoutFocus(MENU_LAYOUT_INDEX, TRUE);
            }
        }
        m_Clear_Indicated = FALSE;
    }
    else
    {
        // No navigation!
    }
}

static void handleFocusedWidgetNavigation(void)
{
    uint8 u8ButtonStatus, u8SignalStatus;

    if (m_currentVehicleSpeed > 0U)
    {
        if (m_Clear_Indicated == TRUE)
        {
            GCL_Read_MenuController_Clear(&u8ButtonStatus, &u8SignalStatus);
        }
        m_Up_Indicated = FALSE;
        m_Down_Indicated = FALSE;
        m_Set_Indicated = FALSE;
        m_Clear_Indicated = FALSE;
    }
    else
    {
        if (m_Up_Indicated == TRUE)
        {
            GCL_Read_MenuController_Up(&u8ButtonStatus, &u8SignalStatus);
            if (u8ButtonStatus == MM_BUTTON_PRESSED)
            {
                UIWidget_HandlePrevious(FocusedWidget);
                refreshCurrentMenuItem();
            }
            m_Up_Indicated = FALSE;
        }
        else if (m_Down_Indicated == TRUE)
        {
            GCL_Read_MenuController_Down(&u8ButtonStatus, &u8SignalStatus);
            if (u8ButtonStatus == MM_BUTTON_PRESSED)
            {
                UIWidget_HandleNext(FocusedWidget);
                refreshCurrentMenuItem();
            }
            m_Down_Indicated = FALSE;
        }
        else if (m_Set_Indicated == TRUE)
        {
            GCL_Read_MenuController_Set(&u8ButtonStatus, &u8SignalStatus);
            if (u8ButtonStatus == MM_BUTTON_PRESSED)
            {
                UIWidget_HandleEnter(FocusedWidget);
                refreshCurrentMenuItem();
            }
            m_Set_Indicated = FALSE;
        }
        else if (m_Clear_Indicated == TRUE)
        {
            GCL_Read_MenuController_Clear(&u8ButtonStatus, &u8SignalStatus);
            if (u8ButtonStatus == MM_BUTTON_PRESSED)
            {
                UIWidget_HandleLeave(FocusedWidget);
                refreshCurrentMenuItem();
            }
            m_Clear_Indicated = FALSE;
        }
        else
        {
            // No more indications to check
        }
    }
}

static void clearButtonIndications(void)
{
    m_Clear_Indicated = FALSE;
    m_Down_Indicated = FALSE;
    m_Set_Indicated = FALSE;
    m_Up_Indicated = FALSE;
}

/**** Predicates ****/
static boolean always(void)
{
    return TRUE;
}

static boolean startingUp(void)
{
    return (m_displayAccessState == DISPLAY_GRANTED) ? TRUE : FALSE;
}

static boolean shuttingDown(void)
{
    return (m_displayAccessState == DISPLAY_DENIED) ? TRUE : FALSE;
}

static boolean focusGivenToWidget(void)
{
    if (m_Set_Indicated == TRUE)
    {
        uint8 u8ButtonStatus, u8SignalStatus;
        GCL_Read_MenuController_Set(&u8ButtonStatus, &u8SignalStatus);
        if (u8ButtonStatus == MM_BUTTON_PRESSED)
        {
            if (CurrentMenuItem->actionEnter == NULLMENUITEM)
            {
                // This is a leaf node, if a widget exists that can be focused, give it focus
                offerWidgetFocus();
            }
        }
    }

    return (FocusedWidget == NULL) ? FALSE : TRUE;

}

static boolean focusFinished(void)
{
    boolean finished = TRUE;
    if (FocusedWidget != NULL)
    {
        finished = UIWidget_FocusFinished(FocusedWidget);

        if (finished == TRUE)
        {
            Logger_Info(MENU_CONTROLLER_LOGNAME, "Focused widget is done, releasing focus");
            FocusedWidget = NULL;
        }
    }

    return finished;
}

static boolean buttonPressed(void)
{
    return ((m_Clear_Indicated == TRUE) ||
            (m_Set_Indicated == TRUE) ||
            (m_Up_Indicated == TRUE) ||
            (m_Down_Indicated == TRUE)) ? TRUE : FALSE;
}

static boolean resourceGranted(void)
{
    return m_hasResource;
}

static boolean resourceDenied(void)
{
    return (m_hasResource == FALSE) ? TRUE : FALSE;
}

static void updateMenuGraphicsInformation()
{
    uint32 CurrentImage = CurrentMenuItem->imageID;
    uint32 PrevImage = 0u, NextImage = 0u, PrevPrevImage = 0u, NextNextImage = 0u;
    uint32 CurrentLabel = CurrentMenuItem->labelID;
    uint32 PrevLabel = 0u, NextLabel = 0u, PrevPrevLabel = 0u, NextNextLabel = 0u;

    if (CurrentMenuItem->actionNext != NULL )
    {
        NextImage = CurrentMenuItem->actionNext->imageID;
        NextLabel = CurrentMenuItem->actionNext->labelID;

        if (CurrentMenuItem->actionNext->actionNext != NULL )
        {
            NextNextImage = CurrentMenuItem->actionNext->actionNext->imageID;
            NextNextLabel = CurrentMenuItem->actionNext->actionNext->labelID;
        }
    }

    if (CurrentMenuItem->actionPrevious != NULL )
    {
        PrevImage = CurrentMenuItem->actionPrevious->imageID;
        PrevLabel = CurrentMenuItem->actionPrevious->labelID;

        if ( CurrentMenuItem->actionPrevious->actionPrevious != NULL )
        {
            PrevPrevImage = CurrentMenuItem->actionPrevious->actionPrevious->imageID;
            PrevPrevLabel = CurrentMenuItem->actionPrevious->actionPrevious->labelID;
        }
    }

    GCL_Write_MenuController_CurrentItemImage(CurrentImage);
    GCL_Write_MenuController_PreviousItemImage(PrevImage);
    GCL_Write_MenuController_NextItemImage(NextImage);
    GCL_Write_MenuController_PrevPrevItemImage(PrevPrevImage);
    GCL_Write_MenuController_NextNextItemImage(NextNextImage);

    GCL_Write_MenuController_CurrentItemLabel(CurrentLabel);
    GCL_Write_MenuController_PreviousItemLabel(PrevLabel);
    GCL_Write_MenuController_NextItemLabel(NextLabel);
    GCL_Write_MenuController_PrevPrevItemLabel(PrevPrevLabel);
    GCL_Write_MenuController_NextNextItemLabel(NextNextLabel);

    if (m_nextItemEvent == 1u)
    {
        GCL_Write_MenuController_NextItemEvent(m_nextItemEvent);
    }
    else if (m_previousItemEvent == 1u)
    {
        GCL_Write_MenuController_PreviousItemEvent(m_previousItemEvent);
    }
    else if (m_enterItemEvent == 1u)
    {
        GCL_Write_MenuController_EnterItemEvent(m_enterItemEvent);
    }
    else if (m_leaveItemEvent == 1u)
    {
        GCL_Write_MenuController_LeaveItemEvent(m_leaveItemEvent);
    }
    else
    {
        // Do Nothing
    }
}
