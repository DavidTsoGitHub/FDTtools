/*****************************************************************/
/*                                                               */
/* FILE        : FreqCntInput.c                                  */
/* DATE        : 2015-06-20                                      */
/* AUTHOR      : Yuan Jia                                        */
/* DESCRIPTION : source file of frequency input capture module   */
/* HISTORY     : 1.0.0 release for the first time.               */
/*                                                               */
/* NOTE        : All Rights reserved by DETC, Technology Centre  */
/*               Last Modified By caowei on 2015-12-30           */
/*                                                               */
/*****************************************************************/
#include "Platform_Types.h"
#include "MCU_FreqCntInput.h"
#include "FreqCntInput.h"
#include "BSP.h"
#include "Cal_cfg.h"
#include "CAL.h"

uint32 ConvCounterToFreq(uint32 counter);                    /* convert the counter value to frequency , which equal to: freq = 1000000/counter */       



/*
 * Name         : ECU_FreqInput_Init
 * Description  : Initialize frequency counter input capture
 * Arguments    : void
 * Return       : void                  
 */
void ECU_FreqInput_Init(void) 
{
	uint8 u8t_CurSel,parameter;                             /* Current Selected way to capture input Signal*/
	CAL_Read8(VEHSPD_PARAMETER, &parameter);             	/* Select method to frequency input capture */        
	u8t_CurSel = parameter & 0x01u;
	MCU_FreqInput_Init(u8t_CurSel);                         /* Set the module corresponding to the selected method */
}

void BSP_GetPulseCount(uint8 u8InputID, uint32* u32PulseCount)
{
    *u32PulseCount = 0u;
    /* TODO: to be implemented if necessary */
}
BSP_StdReturnType BSP_ReportError_PulseCounter(uint8 u8InputID, uint8* FTB)
{
     /* TODO: to be implemented if necessary */
}

BSP_StdReturnType BSP_Set_FrequencyOutput(uint8 FOutputId, FPF_u32_8_Hz Value)
{
     /* TODO: to be implemented if necessary */
}

BSP_StdReturnType BSP_Set_FrequencyDutyCycle(uint8 FOutputId, uint8 u8DutyCycle)
{
     /* TODO: to be implemented if necessary */
}

BSP_StdReturnType BSP_ReportError_FrequencyOutput(uint8 FOutputId, uint8* FTB)
{
     /* TODO: to be implemented if necessary */
}

/*
 * Name        : BSP_Get_Frequency
 * Description : get the frequency by converted from the counter number
 * Arguments   : FInputId ---- frequency input capture ID
 *               *Value   ---- the frequency value captured
 * Return      : BSP_OK
 */
BSP_StdReturnType BSP_Get_Frequency(uint8 FInputId, uint32 *Value)
{
	uint8 u8t_CurSel,parameter;
	uint8 u8t_countingMethodStage;
	uint32 u32t_Freq_Input;
	FInputId = FInputId;
	
	CAL_Read8(VEHSPD_SAMPLEINGPERIOD, &u8t_countingMethodStage);    
	CAL_Read8(VEHSPD_PARAMETER, &parameter);             	
	u8t_CurSel = (parameter & 0x01u);
	
	if(u8t_CurSel == COUNTINGMETHOD)                                               /* if it is counting method */
	{
	    //u32t_Freq_Input = u32g_FreqCntValueInCounting <<(8 - FREQCNTSHIFTING);     /* convert the counter value to frequency */
		u32t_Freq_Input = (u32g_FreqCntValueInCounting <<8)*20/(u8t_countingMethodStage)/FREQINPUTDATABUFFERSIZE;
	    *Value = u32t_Freq_Input;                                                  /* pointer points to the frequency value */
	}
	else if(u8t_CurSel == PERIODICMETHOD)                                          /* if it is period method */
	{                                                                              
        u32t_Freq_Input = ConvCounterToFreq(u32g_FreqCntValueInPeriod);            /* convert the counter value to frequency , which equal to: freq = 1000000/counter */
        *Value = u32t_Freq_Input;                                                  /* pointer points to the frequency value */
	}
	else
	{
		; /* do nothing */
	}
	return BSP_OK;
}

/*
 * Name        : BSP_ReportError_FrequencyCounter
 * Description : while an error occurs , return back the error message
 * Arguments   : FTB --- Failure Type Byte
 * Return      : none
 */
BSP_StdReturnType BSP_ReportError_FrequencyCounter(uint8 FInputId, uint8 *FTB)
{
	FInputId = FInputId;
	*FTB = 0;
	return BSP_OK;
}

/*
 * Name        : ConvCounterToFreq
 * Description : Convert the counter value in one period cycle to Frequency
 *               frequency = 1000000/counter
 * Argument    : uint32
 * Return      : frequency corresponding to the counter value
 */
uint32 ConvCounterToFreq(uint32 counter)
{
	uint32 u32t_Freq_Cnt,u32t_Freq_Cnt_Integer,u32t_Freq_Cnt_Decimal,u32t_Freq_Input_Integer, u32t_Freq_Input_Decimal, u32t_Freq_Input;
	u32t_Freq_Cnt = counter;
	if (u32t_Freq_Cnt > 0)
	{
		u32t_Freq_Input_Integer = 100u * (10000u/u32t_Freq_Cnt) + (100u * (10000u % u32t_Freq_Cnt))/ u32t_Freq_Cnt;    /* 1000000 /u32t_Freq_Cnt , get integer part of frequency */
		u32t_Freq_Cnt_Decimal = (100u * (10000u % u32t_Freq_Cnt))% u32t_Freq_Cnt;                                      /* 1000000 % u32t_Freq_Cnt, get remainder part of frequency counter, to be converted to decimal part of freq */                                                            
		u32t_Freq_Input_Decimal = (uint8)(( u32t_Freq_Cnt_Decimal << 8 )/u32t_Freq_Cnt);                               /* (256*u32t_Freq_Cnt_Decimal)/u32t_Freq_Cnt ,get decimal part of freq */
		u32t_Freq_Input = (u32t_Freq_Input_Integer << 8) + u32t_Freq_Input_Decimal;                                    /* get frequency */
	}
	else
	{
		u32t_Freq_Input = 0;
	}
	return u32t_Freq_Input;                                                                                        /* return frequency value */
}
