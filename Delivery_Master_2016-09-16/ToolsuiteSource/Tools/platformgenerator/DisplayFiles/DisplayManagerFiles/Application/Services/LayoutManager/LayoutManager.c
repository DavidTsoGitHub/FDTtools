/********************************* Includes **********************************/
#include "LayoutManager.h"
#include "ResourceManager.h"
#include "StandardTypes.h"
#include "Logger.h"
#include "systime.h"
#include "GCL_LayoutManager.h"
#ifdef HAS_MENU
#include "MenuController.h"
#endif

/**************************** Constants and Types ****************************/
#define LAYOUTMANAGER_LOG_NAME "LayoutManager"
#define RM_CLIENT_LAYOUTMANAGER 2U

typedef enum
{
    DM_ACCESS_STATE_RELEASED,
    DM_ACCESS_STATE_REQUESTED,
    DM_ACCESS_STATE_GRANTED
} dm_access_state_t;

typedef enum
{
    UP,
    DOWN,
    SET,
    CLEAR
} layout_button_t;

/********************************* Variables *********************************/
static dm_state_t m_currentState;
static dm_access_state_t m_resourceAccess;
static sint8 m_currentLayoutID;
static boolean m_currentLayoutHasFocus;
static boolean m_upIndicated;
static boolean m_downIndicated;
static boolean m_setIndicated;
static boolean m_clearIndicated;
static boolean m_widgetHasFocus = FALSE;
static boolean m_checkMessageShowing = FALSE;

/************************* Local Function Prototypes *************************/
static void findCurrentLayout(void);
static boolean transitionIsTrue(signal_transition_t signalTransition);
static boolean compareConditions(boolean firstCondition, boolean secondCondition, transition_connective_t connective);
static boolean checkCondition(sint32 value, transition_condition_t compareCondition, sint32 compareValue);
static PowerMode getPowerMode(void);
static sint32 getSint32ValueFromDynamicWidgetValue(const DynamicWidgetValue* value);
static void requestResource(void);
static void releaseResource(void);
static void resourceGrantedCallback(void);
static void resourceDeniedCallback(void);
static boolean currentLayoutHasFocus(void);
static void setCurrentLayout(sint8 layoutID);
static void setNextLayoutActive(sint8 layoutID);
static void setAllLayoutsInactive(void);
static void handleWidgetNavigation(layout_button_t button);
static boolean isWidgetPartOfCheckMessageArea(const ui_widget_t* widget);

/***************************** Exported Functions ****************************/
void LayoutManager_Init(void)
{
    RM_RegisterResourceClient(RM_CLIENT_LAYOUTMANAGER, resourceGrantedCallback, resourceDeniedCallback);
    m_currentState = DM_STATE_OFF;
    m_currentLayoutHasFocus = TRUE;
}

void LayoutManager_Run(void)
{
    static uint32 u32TimeOut;

    switch(m_currentState)
    {
        case(DM_STATE_OFF):
            if (getPowerMode() == POWER_IGNITION_ON)
            {
                Logger_Info(LAYOUTMANAGER_LOG_NAME, "State OFF -> STARTUP_REQ");
                m_currentState = DM_STATE_STARTUP_REQUESTED;
                requestResource();
            }
        break;

        case(DM_STATE_STARTUP_REQUESTED):
#ifdef SHOW_STARTUP_LAYOUT
                setCurrentLayout(STARTUP_LAYOUT);
                u32TimeOut = SysTime_GetTimeMs() + CONTENTS_MANAGER_STARTUP_TIME;
#else
                u32TimeOut = 0;
                setCurrentLayout(MAIN_LAYOUT);
#endif
                m_currentState = DM_STATE_STARTUP;
        break;

        case(DM_STATE_STARTUP):

            if (getPowerMode() < POWER_IGNITION_ON)
            {
                Logger_Info(LAYOUTMANAGER_LOG_NAME, "State STARTUP -> OFF");
                m_currentState = DM_STATE_OFF;
            }
            else if (SysTime_GetTimeMs() > u32TimeOut)
            {
                Logger_Info(LAYOUTMANAGER_LOG_NAME, "State STARTUP -> RUN");
                m_currentState = DM_STATE_RUN;
                setCurrentLayout(MAIN_LAYOUT);
                releaseResource();
            }
            else
            {
                // Do nothing
            }
        break;

        case(DM_STATE_RUN):
            if (getPowerMode() < POWER_IGNITION_ON)
            {
                Logger_Info(LAYOUTMANAGER_LOG_NAME, "State RUN -> OFF");
                m_currentState = DM_STATE_OFF;
                setCurrentLayout(-1);
                setAllLayoutsInactive();
            }
            else
            {
                if (m_checkMessageShowing == FALSE)
                {
                    findCurrentLayout();
                }
                else
                {
                    // Clear any button presses received when checkmessage is shown
                    m_upIndicated = FALSE;
                    m_downIndicated = FALSE;
                    m_setIndicated = FALSE;
                    m_clearIndicated = FALSE;
                }
            }
        break;

        default:
        //Intended
        break;
    }
}

