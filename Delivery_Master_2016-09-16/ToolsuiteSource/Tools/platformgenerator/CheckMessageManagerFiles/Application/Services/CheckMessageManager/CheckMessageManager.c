//lint -save
/********************************* Includes **********************************/
#include "CheckMessageManager.h"
#include "CheckMessageManager_cfg.h"
#include "GCL_CheckMessageManager.h"
#include "ContentsManager.h"
#include "LayoutManager.h"
#include "Logger.h"

/********************************* Variables *********************************/
static cm_state_t m_currentState;

/************************* Local Function Prototypes *************************/
static void initCheckMessageStatus(void);
static boolean checkMessageAllowed(uint16 currentMessage, uint8 currentLayout);
static PowerMode getPowerMode(void);
static uint8 getAbsoluteValue(sint8 value);

/***************************** Exported Functions ****************************/
void CheckMessageManager_Init(void)
{
    initCheckMessageStatus();
    m_currentState = CM_STATE_OFF;
}

void CheckMessageManager_Run(void)
{
    sint32 indexToDisplay = -1;
    static sint32 activeMessageIndex = -1;

    switch(m_currentState)
    {
        case(CM_STATE_OFF):
            if (getPowerMode() == POWER_IGNITION_ON)
            {
                Logger_Info(CHECKMESSAGEMANAGER_LOG_NAME, "State OFF -> RUN");
                initCheckMessageStatus();
                m_currentState = CM_STATE_RUN;
            }
        break;

        case(CM_STATE_RUN):
            if (getPowerMode() < POWER_IGNITION_ON)
            {
                Logger_Info(CHECKMESSAGEMANAGER_LOG_NAME, "State RUN -> OFF");
                m_currentState = CM_STATE_OFF;
            }
            else
            {
                for (uint16 messageIndex = 0u; messageIndex < NUMBER_OF_CHECKMESSAGES; ++messageIndex)
                {
                    switch(CheckMessageManager_Messages[messageIndex].state)
                    {
                        case CHECKMESSAGE_INACTIVE:
                        {
                            sint8 currentLayout = LayoutManager_GetCurrentLayoutID();
                            if (currentLayout == -1)
                            {
                                break;
                            }

                            if (checkMessageAllowed(messageIndex, getAbsoluteValue(currentLayout)) == FALSE)
                            {
                                break;
                            }

                            uint8 status;
                            uint8 value;

                            CheckMessageManager_Messages[messageIndex].trigger(&value, &status);

                            if(value > 0u)
                            {
                                CheckMessageManager_Messages[messageIndex].state = CHECKMESSAGE_ACTIVE;
                                Logger_Info(CHECKMESSAGEMANAGER_LOG_NAME, "Message %d ACTIVE", messageIndex);
                                if(indexToDisplay == -1) //Set Only highest priority message (lowest id)
                                {
                                    indexToDisplay = (sint32)messageIndex;
                                }
                            }
                            break;
                        }

                        case CHECKMESSAGE_ACTIVE:
                        {

                            if (CheckMessageManager_Messages[messageIndex].clearCondition == CHECKMESSAGE_CLEAR_BY_TRIGGER)
                            {
                                uint8 status;
                                uint8 value;

                                CheckMessageManager_Messages[messageIndex].trigger(&value, &status);

                                if(value > 0u)
                                {
                                    if(indexToDisplay == -1) //Set Only highest priority message (lowest id)
                                    {
                                        indexToDisplay = (sint32)messageIndex;
                                    }
                                }
                                else
                                {
                                    CheckMessageManager_Messages[messageIndex].state = CHECKMESSAGE_INACTIVE;
                                    Logger_Info(CHECKMESSAGEMANAGER_LOG_NAME, "Message %d INACTIVE by trigger", messageIndex);
                                }
                            }
                            else if(CheckMessageManager_Messages[messageIndex].clearCondition == CHECKMESSAGE_CLEAR_BY_TIMEOUT)
                            {
                                if(indexToDisplay == -1) //Set Only highest priority message (lowest id)
                                {
                                    indexToDisplay = (sint32)messageIndex;
                                }
                                CheckMessageManager_Messages[messageIndex].timer = CheckMessageManager_Messages[messageIndex].timeout - (CHECKMESSAGEMANAGER_CYCLETIME * 2);
                                CheckMessageManager_Messages[messageIndex].state = CHECKMESSAGE_TIMER_RUNNING;
                            }
                            else if (CheckMessageManager_Messages[messageIndex].clearCondition == CHECKMESSAGE_CLEAR_BY_BUTTON)
                            {
                                uint8 clearIndicationValue, clearStatus;

                                GCL_Read_CheckMessageManager_ClearIndication(&clearIndicationValue, &clearStatus);

                                if (clearIndicationValue == 1u)
                                {
                                    if (CheckMessageManager_Messages[messageIndex].reoccuring == TRUE)
                                    {
                                        CheckMessageManager_Messages[messageIndex].state = CHECKMESSAGE_BUTTON_CLEARED;
                                        Logger_Info(CHECKMESSAGEMANAGER_LOG_NAME, "Message %d INACTIVE by button", messageIndex);
                                    }
                                    else
                                    {
                                        CheckMessageManager_Messages[messageIndex].state = CHECKMESSAGE_CLEARED;
                                        Logger_Info(CHECKMESSAGEMANAGER_LOG_NAME, "Message %d CLEARED by button", messageIndex);
                                    }
                                }
                                else
                                {
                                    if(indexToDisplay == -1) //Set Only highest priority message (lowest id)
                                    {
                                        indexToDisplay = (sint32)messageIndex;
                                    }
                                }
                            }
                            else
                            {
                                ; // Do nothing
                            }

                            break;
                        }

                        case CHECKMESSAGE_BUTTON_CLEARED:
                        {
                            uint8 triggerValue, triggerStatus;

                            CheckMessageManager_Messages[messageIndex].trigger(&triggerValue, &triggerStatus);
                            if(triggerValue == 0u )
                            {
                                CheckMessageManager_Messages[messageIndex].state = CHECKMESSAGE_INACTIVE;
                            }

                            break;
                        }

                        case CHECKMESSAGE_TIMER_RUNNING:
                        {
                            if (CheckMessageManager_Messages[messageIndex].timer <= CHECKMESSAGEMANAGER_CYCLETIME)
                            {
                                CheckMessageManager_Messages[messageIndex].timer = 0;

                                if (CheckMessageManager_Messages[messageIndex].reoccuring == TRUE)
                                {
                                    CheckMessageManager_Messages[messageIndex].state = CHECKMESSAGE_TIMER_TIMEDOUT;
                                    Logger_Info(CHECKMESSAGEMANAGER_LOG_NAME, "Message %d INACTIVE by timeout", messageIndex);
                                }
                                else
                                {
                                    CheckMessageManager_Messages[messageIndex].state = CHECKMESSAGE_CLEARED;
                                    Logger_Info(CHECKMESSAGEMANAGER_LOG_NAME, "Message %d CLEARED by timeout", messageIndex);
                                }
                            }
                            else
                            {
                                if(indexToDisplay == -1) //Set Only highest priority message (lowest id)
                                {
                                    indexToDisplay = (sint32)messageIndex;
                                }

                                CheckMessageManager_Messages[messageIndex].timer = CheckMessageManager_Messages[messageIndex].timer - CHECKMESSAGEMANAGER_CYCLETIME;
                            }
                            break;
                        }

                        case CHECKMESSAGE_TIMER_TIMEDOUT:
                        {
                            uint8 triggerValue;
                            uint8 status;

                            CheckMessageManager_Messages[messageIndex].trigger(&triggerValue, &status);
                            if(triggerValue == 0u )
                            {
                                CheckMessageManager_Messages[messageIndex].state = CHECKMESSAGE_INACTIVE;
                            }
                            break;
                        }

                        case CHECKMESSAGE_CLEARED:
                            break;
                        default:
                            // Do nothing
                            break;
                    }
                }
                if(activeMessageIndex != indexToDisplay)
                {
                    activeMessageIndex = indexToDisplay;
                    if (activeMessageIndex != -1)
                    {
                        for(uint8 i = 0u; i < NUMBER_OF_SOURCES; ++i)
                        {
                            CheckMessageManager_Sources[i].write(CheckMessageManager_Messages[(uint16)activeMessageIndex].mappings[i].value);
                        }

                        LayoutManager_CheckMessageShowing(TRUE);
                        ContentsManager_ShowCheckMessage(TRUE);
                    }
                    else
                    {
                        LayoutManager_CheckMessageShowing(FALSE);
                        ContentsManager_ShowCheckMessage(FALSE);
                    }
                }
            }
        break;

        default:
            ; //Intentially left empty
        break;
    }
}


