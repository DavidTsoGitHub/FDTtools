#ifndef CALSTUB_H
#define CALSTUB_H

/********************************* Includes **********************************/
#include "StandardTypes.h"

/**************************** Constants and Types ****************************/


/***************************** Exported Functions ****************************/
void CALStub_Reset(void);

void CALStub_SetTotalFuel(uint32 value);
uint32 CALStub_GetTotalFuel(void);

void CALStub_SetTickValue(uint32 value);
uint32 CALStub_GetTickValue(void);
//uint8 CALStub_GetCalWriteCounter(void);

#endif // CALSTUB_H
