/*****************************************************************/
/*                                                               */
/* FILE        : Analog.c                                        */
/* DATE        : 2015-06-15                                      */
/* AUTHOR      : Cao Wei                                         */
/* DESCRIPTION : source file of Analog module                    */
/* HISTORY     : 1.0.0 release for the first time.               */
/*                                                               */
/* NOTE        : All Rights reserved by DETC, Technology Center  */
/*               Last Modified By Cao wei on       2015-12-29    */
/*                                                               */
/*****************************************************************/
#include "jdp.h"
#include "BSP.h"
#include "Analog.h"
#include "MCU_Analog.h"
#include "AD_Convert.h"
#include "MCU_SwitchMode.h"
#include "Cal_cfg.h"
#include "CAL.h"

static uint32 u32g_DataBuffer[ANALOG_SIGNAL_NUM][ANALOG_BUF_SIZE];
static uint32 u32g_DirectColumIn;
static uint32 u32g_IndirectColumIn;
static uint32 u32g_AnalogValue[ANALOG_SIGNAL_NUM];

/* calibration variables for Battery Voltage input */
static uint16 u16g_BatteryRefOhm;       /* reference resistance */
static uint16 u16g_BatteryInputTbl[16];
static uint16 u16g_BatteryTbl[16];
static uint8  u8g_BatterySgnType;

/*
 * Name        : ECU_AnalogueInput_Init
 * Description : initial ECU collecting analog signals
 * Arguments   : 
 * Return      : void
 */
void ECU_AnalogueInput_Init(void) 
{
	uint8 i, j;

	for(i = 0; i < ANALOG_SIGNAL_NUM; i++)
	{
		for(j = 0; j < ANALOG_BUF_SIZE; j++)
		{
			u32g_DataBuffer[i][j] = 0u;
		}
	}
	u32g_DirectColumIn = 0u;
	u32g_IndirectColumIn = 0u;

	/* Battery input initialization */
	u16g_BatteryRefOhm = 31u;
	u8g_BatterySgnType = 1u;

	u16g_BatteryInputTbl[0] = 18;
	u16g_BatteryInputTbl[1] = 20;
	u16g_BatteryInputTbl[2] = 22;
	u16g_BatteryInputTbl[3] = 24;
	u16g_BatteryInputTbl[4] = 25;
	u16g_BatteryInputTbl[5] = 26;
	u16g_BatteryInputTbl[6] = 27;
	u16g_BatteryInputTbl[7] = 28;
	u16g_BatteryInputTbl[8] = 29;
	u16g_BatteryInputTbl[9] = 30;
	u16g_BatteryInputTbl[10] = 31;
	u16g_BatteryInputTbl[11] = 32;
	u16g_BatteryInputTbl[12] = 33;
	u16g_BatteryInputTbl[13] = 34;
	u16g_BatteryInputTbl[14] = 35;
	u16g_BatteryInputTbl[15] = 36;

	u16g_BatteryTbl[0] = 18;
	u16g_BatteryTbl[1] = 20;
	u16g_BatteryTbl[2] = 22;
	u16g_BatteryTbl[3] = 24;
	u16g_BatteryTbl[4] = 25;
	u16g_BatteryTbl[5] = 26;
	u16g_BatteryTbl[6] = 27;
	u16g_BatteryTbl[7] = 28;
	u16g_BatteryTbl[8] = 29;
	u16g_BatteryTbl[9] = 30;
	u16g_BatteryTbl[10] = 31;
	u16g_BatteryTbl[11] = 32;
	u16g_BatteryTbl[12] = 33;
	u16g_BatteryTbl[13] = 34;
	u16g_BatteryTbl[14] = 35;
	u16g_BatteryTbl[15] = 36;
}

/*
 * Name        : BSP_ADCRunnableInit
 * Description : Initialize AD input module
 * Arguments   : none
 * Return      :     0: success
 *               other: TBD
 */
