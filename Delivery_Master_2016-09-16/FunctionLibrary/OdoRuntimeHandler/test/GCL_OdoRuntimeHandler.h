#ifndef GCL_ODORUNTIMEHANDLER_H
#define GCL_ODORUNTIMEHANDLER_H

/********************************* Includes **********************************/
#include "StandardTypes.h"

/**************************** Constants and Types ****************************/
#define GCL_SIGNAL_OK         (0U)
#define GCL_SIGNAL_TIMEOUT    (1 << 0)
#define GCL_SIGNAL_ERROR      (1 << 1)

/***************************** Exported Functions ****************************/

void GCL_Read_OdoRuntimeHandler_OdoValue(uint32* value, uint8* status);
void GCL_Read_OdoRuntimeHandler_RuntimeValue(uint16* value, uint8* status);

void GCL_Write_OdoRuntimeHandler_Value(uint32 value);
void GCL_Write_OdoRuntimeHandler_ShowDot(boolean value);
void GCL_Write_OdoRuntimeHandler_ShowHourGlass(boolean value);

void OdoRuntimeHandler_ShowNext_Indication();

#endif // GCL_ODORUNTIMEHANDLER_H