void LayoutManager_CheckMessageShowing(boolean showing)
{
    m_checkMessageShowing = showing;
}

dm_state_t LayoutManager_GetState(void)
{
    return m_currentState;
}

sint8 LayoutManager_GetCurrentLayoutID(void)
{
    return m_currentLayoutID;
}

void LayoutManager_SetLayoutFocus(sint8 layoutID, boolean hasFocus)
{
    if (layoutID == m_currentLayoutID)
    {
        m_currentLayoutHasFocus = hasFocus;
    }
}

void LayoutManager_Up_Indication(void)
{
    if (m_widgetHasFocus == TRUE)
    {
        uint8 value = 0;
        uint8 signalStatus = 0;
        GCL_Read_LayoutManager_Up(&value, &signalStatus);

        if (value == 1u)
        {
            handleWidgetNavigation(UP);
        }
    }
    else if (currentLayoutHasFocus() == TRUE)
    {
        uint8 value = 0;
        uint8 signalStatus = 0;

        GCL_Read_LayoutManager_Up(&value, &signalStatus);

        if ((value == 1u) && (m_checkMessageShowing == FALSE))
        {
            if (ContentsManager_Layouts[m_currentLayoutID].GCLWriteUpButtonEvent != NULL)
            {
                ContentsManager_Layouts[m_currentLayoutID].GCLWriteUpButtonEvent();
            }
            m_upIndicated = TRUE;
        }
    }
    else
    {
        m_upIndicated = FALSE;
    }
}

void LayoutManager_Down_Indication(void)
{
    if (m_widgetHasFocus == TRUE)
    {
        uint8 value = 0;
        uint8 signalStatus = 0;
        GCL_Read_LayoutManager_Down(&value, &signalStatus);

        if (value == 1u)
        {
            handleWidgetNavigation(DOWN);
        }
    }
    else if (currentLayoutHasFocus() == TRUE)
    {
        uint8 value = 0;
        uint8 signalStatus = 0;

        GCL_Read_LayoutManager_Down(&value, &signalStatus);

        if ((value == 1u) && (m_checkMessageShowing == FALSE))
        {
            if (ContentsManager_Layouts[m_currentLayoutID].GCLWriteDownButtonEvent != NULL)
            {
                ContentsManager_Layouts[m_currentLayoutID].GCLWriteDownButtonEvent();
            }
            m_downIndicated = TRUE;
        }
    }
    else
    {
        m_downIndicated = FALSE;
    }
}

void LayoutManager_Set_Indication(void)
{
    if (m_widgetHasFocus == TRUE)
    {
        uint8 value = 0;
        uint8 signalStatus = 0;
        GCL_Read_LayoutManager_Set(&value, &signalStatus);

        if (value == 1u)
        {
            handleWidgetNavigation(SET);
        }
    }
    else if (currentLayoutHasFocus() == TRUE)
    {
        uint8 value = 0;
        uint8 signalStatus = 0;

        GCL_Read_LayoutManager_Set(&value, &signalStatus);

        if ((value == 1u) && (m_checkMessageShowing == FALSE))
        {
            if (ContentsManager_Layouts[m_currentLayoutID].GCLWriteSetButtonEvent != NULL)
            {
                ContentsManager_Layouts[m_currentLayoutID].GCLWriteSetButtonEvent();
            }
            m_setIndicated = TRUE;
        }
    }
    else
    {
        m_setIndicated = FALSE;
    }
}

