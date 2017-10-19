/*** File generated 2015-11-04 14:03:34 ***/
#ifndef LABEL_MANAGER_H
#define LABEL_MANAGER_H

/********************************* Includes **********************************/
#include "StandardTypes.h"
#include "Font.h"

/**************************** Constants and Types ****************************/

#define NUMBER_OF_LANGUAGES 3

typedef struct
{
    wchar* text;
    const font_t* font;
    boolean useMaster;
} label_t;

typedef struct
{
    uint32 labelID;
    label_t master;
    label_t translations[NUMBER_OF_LANGUAGES];
} label_definition_t;

/***************************** Exported Functions ****************************/

void LabelManager_SetLanguage(uint8 languageID)
{
    
}

const label_t* LabelManager_GetLabel(uint32 labelID);
const font_t* LabelManager_GetDefaultFont(void);

#endif
