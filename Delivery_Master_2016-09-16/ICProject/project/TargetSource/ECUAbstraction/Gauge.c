/*                                                               */
/* FILE        : Gauge.c                                         */
/* DATE        : 2015-06-24                                      */
/* AUTHOR      : Cao Wei                                         */
/* DESCRIPTION : source file of motor module                     */
/* HISTORY     : 1.0.0 release for the first time.               */
/*                                                               */
/* NOTE        : All Rights reserved by DETC, Technology Center  */
/*               Last Modified By Cao Wei on 2015-06-24          */
/*                                                               */
/*****************************************************************/
#include "MCU_Gauge.h"
#include "Gauge.h"
#include "PIT.h"
#include "Cal_cfg.h"
#include "CAL.h"

uint16 Speedometer_rf,EngineRPMS_rf,WaterCoolantTmp_rf,RightBrakePressure_rf,LeftBrakePressure_rf,FuelMeter_rf;
uint8  u8g_returnZeroMode,u8g_BigReturnZeroSpeedStage,u8g_IgnitionOffReturnZeroPeriod_Ms;

uint16 u16g_UpdateSpeedometerPeriod_Ms;
uint16 u16g_UpdateEnginRPMPeriod_Ms;

uint16 Motor0Cnt;
uint16 Motor1Cnt;
uint16 Motor2Cnt;
uint16 Motor3Cnt;
uint16 Motor4Cnt;
uint16 Motor5Cnt;
uint16 u8g_IgnitionOffReturnZero_count;
uint16 ArraGaugePos[6];
uint8 CurrentStateflag;

/*
 * Name        : ECU_StepMotor_Init
 * Description :initial all gauges
 * Arguments   : void
 * Return      : void
 */
void ECU_StepMotor_Init(void)
{
	uint8 i;
	
	speed_inc = 0u;
	tacho_inc = 0u;
	for (i=0; i<NUM_OF_FILTER; i++)
	{
	   speed_speed[i] = 65535u;
	   tacho_speed[i] = 65535u;
	}
	CAL_Read16(VEHSPD_STARTPOS, &ArraGaugePos[0]);
	CAL_Read16(ENGINERPM_STARTPOS, &ArraGaugePos[1]);
	CAL_Read16(FUEL_STARTPOS, &ArraGaugePos[2]);
	CAL_Read16(WATERCOOLANTTEMP_STARTPOS, &ArraGaugePos[3]);
	CAL_Read16(LEFTBRAKEPRESSURE_STARTPOS, &ArraGaugePos[4]);
	CAL_Read16(RIGHTBRAKEPRESSURE_PARAMETER, &ArraGaugePos[5]);
	
	u16g_UpdateSpeedometerPeriod_Ms = 10u;
	u16g_UpdateEnginRPMPeriod_Ms = 10u;
	
	/*flag of direction of the motor'scale mark*/
	Meter0_Flag.FLAG.BIT.PlateCharacteristic = 1u;
	Meter1_Flag.FLAG.BIT.PlateCharacteristic = 1u;
	Meter2_Flag.FLAG.BIT.PlateCharacteristic = 1u;
	Meter3_Flag.FLAG.BIT.PlateCharacteristic = 1u;
	Meter4_Flag.FLAG.BIT.PlateCharacteristic = 1u;
	Meter5_Flag.FLAG.BIT.PlateCharacteristic = 1u;
	
	Accelerate_Backzero(6,BIG_RETURN_ZERO);
}


/*
 * Name        : BSP_ReportError_Gauge
 * Description : set the pointer's position
 * Arguments   : GaugeId    - 
 *			   : GaugeValue - the engineering value of gauge 
 * Return      : BSP standard return type
 */	
