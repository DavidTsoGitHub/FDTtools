/*                                                                */
/* FILE        : MCU_FreqCntInput.c                               */
/* DATA        : 2015-07-15                                       */
/* AUTHOR      : Yuan Jia                                         */
/* DESCRIPTION : source file of freq input capture module in MCU  */
/* HISTORY     : 1.0.0 release for the first time                 */
/*                                                                */
/* NOTE        : All Rights reserved by DETC, Technology Centre   */
/*               Last Modified By caowei on 2015-12-30          */
/*                                                                */
/******************************************************************/
#include "jdp.h"
#include "IODefine.h"
#include "Platform_Types.h"
#include "MCU_FreqCntInput.h"
#include "BSP.h"
#include "Cal_cfg.h"
#include "CAL.h"
        
static void FreqInputCap_Timer_Init(void);               /* set some registers in system timer0 to get 1s timer  */
static void FreqInputCap_Timer_Dis(void);                /* Disable system timer0 , which is needn't while in Period Method */
static void FreqInputCap_Timer_En(void);                 /* Enable system timer0 , get the counting value by this timer while in Counting Method */
static void eMIOS16_CountOFR_Dis(void);                  /* eMIOS16 counting overflow interrupt disable */
static void eMIOS16_CountOFR_En(void);                   /* eMIOS16 counting overflow interrupt enable */

uint32 u32g_FreqCntValueInPeriod;                        /* Freq Signal Input Counter value in period method after Filter */
uint32 u32g_FreqCntValueInCounting;                      /* Freq Signal Input Counter value in Counting method after Filter */
static uint8   u8g_CurMethod;                            /* Current Selected way to capture input Signal*/
static uint8   u8g_FreqInputFlag_InCntMethod;            /* Freq Signal Input Flag in counting method, 0 ----- signal on; 1 ----- signal off */
static uint32  u32g_FreqInputCounter_InCntMethod;        /* Freq Signal Input Counter in Counting Method */ 
static uint8   u8g_FreqInputDataArray_Counter;           /* Current data number in Freq Signal Input Data Array */
static uint32  u32g_FreqInputDataArray[FREQINPUTDATABUFFERSIZE];    /* Freq Signal Input Data Buffer */
static uint32  u32g_FreqInputDataSum_InArray;            /* Sum of Freq Signal Input Data Value in array of FREQINPUTDATABUFFERSIZE */
static uint8   u8g_FreqInputFlag_InPodMethod;            /* Freq Signal Input Flag in Period method, 0 ------ signal on; 1 ----- signal off */
static uint16  u16g_FreqInputCount_OFC;                  /* The counter of freq input counter overflows */
static uint32  u32g_TotalCntOnceInPodMethod;             /* The total counter value between two adjacent Freq Signal in period method */
static uint32  u32g_FreeRunCntRegPreValue;               /* The value of eMIOS18 counter in previous frequency signal input */
static uint8   u8t_loop_flag, u8t_loop_cnt; 
static uint32  u32t_FreqInputTemp;


/*
 * Name         : MCU_FreqInput_Init
 * Description  : Initialize frequency counter input module in MCU
 * Arguments    : void
 * Return       : void                  
 */
