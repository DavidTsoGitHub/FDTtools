/********************************* Includes **********************************/
#include "SegmentDisplayManager.h"
#include "SegmentDisplayManager_cfg.h"
#include "BSP.h"
#include "StandardTypes.h"
#include "GCL_SegmentDisplayManager.h"

/**************************** Constants and Types ****************************/
typedef enum
{
    SDM_OFF,
    SDM_RUNNING
} SDM_State;

/********************************* Variables *********************************/
static SDM_State currentState;
static uint32 lastValue;
static boolean refreshSegmentsOnBSP;

/************************* Local Function Prototypes *************************/
static void SDM_HandleDisplayValue(void);
static void SDM_HandleSpecialSegments(void);
static void SDM_IgnitionOn(void);
static void SDM_IgnitionOff(void);
static void SDM_SetDisplayValue(uint32 displayValue);

/***************************** Exported Functions ****************************/
void SegmentDisplayManager_Init()
{
    lastValue = 0xFFFFFFFFU;
    refreshSegmentsOnBSP = FALSE;
    SDM_IgnitionOff();
}

void SegmentDisplayManager_Run()
{
    PowerMode powerMode;
    uint8 signalStatus;

    GCL_Read_SegmentDisplayManager_PowerMode(&powerMode, &signalStatus);

    switch (currentState)
    {
        case SDM_OFF:
            if (powerMode == POWER_IGNITION_ON)
            {
                SDM_IgnitionOn();
            }
        break;
        case SDM_RUNNING:
            if (powerMode != POWER_IGNITION_ON)
            {
                SDM_IgnitionOff();
            }
            else
            {
                SDM_HandleDisplayValue();
                SDM_HandleSpecialSegments();

                if (refreshSegmentsOnBSP == TRUE)
                {
                    BSP_Set_DisplaySegments(SegmentData, SegmentDigitCount);
                    refreshSegmentsOnBSP = FALSE;
                }
            }
        break;
        default:
            // Should never happen
        break;
    }
}

/****************************** Local Functions  *****************************/
static void SDM_HandleDisplayValue()
{
    uint32 displayValue;
    uint8 signalStatus;

    GCL_Read_SegmentDisplayManager_DisplayValue(&displayValue, &signalStatus);

    if (displayValue != lastValue)
    {
        refreshSegmentsOnBSP = TRUE;
        SDM_SetDisplayValue(displayValue);
    }
}

static void SDM_HandleSpecialSegments()
{
    uint8 i;
    for (i = 0; i<SpecialSegmentsCount; ++i)
    {
        uint8 segmentStatus, signalStatus;

        SpecialSegments[i].gclReadFunction(&segmentStatus, &signalStatus);

        if (segmentStatus != SpecialSegments[i].lastStatus)
        {
            if (segmentStatus == 1U)
            {
                SegmentData[SpecialSegments[i].byteIndex] |= (uint8)(0x01U << SpecialSegments[i].bitIndex);
            }
            else
            {
                SegmentData[SpecialSegments[i].byteIndex] &= ~((uint8)(0x01U << SpecialSegments[i].bitIndex));
            }
            refreshSegmentsOnBSP = TRUE;
            SpecialSegments[i].lastStatus = segmentStatus;
        }
    }
}

static void SDM_SetDisplayValue(uint32 displayValue)
{
    sint8 i;
    uint8 digitValue;
    lastValue = displayValue;

    // For each digit we need to make sure we don't touch any special segments which might be
    // a part of the current digits segment bit pattern.
    for (i = ((sint8)SegmentDigitCount - 1); i>=0; --i)
    {
        digitValue = (uint8)(displayValue % 10U);

        // First we use AND with the inverted bit pattern for digit 8 to clear all digit related segments
        SegmentData[i] &= ~(DigitBitPattern[i][8]);


        if (digitValue <= (SegmentPatternCount - 1U))
        {
            // Then we set the bit pattern for the current digit using OR
            SegmentData[i] |= DigitBitPattern[i][digitValue];
        }

        displayValue /= 10U;
    }
}

static void SDM_IgnitionOn(void)
{
    currentState = SDM_RUNNING;
    refreshSegmentsOnBSP = TRUE;
}

static void SDM_IgnitionOff(void)
{
    currentState = SDM_OFF;
}