BSP_StdReturnType BSP_Set_NeedlePosition(uint8 GaugeId, uint16 GaugeValue)
{
	uint8 i;
	uint8 u8t_angCoeff;
	uint8 parameter,u8t_IndicatedByStepper;  
	
	CAL_Read8(ANGCOEFF, &u8t_angCoeff);
	
	switch(GaugeId)
	{
		case SPEEDOMETER:
			                      
			CAL_Read8(VEHSPD_PARAMETER, &parameter);             	      
			u8t_IndicatedByStepper = (parameter & 0x10u) >> 4;
			
			if(u8t_IndicatedByStepper == 0x01u)
			{
				uint8 SpeedometerCorres_Tmpvalue[8];
				uint16 SpeedometerCorres_value[8];
				uint16 SpeedometerAng[8];
				uint16 u16t_MaxSpeedometer;
				uint16 u16t_VehicleSpeedResetZeroPosition,u16t_VehicleSpeedStartZeroPosition;
				
				/*read calibration Vehicle speed*/
				for(i=0; i<8u; i++)
				{
					CAL_Read8((calibration_parameter_id_t)(VEHSPD_LEVEL1+i), &SpeedometerCorres_Tmpvalue[i]);
					SpeedometerCorres_value[i] = SpeedometerCorres_Tmpvalue[i];
					CAL_Read16((calibration_parameter_id_t)(VEHSPD_STEP1+i), &SpeedometerAng[i]);
				}
				CAL_Read16(VEHSPD_STEP8, &u16t_MaxSpeedometer);
				CAL_Read16(VEHSPD_STARTPOS, &u16t_VehicleSpeedStartZeroPosition);
				
				if(GaugeValue == REST)
				{
					CAL_Read16(VEHSPD_RESTPOS, &u16t_VehicleSpeedResetZeroPosition);
					ArraGaugePos[GaugeId] = u16t_VehicleSpeedResetZeroPosition;
					break;
				}
				else if(GaugeValue > u16t_MaxSpeedometer)
				{	
					GaugeValue = u16t_MaxSpeedometer;
				}
				else{}
		
				ArraGaugePos[GaugeId] = calculate_ang(SPEEDOMETER,Speedometer_rf, GaugeValue, SpeedometerCorres_value, SpeedometerAng);
				ArraGaugePos[GaugeId] *= u8t_angCoeff;
				
				ArraGaugePos[GaugeId] += u16t_VehicleSpeedStartZeroPosition;
			}
			else
			{
				;
			}
			break;
			
		case ENGINERPM:
			
			CAL_Read8(ENGINERPM_PARAMETER, &parameter);             	      
			u8t_IndicatedByStepper = (parameter & 0x10u) >> 4;
			
			if(u8t_IndicatedByStepper == 0x01u)
			{
				uint16 EnginRPMCorres_value[8];
				uint16 EnginRPMAng[8];
				uint16 u16t_MaxEngineRPM;
				uint16 u16t_EnginRPMResetZeroPosition,u16t_EnginRPMStartZeroPosition;
				
				/*read calibration Vehicle speed*/
				for(i=0; i<8u; i++)
				{
					CAL_Read16((calibration_parameter_id_t)(ENGINERPM_LEVEL1+i), &EnginRPMCorres_value[i]);
					CAL_Read16((calibration_parameter_id_t)(ENGINERPM_STEP1+i), &EnginRPMAng[i]);
				}
				//CAL_Read16(MAX_ENGINERPM, &u16t_MaxEngineRPM);
				CAL_Read16(ENGINERPM_LEVEL8, &u16t_MaxEngineRPM);
				CAL_Read16(ENGINERPM_STARTPOS, &u16t_EnginRPMStartZeroPosition);
				
				if(GaugeValue == REST)
				{
					CAL_Read16(ENGINERPM_RESTPOS, &u16t_EnginRPMResetZeroPosition);
					ArraGaugePos[GaugeId] = u16t_EnginRPMResetZeroPosition;
					break;
				}
				else if(GaugeValue > u16t_MaxEngineRPM)
				{	
					GaugeValue = u16t_MaxEngineRPM;				
				}
				else{}
				ArraGaugePos[GaugeId] = calculate_ang(ENGINERPM,EngineRPMS_rf, GaugeValue, EnginRPMCorres_value, EnginRPMAng);
				
				ArraGaugePos[GaugeId] *= u8t_angCoeff;					
				ArraGaugePos[GaugeId] += u16t_EnginRPMStartZeroPosition;
			}
			else
			{
				;
			}
			break;
			
		case WATERCOOLANTTEMP:
			
			CAL_Read8(WATERCOOLANTTEMP_PARAMETER, &parameter);             	      
			u8t_IndicatedByStepper = (parameter & 0x10u) >> 4;
			
			if(u8t_IndicatedByStepper == 0x01u)
			{
				uint8 WaterCoolantTmpCorres_TempValue[16];
				uint16 WaterCoolantTmpCorres_value[16];
				uint16 WaterCoolantTmpAng[16];
				uint8 u8t_MaxWaterCoolantTmp,u8t_MinWaterCoolantTmp;
				uint16 u16t_WaterCoolantTmpResetZeroPosition,u16t_WaterCoolantTmpStartZeroPosition;
				
				/* read calibration fuellevel */
				for(i=0; i<16u; i++)
				{
					CAL_Read8((calibration_parameter_id_t)(WATERCOOLANTTEMP_OUTPUT1+i),&WaterCoolantTmpCorres_TempValue[i]);
					WaterCoolantTmpCorres_value[i] = WaterCoolantTmpCorres_TempValue[i];
					CAL_Read16((calibration_parameter_id_t)(WATERCOOLANTTEMP_STEP1+i), &WaterCoolantTmpAng[i]);
				}
				
				if((parameter&0x04) == 0x00u)
				{
					CAL_Read8(WATERCOOLANTTEMP_OUTPUT16, &u8t_MaxWaterCoolantTmp);
					CAL_Read8(WATERCOOLANTTEMP_OUTPUT1, &u8t_MinWaterCoolantTmp);
				}
				else
				{
					CAL_Read8(WATERCOOLANTTEMP_OUTPUT1, &u8t_MaxWaterCoolantTmp);
					CAL_Read8(WATERCOOLANTTEMP_OUTPUT16, &u8t_MinWaterCoolantTmp);
				}
				CAL_Read16(WATERCOOLANTTEMP_STARTPOS, &u16t_WaterCoolantTmpStartZeroPosition);
				
				if(GaugeValue == REST)
				{
					CAL_Read16(WATERCOOLANTTEMP_RESTPOS, &u16t_WaterCoolantTmpResetZeroPosition);
					ArraGaugePos[GaugeId] = u16t_WaterCoolantTmpResetZeroPosition;
					break;
				}
				else if(GaugeValue > u8t_MaxWaterCoolantTmp)
				{	
					GaugeValue = u8t_MaxWaterCoolantTmp;				
				}
				else if(GaugeValue < u8t_MinWaterCoolantTmp)
				{	
					GaugeValue = u8t_MinWaterCoolantTmp;
				}
				else{}
		
				ArraGaugePos[GaugeId] = calculate_ang(WATERCOOLANTTEMP,WaterCoolantTmp_rf, GaugeValue, WaterCoolantTmpCorres_value, WaterCoolantTmpAng);
				ArraGaugePos[GaugeId] *= u8t_angCoeff;
				
				ArraGaugePos[GaugeId] += u16t_WaterCoolantTmpStartZeroPosition;
			}
			else
			{
				;
			}
			break;
			
		case RIGHTBRAKEPRESSURE:
			
			CAL_Read8(RIGHTBRAKEPRESSURE_PARAMETER, &parameter);             	      
			u8t_IndicatedByStepper = (parameter & 0x10u) >> 4;
			
			if(u8t_IndicatedByStepper == 0x01u)
			{
				uint8 RightBrakePressureCorres_TempValue[16],u8t_MaxRightBrakePressure;
				uint16 RightBrakePressureCorres_value[16];
				uint16 RightBrakePressureAng[16];
				uint16 u16t_RightBrakePressureResetZeroPosition,u16t_RightBrakePressureStartZeroPosition;
				
				/* read calibration fuellevel */
				for(i=0; i<16u; i++)
				{
					CAL_Read8((calibration_parameter_id_t)(RIGHTBRAKEPRESSURE_OUTPUT1+i),&RightBrakePressureCorres_TempValue[i]);
					RightBrakePressureCorres_value[i] = RightBrakePressureCorres_TempValue[i];
					CAL_Read16((calibration_parameter_id_t)(RIGHTBRAKEPRESSURE_STEP1+i), &RightBrakePressureAng[i]);
				}
				
				if((parameter&0x04) == 0x00u)
					CAL_Read8(RIGHTBRAKEPRESSURE_OUTPUT16, &u8t_MaxRightBrakePressure);
				else
					CAL_Read8(RIGHTBRAKEPRESSURE_OUTPUT1, &u8t_MaxRightBrakePressure);
				CAL_Read16(RIGHTBRAKEPRESSURE_STARTPOS, &u16t_RightBrakePressureStartZeroPosition);
				
				if(GaugeValue == REST)
				{
					CAL_Read16(RIGHTBRAKEPRESSURE_RESTPOS, &u16t_RightBrakePressureResetZeroPosition);
					ArraGaugePos[GaugeId] = u16t_RightBrakePressureResetZeroPosition;
					break;
				}
				else if(GaugeValue > u8t_MaxRightBrakePressure)
				{	
					GaugeValue = u8t_MaxRightBrakePressure;				
				}
				else{}
		
				ArraGaugePos[GaugeId] = calculate_ang(RIGHTBRAKEPRESSURE,RightBrakePressure_rf, GaugeValue, RightBrakePressureCorres_value, RightBrakePressureAng);
				ArraGaugePos[GaugeId] *= u8t_angCoeff;
				
				ArraGaugePos[GaugeId] += u16t_RightBrakePressureStartZeroPosition;
			}
			else
			{
				;
			}
			break;
		case LEFTBRAKEPRESSURE:
			
			CAL_Read8(LEFTBRAKEPRESSURE_PARAMETER, &parameter);             	      
			u8t_IndicatedByStepper = (parameter & 0x10u) >> 4;
			
			if(u8t_IndicatedByStepper == 0x01u)
			{
				uint8 LeftBrakePressureCorres_TempValue[16],u8t_MaxLeftBrakePressure;
				uint16 LeftBrakePressureCorres_value[16];
				uint16 LeftBrakePressureAng[16];
				uint16 u16t_LeftBrakePressureResetZeroPosition,u16t_LeftBrakePressureStartZeroPosition;
				
				/* read calibration fuellevel */
				for(i=0; i<16u; i++)
				{
					CAL_Read8((calibration_parameter_id_t)(LEFTBRAKEPRESSURE_OUTPUT1+i),&LeftBrakePressureCorres_TempValue[i]);
					LeftBrakePressureCorres_value[i] = LeftBrakePressureCorres_TempValue[i];
					CAL_Read16((calibration_parameter_id_t)(LEFTBRAKEPRESSURE_STEP1+i), &LeftBrakePressureAng[i]);
				}
				
				if((parameter&0x04) == 0x00u)
					CAL_Read8(LEFTBRAKEPRESSURE_OUTPUT16, &u8t_MaxLeftBrakePressure);
				else
					CAL_Read8(LEFTBRAKEPRESSURE_OUTPUT1, &u8t_MaxLeftBrakePressure);
				CAL_Read16(LEFTBRAKEPRESSURE_STARTPOS, &u16t_LeftBrakePressureStartZeroPosition);
				
				if(GaugeValue == REST)
				{
					CAL_Read16(LEFTBRAKEPRESSURE_RESTPOS, &u16t_LeftBrakePressureResetZeroPosition);
					ArraGaugePos[GaugeId] = u16t_LeftBrakePressureResetZeroPosition;
					break;
				}
				else if(GaugeValue > u8t_MaxLeftBrakePressure)
				{	
					GaugeValue = u8t_MaxLeftBrakePressure;				
				}
				else{}
		
				ArraGaugePos[GaugeId] = calculate_ang(LEFTBRAKEPRESSURE,LeftBrakePressure_rf, GaugeValue, LeftBrakePressureCorres_value, LeftBrakePressureAng);
				ArraGaugePos[GaugeId] *= u8t_angCoeff;
				
				ArraGaugePos[GaugeId] += u16t_LeftBrakePressureStartZeroPosition;
			}
			else
			{
				;
			}
			break;
		case FUELMETER:

			CAL_Read8(FUEL_PARAMETER, &parameter);             	      
			u8t_IndicatedByStepper = (parameter & 0x10u) >> 4;
			
			if(u8t_IndicatedByStepper == 0x01u)
			{
				uint8 FuelMeterCorres_TempValue[16];
				uint16 FuelMeterCorres_value[16];
				uint16 FuelMeterAng[16];
				uint8 u8t_MaxFuelMeter;
				uint16 u16t_FuelMeterResetZeroPosition,u16t_FuelMeterStartZeroPosition;
				
				/* read calibration fuellevel */
				for(i=0; i<16u; i++)
				{
					CAL_Read8((calibration_parameter_id_t)(FUEL_OUTPUT1+i),&FuelMeterCorres_TempValue[i]);
					FuelMeterCorres_value[i] = FuelMeterCorres_TempValue[i];
					CAL_Read16((calibration_parameter_id_t)(FUEL_STEP1+i), &FuelMeterAng[i]);
				}
				
				if((parameter&0x04) == 0x00u)
					CAL_Read8(FUEL_OUTPUT16, &u8t_MaxFuelMeter);
				else
					CAL_Read8(FUEL_OUTPUT1, &u8t_MaxFuelMeter);
				CAL_Read16(FUEL_STARTPOS, &u16t_FuelMeterStartZeroPosition);
				
				if(GaugeValue == REST)
				{
					CAL_Read16(FUEL_RESTPOS, &u16t_FuelMeterResetZeroPosition);
					ArraGaugePos[GaugeId] = u16t_FuelMeterResetZeroPosition;
					break;
				}
				else if(GaugeValue > u8t_MaxFuelMeter)
				{	
					GaugeValue = u8t_MaxFuelMeter;				
				}
				else
				{}
				
				ArraGaugePos[GaugeId] = calculate_ang(FUELMETER,FuelMeter_rf, GaugeValue, FuelMeterCorres_value, FuelMeterAng);
				ArraGaugePos[GaugeId] *= u8t_angCoeff;
				
				ArraGaugePos[GaugeId] += u16t_FuelMeterStartZeroPosition;
			}
			else
			{
				;
			}
			break;
		default:
			return BSP_INVALID_RANGE;
			break;
	}
	
	return BSP_OK;
}