uint16 BSP_ADCRunnableInit(void)
{
	
}

/*
 * Name        : ECU_ADC_Runnable
 * Description : runnable for AD input module, 5ms
 * Arguments   : none
 * Return      : none
 * Note        : This function might be blocked!
 */
void ECU_ADC_Runnable(void)
{
	uint32 i, j;
	uint32 u32t_index;
	uint32 u32t_IndirectCnt;
	uint32 u32t_DirectCnt;
	uint32 u32t_AdValue;
	uint32 u32t_AdAddr;
	uint32 u32t_sum;
	static uint8 u8t_PortA = 0;
	static uint8 u8t_PortB = 0;
	static uint8 u8t_PortC = 0;
	static uint8 u8s_portSeq = 0u;

    if(MCU_CUR_MODE != MCU_NORMAL_MODE)
    {
        return;
    }
	START_ADC();                  /*start collecting analogue value*/
	WAIT_FOR_ADC_DONE();          /* waiting for AD conversion completion */

	/* scan ADC ports */
	u32t_IndirectCnt = 0;
	u32t_DirectCnt = 0;
	for(i = 0u; i < ANALOG_CHANNEL_NUM; i++)
	{
		u32t_AdAddr = BASIC_ADDR + i;
		if(AnalogConf[i] == INDIRECT_ANALOG)
		{
			DATA_VALID(u32t_AdAddr);
		    u32t_AdValue = AD_GetInput(u32t_AdAddr);
		    u32t_AdValue = u32t_AdValue & 0x03FFuL;           /* only low 11 bits are valid */
		    u32t_index = u32t_IndirectCnt * 8u + u32t_DirectCnt + u8s_portSeq;
		    u32g_DataBuffer[u32t_index][u32g_IndirectColumIn] = u32t_AdValue;
		    u32t_IndirectCnt++;
		}
		else if(AnalogConf[i] == DIRECT_ANALOG)
		{
			DATA_VALID(u32t_AdAddr);
			u32t_AdValue = AD_GetInput(u32t_AdAddr);
			u32t_AdValue = u32t_AdValue & 0x03FFuL;            /* only low 11 bits is valid */
			u32t_index = u32t_IndirectCnt * 8u + u32t_DirectCnt;
			u32g_DataBuffer[u32t_index][u32g_DirectColumIn] = u32t_AdValue;
			u32t_DirectCnt++;
		}
		else
		{
			;  /* dummy */
		}
	}

	/* port sequence increase by 1 after a scanning loop */
	if(u8s_portSeq < 7u)
	{
		u8s_portSeq++;
	}
	else
	{
		u8s_portSeq = 0u;
		u32g_IndirectColumIn = (u32g_IndirectColumIn + 1u) % ANALOG_BUF_SIZE;    /* u32g_In increase */
	}
	u8t_PortA   = u8s_portSeq & 0x01u;
	u8t_PortB   = (u8s_portSeq & 0x02u) >> 1u;
	u8t_PortC   = (u8s_portSeq & 0x04u) >> 2u;
	MCU_OpenADSwitchPort(u8t_PortA, u8t_PortB, u8t_PortC);

	if(u32g_DirectColumIn < ANALOG_BUF_SIZE - 1)
	{
		u32g_DirectColumIn++;
	}
	else
	{
		u32g_DirectColumIn = 0;
	}
	
	/* filter */
	for(i = 0; i < ANALOG_SIGNAL_NUM; i++)
	{
		u32t_sum = 0;
		for(j = 0; j < ANALOG_BUF_SIZE; j++)
		{
			u32t_sum = u32t_sum + u32g_DataBuffer[i][j];
		}
		u32g_AnalogValue[i] = u32t_sum / ANALOG_BUF_SIZE;
	}
}

/*
 * Name        : BSP_Get_BatteryVoltage
 * Description : Get battery Voltage
 * Arguments   : BatteryVoltage - the pointer to  Battery Voltage value
 * Return      : BSP standard return type
 */	
