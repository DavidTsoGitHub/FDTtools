#ifndef GCL_OutputShaftRotationSpeed_H
#define GCL_OutputShaftRotationSpeed_H

/********************************* Includes **********************************/
#include "StandardTypes.h"

/**************************** Constants and Types ****************************/
#define GCL_SIGNAL_OK         (0U)
#define GCL_SIGNAL_TIMEOUT    (1 << 0)
#define GCL_SIGNAL_ERROR      (1 << 1)

/***************************** Exported Functions ****************************/
// Define the GCL_(Read|Write)_OutputShaftRotationSpeed_<SignalName>(...) and indication callback methods that will be called by the function block
//
// Read function template:
void GCL_Read_OutputShaftRotationSpeed_VehSpdFreqInput(uint32* value, uint8* status);
//
// Write function template:
void GCL_Write_OutputShaftRotationSpeed_OutputShaftRotationSpeed(uint16 value);
//
// Indication callback template
// void OutputShaftRotationSpeed_<SignalName>_Indication(void);

#endif // GCL_OutputShaftRotationSpeed_H
