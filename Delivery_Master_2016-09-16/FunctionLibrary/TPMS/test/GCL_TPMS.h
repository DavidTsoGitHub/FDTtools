#ifndef GCL_TPMS_H
#define GCL_TPMS_H

/********************************* Includes **********************************/
#include "Platform_Types.h"
#include "StandardTypes.h"
#include "TPMS.h"

/**************************** Constants and Types ****************************/
#define GCL_SIGNAL_OK         (0U)
#define GCL_SIGNAL_TIMEOUT    (1 << 0)
#define GCL_SIGNAL_ERROR      (1 << 1)

/***************************** Exported Functions ****************************/
// Define the GCL_TPMS_(Read|Write)_<SignalName>(...) and indication callback methods that will be called by the function block
//
// Read function template:
void GCL_Read_TPMS_SystemPowerMode(PowerMode* mode, uint8* status);
void GCL_Read_TPMS_TPMSTireLocation(uint8* value, uint8* status);
void GCL_Read_TPMS_TPMSTirePressure(uint8* value, uint8* status);
void GCL_Read_TPMS_TPMSTirePressureWarning(uint8* value, uint8* status);
//
// Write function template:
void GCL_Write_TPMS_TPMSFrontLeftTireValue(DynamicWidgetValue value);
void GCL_Write_TPMS_TPMSFrontRightTireValue(DynamicWidgetValue value);
void GCL_Write_TPMS_TPMSMiddleAxleLeftOuterTireValue(DynamicWidgetValue value);
void GCL_Write_TPMS_TPMSMiddleAxleLeftInnerTireValue(DynamicWidgetValue value);
void GCL_Write_TPMS_TPMSMiddleAxleRightOuterTireValue(DynamicWidgetValue value);
void GCL_Write_TPMS_TPMSMiddleAxleRightInnerTireValue(DynamicWidgetValue value);
void GCL_Write_TPMS_TPMSRearAxleLeftOuterTireValue(DynamicWidgetValue value);
void GCL_Write_TPMS_TPMSRearAxleLeftInnerTireValue(DynamicWidgetValue value);
void GCL_Write_TPMS_TPMSRearAxleRightOuterTireValue(DynamicWidgetValue value);
void GCL_Write_TPMS_TPMSRearAxleRightInnerTireValue(DynamicWidgetValue value);

//
// Indication callback template
void GCL_Write_TPMS_TPMSIndicatorValue(uint8 state);

#endif // GCL_TPMS_H
