#include "GCL_TPMS.h"
#include "GCL_Stub.h"

static uint8 SystemPowerMode = 0;
static uint8 SystemPowerModeSignalStatus = GCL_SIGNAL_OK;

DynamicWidgetValue TPMSValues[NUMBER_OF_TIRES];
uint8 TirePressures[NUMBER_OF_TIRES];
uint8 TirePressureWarnings[NUMBER_OF_TIRES];
uint8 TireIndex;
uint8 IndicatorValue;

void GCLStub_Reset()
{
	for (int i = 0; i < NUMBER_OF_TIRES; ++i)
	{
		TPMSValues[i].Value.U8 = 0xFF;
		TPMSValues[i].Valid = FALSE;
		TPMSValues[i].Flashing = FALSE;
		TirePressures[i] = 0xFE;
		TirePressureWarnings[i] = NO_WARNING_PRESSURE;
	}

	TireIndex = 0;
	IndicatorValue = 0xFF;
    // Resets the variables to their initial state
    // Called by the test fixture setup
    // SystemPowerMode = 0;
    // SystemPowerModeSignalStatus = SIGNAL_STATUS_OK;
    // HighBeamTellTaleValue = 255;
}

// Implement the GCL_Stub control methods defined in GCLStub.h
void GCLStub_SetTirePressure(uint8 id, uint8 pressure)
{
	TirePressures[id] = pressure;
}

void GCLStub_SetTirePressureWarning(uint8 id, uint8 warningValue)
{
	TirePressureWarnings[id] = warningValue;
}

DynamicWidgetValue GCLStub_GetTPMSValue(uint8 id)
{
	return TPMSValues[id];
}

uint8 GCLStub_GetTPMSIndicatorValue()
{
	return IndicatorValue;
}

void GCLStub_SetPowerMode(uint8 powerMode)
{
	SystemPowerMode = powerMode;
}

void GCLStub_SetPowerModeSignalStatus(uint8 signalStatus)
{
	SystemPowerModeSignalStatus = signalStatus;
}

// Implement the GCL_TPMS_(Read|Write)_<SignalName>(...) methods that will be called by the function block.
// Use the stubbed GCL signal variables in the top of this file

void GCL_Read_TPMS_SystemPowerMode(PowerMode* mode, uint8* status)
{
	*mode = SystemPowerMode;
	*status = SystemPowerModeSignalStatus;
}

void GCL_Read_TPMS_TPMSTireLocation(uint8* TPMSTireLocation, uint8* status)
{
	*status = GCL_SIGNAL_OK;
	switch (TireIndex)
	{
		case 0:
			*TPMSTireLocation = TPMS_FRONT_LEFT_TIRE;
			break;
		case 1:
			*TPMSTireLocation = TPMS_FRONT_RIGHT_TIRE;
			break;
		case 2:
			*TPMSTireLocation = TPMS_MIDDLE_AXLE_LEFT_OUTER_TIRE;
			break;
		case 3:
			*TPMSTireLocation = TPMS_MIDDLE_AXLE_LEFT_INNER_TIRE;
			break;
		case 4:
			*TPMSTireLocation = TPMS_MIDDLE_AXLE_RIGHT_INNER_TIRE;
			break;
		case 5:
			*TPMSTireLocation = TPMS_MIDDLE_AXLE_RIGHT_OUTER_TIRE;
			break;
		case 6:
			*TPMSTireLocation = TPMS_REAR_AXLE_LEFT_OUTER_TIRE;
			break;
		case 7:
			*TPMSTireLocation = TPMS_REAR_AXLE_LEFT_INNER_TIRE;
			break;
		case 8:
			*TPMSTireLocation = TPMS_REAR_AXLE_RIGHT_INNER_TIRE;
			break;
		case 9:
			*TPMSTireLocation = TPMS_REAR_AXLE_RIGHT_OUTER_TIRE;
			break;
	}
}

void GCL_Read_TPMS_TPMSTirePressure(uint8* TPMSTirePressure, uint8* status)
{
	*status = GCL_SIGNAL_OK;
	*TPMSTirePressure = TirePressures[TireIndex];
}

void GCL_Read_TPMS_TPMSTirePressureWarning(uint8* TPMSTirePressureWarning, uint8* status)
{
	*status = GCL_SIGNAL_OK;
	*TPMSTirePressureWarning = TirePressureWarnings[TireIndex];

	if (TireIndex >= 9)
	{
		TireIndex = 0;
	}
	else
	{
		++TireIndex;
	}
}

void GCL_Write_TPMS_TPMSFrontLeftTireValue(DynamicWidgetValue value)
{
	TPMSValues[0].Value.U16 = value.Value.U16;
	TPMSValues[0].Flashing = value.Flashing;
	TPMSValues[0].Valid = value.Valid;
}

void GCL_Write_TPMS_TPMSFrontRightTireValue(DynamicWidgetValue value)
{
	TPMSValues[1].Value.U16 = value.Value.U16;
	TPMSValues[1].Flashing = value.Flashing;
	TPMSValues[1].Valid = value.Valid;
}

void GCL_Write_TPMS_TPMSMiddleAxleLeftOuterTireValue(DynamicWidgetValue value)
{
	TPMSValues[2].Value.U16 = value.Value.U16;
	TPMSValues[2].Flashing = value.Flashing;
	TPMSValues[2].Valid = value.Valid;
}

void GCL_Write_TPMS_TPMSMiddleAxleLeftInnerTireValue(DynamicWidgetValue value)
{
	TPMSValues[3].Value.U16 = value.Value.U16;
	TPMSValues[3].Flashing = value.Flashing;
	TPMSValues[3].Valid = value.Valid;
}

void GCL_Write_TPMS_TPMSMiddleAxleRightInnerTireValue(DynamicWidgetValue value)
{
	TPMSValues[4].Value.U16 = value.Value.U16;
	TPMSValues[4].Flashing = value.Flashing;
	TPMSValues[4].Valid = value.Valid;
}

void GCL_Write_TPMS_TPMSMiddleAxleRightOuterTireValue(DynamicWidgetValue value)
{
	TPMSValues[5].Value.U16 = value.Value.U16;
	TPMSValues[5].Flashing = value.Flashing;
	TPMSValues[5].Valid = value.Valid;
}

void GCL_Write_TPMS_TPMSRearAxleLeftOuterTireValue(DynamicWidgetValue value)
{
	TPMSValues[6].Value.U16 = value.Value.U16;
	TPMSValues[6].Flashing = value.Flashing;
	TPMSValues[6].Valid = value.Valid;
}

void GCL_Write_TPMS_TPMSRearAxleLeftInnerTireValue(DynamicWidgetValue value)
{
	TPMSValues[7].Value.U16 = value.Value.U16;
	TPMSValues[7].Flashing = value.Flashing;
	TPMSValues[7].Valid = value.Valid;
}

void GCL_Write_TPMS_TPMSRearAxleRightInnerTireValue(DynamicWidgetValue value)
{
	TPMSValues[8].Value.U16 = value.Value.U16;
	TPMSValues[8].Flashing = value.Flashing;
	TPMSValues[8].Valid = value.Valid;
}

void GCL_Write_TPMS_TPMSRearAxleRightOuterTireValue(DynamicWidgetValue value)
{
	TPMSValues[9].Value.U16 = value.Value.U16;
	TPMSValues[9].Flashing = value.Flashing;
	TPMSValues[9].Valid = value.Valid;
}

void GCL_Write_TPMS_TPMSIndicatorValue(uint8 state)
{
	IndicatorValue = state;
}
