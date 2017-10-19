/*** File generated 2015-11-13 09:45:24 ***/
#ifndef CAL_CFG_H
#define CAL_CFG_H

/********************************* Includes **********************************/
#include "Platform_Types.h"

/**************************** Constants and Types ****************************/
/* This enum is generated based on the 
   calibration parameters defined in 
   the function blocks and the BSP */
typedef enum
{
    //Static ODO configuration
    ODO_STATUS,
    ODO_VALUE1,
    ODO_VALUE2,
    ODO_VALUE3,
    ODO_VALUE4,
    ODO_VALUE5,
    ODO_KM_REST_PULSES,
    //Function block calibrations
    CAL_CHECKSUM,  //CALIBRATION CHECKSUM
    /* VehSpdGauge */
    VEHSPD_MAXINPUTFREQUENCY,
    VEHSPD_MAXSPEEDVALUE,
    VEHSPD_FILTERC,
    /* TurnIndication */
    TURNINDICATOR_TELLTALEDELAY,
    /* FuelLevel */
    FUEL_WARNINGTHRESHOLD,
    FUEL_WARNINGTOLERANCE,
    /* TPMS */
    TPMS_INSTALLED,
    /* LocalizationManager */
    INITIAL_LANGUAGE_VALUE,
    /* ODOTrip */
    ODO_HM_REST_PULSES_TRIP_A,
    ODO_HM_REST_PULSES_TRIP_B,
    ODO_PULSES_PER_KM,
    TRIP_A_VALUE,
    TRIP_B_VALUE,
    /* BSP Calibration parameters */
    FUEL_STEP8,
    FUEL_STEP9,
    FUEL_STEP10,
    FUEL_STEP11,
    FUEL_STEP12,
    FUEL_STEP13,
    FUEL_STEP14,
    FUEL_STEP15,
    FUEL_STEP16,
    FUEL_STEP_INTERVAL,
    FUEL_RESTPOS,
    FUEL_STARTPOS,
    TRIP_MILEAGE_A,
    TRIP_MILEAGE_B,
    TRIP_PULSE_NUM_A,
    TRIP_PULSE_NUM_B,
    TOTAL_PULSE_NUM,
    PULSE_NUM_PER_KM,
    TOTAL_MILEAGE,
    MAINTAIN_MILEAGE,
    VEHSPD_PULSE_NUM,
    VEHSPD_GAUGE_COEFF,
    VEHSPD_LEVEL1,
    VEHSPD_LEVEL2,
    VEHSPD_LEVEL3,
    VEHSPD_LEVEL4,
    VEHSPD_LEVEL5,
    VEHSPD_LEVEL6,
    VEHSPD_LEVEL7,
    VEHSPD_LEVEL8,
    VEHSPD_STEP1,
    VEHSPD_STEP2,
    VEHSPD_STEP3,
    VEHSPD_STEP4,
    VEHSPD_STEP5,
    VEHSPD_STEP6,
    VEHSPD_STEP7,
    VEHSPD_STEP8,
    VEHSPD_RESTPOS,
    VEHSPD_STARTPOS,
    FUEL_INPUT1,
    FUEL_INPUT2,
    FUEL_INPUT3,
    FUEL_INPUT4,
    FUEL_INPUT5,
    FUEL_INPUT6,
    FUEL_INPUT7,
    FUEL_INPUT8,
    FUEL_INPUT9,
    FUEL_INPUT10,
    FUEL_INPUT11,
    FUEL_INPUT12,
    FUEL_INPUT13,
    FUEL_INPUT14,
    FUEL_INPUT15,
    FUEL_INPUT16,
    FUEL_INPUT_REF_R,
    FUEL_OUTPUT1,
    FUEL_OUTPUT2,
    FUEL_OUTPUT3,
    FUEL_OUTPUT4,
    FUEL_OUTPUT5,
    FUEL_OUTPUT6,
    FUEL_OUTPUT7,
    FUEL_OUTPUT8,
    FUEL_OUTPUT9,
    FUEL_OUTPUT10,
    FUEL_OUTPUT11,
    FUEL_OUTPUT12,
    FUEL_OUTPUT13,
    FUEL_OUTPUT14,
    FUEL_OUTPUT15,
    FUEL_OUTPUT16,
    FUEL_STEP1,
    FUEL_STEP2,
    FUEL_STEP3,
    FUEL_STEP4,
    FUEL_STEP5,
    FUEL_STEP6,
    FUEL_STEP7,
    CALIBRATIONPARAMETERCOUNT
} calibration_parameter_id_t;

typedef struct
{
    uint16 Offset;
    uint8 Length;
} calibration_parameter_t;

#define CAL_TOTALPARAMETERLENGTH  211U  // 0xd3
#define CAL_START_ADDR 0x0020

extern const calibration_parameter_t CAL_CalibrationParameters[CALIBRATIONPARAMETERCOUNT];
extern uint8 CAL_DefaultCalibrationParameters[CAL_TOTALPARAMETERLENGTH];

#endif // CAL_CFG_H