BSP_StdReturnType BSP_Service_GaugeTest(uint8 GaugeId, BSP_TestCommand Command)
{
    /* TODO: to be implemented */
}

/*
 * Name        : BSP_ReportError_Gauge
 * Description : Report the error of Gauges
 * Arguments   : GaugeId - 
 *			   : FTB    -
 * Return      : BSP standard return type
 */	
BSP_StdReturnType BSP_ReportError_Gauge(uint8 GaugeId, uint8 *FTB)
{
	GaugeId = GaugeId;
	*FTB = 0;
	return BSP_OK;
}

/*
 * Name        : Accelerate_Backzero(uint8 u8t_TotalNumber)
 * Description : move all the motors to o position 
 * Arguments   : void
 * Return      : void
 */
void Accelerate_Backzero(uint8 u8t_TotalNumber,uint8 Return_Zero_Mode)
{
	u8t_TotalNumber = u8t_TotalNumber;
	u8g_returnZeroMode = Return_Zero_Mode;
	
	u8g_BigReturnZeroSpeedStage = 1;
	CurrentStateflag = 1u;
}

/*
 * Name        : StepMotorUpdate(uint8 GaugeId, uint16 GaugePos)
 * Description : set pointer's position
 * Arguments   : GaugeId  - 
 *			   : GaugePos - target position the num of steps
 * Return      : void
 */	               
