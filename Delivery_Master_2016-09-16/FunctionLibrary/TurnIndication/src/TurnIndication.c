/********************************* Includes **********************************/
#include "TurnIndication.h"
#include "GCL_TurnIndication.h"
#include "Platform_Types.h"
#include "CAL.h"
#include "SoundManager.h"
#include "Logger.h"

/**************************** Constants and Types ****************************/
#define COMPONENTNAME_TI      "TurnIndication"

#define INDICATION_ON         1U

#define TELLTALE_OFF_VALUE    0U
#define TELLTALE_ON_VALUE     1U

typedef enum
{
    BOTH_INACTIVE,
    LEFT_ACTIVE,
    RIGHT_ACTIVE
} indication_state_t;

typedef enum
{
    LEFT_TELLTALE,
    RIGHT_TELLTALE
} telltales_t;

typedef enum
{
    OFF,
    ON,
    OFF_REQUESTED,
    ON_REQUESTED
} telltale_state_t;

/********************************* Variables *********************************/
static indication_state_t currentIndicationState;
static telltale_state_t leftTelltaleState;
static telltale_state_t rightTelltaleState;
static uint8 telltaleDelayCounter;

/************************* Local Function Prototypes *************************/
static boolean leftIndicationActive(void);
static boolean rightIndicationActive(void);
static void runLeftTelltaleStateMachine(void);
static void runRightTelltaleStateMachine(void);
static void requestTelltaleOn(telltales_t telltale);
static void requestTelltaleOff(telltales_t telltale);

/***************************** Exported Functions ****************************/
void TurnIndication_Init(void)
{
        currentIndicationState = BOTH_INACTIVE;
        leftTelltaleState = OFF;
        rightTelltaleState = OFF;
        telltaleDelayCounter = 0U;
}

void TurnIndication_Run(void)
{   
    switch (currentIndicationState)
    {
        case BOTH_INACTIVE:
            if (leftIndicationActive() == TRUE)
            {
                SoundManager_RequestSound(BSP_SOUND_TICK);
                requestTelltaleOn(LEFT_TELLTALE);
                currentIndicationState = LEFT_ACTIVE;
            }
            else if (rightIndicationActive() == TRUE)
            {
                SoundManager_RequestSound(BSP_SOUND_TICK);
                requestTelltaleOn(RIGHT_TELLTALE);
                currentIndicationState = RIGHT_ACTIVE;
            }
            else
            {
                // Do nothing
            }
        break;
        case LEFT_ACTIVE:
            if (leftIndicationActive() == FALSE)
            {
                SoundManager_RequestSound(BSP_SOUND_TACK);
                requestTelltaleOff(LEFT_TELLTALE);
                currentIndicationState = BOTH_INACTIVE;
            }
        break;
        case RIGHT_ACTIVE:
            if (rightIndicationActive() == FALSE)
            {
                SoundManager_RequestSound(BSP_SOUND_TACK);
                requestTelltaleOff(RIGHT_TELLTALE);
                currentIndicationState = BOTH_INACTIVE;
            }
        break;
        default:
             // Do nothing
        break;
    }
    
    runLeftTelltaleStateMachine();
    runRightTelltaleStateMachine();
}

/****************************** Local Functions ******************************/
static boolean leftIndicationActive(void)
{
    uint8 turnIndicationStatus, signalStatus;
    GCL_Read_TurnIndication_LeftTurnIndication(&turnIndicationStatus, &signalStatus);
    boolean ret;
    
    if(turnIndicationStatus == INDICATION_ON)
    {
        ret = TRUE;
    }
    else
    {
        ret = FALSE;
    }
    return ret;
}

static boolean rightIndicationActive(void)
{
    uint8 turnIndicationStatus, signalStatus;
    GCL_Read_TurnIndication_RightTurnIndication(&turnIndicationStatus, &signalStatus);
    boolean ret;
    
    if(turnIndicationStatus == INDICATION_ON)
    {
        ret = TRUE;
    }
    else
    {
        ret =  FALSE;
    }
    return ret;
}

static void runLeftTelltaleStateMachine()
{
    switch (leftTelltaleState)
    {
        case OFF:
        break;
        case ON:
        break;
        case OFF_REQUESTED:
            if (telltaleDelayCounter == 0U)
            {
                GCL_Write_TurnIndication_TurnLeftTelltaleValue(TELLTALE_OFF_VALUE);
                leftTelltaleState = OFF;
            }
            else
            {
                --telltaleDelayCounter;
            }
        break;
        case ON_REQUESTED:
            if (telltaleDelayCounter == 0U)
            {
                GCL_Write_TurnIndication_TurnLeftTelltaleValue(TELLTALE_ON_VALUE);
                leftTelltaleState = ON;
            }
            else
            {
                --telltaleDelayCounter;
            }
        break;
        default:
            // Do nothing
        break;
    }
}

static void runRightTelltaleStateMachine()
{
    switch (rightTelltaleState)
    {
        case OFF:
        break;
        case ON:
        break;
        case OFF_REQUESTED:
            if (telltaleDelayCounter == 0U)
            {
                GCL_Write_TurnIndication_TurnRightTelltaleValue(TELLTALE_OFF_VALUE);
                rightTelltaleState = OFF;
            }
            else
            {
                --telltaleDelayCounter;
            }
        break;
        case ON_REQUESTED:
            if (telltaleDelayCounter == 0U)
            {
                GCL_Write_TurnIndication_TurnRightTelltaleValue(TELLTALE_ON_VALUE);
                rightTelltaleState = ON;
            }
            else
            {
                --telltaleDelayCounter;
            }
        break;
        default:
            // Do nothing
        break;
    }
}

static void requestTelltaleOn(telltales_t telltale)
{    
    if(CAL_Read8(TURNINDICATOR_TELLTALEDELAY, &telltaleDelayCounter) == CAL_NOK)
    {
        Logger_Info(COMPONENTNAME_TI, "Can't read CAL PARAMETER: TURNINDICATOR_TELLTALEDELAY");
    }    
    
    switch(telltale)
    {
        case LEFT_TELLTALE:
            leftTelltaleState = ON_REQUESTED;
            GCL_Write_TurnIndication_TurnRightTelltaleValue(TELLTALE_OFF_VALUE);
            rightTelltaleState = OFF;
        break;
        case RIGHT_TELLTALE:
            rightTelltaleState = ON_REQUESTED;
            GCL_Write_TurnIndication_TurnLeftTelltaleValue(TELLTALE_OFF_VALUE);
            leftTelltaleState = OFF;
        break;
        default:
            // Do nothing
        break;
    }
}

static void requestTelltaleOff(telltales_t telltale)
{
    if(CAL_Read8(TURNINDICATOR_TELLTALEDELAY, &telltaleDelayCounter) == CAL_NOK)
    {
        Logger_Info(COMPONENTNAME_TI, "Can't read CAL PARAMETER: TURNINDICATOR_TELLTALEDELAY");
    }
    
    switch(telltale)
    {
        case LEFT_TELLTALE:
            leftTelltaleState = OFF_REQUESTED;
        break;
        case RIGHT_TELLTALE:
            rightTelltaleState = OFF_REQUESTED;
        break;
        default:
            // Do nothing
        break;        
    }
}
