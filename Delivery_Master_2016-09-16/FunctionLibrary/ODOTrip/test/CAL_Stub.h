#ifndef CALSTUB_H
#define CALSTUB_H

/********************************* Includes **********************************/
#include "Platform_Types.h"
#include "CAL_cfg.h"

/**************************** Constants and Types ****************************/

/***************************** Exported Functions ****************************/

void CALStub_SetCurrentOdo(uint16 parameterId, uint32 value);
void CALStub_GetCurrentOdo(uint16 parameterId, uint32* value);

void CALStub_SetTripValue(uint16 parameterId, uint32 value);

uint32 CALStub_GetODORestPulseValue();

uint32 CALStub_GetTripARestPulseValue();

uint32 CALStub_GetTripBRestPulseValue();


#endif // CALSTUB_H