BSP_StdReturnType BSP_Get_BatteryVoltage(uint16 *BatteryVoltage)
{
	uint16 ret;
	uint16 u16t_K1, u16t_K2;
	uint8 u8t_K1, u8t_K2;
	
	CAL_Read8(AD_K1, &u8t_K1);
	u16t_K1 = (uint16)u8t_K1;
	CAL_Read8(AD_K2, &u8t_K2);
	u16t_K2 = (uint16)u8t_K2;
	
	ret = (uint16)u32g_AnalogValue[IGN_VOLT_IND];
	*BatteryVoltage = BSP_GetEnginValue(u16g_BatteryRefOhm, ret, u16g_BatteryInputTbl, u16g_BatteryTbl,
			                       (uint16)u32g_AnalogValue[REF_VOLT_IND], 
			                       u8g_BatterySgnType, u16t_K1, u16t_K2);
	//*BatteryVoltage = (uint16)u32g_AnalogValue[IGN_VOLT_IND];
	return BSP_OK;
}

/*
 * Name        : BSP_Get_FuelLevel
 * Description : Get Fuel Level
 * Arguments   : FuelLevel - the pointer to  FuelLevel
 * Return      : BSP standard return type
 */	
BSP_StdReturnType BSP_Get_FuelLevel(uint16 *FuelLevel)
{
	uint16 u16t_FuelRefOhm;       /* reference resistance */
	uint16 u16t_FuelInputTbl[16];
	uint16 u16t_FuelTbl[16],u16t_K1, u16t_K2;
	uint8 u8t_FuelTblTmp[16],u8t_K1, u8t_K2;
	uint16 ret;
	uint8  i;
	uint8  u8t_CAlParameter,u8t_FuelSgnType;
	
	CAL_Read16(FUEL_INPUT_REF_R, &u16t_FuelRefOhm);
	CAL_Read8(FUEL_PARAMETER, &u8t_CAlParameter);
	
	u8t_FuelSgnType   = u8t_CAlParameter & 0x01u;
	direct_proportion = (u8t_CAlParameter&0x04) >> 2u;
	
	CAL_Read8(AD_K1, &u8t_K1);
	u16t_K1 = (uint16)u8t_K1;
	CAL_Read8(AD_K2, &u8t_K2);
	u16t_K2 = (uint16)u8t_K2;
	
	for(i=0; i<16; i++)
	{
			CAL_Read16((calibration_parameter_id_t)(FUEL_INPUT1+i), &u16t_FuelInputTbl[i]);
			CAL_Read8((calibration_parameter_id_t)(FUEL_OUTPUT1+i), &u8t_FuelTblTmp[i]);
			u16t_FuelTbl[i] = u8t_FuelTblTmp[i];
	}

	ret = (uint16)u32g_AnalogValue[FUEL_LEVEL_SEN_IND];
	*FuelLevel = BSP_GetEnginValue(u16t_FuelRefOhm, ret, u16t_FuelInputTbl, u16t_FuelTbl,
			                       (uint16)u32g_AnalogValue[REF_VOLT_IND], 
			                       u8t_FuelSgnType, u16t_K1, u16t_K2);

	return BSP_OK;
}

/*
 * Name        : BSP_FrontAxleAirPressure
 * Description : Get front axle airPressure
 * Arguments   : FrontAxleAirPressure - the pointer to  front axle airPressure value
 * Return      : BSP standard return type
 */