void LayoutManager_Clear_Indication(void)
{
    if ((m_widgetHasFocus == TRUE) && (m_checkMessageShowing == FALSE))
    {
        uint8 value = 0;
        uint8 signalStatus = 0;
        GCL_Read_LayoutManager_Clear(&value, &signalStatus);

        if (value == 1u)
        {
            handleWidgetNavigation(CLEAR);
            if (ContentsManager_Layouts[m_currentLayoutID].GCLWriteClearButtonEvent != NULL)
            {
                ContentsManager_Layouts[m_currentLayoutID].GCLWriteClearButtonEvent();
            }
            m_clearIndicated = TRUE;
        }
    }

    if (currentLayoutHasFocus() == TRUE)
    {
        uint8 value = 0;
        uint8 signalStatus = 0;

        GCL_Read_LayoutManager_Clear(&value, &signalStatus);

        if ((value == 1u) && (m_checkMessageShowing == FALSE))
        {
            if (ContentsManager_Layouts[m_currentLayoutID].GCLWriteClearButtonEvent != NULL)
            {
                ContentsManager_Layouts[m_currentLayoutID].GCLWriteClearButtonEvent();
            }
            m_clearIndicated = TRUE;
        }
    }
    else
    {
        m_clearIndicated = FALSE;
    }
}

void LayoutManager_SetWidgetFocus(const ui_widget_t* widget, boolean wantFocus)
{
    if (wantFocus == TRUE)
    {
        if (isWidgetPartOfCheckMessageArea(widget) == FALSE)
        {
            m_widgetHasFocus = UIWidget_OfferFocus(m_currentLayoutID, widget);
            Logger_Info(LAYOUTMANAGER_LOG_NAME, "Offering focus to widget %d", widget->widgetID);
        }
    }
}

/****************************** Local Functions ******************************/
static boolean isWidgetPartOfCheckMessageArea(const ui_widget_t* widget)
{
    boolean widgetInCheckMessageArea = FALSE;

    for (uint8 areaIterator = 0u; areaIterator < ContentsManager_Layouts[m_currentLayoutID].areaCount; areaIterator++)
    {
        if (ContentsManager_Layouts[m_currentLayoutID].areas[areaIterator].isCheckMessageArea == TRUE)
        {
            for (uint8 widgetIterator = 0u; widgetIterator < ContentsManager_Layouts[m_currentLayoutID].areas[areaIterator].widgetCount; widgetIterator++)
            {
                if (ContentsManager_Layouts[m_currentLayoutID].areas[areaIterator].widgets[widgetIterator].widgetID == widget->widgetID)
                {
                    widgetInCheckMessageArea = TRUE;
                }
            }
        }
    }

    return widgetInCheckMessageArea;
}

