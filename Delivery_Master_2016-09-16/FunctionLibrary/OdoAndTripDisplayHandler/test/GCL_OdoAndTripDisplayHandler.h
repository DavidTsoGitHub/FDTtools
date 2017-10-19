#ifndef GCL_ODOANDTRIPDISPLAYHANDLER_H
#define GCL_ODOANDTRIPDISPLAYHANDLER_H

/********************************* Includes **********************************/
#include "StandardTypes.h"

/**************************** Constants and Types ****************************/
#define GCL_SIGNAL_OK         (0U)
#define GCL_SIGNAL_TIMEOUT    (1 << 0)
#define GCL_SIGNAL_ERROR      (1 << 1)

/***************************** Exported Functions ****************************/

void GCL_Read_OdoAndTripDisplayHandler_ShowNext(uint8* value, uint8* status);
void GCL_Read_OdoAndTripDisplayHandler_OdoValue(uint32* value, uint8* status);
void GCL_Read_OdoAndTripDisplayHandler_TripValue(uint32* value, uint8* status);
void GCL_Read_OdoAndTripDisplayHandler_InstantaneousFuelConsumption(uint16* value, uint8* status);
void GCL_Read_OdoAndTripDisplayHandler_TripFuelConsumption(uint32* value, uint8* status);

void GCL_Write_OdoAndTripDisplayHandler_TripFieldText(wchar* value);
void GCL_Write_OdoAndTripDisplayHandler_OdoAndInstFuelFieldText(wchar* value);

void OdoAndTripDisplayHandler_ShowNextTrigger1_Indication();
void OdoAndTripDisplayHandler_ShowNextTrigger2_Indication();

#endif // GCL_ODOANDTRIPDISPLAYHANDLER_H