BSP_StdReturnType BSP_Get_Front_Brake_Pressure(uint16* data)
{
	uint16 u16t_FrontAxlePressureRefOhm;       /* reference resistance */
	uint16 u16t_FrontAxlePressureInputTbl[16];
	uint16 u16t_FrontAxlePressureTbl[16],u16t_K1, u16t_K2;
	uint8 u8t_FrontAxlePressureTblTmp[16],u8t_K1, u8t_K2;
	uint16 ret;
	uint8  i;
	uint8  u8t_CAlParameter,u8t_FrontAxlePressureSgnType;
	
	CAL_Read16(LEFTBRAKEPRESSURE_INPUT_REF_R, &u16t_FrontAxlePressureRefOhm);
	CAL_Read8(LEFTBRAKEPRESSURE_PARAMETER, &u8t_CAlParameter);
	
	u8t_FrontAxlePressureSgnType   = u8t_CAlParameter & 0x01u;
	direct_proportion = (u8t_CAlParameter&0x04) >> 2u;
	
	CAL_Read8(AD_K1, &u8t_K1);
	u16t_K1 = (uint16)u8t_K1;
	CAL_Read8(AD_K2, &u8t_K2);
	u16t_K2 = (uint16)u8t_K2;
	
	for(i=0; i<16; i++)
	{
			CAL_Read16((calibration_parameter_id_t)(LEFTBRAKEPRESSURE_INPUT1+i), &u16t_FrontAxlePressureInputTbl[i]);
			CAL_Read8((calibration_parameter_id_t)(LEFTBRAKEPRESSURE_OUTPUT1+i), &u8t_FrontAxlePressureTblTmp[i]);
			u16t_FrontAxlePressureTbl[i] = u8t_FrontAxlePressureTblTmp[i];
	}

	ret = (uint16)u32g_AnalogValue[FRONT_RES_PRESSURE_SEN_IND];
	*data = BSP_GetEnginValue(u16t_FrontAxlePressureRefOhm, ret, u16t_FrontAxlePressureInputTbl, u16t_FrontAxlePressureTbl,
								   (uint16)u32g_AnalogValue[REF_VOLT_IND], 
								   u8t_FrontAxlePressureSgnType, u16t_K1, u16t_K2);
	
//	*FrontAxleAirPressure = (uint16)u32g_AnalogValue[FRONT_RES_PRESSURE_SEN_IND];
	return BSP_OK;
}

/*
 * Name        : BSP_ Get_ Rear_Brake_Pressure
 * Description : Get rear axle air pressure
 * Arguments   : RearAxleAirPressure - the pointer to  rear axle air pressure value
 * Return      : BSP standard return type
 */
BSP_StdReturnType BSP_Get_Rear_Brake_Pressure(uint16* data)

{
	uint16 u16t_RearAxlePressureRefOhm;       /* reference resistance */
	uint16 u16t_RearAxlePressureInputTbl[16];
	uint16 u16t_RearAxlePressureTbl[16],u16t_K1, u16t_K2;
	uint8 u8t_RearAxlePressureTblTmp[16],u8t_K1, u8t_K2;
	uint16 ret;
	uint8  i;
	uint8  u8t_CAlParameter,u8t_RearAxlePressureSgnType;
		
	CAL_Read16(RIGHTBRAKEPRESSURE_INPUT_REF_R, &u16t_RearAxlePressureRefOhm);
	CAL_Read8(RIGHTBRAKEPRESSURE_PARAMETER, &u8t_CAlParameter);
		
	u8t_RearAxlePressureSgnType   = u8t_CAlParameter & 0x01u;
	direct_proportion = (u8t_CAlParameter&0x04) >> 2u;
	
	CAL_Read8(AD_K1, &u8t_K1);
	u16t_K1 = (uint16)u8t_K1;
	CAL_Read8(AD_K2, &u8t_K2);
	u16t_K2 = (uint16)u8t_K2;
	
	for(i=0; i<16; i++)
	{
			CAL_Read16((calibration_parameter_id_t)(RIGHTBRAKEPRESSURE_INPUT1+i), &u16t_RearAxlePressureInputTbl[i]);
			CAL_Read8((calibration_parameter_id_t)(RIGHTBRAKEPRESSURE_OUTPUT1+i), &u8t_RearAxlePressureTblTmp[i]);
			u16t_RearAxlePressureTbl[i] = u8t_RearAxlePressureTblTmp[i];
	}

	ret = (uint16)u32g_AnalogValue[BACK_RES_PRESSURE_SEN_IND];
	*data = BSP_GetEnginValue(u16t_RearAxlePressureRefOhm, ret, u16t_RearAxlePressureInputTbl, u16t_RearAxlePressureTbl,
									   (uint16)u32g_AnalogValue[REF_VOLT_IND], 
									   u8t_RearAxlePressureSgnType, u16t_K1, u16t_K2);
		
	
	//*RearAxleAirPressure = (uint16)u32g_AnalogValue[BACK_RES_PRESSURE_SEN_IND];
	
	
	
	return BSP_OK;
}