static void findCurrentLayout(void)
{
    boolean foundLayout = FALSE;
    sint8 layoutID;
    sint8 activeLayout = -1;

    for (layoutID = 0; layoutID < CONTENTS_MANAGER_LAYOUT_COUNT; ++layoutID)
    {
        // Check if there is a layout that has previously been actived through a button transition
        if (ContentsManager_Layouts[layoutID].activeLayout == TRUE)
        {
            if (activeLayout == -1)
            {
                activeLayout = layoutID;
            }
        }

        // Check for signal triggered Transitions
        for (uint8 index = 0; index < ContentsManager_Layouts[layoutID].signalTransitionsCount; ++index)
        {
            if (transitionIsTrue(ContentsManager_Layouts[layoutID].signalTransitions[index]) == TRUE)
            {
                foundLayout = TRUE;
                break;
            }
            else
            {
                //do nothing
            }
        }

        if (foundLayout == TRUE)
        {
            break;
        }
    }

    if (foundLayout == FALSE)
    {
        // Check if there is a new Layout that has been activated through a button transition
        if (currentLayoutHasFocus() == TRUE)
        {
            if (m_upIndicated == TRUE)
            {
                if (ContentsManager_Layouts[m_currentLayoutID].buttonUpLayout != -1)
                {
                    foundLayout = TRUE;
                    layoutID = ContentsManager_Layouts[m_currentLayoutID].buttonUpLayout;
                    setNextLayoutActive(layoutID);
                }
                m_upIndicated = FALSE;
            }
            else if (m_downIndicated == TRUE)
            {
                if (ContentsManager_Layouts[m_currentLayoutID].buttonDownLayout != -1)
                {
                    foundLayout = TRUE;
                    layoutID = ContentsManager_Layouts[m_currentLayoutID].buttonDownLayout;
                    setNextLayoutActive(layoutID);
                }
                m_downIndicated = FALSE;
            }
            else if (m_setIndicated == TRUE)
            {
                if (ContentsManager_Layouts[m_currentLayoutID].buttonSetLayout != -1)
                {
                    foundLayout = TRUE;
                    layoutID = ContentsManager_Layouts[m_currentLayoutID].buttonSetLayout;
                    setNextLayoutActive(layoutID);
                }
                m_setIndicated = FALSE;
            }
            else if (m_clearIndicated == TRUE)
            {
                if (ContentsManager_Layouts[m_currentLayoutID].buttonClearLayout != -1)
                {
                    foundLayout = TRUE;
                    layoutID = ContentsManager_Layouts[m_currentLayoutID].buttonClearLayout;
                    setNextLayoutActive(layoutID);
                }
                m_clearIndicated = FALSE;
            }
            else
            {
                //Do nothing
            }
        }
    }

    if (foundLayout == TRUE)
    {
        if (m_currentLayoutID != layoutID)
        {
            m_widgetHasFocus = FALSE;
            m_currentLayoutHasFocus = TRUE;
            UIWidget_ForceFocusRelease(ContentsManager_Layouts[m_currentLayoutID].focusedWidget);
#ifdef HAS_MENU
            if (m_currentLayoutID == MENU_LAYOUT_INDEX)
            {
                MenuController_ForceWidgetFocusRelease();
            }
#endif
            setCurrentLayout(layoutID);
        }
    }
    else if (activeLayout != -1)
    {
        if (activeLayout != m_currentLayoutID)
        {
            m_widgetHasFocus = FALSE;
            UIWidget_ForceFocusRelease(ContentsManager_Layouts[m_currentLayoutID].focusedWidget);
#ifdef HAS_MENU
            if (m_currentLayoutID == MENU_LAYOUT_INDEX)
            {
                MenuController_ForceWidgetFocusRelease();
            }
#endif
            setCurrentLayout(activeLayout);
        }
    }
    else
    {
        if (m_currentLayoutID != MAIN_LAYOUT)
        {
            m_widgetHasFocus = FALSE;
            m_currentLayoutHasFocus = TRUE;
            UIWidget_ForceFocusRelease(ContentsManager_Layouts[m_currentLayoutID].focusedWidget);
#ifdef HAS_MENU
            if (m_currentLayoutID == MENU_LAYOUT_INDEX)
            {
                MenuController_ForceWidgetFocusRelease();
            }
#endif
            setCurrentLayout(MAIN_LAYOUT);
        }
    }
}

static boolean transitionIsTrue(signal_transition_t signalTransition)
{
    DynamicWidgetValue firstValue;
    uint8 signalStatus = 0;
    signalTransition.firstConditionGCLSink(&firstValue, &signalStatus);
    sint32 firstSint32Value = getSint32ValueFromDynamicWidgetValue(&firstValue);
    boolean conditionIsTrue = checkCondition(firstSint32Value, signalTransition.firstConditionCompareOperator, signalTransition.firstConditionCompareValue);

    if (signalTransition.connective != NONE)
    {
        boolean firstConditionIsTrue = conditionIsTrue;
        DynamicWidgetValue secondValue;
        signalTransition.secondConditionGCLSink(&secondValue, &signalStatus);
        sint32 secondSint32Value = getSint32ValueFromDynamicWidgetValue(&secondValue);
        boolean secondConditionIsTrue = checkCondition(secondSint32Value, signalTransition.secondConditionCompareOperator, signalTransition.secondConditionCompareValue);

        conditionIsTrue = compareConditions(firstConditionIsTrue, secondConditionIsTrue, signalTransition.connective);
    }

    return conditionIsTrue;
}

static boolean compareConditions(boolean firstCondition, boolean secondCondition, transition_connective_t connective)
{
    boolean retVal = FALSE;
    switch (connective)
    {
    case AND:
        retVal = ((firstCondition == TRUE) && (secondCondition == TRUE)) ? TRUE : FALSE;
        break;
    case XOR:
        retVal = (firstCondition != secondCondition) ? TRUE : FALSE;
        break;
    default:
        retVal = FALSE;
        break;
    }
    return retVal;
}

static boolean checkCondition(sint32 value, transition_condition_t compareCondition, sint32 compareValue)
{
    boolean retVal = FALSE;
    switch (compareCondition)
    {
        case EQUALS_TO:
            retVal = (value == compareValue) ? TRUE : FALSE;
            break;
        case GREATER_THAN:
            retVal = (value > compareValue) ? TRUE : FALSE;
            break;
        case EQUALS_OR_GREATER_THAN:
            retVal = (value >= compareValue) ? TRUE : FALSE;
            break;
        case LESS_THAN:
            retVal = (value < compareValue) ? TRUE : FALSE;
            break;
        case EQUALS_OR_LESS_THAN:
            retVal = (value <= compareValue) ? TRUE : FALSE;
            break;
        default:
            retVal = FALSE;
            break;
    }

    return retVal;
}

