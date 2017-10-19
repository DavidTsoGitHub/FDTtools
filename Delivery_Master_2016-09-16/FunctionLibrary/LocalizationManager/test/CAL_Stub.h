#ifndef CALSTUB_H
#define CALSTUB_H

/********************************* Includes **********************************/
#include "StandardTypes.h"

/**************************** Constants and Types ****************************/
typedef enum
{
    INITIAL_LANGUAGE_VALUE
} calibration_parameter_id_t;

/***************************** Exported Functions ****************************/
void CALStub_Reset(void);

void CALStub_SetLanguageValue(uint8 value);
uint8 CALStub_GetLanguageValue(void);
uint8 CALStub_GetCalWriteCounter(void);

#endif // CALSTUB_H