void MCU_FreqInput_Init(uint8 u8t_CurSel) 
{
	u8g_CurMethod = u8t_CurSel;
	FreqInputCap_Timer_Init();                /* System timer0 initialization , for frequency input capture */
  
    EMIOS_0.UCDIS.B.CHDIS16 = 0;              /* Enable EMIOS Channel16  */
    EMIOS_0.UCDIS.B.CHDIS18 = 0;              /* Enable EMIOS Channel18  */
           
    EMIOS_0.CH[16].CADR.R = 0x00008000;//0x000003E8;       /*  Channel eMIOS_0_CADR16 Data Register - eMIOS_0_CADR16 : Value = 500 */
    EMIOS_0.CH[16].CBDR.R = 0x00000000;       /*  Channel eMIOS_0_CBDR16 Data Register - eMIOS_0_CBDR16 : Value = 0 */     
    EMIOS_0.CH[18].CADR.R = 0x00000000;       /*  Channel eMIOS_0_CADR18 Data Register - eMIOS_0_CADR18 : Value = 0 */                                 
    EMIOS_0.CH[18].CBDR.R = 0x00000000;       /*  Channel eMIOS_0_CBDR18 Data Register - eMIOS_0_CBDR18 : Value = 0 */        
    
    if(u8g_CurMethod == COUNTINGMETHOD)
    {
    	EMIOS_0.CH[16].CCR.R = 0x02020250;    /* Set Channel eMIOS_0 16 Control Register  */
	    EMIOS_0.CH[18].CCR.R = 0x02120282;    /* Set Channel eMIOS_0 18 Control Register  */
	    FreqInputCap_Timer_En();              /* Enable STM Timer0 for freq input capture*/
	    eMIOS16_CountOFR_Dis();               /* eMIOS16 counting overflow interrupt disable */
    }
    else  if(u8g_CurMethod == PERIODICMETHOD)
    {
    	EMIOS_0.CH[16].CCR.R = 0x02023650;    /* Set Channel eMIOS_0 16 Control Register  */
	    EMIOS_0.CH[18].CCR.R = 0x02120282;    /* Set Channel eMIOS_0 18 Control Register  */              
	    eMIOS16_CountOFR_En();                /* eMIOS16 counting overflow interrupt enable */
        FreqInputCap_Timer_Dis();             /* Disable STM Timer0 */
    }
    else
    {
    	;                                     /* doing nothing*/
    }
    
    EMIOS_0.UCDIS.B.CHDIS16 = 0;              /* Enable EMIOS Channel16        */
    EMIOS_0.UCDIS.B.CHDIS18 = 0;              /* Enable EMIOS Channel18        */
    
    SIU.PCR[121].R = 0x0901;                  /* set PK0 as frequency signal input capture pin */
    SIU.PSMI[11].R = 0x01;
    SIU.PSMI[12].R = 0x01;
    SIU.PSMI[25].R = 0x01;
    SIU.PSMI[26].R = 0x01;

}


/*
 * Name        : FreqInputCapTimerISR
 * Description : system timer in 1s, which is used to count the counting-value when in CountingMethod
 * Arguments   : void
 * Return      : void
 */
void FreqInputCapTimerISR (void)
{
    STM.CIR0.B.CIF = 1;
    STM.CNT0.R = 0; 
    if(u8g_FreqInputFlag_InCntMethod == 1)
	{
		u8g_FreqInputFlag_InCntMethod = 0;                                     /* clear frequency signal input flag in counting method */
		u32g_FreqInputDataArray[u8g_FreqInputDataArray_Counter] = u32g_FreqInputCounter_InCntMethod;	/* move the new freq input counter value to the Input Data Buffer*/
		u8g_FreqInputDataArray_Counter ++;                                     /* Current data number in Freq Signal Input Data Array add by 1 */
		u8g_FreqInputDataArray_Counter = u8g_FreqInputDataArray_Counter % FREQINPUTDATABUFFERSIZE;   
		u32g_FreqInputDataSum_InArray = 0;                                     /* initialize the Sum of Freq Signal Input Data Value in array */
		for(u8t_loop_flag=0;u8t_loop_flag < FREQINPUTDATABUFFERSIZE;u8t_loop_flag++)   /* do a loop to get the sum of frequency count value in the Freq Input Data Buffer*/
		{
		    u32g_FreqInputDataSum_InArray +=u32g_FreqInputDataArray[u8t_loop_flag];	
		}
		u32g_FreqCntValueInCounting = u32g_FreqInputDataSum_InArray;            /* get the Freq Signal Input Counter value in Counting method after Filter*/ 
		u32g_FreqInputCounter_InCntMethod = 0;                                  /* clear frequency signal input counter value, for next time counting */
	}
}