static PowerMode getPowerMode(void)
{
    uint8 status;
    PowerMode powerMode;
    GCL_Read_LayoutManager_SystemPowerMode(&powerMode, &status);

    return powerMode;
}

static void setCurrentLayout(sint8 layoutID)
{
    if (m_currentLayoutID != layoutID)
    {
        m_currentLayoutID = layoutID;

#ifdef HAS_MENU
        if (m_currentLayoutID == MENU_LAYOUT_INDEX)
        {
            m_currentLayoutHasFocus = MenuController_AtStartMenuItem();
        }
        else
        {
            m_currentLayoutHasFocus = TRUE;
        }
#else
        m_currentLayoutHasFocus = TRUE;
#endif

        m_upIndicated = FALSE;
        m_downIndicated = FALSE;
        m_setIndicated = FALSE;
        m_clearIndicated = FALSE;
    }
}

static void setNextLayoutActive(sint8 layoutID)
{
    ContentsManager_Layouts[m_currentLayoutID].activeLayout = FALSE;
    ContentsManager_Layouts[layoutID].activeLayout = TRUE;
}

static void requestResource(void)
{
    if (m_resourceAccess < DM_ACCESS_STATE_REQUESTED)
    {
        Logger_Info(LAYOUTMANAGER_LOG_NAME, "Resource requested");
        m_resourceAccess = DM_ACCESS_STATE_REQUESTED;
        RM_RequestResource(RM_CLIENT_LAYOUTMANAGER, RM_PRIO_STARTUP);
    }
}

static void releaseResource(void)
{
    Logger_Info(LAYOUTMANAGER_LOG_NAME, "Resource release");
    RM_ReleaseResource(RM_CLIENT_LAYOUTMANAGER);
}

static void resourceGrantedCallback(void)
{
    m_resourceAccess = DM_ACCESS_STATE_GRANTED;
    Logger_Info(LAYOUTMANAGER_LOG_NAME, "Resource granted");
}

static void resourceDeniedCallback(void)
{
    Logger_Info(LAYOUTMANAGER_LOG_NAME, "Resource denied");
    m_resourceAccess = DM_ACCESS_STATE_RELEASED;
}

static boolean currentLayoutHasFocus(void)
{
    return m_currentLayoutHasFocus;
}

static sint32 getSint32ValueFromDynamicWidgetValue(const DynamicWidgetValue* value)
{
    sint32 returnValue = 0;

    switch (value->DataType)
    {
        case STD_DATATYPE_UINT8:
            returnValue = (sint32)value->Value.U8;
            break;
        case STD_DATATYPE_UINT16:
            returnValue = (sint32)value->Value.U16;
            break;
        case STD_DATATYPE_UINT32:
            returnValue = (sint32)value->Value.U32;
            break;
        case STD_DATATYPE_INT8:
            returnValue = (sint32)value->Value.S8;
            break;
        case STD_DATATYPE_INT16:
            returnValue = (sint32)value->Value.S16;
            break;
        case STD_DATATYPE_INT32:
            returnValue = value->Value.S32;
            break;
        default:
            // Do nothing
            break;
    }

    return returnValue;
}

static void setAllLayoutsInactive()
{
    for (sint8 i = 0; i < CONTENTS_MANAGER_LAYOUT_COUNT; ++i)
    {
        ContentsManager_Layouts[i].activeLayout = FALSE;
    }
}

static void handleWidgetNavigation(layout_button_t button)
{
    switch (button)
    {
        case UP:
            UIWidget_HandlePrevious(ContentsManager_Layouts[m_currentLayoutID].focusedWidget);
            break;
        case DOWN:
            UIWidget_HandleNext(ContentsManager_Layouts[m_currentLayoutID].focusedWidget);
            break;
        case SET:
            UIWidget_HandleEnter(ContentsManager_Layouts[m_currentLayoutID].focusedWidget);
            LayoutManager_SetWidgetFocus(ContentsManager_Layouts[m_currentLayoutID].focusedWidget, TRUE);
            break;
        case CLEAR:
            // Run through
        default:
            // Do nothing
            break;
    }
}
