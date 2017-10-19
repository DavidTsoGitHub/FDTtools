/********************************* Includes **********************************/
#include "TPMS.h"
#include "GCL_TPMS.h"
#include "Platform_Types.h"
#include "CAL.h"
#include "Logger.h"

/**************************** Constants and Types ****************************/
#define COMPONENTNAME_TPMS "TPMS"

/********************************* Variables *********************************/
static boolean WarningActive[NUMBER_OF_TIRES];

/************************* Local Function Prototypes *************************/
static void TPMS_SendTireInformation(uint8 tireLocation, const DynamicWidgetValue* value);

/***************************** Exported Functions ****************************/
void TPMS_Init(void)
{
    uint8 tirePositions[NUMBER_OF_TIRES] = {
        TPMS_FRONT_LEFT_TIRE,
        TPMS_FRONT_RIGHT_TIRE,
        TPMS_MIDDLE_AXLE_LEFT_OUTER_TIRE,
        TPMS_MIDDLE_AXLE_LEFT_INNER_TIRE,
        TPMS_MIDDLE_AXLE_RIGHT_INNER_TIRE,
        TPMS_MIDDLE_AXLE_RIGHT_OUTER_TIRE,
        TPMS_REAR_AXLE_LEFT_OUTER_TIRE,
        TPMS_REAR_AXLE_LEFT_INNER_TIRE,
        TPMS_REAR_AXLE_RIGHT_INNER_TIRE,
        TPMS_REAR_AXLE_RIGHT_OUTER_TIRE
    };
    
    for (uint8 i = 0; i < NUMBER_OF_TIRES; ++i)
    {
        DynamicWidgetValue value;
        value.Value.U16 = 0;
        value.DataType = STD_DATATYPE_UINT16;
        value.Flashing = FALSE;
        value.Valid = FALSE;
        TPMS_SendTireInformation(tirePositions[i], &value);
    }
}

void TPMS_Run(void)
{
    static uint8 LastValue = 0;
    uint8 tpms_installed;
    if(CAL_Read8(TPMS_INSTALLED, &tpms_installed) == CAL_NOK)
    {
        Logger_Info(COMPONENTNAME_TPMS, "Can't read CAL PARAMETER: TPMS_INSTALLED");
    }

    if (tpms_installed == 1U)
    {
        /** Input signal reading samples **/
        uint8 u8SignalStatus;
        uint8 TPMSTireLocation;
        uint8 TPMSTirePressure;
        uint8 TPMSTirePressureWarning;
        PowerMode mode;
        uint8 powerModeStatus;

        GCL_Read_TPMS_SystemPowerMode(&mode, &powerModeStatus);

        if ((mode == POWER_IGNITION_ON) && (powerModeStatus == GCL_SIGNAL_OK))
        {
            DynamicWidgetValue value;
            GCL_Read_TPMS_TPMSTireLocation(&TPMSTireLocation, &u8SignalStatus);
            GCL_Read_TPMS_TPMSTirePressure(&TPMSTirePressure, &u8SignalStatus);
            GCL_Read_TPMS_TPMSTirePressureWarning(&TPMSTirePressureWarning, &u8SignalStatus);

            value.Value.U16 = (uint16)TPMSTirePressure * (uint16)TPMS_TIRE_PRESSURE_GAIN;
            value.DataType = STD_DATATYPE_UINT16;

            if (TPMSTirePressureWarning == NO_WARNING_PRESSURE)
            {
                value.Flashing = FALSE;
            }
            else
            {
                value.Flashing = TRUE;
            }

            if (value.Value.U16 <= VALID_HIGH_LIMIT)
            {
                value.Valid = TRUE;
            }
            else
            {
                value.Valid = FALSE;
            }

            TPMS_SendTireInformation(TPMSTireLocation, &value);

            uint8 indicatorValue = 0;

            for (uint8 i = 0; i < NUMBER_OF_TIRES; ++i)
            {
                if (WarningActive[i] == TRUE)
                {
                    indicatorValue = 1;
                    break;
                }
            }
            
            if (LastValue != indicatorValue)
            {
                GCL_Write_TPMS_TPMSIndicatorValue(indicatorValue);
                LastValue = indicatorValue;
            }
        }
        else
        {
            // Power off, do nothing
        }
    }
    else
    {
        // TPMS not installed, do nothing!
    }
}

/****************************** Local Functions ******************************/
static void TPMS_SendTireInformation(uint8 tireLocation, const DynamicWidgetValue* value)
{
    switch (tireLocation)
    {
        case TPMS_FRONT_LEFT_TIRE:
            WarningActive[0] = value->Flashing;
            GCL_Write_TPMS_TPMSFrontLeftTireValue(*value);
            break;
        case TPMS_FRONT_RIGHT_TIRE:
            WarningActive[1] = value->Flashing;
            GCL_Write_TPMS_TPMSFrontRightTireValue(*value);
            break;
        case TPMS_MIDDLE_AXLE_LEFT_OUTER_TIRE:
            WarningActive[2] = value->Flashing;
            GCL_Write_TPMS_TPMSMiddleAxleLeftOuterTireValue(*value);
            break;
        case TPMS_MIDDLE_AXLE_LEFT_INNER_TIRE:
            WarningActive[3] = value->Flashing;
            GCL_Write_TPMS_TPMSMiddleAxleLeftInnerTireValue(*value);
            break;
        case TPMS_MIDDLE_AXLE_RIGHT_INNER_TIRE:
            WarningActive[4] = value->Flashing;
            GCL_Write_TPMS_TPMSMiddleAxleRightInnerTireValue(*value);
            break;
        case TPMS_MIDDLE_AXLE_RIGHT_OUTER_TIRE:
            WarningActive[5] = value->Flashing;
            GCL_Write_TPMS_TPMSMiddleAxleRightOuterTireValue(*value);
            break;
        case TPMS_REAR_AXLE_LEFT_OUTER_TIRE:
            WarningActive[6] = value->Flashing;
            GCL_Write_TPMS_TPMSRearAxleLeftOuterTireValue(*value);
            break;
        case TPMS_REAR_AXLE_LEFT_INNER_TIRE:
            WarningActive[7] = value->Flashing;
            GCL_Write_TPMS_TPMSRearAxleLeftInnerTireValue(*value);
            break;
        case TPMS_REAR_AXLE_RIGHT_INNER_TIRE:
            WarningActive[8] = value->Flashing;
            GCL_Write_TPMS_TPMSRearAxleRightInnerTireValue(*value);
            break;
        case TPMS_REAR_AXLE_RIGHT_OUTER_TIRE:
            WarningActive[9] = value->Flashing;
            GCL_Write_TPMS_TPMSRearAxleRightOuterTireValue(*value);
            break;
        default:
            // Unsupported Tire
            break;
    }
}