/*
 * Name        : BSP_ Get_Front_Airtank_Pressure
 * Description : Get front air reservior pressure
 * Arguments   : data - the pointer to front air reservior pressure
 * Return      : BSP standard return type
 */
BSP_StdReturnType BSP_Get_Front_Airtank_Pressure(uint16* data)
{
    *data = 0u;
    /* TODO: to be implemented */
}

/*
 * Name        : BSP_Get_Rear_Airtank_Pressure
 * Description : Get front air reservior pressure
 * Arguments   : data - the pointer to front air reservior pressure
 * Return      : BSP standard return type
 */
BSP_StdReturnType BSP_Get_Rear_Airtank_Pressure(uint16* data)
{
    *data = 0u;
    /* TODO: to be implemented */
}

/*
 * Name        : BSP_Get_Gearbox_Temp_Sensor
 * Description : Get gear box temperature
 * Arguments   : data - the pointer to gear box temperature
 * Return      : BSP standard return type
 */
BSP_StdReturnType BSP_Get_Gearbox_Temp_Sensor(uint16* data)
{
    *data = 0u;
    /* TODO: to be implemented */
}
/*
 * Name        : BSP_Get_ Park_Brake_Pressure
 * Description : Get park brake airPressure
 * Arguments   : ParkBrakeAirPressure - the pointer to park brake airPressure value
 * Return      : BSP standard return type
 */
BSP_StdReturnType BSP_Get_Park_Brake_Pressure(uint16 *ParkBrakeAirPressure)
{
	*ParkBrakeAirPressure = (uint16)u32g_AnalogValue[PARK_PRESSURE_SEN_IND];
	return BSP_OK;
}

/*
 * Name        : BSP_Get_FrontLimiterPressure
 * Description : Get front loop airPressure
 * Arguments   : FrontLoopAirPressure - the pointer to front loop airPressure value
 * Return      : BSP standard return type
 */
BSP_StdReturnType BSP_Get_FrontLimiterPressure(uint16 *FrontLoopAirPressure)
{
	*FrontLoopAirPressure = (uint16)u32g_AnalogValue[FRONT_LIMITER_AIRPRESSURE];
	return BSP_OK;
}

/*
 * Name        : BSP_Get_RearLimiterPressure
 * Description : Get rear loop airPressure
 * Arguments   : RearLoopAirPressure - the pointer to rear loop airPressure value
 * Return      : BSP standard return type
 */
BSP_StdReturnType BSP_Get_RearLimiterPressure(uint16 *RearLoopAirPressure)
{
	*RearLoopAirPressure = (uint16)u32g_AnalogValue[BACK_LIMITER_AIRPRESSURE];
	return BSP_OK;
}


/*
 * Name        : BSP_ReportError_AnalogeInput
 * Description : report Error of analogue Input
 * Arguments   : AInputId - analogue input's id
 * 			   :FTB       - the pointer to failure type byte
 * Return      : BSP standard return type
 */
BSP_StdReturnType BSP_ReportError_AnalogeInput(uint8 ADInputId, uint8 *FTB)
{
	ADInputId = 0;
	*FTB = 0;
}
