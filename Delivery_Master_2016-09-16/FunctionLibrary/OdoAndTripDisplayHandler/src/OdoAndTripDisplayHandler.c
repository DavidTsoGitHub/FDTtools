/********************************* Includes **********************************/
#include "OdoAndTripDisplayHandler.h"
#include "GCL_OdoAndTripDisplayHandler.h"
#include "StandardTypes.h"
#include "WCharDefines.h"

/**************************** Constants and Types ****************************/
#define MAX_LABEL_LENGTH 16

/********************************* Variables *********************************/
static boolean showNextIndicated;
static boolean showingDistance;
static boolean forceRefresh;
static uint32 lastTripValue;
static uint32 lastOdoValue;
static uint16 lastInstantaneousFuelValue;
static uint32 lastTripFuelValue;
static wchar tripFieldText[MAX_LABEL_LENGTH];
static wchar odoAndConsumptionFieldText[MAX_LABEL_LENGTH];

/************************* Local Function Prototypes *************************/
static void refreshDistance(void);
static void updateOdoText(void);
static void updateTripText(void);

static void refreshFuelConsumption(void);
static void updateInstantaneousFuelText(void);
static void updateTripFuelText(void);

/***************************** Exported Functions ****************************/
void OdoAndTripDisplayHandler_Init(void)
{
    lastTripValue = 0xFFFFFFFFU;
    lastOdoValue = 0xFFFFFFFFU;
    lastInstantaneousFuelValue = 0xFFFF;
    lastTripFuelValue = 0xFFFFFFFFU;
    
     // These two lines will cause the next Run to update and show distance labels 
    showingDistance = FALSE;
    showNextIndicated = TRUE;
    
    tripFieldText[0] = WCHAR_CAPITAL_I;
    tripFieldText[1] = WCHAR_CAPITAL_N;
    tripFieldText[2] = WCHAR_CAPITAL_I;
    tripFieldText[3] = WCHAR_CAPITAL_T;
    tripFieldText[4] = WCHAR_EOL;
    
    odoAndConsumptionFieldText[0] = WCHAR_CAPITAL_I;
    odoAndConsumptionFieldText[1] = WCHAR_CAPITAL_N;
    odoAndConsumptionFieldText[2] = WCHAR_CAPITAL_I;
    odoAndConsumptionFieldText[3] = WCHAR_CAPITAL_T;
    odoAndConsumptionFieldText[4] = WCHAR_EOL;
    
    GCL_Write_OdoAndTripDisplayHandler_TripFieldText(tripFieldText);
    GCL_Write_OdoAndTripDisplayHandler_OdoAndInstFuelFieldText(odoAndConsumptionFieldText);
}

void OdoAndTripDisplayHandler_Run(void)
{
    if (showNextIndicated == TRUE)
    {
        //showingDistance = !showingDistance;
        showingDistance = (showingDistance == TRUE) ? FALSE : TRUE; 
        forceRefresh = TRUE;
        showNextIndicated = FALSE;
    }
    
    if (showingDistance == TRUE)
    {
        refreshDistance();
    }
    else
    {
        refreshFuelConsumption();
    }
}

void OdoAndTripDisplayHandler_ShowNextTrigger1_Indication()
{
    showNextIndicated = TRUE;
}

void OdoAndTripDisplayHandler_ShowNextTrigger2_Indication()
{
    showNextIndicated = TRUE;
}

/****************************** Local Functions ******************************/
static void refreshDistance(void)
{
    updateOdoText();
    updateTripText();
    forceRefresh = FALSE;
}

static void refreshFuelConsumption(void)
{
    updateInstantaneousFuelText();
    updateTripFuelText();
    forceRefresh = FALSE;
}