/****************************** Local Functions ******************************/
static void initCheckMessageStatus()
{
    Logger_Info(CHECKMESSAGEMANAGER_LOG_NAME, "Initializing checkmessages status");
    for(uint16 messageIndex = 0u; messageIndex < NUMBER_OF_CHECKMESSAGES; ++messageIndex)
    {
        CheckMessageManager_Messages[messageIndex].state = CHECKMESSAGE_INACTIVE;
    }
}

static boolean checkMessageAllowed(uint16 currentMessage, uint8 currentLayout)
{
    boolean allowed = FALSE;

    for (uint8 index = 0; index < CheckMessageManager_Messages[currentMessage].allowedInLayoutCount; ++index)
    {
        if ((CheckMessageManager_Messages[currentMessage].allowedInLayout != NULL) && (CheckMessageManager_Messages[currentMessage].allowedInLayout[index] == currentLayout))
        {
            allowed = TRUE;
            break;
        }
    }

    return allowed;
}

static PowerMode getPowerMode(void)
{
    uint8 status;
    PowerMode powerMode;
    GCL_Read_LayoutManager_SystemPowerMode(&powerMode, &status);

    return powerMode;
}

static uint8 getAbsoluteValue(sint8 value)
{
    //lint --e(9033) --e(571) [Convert signed to unsigned, used for absolute coordinate calculations] [- '' -]
    return (uint8)((value < 0) ? (0 - value) : value);
}

//lint -restore