/*
 * Name        : FreqInputISR
 * Description : frequency signal input interrupt handler, in channel eMIOS0_18
 * Arguments   : void
 * Return      : void
 */
void FreqInputISR (void)
{
	unsigned long u32t_temp;
	u32t_temp = 0;
    if(u8g_CurMethod == COUNTINGMETHOD)                                         /* if it is counting method */
    {   
    	if(EMIOS_0.CH[18].CSR.B.FLAG == 1)
	    {
	        u8g_FreqInputFlag_InCntMethod = 1;                                  /* Set Frequency signal input flag in using counting method */
			u32g_FreqInputCounter_InCntMethod++;                                /* Accumulated once a time in u32g_FreqInputCounter_InCntMethod */
			EMIOS_0.CH[18].CSR.B.FLAG = 1;	                                    /* clear frequency signal input interrupt flag */
	    }
	    if(EMIOS_0.CH[19].CSR.B.FLAG == 1)
	    {
	    	EMIOS_0.CH[19].CSR.B.FLAG = 1;
	    }
    }
    else if(u8g_CurMethod == PERIODICMETHOD)                                    /* if it is period method */
    {
    	if(EMIOS_0.CH[18].CSR.B.FLAG == 1)                                      
		{
    		if(u8g_FreqInputFlag_InPodMethod == 1)
    		{
    			u32t_temp = EMIOS_0.CH[18].CADR.R;
    			u32g_FreqInputDataArray[u8g_FreqInputDataArray_Counter] = ((uint32)u16g_FreqInputCount_OFC << 15) + u32t_temp - u32g_FreeRunCntRegPreValue;
    			if (u32g_FreqInputDataArray[u8g_FreqInputDataArray_Counter] > 8000)
    			{
    				__asm(" nop");
    			}
    			u32g_FreeRunCntRegPreValue = u32t_temp;
    			u16g_FreqInputCount_OFC = 0;                              
    			u8g_FreqInputDataArray_Counter++;
    			if(u8g_FreqInputDataArray_Counter >= FREQINPUTDATABUFFERSIZE)
    			{
    				u8g_FreqInputDataArray_Counter = 0;
    			}
    			for( u8t_loop_cnt = FREQINPUTDATABUFFERSIZE - 1;u8t_loop_cnt > 0; u8t_loop_cnt --)
    			{		
    			   	for(u8t_loop_flag = 0; u8t_loop_flag < u8t_loop_cnt; u8t_loop_flag++)
    			   	{
						if(u32g_FreqInputDataArray[u8t_loop_flag + 1] > u32g_FreqInputDataArray[u8t_loop_flag])
						{
							u32t_FreqInputTemp = u32g_FreqInputDataArray[u8t_loop_flag];
							u32g_FreqInputDataArray[u8t_loop_flag] = u32g_FreqInputDataArray[u8t_loop_flag + 1];
							u32g_FreqInputDataArray[u8t_loop_flag + 1] = u32t_FreqInputTemp;
						}
    			   	}
    			}
    			u32g_FreqCntValueInPeriod = u32g_FreqInputDataArray[FREQINPUTDATABUFFERSIZE / 2 ];
    		}
    		else                                                                /* if it is the first time to enter this interrupt */
   	    	{                        
    		  	u8g_FreqInputFlag_InPodMethod = 1;                              /* Set Frequency signal input Flag in period method */
    	   		u32g_FreeRunCntRegPreValue = EMIOS_0.CH[18].CADR.R;             /* get the free-run counter register counter-value , as the start counter-value in a period method cycle */
    			u16g_FreqInputCount_OFC = 0;  		                            /* clear free-run counter register overflow counter */
    			u8g_FreqInputDataArray_Counter = 0;                             /* initialize frequency input data array counter */
    			for(u8t_loop_flag = 0; u8t_loop_flag < FREQINPUTDATABUFFERSIZE ; u8t_loop_flag ++)  /* initialize frequency input data buffer array */
    			{
        		    u32g_FreqInputDataArray[u8t_loop_flag] = 0u;
    			}
   	    	}
    		EMIOS_0.CH[18].CSR.B.FLAG = 1;
		}
		
		if(EMIOS_0.CH[19].CSR.B.FLAG == 1)
		{
			EMIOS_0.CH[19].CSR.B.FLAG = 1;
		}

    }
    else
    {
    	; /* Do nothing */
    }
	   
}