static void updateOdoText()
{
    uint32 u32CurrentOdoValue;
    uint8 u8Status, u8ReversedStringIndex, u8NormalStringIndex, u8Remainder;
    wchar reversedText[MAX_LABEL_LENGTH];
    u8ReversedStringIndex = 0;
    u8NormalStringIndex = 0;
    
    GCL_Read_OdoAndTripDisplayHandler_OdoValue(&u32CurrentOdoValue, &u8Status);
    
    if (!((u32CurrentOdoValue == lastOdoValue) && (forceRefresh == FALSE)))
    {
        lastOdoValue = u32CurrentOdoValue;

        do
        {
            u8Remainder = (uint8)(u32CurrentOdoValue % 10U);
            reversedText[u8ReversedStringIndex] = DigitToWchar(u8Remainder);
            ++u8ReversedStringIndex;
            u32CurrentOdoValue -= u8Remainder;
            u32CurrentOdoValue /= 10U;
        } while (u32CurrentOdoValue > 0U);
    
        do 
        {
            --u8ReversedStringIndex;
            odoAndConsumptionFieldText[u8NormalStringIndex] = reversedText[u8ReversedStringIndex];
            ++u8NormalStringIndex;
        } while (u8ReversedStringIndex>0U);
        odoAndConsumptionFieldText[u8NormalStringIndex] = WCHAR_K; // k
        ++u8NormalStringIndex;
        odoAndConsumptionFieldText[u8NormalStringIndex] = WCHAR_M; // m
        ++u8NormalStringIndex;        
        odoAndConsumptionFieldText[u8NormalStringIndex] = WCHAR_EOL;
    }
}

static void updateTripText()
{
    uint32 u32CurrentTripValue;
    uint8 u8Status, u8ReversedStringIndex, u8NormalStringIndex, u8Remainder;
    wchar reversedText[MAX_LABEL_LENGTH];
    u8ReversedStringIndex = 0;
    u8NormalStringIndex = 0;
    
    GCL_Read_OdoAndTripDisplayHandler_TripValue(&u32CurrentTripValue, &u8Status);
    
    if (!((u32CurrentTripValue == lastTripValue) && (forceRefresh == FALSE)))
    {
        lastTripValue = u32CurrentTripValue;

        if (u32CurrentTripValue < 10U)
        {
            tripFieldText[u8NormalStringIndex] = DigitToWchar(0); 
            ++u8NormalStringIndex;
            tripFieldText[u8NormalStringIndex] = WCHAR_DECIMAL_POINT;
            ++u8NormalStringIndex;
            tripFieldText[u8NormalStringIndex] = DigitToWchar((uint8)u32CurrentTripValue);
            ++u8NormalStringIndex;
        }
        else
        {
            do
            {
                u8Remainder = (uint8)(u32CurrentTripValue % 10U);
                reversedText[u8ReversedStringIndex] = DigitToWchar(u8Remainder);
                ++u8ReversedStringIndex;
                u32CurrentTripValue -= u8Remainder;
                u32CurrentTripValue /= 10U;
            } while (u32CurrentTripValue > 0U);
        
            do 
            {
                --u8ReversedStringIndex;
                if (u8ReversedStringIndex == 0U)
                {
                    tripFieldText[u8NormalStringIndex] = WCHAR_DECIMAL_POINT;
                    ++u8NormalStringIndex;
                }
                tripFieldText[u8NormalStringIndex] = reversedText[u8ReversedStringIndex];
                ++u8NormalStringIndex;
            } while (u8ReversedStringIndex > 0U);
        }
        tripFieldText[u8NormalStringIndex] = WCHAR_K;
        ++u8NormalStringIndex;
        tripFieldText[u8NormalStringIndex] = WCHAR_M;
        ++u8NormalStringIndex;        
        tripFieldText[u8NormalStringIndex] = WCHAR_EOL;
    }
}

