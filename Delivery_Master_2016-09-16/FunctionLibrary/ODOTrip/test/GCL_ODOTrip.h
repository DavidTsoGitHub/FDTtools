/*** File generated 2015-11-13 09:45:24 ***/
#ifndef GCL_ODOTRIP_H
#define GCL_ODOTRIP_H
/*** Generated GCL header file for ODOTrip ***/

#include "StandardTypes.h"
#include "GCL_Types.h"

/*** Input signals ***/
void GCL_Read_ODOTrip_VehSpdFrequency(uint32* value, uint8* status);
void GCL_Read_ODOTrip_SystemPowerMode(PowerMode* value, uint8* status);
void GCL_Read_ODOTrip_TripResetA(uint8* value, uint8* status);
void GCL_Read_ODOTrip_TripResetB(uint8* value, uint8* status);

/*** Output signals ***/
void GCL_Write_ODOTrip_CurrentODOValue(uint32 value);
void GCL_Write_ODOTrip_CurrentTripValueA(uint32 value);
void GCL_Write_ODOTrip_CurrentTripValueB(uint32 value);

/*** Signal indication callbacks ***/
//void ODOTrip_TripResetA_Indication(void);
//void ODOTrip_TripResetB_Indication(void);

#endif // GCL_ODOTRIP_H