void StepMotorUpdate(uint8 GaugeId, uint16 GaugePos)
{

	switch (GaugeId)
	{
		case 0u:
			
			if (GaugePos > STEPMOTOR0MAX)
				GaugePos = STEPMOTOR0MAX;
			
			smctrl_gauge[0].TAR_POS = GaugePos;
			smctrl_gauge[0].ACTIONFLAG = 1;
			Motor_Speed (0);
			
			break;	

		case 1u:
			
			if (GaugePos > STEPMOTOR1MAX)
				GaugePos = STEPMOTOR1MAX;
			
			smctrl_gauge[1].TAR_POS = GaugePos;
			smctrl_gauge[1].ACTIONFLAG = 1;
			Motor_Tacho (1);
			
			break;

		case 2u:
			
			if (GaugePos > STEPMOTOR2MAX)
				GaugePos = STEPMOTOR2MAX;
			
			smctrl_gauge[2].TAR_POS = GaugePos;
			smctrl_gauge[2].ACTIONFLAG = 1u;
			Motor_Driver (2);
			
			break;

		case 3u:

			if (GaugePos > STEPMOTOR3MAX)
				GaugePos = STEPMOTOR3MAX;
			
			smctrl_gauge[3].TAR_POS = GaugePos;
			smctrl_gauge[3].ACTIONFLAG = 1u;
			Motor_Driver (3);
			
			break;

		case 4u:

			if (GaugePos > STEPMOTOR4MAX)
				GaugePos = STEPMOTOR4MAX;
			
			smctrl_gauge[4].TAR_POS = GaugePos;
			smctrl_gauge[4].ACTIONFLAG = 1u;
			Motor_Driver (4);
			
			break;	

		case 5u:

			if (GaugePos > STEPMOTOR5MAX)
				GaugePos = STEPMOTOR5MAX;
			
			smctrl_gauge[5].TAR_POS = GaugePos;
			smctrl_gauge[5].ACTIONFLAG = 1u;
			Motor_Driver (5);
			
			break;	
			
		default:
			break;
	}
}


/*
 * Name        : PIT_ISR3
 * Description : deal with motor's routine
 * Arguments   : none
 * Return      : none
 */