/*
 * Name        : FreqInputCntOverflowISR
 * Description : counting overflow ISR while in PeriodMethod 
 * Arguments   : void
 * Return      : void
 */
void FreqInputCntOverflowISR (void)
{
    if(EMIOS_0.CH[16].CSR.B.FLAG == 1)
	{
    	if(u8g_FreqInputFlag_InPodMethod == 1)
    	{
    		u16g_FreqInputCount_OFC++;                                           /* free-run counter register overflow counter add by 1 while it overflows */
    		if (u16g_FreqInputCount_OFC > 30)
    		{
    			u16g_FreqInputCount_OFC = 0;
    			u32g_FreqCntValueInPeriod = 0;
    			u8g_FreqInputFlag_InPodMethod = 0;
    		}
    	}
		EMIOS_0.CH[16].CSR.B.FLAG = 1;                                           /* clear free-run counter register overflows flag */
	}
	if(EMIOS_0.CH[17].CSR.B.FLAG == 1)
	{
		EMIOS_0.CH[17].CSR.B.FLAG = 1;
	}
}

/*
 * Name        : FreqInputCap_Timer_Init
 * Description : set some registers in system timer0 to get 1s timer 
 * Arguments   : void
 * Return      : void
 */
void FreqInputCap_Timer_Init(void)
{
	uint8 u8t_countingMethodStage;
	CAL_Read8(VEHSPD_SAMPLEINGPERIOD, &u8t_countingMethodStage);             	
	
    STM.CR0.B.TEN = 0x0;                        		/* Disable System Timer Module for initialization*/
    STM.CR0.B.CPS = 63;                         		/* Counter Prescaler is 64*/
    STM.CMP0.R = 50000ul * u8t_countingMethodStage;     /* System Timer Channel 0's Compare value is : 50,000    */    
    STM.CR0.B.TEN = 1;                         			/* System Timer Module is:  Enabled */
}

/*
 * Name        : FreqInputCap_Timer_Dis
 * Description : Disable system timer0 , which is needn't while in PeriodMethod
 * Arguments   : void
 * Return      : void
 */
void FreqInputCap_Timer_Dis(void)
{
    STM.CCR0.B.CEN = 0x0;                       /* disable system timer0 */    
}


/*
 * Name        : FreqInputCap_Timer_En
 * Description : Enable system timer0 , get the counting value by this timer while in CountingMethod
 * Arguments   : void
 * Return      : void
 */
void FreqInputCap_Timer_En(void)
{
    STM.CCR0.B.CEN = 0x1;       /* enable System Timer0  */    
}

/*
 * Name        : eMIOS16_CountOFR_Dis
 * Description : eMIOS16 counting overflow interrupt disable
 * Arguments   : void
 * Return      : void
 */
void eMIOS16_CountOFR_Dis(void)
{
	INTC.PSR[145].R = 0x00;    /* Disable eMIOS16 counting overflow Interrupt */
}

/*
 * Name        : eMIOS16_CountOFR_En
 * Description : eMIOS16 counting overflow interrupt enable
 * Arguments   : void
 * Return      : void
 */
void eMIOS16_CountOFR_En(void)
{
	INTC.PSR[145].R = 0x05;    /* Enable eMIOS16 counting overflow Interrupt */
}


