/********************************* Includes **********************************/
#include "LocalizationManager.h"
#include "LocalizationManager_cfg.h"
#include "GCL_LocalizationManager.h"
#include "StandardTypes.h"
#include "CAL.h"
#include "Logger.h"

/**************************** Constants and Types ****************************/
#define COMPONENTNAME_LM "LocalizationManager"

/********************************* Variables *********************************/
static uint8 m_u8PreviousLanguageInCAL = 0;
static boolean m_newLanguageSelectionIndicated;

void LocalizationManager_Init(void)
{   
    uint8 u8CurrentLanguageInCAL;
    if(CAL_Read8(INITIAL_LANGUAGE_VALUE, &u8CurrentLanguageInCAL) == CAL_NOK)
    {
        Logger_Info(COMPONENTNAME_LM, "Can't read CAL PARAMETER: INITIAL_LANGUAGE_VALUE");
    }
    m_u8PreviousLanguageInCAL = u8CurrentLanguageInCAL;
    
    m_newLanguageSelectionIndicated = FALSE;
    GCL_Write_LocalizationManager_CurrentLanguageValue(u8CurrentLanguageInCAL);
    GCL_Write_LocalizationManager_Languages(&languages);
}

void LocalizationManager_Run(void)
{
    uint8 u8CurrentLanguageInCAL;
    uint8 u8SelectedLanguageStatus;
    uint8 u8SelectedLanguageValue;
    
    if(CAL_Read8(INITIAL_LANGUAGE_VALUE, &u8CurrentLanguageInCAL) == CAL_NOK)
    {
        Logger_Info(COMPONENTNAME_LM, "Can't read CAL PARAMETER: INITIAL_LANGUAGE_VALUE");
    }
    
    if (m_u8PreviousLanguageInCAL != u8CurrentLanguageInCAL)
    {
        m_u8PreviousLanguageInCAL = u8CurrentLanguageInCAL;
        GCL_Write_LocalizationManager_CurrentLanguageValue(u8CurrentLanguageInCAL);
    }
    else if (m_newLanguageSelectionIndicated == TRUE)
    {
        GCL_Read_LocalizationManager_SelectedLanguageValue(&u8SelectedLanguageValue, &u8SelectedLanguageStatus);
        if (u8SelectedLanguageValue != u8CurrentLanguageInCAL)
        {
            if(CAL_Write(INITIAL_LANGUAGE_VALUE, &u8SelectedLanguageValue, 1) == CAL_NOK)
            {
                Logger_Info(COMPONENTNAME_LM, "Can't write CAL PARAMETER: INITIAL_LANGUAGE_VALUE");
            }
            GCL_Write_LocalizationManager_CurrentLanguageValue(u8SelectedLanguageValue);
        }
        m_newLanguageSelectionIndicated = FALSE;
    }
    else
    {
        // Do nothing
    }
}

void LocalizationManager_SelectedLanguageValue_Indication()
{
    m_newLanguageSelectionIndicated = TRUE;
}