void PIT_Isr3(void)
{
	uint16 u16t_CallMotor2MoveOneStepTime,u16t_CallMotor3MoveOneStepTime,u16t_CallMotor4MoveOneStepTime,u16t_CallMotor5MoveOneStepTime;
	uint16 u16t_VehicleSpeedStartZeroPosition,u16t_EnginRPMStartZeroPosition,u16t_FuelMeterStartZeroPosition,
	       u16t_WaterCoolantTmpStartZeroPosition,u16t_LeftBrakePressureStartZeroPosition,u16t_RightBrakePressureStartZeroPosition;
							
	static uint16 j = 0;
	uint8 i;
	static uint16 M0ReturnZeroStepsTemp;
	static uint16 M1ReturnZeroStepsTemp;
	static uint16 M2ReturnZeroStepsTemp;
	static uint16 M3ReturnZeroStepsTemp;
	static uint16 M4ReturnZeroStepsTemp;
	static uint16 M5ReturnZeroStepsTemp;
	
	CAL_Read16(VEHSPD_STARTPOS, &u16t_VehicleSpeedStartZeroPosition);
	CAL_Read16(ENGINERPM_STARTPOS, &u16t_EnginRPMStartZeroPosition);
	CAL_Read16(FUEL_STARTPOS, &u16t_FuelMeterStartZeroPosition);
	CAL_Read16(WATERCOOLANTTEMP_STARTPOS, &u16t_WaterCoolantTmpStartZeroPosition);
	CAL_Read16(LEFTBRAKEPRESSURE_STARTPOS, &u16t_LeftBrakePressureStartZeroPosition);
	CAL_Read16(RIGHTBRAKEPRESSURE_STARTPOS, &u16t_RightBrakePressureStartZeroPosition);
	
	uint8 u8t_parameter = 0x03;
	uint8 stepper_back_par = 4;//3u;
	
	CLEAR_PIT_INT_FLAG(3);       /* clear PIT 1 interrupt flag */

	if(CurrentStateflag == 1u)
	{
		if(u8g_returnZeroMode == BIG_RETURN_ZERO)
		{
			if( (u8t_parameter & 0x03u) == 0x01)
			{	
				if(u8g_BigReturnZeroSpeedStage == 0)
				{
					PIT.CH[3].LDVAL.R  = 64uL*1000uL;/*speed=83¡ã/s */
					/* fisrt walk 5 degree*/
					if(j < 60u)
					{
						j++;
						ms_forward();
						if(j == 60u)
						{
							u8g_BigReturnZeroSpeedStage = 1u;
							j = 0;
						}
						return;
					}	
				}
			}
			
			if(u8g_BigReturnZeroSpeedStage == 1)
			{
				if(j < 20u)
				{
					j++;
					ms_backzero();
					if(j == 20u)
					{
						u8g_BigReturnZeroSpeedStage = 2u;
						j = 0;
					}
					return;
				}
			}
			else if(u8g_BigReturnZeroSpeedStage == 2u)
			{
				PIT.CH[3].LDVAL.R = 64uL*560uL;	/*speed=148¡ã/s */
				if(j < 12u)
				{ 
					j++;
					ms_backzero();
					if(j == 12u)
					{
						u8g_BigReturnZeroSpeedStage = 3u;
						j = 0;
					}
					return;
				}
			}
			else if(u8g_BigReturnZeroSpeedStage == 3u)
			{
				PIT.CH[3].LDVAL.R = 64uL*417uL;	/*speed=200¡ã/s */
				if(j < 12u)
				{ 
					j++;
					ms_backzero();
					if(j == 12u)
					{
						u8g_BigReturnZeroSpeedStage = 4u;
						j = 0;
					}
					return;
				}
			}
			else if(u8g_BigReturnZeroSpeedStage == 4u)
			{
				PIT.CH[3].LDVAL.R = 64uL*278uL;	/*speed=300¡ã/s */
				if(j < 12u)
				{ 
					j++;
					ms_backzero();
					if(j == 12u)
					{
						u8g_BigReturnZeroSpeedStage = 5u;
						j = 0;
					}
					return;
				}
			}
			else if(u8g_BigReturnZeroSpeedStage == 5u)
			{	
				if( stepper_back_par > 1 )
					PIT.CH[3].LDVAL.R = 64uL*231uL;	/*speed=360¡ã/s */
				if(j < 12u)
				{ 
					j++;
					ms_backzero();
					if(j == 12u)
					{
						u8g_BigReturnZeroSpeedStage = 6u;
						j = 0;
					}
					return;
				}
			}
			else if(u8g_BigReturnZeroSpeedStage == 6u)
			{
				if( stepper_back_par > 1 )
					PIT.CH[3].LDVAL.R = 64uL*208uL;	/*speed=400¡ã/s */
				if(j < 12u)
				{ 
					j++;
					ms_backzero();
					if(j == 12u)
					{
						u8g_BigReturnZeroSpeedStage = 7u;
						j = 0;
					}
					return;
				}
			}
			else if(u8g_BigReturnZeroSpeedStage == 7u)
			{
				if( stepper_back_par > 2 )
					PIT.CH[3].LDVAL.R = 64uL*194uL;	/*speed=430¡ã/s */
				if(j < 6u)
				{ 
					j++;
					ms_backzero();
					if(j == 6u)
					{
						u8g_BigReturnZeroSpeedStage = 8u;
						j = 0;
					}
					return;
				}
			}
			else if(u8g_BigReturnZeroSpeedStage == 8u)
			{
				if( stepper_back_par > 2 )
					PIT.CH[3].LDVAL.R = 64uL*185uL;	/*speed=458¡ã/s */
				if(j < 6u)
				{ 
					j++;
					ms_backzero();
					if(j == 6u)
					{
						u8g_BigReturnZeroSpeedStage = 9u;
						j = 0;
					}
					return;
				}
			}
			else if(u8g_BigReturnZeroSpeedStage == 9u)
			{
				if( stepper_back_par > 2 )
					PIT.CH[3].LDVAL.R = 64uL*185uL;	/*speed=458¡ã/s */
				if(j < 6u)
				{ 
					j++;
					ms_backzero();
					if(j == 6u)
					{
						u8g_BigReturnZeroSpeedStage = 10u;
						j = 0;
					}
					return;
				}
			}
			else if(u8g_BigReturnZeroSpeedStage == 10u)
			{
				if( stepper_back_par > 2 )
					PIT.CH[3].LDVAL.R = 64uL*179uL;	/*speed=466¡ã/s */
				if(j < 6u)
				{ 
					j++;
					ms_backzero();
					if(j == 6u)
					{
						u8g_BigReturnZeroSpeedStage = 11u;
						j = 0;
					}
					return;
				}
			}
			else if(u8g_BigReturnZeroSpeedStage == 11u)
			{
				if( stepper_back_par > 2 )
					PIT.CH[3].LDVAL.R = 64uL*174uL;	/*speed=479¡ã/s */
				if(j < 6u)
				{ 
					j++;
					ms_backzero();
					if(j == 6u)
					{
						u8g_BigReturnZeroSpeedStage = 12u;
						j = 0;
					}
					return;
				}
			}
			else if(u8g_BigReturnZeroSpeedStage == 12u)
			{
				if( stepper_back_par > 2 )
					PIT.CH[3].LDVAL.R = 64uL*167uL;	/*speed=500¡ã/s */
				if(j < 12u)
				{ 
					j++;
					ms_backzero();
					if(j == 12u)
					{
						u8g_BigReturnZeroSpeedStage = 13u;
						j = 0;
					}
					return;
				}
			}
			else if(u8g_BigReturnZeroSpeedStage == 13u)
			{
				if( stepper_back_par > 3 )
					PIT.CH[3].LDVAL.R = 64uL*162uL;	/*speed=514¡ã/s */
				if(j < 12u)
				{ 
					j++;
					ms_backzero();
					if(j == 12u)
					{
						u8g_BigReturnZeroSpeedStage = 14u;
						j = 0;
					}
					return;
				}
			}
			else if(u8g_BigReturnZeroSpeedStage == 14u)
			{
				if( stepper_back_par > 3 )
					PIT.CH[3].LDVAL.R = 64uL*157uL;	/*speed=530¡ã/s */
				if(j < 12*2u)
				{ 
					j++;
					ms_backzero();
					if(j == 12u*2)
					{
						u8g_BigReturnZeroSpeedStage = 15u;
						j = 0;
					}
					return;
				}
			}
			else if(u8g_BigReturnZeroSpeedStage == 15u)
			{
				if( stepper_back_par > 3 )
					PIT.CH[3].LDVAL.R = 64uL*154uL;	/*speed=541¡ã/s */
				if(j < 12*2u)
				{ 
					j++;
					ms_backzero();
					if(j == 12u*2)
					{
						u8g_BigReturnZeroSpeedStage = 16u;
						j = 0;
					}
					return;
				}
			}
			else if(u8g_BigReturnZeroSpeedStage == 16u)
			{
				if( stepper_back_par > 3 )
					PIT.CH[3].LDVAL.R = 64uL*152uL;	/*speed=548¡ã/s */
				if(j < 12*2u)
				{ 
					j++;
					ms_backzero();
					if(j == 12u*2)
					{
						u8g_BigReturnZeroSpeedStage = 17u;
						j = 0;
					}
					return;
				}
			}
			else if(u8g_BigReturnZeroSpeedStage == 17u)
			{
				if( stepper_back_par > 4u )
					PIT.CH[3].LDVAL.R = 64uL*150uL;	/*speed=554¡ã/s */
				if(j < 12u*2)
				{ 
					j++;
					ms_backzero();
					if(j == 12u*2)
					{
						u8g_BigReturnZeroSpeedStage = 18u;
						j = 0;
					}
					return;
				}
			}
			else if(u8g_BigReturnZeroSpeedStage == 18u)
			{
				if( stepper_back_par > 4u )
					PIT.CH[3].LDVAL.R = 64uL*149uL;	/*speed=559¡ã/s */
				if(j < 12u*3)
				{ 
					j++;
					ms_backzero();
					if(j == 12u*3)
					{
						u8g_BigReturnZeroSpeedStage = 19u;
						j = 0;
					}
					return;
				}
			}
			else if(u8g_BigReturnZeroSpeedStage == 19u)
			{
				if( stepper_back_par > 4u )
					PIT.CH[3].LDVAL.R = 64uL*148uL;	/*speed=565¡ã/s */
				if(j < 12u*3)
				{ 
					j++;
					ms_backzero();
					if(j == 12u*3)
					{
						u8g_BigReturnZeroSpeedStage = 20u;
						j = 0;
					}
					return;
				}
			}
			else if(u8g_BigReturnZeroSpeedStage == 20u)
			{
				if( stepper_back_par == 6u )
					PIT.CH[3].LDVAL.R = 64uL*147uL;	/*speed=570¡ã/s */
				if(j < 12u*3)
				{ 
					j++;
					ms_backzero();
					if(j == 12u*3)
					{
						u8g_BigReturnZeroSpeedStage = 21u;
						j = 0;
					}
					return;
				}
			}
			else if(u8g_BigReturnZeroSpeedStage == 21u)
			{
				if( stepper_back_par == 6u )
					PIT.CH[3].LDVAL.R = 64uL*145uL;	/*speed=575¡ã/s */
				if(j < 12*3u)
				{ 
					j++;
					ms_backzero();
					if(j == 12u*3)
					{
						u8g_BigReturnZeroSpeedStage = 22u;
						j = 0;
					}
					return;
				}
			}
			else if(u8g_BigReturnZeroSpeedStage == 22u)
			{
				if( stepper_back_par == 6u )
					PIT.CH[3].LDVAL.R = 64uL*144uL;	/*speed=580¡ã/s */
				if(j < 4000u)
				{ 
					j++;
					ms_backzero();
					if(j == 4000u)
					{
						u8g_BigReturnZeroSpeedStage = 23u;
						j = 0;
					}
					return;
				}
			}
			else if(u8g_BigReturnZeroSpeedStage == 23u)
			{
				/*moving in low speed to eliminate motor'rebonding*/
				PIT.CH[3].LDVAL.R = 64uL*1024uL;	/*speed=81¡ã/s */
				if(j < 24u)
				{ 
					j++;
					ms_backzero();
					if(j == 24u)
					{
						u8g_BigReturnZeroSpeedStage = 24u;
						j = 0;
						

						M0ReturnZeroStepsTemp = u16t_VehicleSpeedStartZeroPosition;
						M1ReturnZeroStepsTemp = u16t_EnginRPMStartZeroPosition;
						M2ReturnZeroStepsTemp = u16t_FuelMeterStartZeroPosition;
						M3ReturnZeroStepsTemp = u16t_WaterCoolantTmpStartZeroPosition;
						M4ReturnZeroStepsTemp = u16t_LeftBrakePressureStartZeroPosition;
						M5ReturnZeroStepsTemp = u16t_RightBrakePressureStartZeroPosition;
					}
					return;
				}
			}
			
			if(u8g_returnZeroMode != BIG_RETURN_ZERO_METHOD_1)
			{
				/*return to 0¡ã*/
				//if (u8t_parameter & 0x3 == 0x3)
				{					
					if(M0ReturnZeroStepsTemp||M1ReturnZeroStepsTemp||M2ReturnZeroStepsTemp||M3ReturnZeroStepsTemp||M4ReturnZeroStepsTemp||M5ReturnZeroStepsTemp)
					{		
						ms_forward();
						
						if(M0ReturnZeroStepsTemp>0u)
							M0ReturnZeroStepsTemp--;
						if(M1ReturnZeroStepsTemp>0u)
							M1ReturnZeroStepsTemp--;
						if(M2ReturnZeroStepsTemp>0u)
							M2ReturnZeroStepsTemp--;
						if(M3ReturnZeroStepsTemp>0u)
							M3ReturnZeroStepsTemp--;
						if(M4ReturnZeroStepsTemp>0u)
							M4ReturnZeroStepsTemp--;
						if(M5ReturnZeroStepsTemp>0u)
							M5ReturnZeroStepsTemp--;
						
						return;
					}
				}

				smctrl_gauge[SPEEDOMETER].CUR_POS = u16t_VehicleSpeedStartZeroPosition; /* 0 degree position, max position*/
				smctrl_gauge[SPEEDOMETER].TAR_POS = u16t_VehicleSpeedStartZeroPosition;
				smctrl_gauge[ENGINERPM].CUR_POS = u16t_EnginRPMStartZeroPosition;
				smctrl_gauge[ENGINERPM].TAR_POS = u16t_EnginRPMStartZeroPosition;
				smctrl_gauge[FUELMETER].CUR_POS = u16t_FuelMeterStartZeroPosition;
				smctrl_gauge[FUELMETER].TAR_POS = u16t_FuelMeterStartZeroPosition;
				smctrl_gauge[WATERCOOLANTTEMP].CUR_POS = u16t_WaterCoolantTmpStartZeroPosition;
				smctrl_gauge[WATERCOOLANTTEMP].TAR_POS = u16t_WaterCoolantTmpStartZeroPosition;
				smctrl_gauge[LEFTBRAKEPRESSURE].CUR_POS = u16t_LeftBrakePressureStartZeroPosition;
				smctrl_gauge[LEFTBRAKEPRESSURE].TAR_POS = u16t_LeftBrakePressureStartZeroPosition;
				smctrl_gauge[RIGHTBRAKEPRESSURE].CUR_POS = u16t_RightBrakePressureStartZeroPosition;
				smctrl_gauge[RIGHTBRAKEPRESSURE].TAR_POS = u16t_RightBrakePressureStartZeroPosition;
				
				for (i = 0; i < 6u; i++) {
			 
					smctrl_gauge[i].DIR = 0;
					smctrl_gauge[i].STATE_FLAG = 0;
					smctrl_gauge[i].Distance_Gap = 0;
					smctrl_gauge[i].ACTIONFLAG = 1;
					}
				
				j = 0u;
				PIT.CH[3].LDVAL.R  = 64*500;
				
				u8g_returnZeroMode = NORMAL_OPRATION;				        	     
			}
		}
		else if(u8g_returnZeroMode == IGNITION_OFF_RETURN_ZERO)
		{
			uint8 parameter;
			u8g_IgnitionOffReturnZero_count++;
			if(u8g_IgnitionOffReturnZero_count >= IGNITION_OFF_RETURNZERO_PERIOD)
			{
				u8g_IgnitionOffReturnZero_count = 0u;
				if( (smctrl_gauge[SPEEDOMETER].CUR_POS > u16t_VehicleSpeedStartZeroPosition)||
						 (smctrl_gauge[FUELMETER].CUR_POS > u16t_FuelMeterStartZeroPosition)||
						 (smctrl_gauge[WATERCOOLANTTEMP].CUR_POS > u16t_WaterCoolantTmpStartZeroPosition) ||
					(smctrl_gauge[WATERCOOLANTTEMP].CUR_POS > u16t_WaterCoolantTmpStartZeroPosition)||
					(smctrl_gauge[LEFTBRAKEPRESSURE].CUR_POS > u16t_LeftBrakePressureStartZeroPosition)||
					(smctrl_gauge[RIGHTBRAKEPRESSURE].CUR_POS > u16t_RightBrakePressureStartZeroPosition) )
				{		
					if(smctrl_gauge[SPEEDOMETER].CUR_POS > u16t_VehicleSpeedStartZeroPosition)
						Turn_CCW(SPEEDOMETER);
					if(smctrl_gauge[ENGINERPM].CUR_POS > u16t_EnginRPMStartZeroPosition)
						Turn_CCW(ENGINERPM);
					if(smctrl_gauge[FUELMETER].CUR_POS > u16t_FuelMeterStartZeroPosition)
					{
						CAL_Read8(FUEL_PARAMETER, &parameter);
						if((parameter&0x08) == 0x00)
							Turn_CCW(FUELMETER);
					}
					if(smctrl_gauge[WATERCOOLANTTEMP].CUR_POS > u16t_WaterCoolantTmpStartZeroPosition)
					{
						CAL_Read8(WATERCOOLANTTEMP_PARAMETER, &parameter);
						if((parameter&0x08) == 0x00)
							Turn_CCW(WATERCOOLANTTEMP);
					}
					if(smctrl_gauge[LEFTBRAKEPRESSURE].CUR_POS > u16t_LeftBrakePressureStartZeroPosition)
					{
						CAL_Read8(LEFTBRAKEPRESSURE_PARAMETER, &parameter);
						if((parameter&0x08) == 0x00)
							Turn_CCW(LEFTBRAKEPRESSURE);	
					}
					if(smctrl_gauge[RIGHTBRAKEPRESSURE].CUR_POS > u16t_RightBrakePressureStartZeroPosition)
					{
						CAL_Read8(RIGHTBRAKEPRESSURE_PARAMETER, &parameter);
						if((parameter&0x08) == 0x00)
							Turn_CCW(RIGHTBRAKEPRESSURE);	
					}
					
					return;
				}
				u8g_returnZeroMode = DO_NOTING;
			}
		}
	  else if(u8g_returnZeroMode == NORMAL_OPRATION)
	  {
		  	CAL_Read16(FUEL_STEP_INTERVAL, &u16t_CallMotor2MoveOneStepTime);
		  	CAL_Read16(WATERCOOLANTTEMP_STEP_INTERVAL, &u16t_CallMotor3MoveOneStepTime);
		  	CAL_Read16(LEFTBRAKEPRESSURE_STEP_INTERVAL, &u16t_CallMotor4MoveOneStepTime);
		  	CAL_Read16(RIGHTBRAKEPRESSURE_STEP_INTERVAL, &u16t_CallMotor5MoveOneStepTime);
	  	
			Motor0Cnt++;
			Motor1Cnt++;
			Motor2Cnt++;
			Motor3Cnt++;
			Motor4Cnt++;
			Motor5Cnt++;
			if(Motor0Cnt >= u16g_UpdateSpeedometerPeriod_Ms)
			{
				Motor0Cnt = 0;         
				StepMotorUpdate(0,ArraGaugePos[0]);
			}
			
			if(Motor1Cnt >= u16g_UpdateEnginRPMPeriod_Ms)
			{
				Motor1Cnt = 0;         
				StepMotorUpdate(1,ArraGaugePos[1]);
			}
			
			if(Motor2Cnt >= u16t_CallMotor2MoveOneStepTime)
			{
				Motor2Cnt = 0;         
				StepMotorUpdate(2,ArraGaugePos[2]);
			}
			
			if(Motor3Cnt >= u16t_CallMotor3MoveOneStepTime)
			{
				Motor3Cnt = 0;         
				StepMotorUpdate(3,ArraGaugePos[3]);
			}
			
			if(Motor4Cnt >= u16t_CallMotor4MoveOneStepTime)
			{
				Motor4Cnt = 0;         
				StepMotorUpdate(4,ArraGaugePos[4]);
			}
			
			if(Motor5Cnt >= u16t_CallMotor5MoveOneStepTime)
			{
				Motor5Cnt = 0;         
				StepMotorUpdate(5,ArraGaugePos[5]);
			}
	  }
	} 
}

