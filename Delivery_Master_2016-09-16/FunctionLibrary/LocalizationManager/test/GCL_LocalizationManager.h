#ifndef GCL_LocalizationManager_H
#define GCL_LocalizationManager_H

/********************************* Includes **********************************/
#include "StandardTypes.h"

/**************************** Constants and Types ****************************/
#define GCL_SIGNAL_OK         (0U)
#define GCL_SIGNAL_TIMEOUT    (1 << 0)
#define GCL_SIGNAL_ERROR      (1 << 1)

/***************************** Exported Functions ****************************/

void GCL_Read_LocalizationManager_SelectedLanguageValue(uint8* value, uint8* status);
void GCL_Write_LocalizationManager_CurrentLanguageValue(uint8 value);
void GCL_Write_LocalizationManager_Languages(const LabelMappings* languages);

void LocalizationManager_SelectedLanguageValue_Indication(void);
#endif // GCL_LocalizationManager_H