static void updateInstantaneousFuelText(void)
{
    uint16 u16CurrentInstantaneousFuelValue;
    uint8 u8Status, u8ReversedStringIndex, u8NormalStringIndex, u8Remainder;
    wchar reversedText[MAX_LABEL_LENGTH];
    u8ReversedStringIndex = 0;
    u8NormalStringIndex = 0;
    
    GCL_Read_OdoAndTripDisplayHandler_InstantaneousFuelConsumption(&u16CurrentInstantaneousFuelValue, &u8Status);
    
    if (!((u16CurrentInstantaneousFuelValue == lastInstantaneousFuelValue) && (forceRefresh == FALSE)))
    {
        lastInstantaneousFuelValue = u16CurrentInstantaneousFuelValue;

        if (u16CurrentInstantaneousFuelValue < 10U)
        {
            odoAndConsumptionFieldText[u8NormalStringIndex] = DigitToWchar(0U);
            ++u8NormalStringIndex;
            odoAndConsumptionFieldText[u8NormalStringIndex] = WCHAR_DECIMAL_POINT;
            ++u8NormalStringIndex;
            odoAndConsumptionFieldText[u8NormalStringIndex] = DigitToWchar((uint8)u16CurrentInstantaneousFuelValue);
            ++u8NormalStringIndex;
        }
        else
        {
            do
            {
                u8Remainder = (uint8)(u16CurrentInstantaneousFuelValue % 10U);
                reversedText[u8ReversedStringIndex] = DigitToWchar(u8Remainder);
                ++u8ReversedStringIndex;
                u16CurrentInstantaneousFuelValue -= u8Remainder;
                u16CurrentInstantaneousFuelValue /= 10U;
            } while (u16CurrentInstantaneousFuelValue > 0U);
        
            do 
            {
                --u8ReversedStringIndex;
                if (u8ReversedStringIndex == 0U)
                {
                    odoAndConsumptionFieldText[u8NormalStringIndex] = WCHAR_DECIMAL_POINT;
                    ++u8NormalStringIndex;
                }
                odoAndConsumptionFieldText[u8NormalStringIndex] = reversedText[u8ReversedStringIndex];
                ++u8NormalStringIndex;
            } while (u8ReversedStringIndex>0U);
        }
    
        odoAndConsumptionFieldText[u8NormalStringIndex] = WCHAR_CAPITAL_L; 
        ++u8NormalStringIndex;
        odoAndConsumptionFieldText[u8NormalStringIndex] = WCHAR_SLASH;
        ++u8NormalStringIndex;
        odoAndConsumptionFieldText[u8NormalStringIndex] = DigitToWchar(1U);
        ++u8NormalStringIndex;
        odoAndConsumptionFieldText[u8NormalStringIndex] = DigitToWchar(0U);
        ++u8NormalStringIndex;    
        odoAndConsumptionFieldText[u8NormalStringIndex] = DigitToWchar(0U);
        ++u8NormalStringIndex;
        odoAndConsumptionFieldText[u8NormalStringIndex] = WCHAR_K;
        ++u8NormalStringIndex;
        odoAndConsumptionFieldText[u8NormalStringIndex] = WCHAR_M;
        ++u8NormalStringIndex;
        odoAndConsumptionFieldText[u8NormalStringIndex] = WCHAR_EOL;
    }
}

static void updateTripFuelText(void)
{
    uint32 u32CurrentTripFuelValue;
    uint8 u8Status, u8ReversedStringIndex, u8NormalStringIndex, u8Remainder;
    wchar reversedText[MAX_LABEL_LENGTH];
    u8ReversedStringIndex = 0;
    u8NormalStringIndex = 0;
    
    GCL_Read_OdoAndTripDisplayHandler_TripFuelConsumption(&u32CurrentTripFuelValue, &u8Status);
    
    if (!((u32CurrentTripFuelValue == lastTripFuelValue) && (forceRefresh == FALSE)))
    {
    
        lastTripFuelValue = u32CurrentTripFuelValue;
        
        if (u32CurrentTripFuelValue < 10U)
        {
            tripFieldText[u8NormalStringIndex] = DigitToWchar(0U); 
            ++u8NormalStringIndex;
            tripFieldText[u8NormalStringIndex] = WCHAR_DECIMAL_POINT;
            ++u8NormalStringIndex;
            tripFieldText[u8NormalStringIndex] = DigitToWchar((uint8)u32CurrentTripFuelValue);
            ++u8NormalStringIndex;
        }
        else
        {
            do
            {
                u8Remainder = (uint8)(u32CurrentTripFuelValue % 10U);
                reversedText[u8ReversedStringIndex] = DigitToWchar(u8Remainder);
                ++u8ReversedStringIndex;
                u32CurrentTripFuelValue -= u8Remainder;
                u32CurrentTripFuelValue /= 10U;
            } while (u32CurrentTripFuelValue > 0U);
            
            do 
            {
                --u8ReversedStringIndex;
                if (u8ReversedStringIndex == 0U)
                {
                    tripFieldText[u8NormalStringIndex] = WCHAR_DECIMAL_POINT;
                    ++u8NormalStringIndex;
                }
                tripFieldText[u8NormalStringIndex] = reversedText[u8ReversedStringIndex];
                ++u8NormalStringIndex;
                
            } while (u8ReversedStringIndex > 0U);
            
        }        
        tripFieldText[u8NormalStringIndex] = WCHAR_CAPITAL_L; 
        ++u8NormalStringIndex;
        tripFieldText[u8NormalStringIndex] = WCHAR_EOL;
    }
}