/************************************************************************

  according the collecting value to calculate the angle value by the means of ten-sect line
  parameter£ºunsigned short meter_rf,unsigned short data_input,unsigned short *r_pointer,unsigned short *ang_pointer
  meter_rf£º	  ---- reference resistor value
  data_input£º  ---- input data
  
  corres_value  ---- the pointer to engineering value   
  ang_pointer£º	---- the pointer to angle value   
  
  **********************************************************************/

uint16 calculate_ang(uint8 motorId ,uint16 meter_rf,uint16 data_input,uint16 *corres_value,uint16 *ang_pointer)
{
	uint8 i,Cnt;
	uint16 ang,TempValue;
	uint16 * pStartAddr;
	uint8 parameter;
	
	TempValue = data_input;
	pStartAddr = corres_value;
	
	if(motorId < 2u)
	{
		Cnt = 8u;
	}
	else
	{
		Cnt = 16u;
	}
	switch(motorId)
	{
	case SPEEDOMETER:
		CAL_Read8(VEHSPD_PARAMETER,&parameter);
		break;
	case ENGINERPM:
		CAL_Read8(ENGINERPM_PARAMETER,&parameter);
		break;
	case FUELMETER:
		CAL_Read8(FUEL_PARAMETER,&parameter);
		break;
	case WATERCOOLANTTEMP:
		CAL_Read8(WATERCOOLANTTEMP_PARAMETER,&parameter);
		break;
	case LEFTBRAKEPRESSURE:
		CAL_Read8(LEFTBRAKEPRESSURE_PARAMETER,&parameter);
		break;
	case RIGHTBRAKEPRESSURE:
		CAL_Read8(RIGHTBRAKEPRESSURE_PARAMETER,&parameter);
		break;
	default:
		break;
	}
	meter_rf = meter_rf*data_input;
					
	for(i = 0;i < Cnt;i++)
	{
		if(motorId < 2u)
		{
			if(data_input <= (*corres_value++))
			{
				break;
			}
		}
		else
		{
			if((parameter&0x04) == 0x00u)
			{
				if(data_input <= (*corres_value++))
				{
					break;
				}
			}
			else
			{
				if(data_input >= (*corres_value++))
				{
					break;
				}
			}
		}
	}
	if(i == 0u)
	{
		ang = ang_pointer[0];
	}
	else if(i == Cnt)
	{
		ang = ang_pointer[Cnt - 1];
	}
	else
	{
		if(pStartAddr[i] >= pStartAddr[i-1])
		{
			TempValue = (ang_pointer[i] - ang_pointer[i-1]) * (uint16)(pStartAddr[i] - data_input)/(uint16)(pStartAddr[i] - pStartAddr[i-1]);
							
			ang = ang_pointer[i] - TempValue;
		}
		else
		{
			TempValue = (ang_pointer[i-1] - ang_pointer[i]) * (uint16)(pStartAddr[i] - data_input)/(uint16)(pStartAddr[i-1] - pStartAddr[i]);
							
			ang = ang_pointer[i - 1] - TempValue;
		}
	}
	
	return ang;
}

