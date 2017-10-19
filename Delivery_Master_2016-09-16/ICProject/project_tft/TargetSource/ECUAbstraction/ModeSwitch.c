/***************************************************************************************/
/*                                                                                     */
/* FILE        : SwitchMode.c                                                          */
/* DATE        : 2015-12-15                                                            */
/* AUTHOR      : Cao chenguang                                                         */
/* DESCRIPTION : implementation file of mode switch                                    */
/* NOTE        : All Rights reserved by DETC, Technology Center                        */
/*                                                                                     */
/***************************************************************************************/
#include "BSP.h"
#include "jdp.h"
#include "MCU_SwitchMode.h"
#include "FreqCntInput.h"
#include "Sound.h"
#include "SIUL.h"
#include "IODefine.h"
#include "pit_init.h"
#include "Analog.h"
#include "Gauge.h"
#include "Watchdog.h"

static uint8 u8g_TargetMode = 0u;

/*
 * Name        : BSP_Service_ShutdownAbort
 * Description : abort shut down
 * Arguments   : None
 * Return      : None
 */
void BSP_Service_ShutdownAbort(void)
{
    /* TODO: how to shut down? */
}

/*
 * Name        : BSP_RequestModeChange
 * Description : switch current mode to required mode
 * Arguments   : newMode - target mode
 * Return      : None
 */
void BSP_RequestModeChange(BSP_ECUModes newMode)
{

    if(newMode == BSP_MODE_SLEEP)
    {
    	u8g_TargetMode = 1u;     /* enter sleep mode */
    }
    else if(newMode == BSP_MODE_LOWPOWER)
    {
    	u8g_TargetMode = 2u;    /* enter low-power mode */
    }
    else if(newMode == BSP_MODE_NORMAL)
    {
    	u8g_TargetMode = 3u;    /* enter normal mode */                     
    }
    else
    {
    	u8g_TargetMode = 0u;    /* dummy */
    }
}

/*
 * Name        : BSP_ModeChangeRunnable() 
 * Description : switch current mode to required mode
 * Arguments   : None
 * Return      : None
 */
void BSP_ModeChangeRunnable(void)
{
    if(u8g_TargetMode == 1u)   /* sleep mode entering */
    {
        u8g_TargetMode = 0u;
        MCU_DisableWatchdog();

    	/* preparations for entering sleep mode */
    	asm(" wrteei 0");                               /* disable interrupt */
    	SIUL_Output(VCC_CTL, 1u);                       /* turn off VCC */
	    SIUL_Output(LED_OE,  0u);                      /* OE down */
    	/* TODO other modules shall be turned off ? */

        APPL_NotifyModeChange(BSP_MODE_SLEEP);          /* MUST be called before stop mode */
        SIU.PCR[19].R = 0x0101u;        /* PC0 enable as input */
	    //SIU.PCR[16].R = 0x0624u;        /* PB0 as CAN0 Tx */
	    //SIU.PCR[17].R = 0x0500u;        /* PB1 as CAN0 Rx */
	    WKUP.WIFER.R  = 0x00000006;     /* Enable analog filters for Ignition */
	    WKUP.WIREER.R = 0x00000000;     /* Disable rising edge events on Ignition */
	    WKUP.WIFEER.R = 0x00000006;     /* Enable falling edge events on Ignition */
	    WKUP.WRER.R   = 0x00000006;     /* Enable wake up events for Ignition  */
	    WKUP.WISR.R   = 0x001FFFFF;     /* clear all wake up flags */
	    WKUP.WIPUER.R = 0x001FFFFF;     /* Enable WKUP pins pullups to stop leakage*/    	
	    MCU_StopModeEnter();
        /* resume from sleep mode */
    	if((WKUP.WISR.R & 0x00000002u) != 0u)
    	{
    		WKUP.WISR.R   = 0x001FFFFF;     /* clear all wake up flags */
    		APPL_WakeupNotify(BSP_WAKEUP_CAN_COMM);
    	}
    	else if((WKUP.WISR.R & 0x00000004u) != 0u)
    	{
    		WKUP.WISR.R   = 0x001FFFFF;     /* clear all wake up flags */
    		SIUL_Output(VCC_CTL, 0u);       /* turn on VCC */
    		/* TODO: other modules? */
    		APPL_WakeupNotify(BSP_WAKEUP_IGN);
    	}
    	else    /* unknown wakeup, enter stop mode again */
    	{
    		u8g_TargetMode = 1u;
    	}
    	 __asm(" wrteei 1");
         MCU_EnableWatchdog();
    }
   else if(u8g_TargetMode == 2u)     /* low-power mode entering */
    {
    	u8g_TargetMode = 0u;
    	/* preparations for entering low-power moded */
		SIUL_Output(LCD_BL,  0);    /* close illumination */
		ECU_Sound_Init();
	    /* TODO: other modules? */

    	CurrentStateflag = 0u;						   /* forbid the motors'moving */
		MCU_LowPowerModeEnter();
    	APPL_NotifyModeChange(BSP_MODE_LOWPOWER);      /* MUST be called after low-power mode */
    }
    else if(u8g_TargetMode == 3u)    /* normal mode entering */
    {
    	u8g_TargetMode = 0u;
    	if(MCU_CUR_MODE != MCU_NORMAL_MODE)
     	{
     	    MCU_NormalModeEnter();      /* enter normal mode */
     	    SIUL_Output(VCC_CTL, 0u);   /* turn on VCC */
			ECU_Sound_Init();           /* initialize sound */
            ECU_FreqInput_Init();
	        SIUL_Output(LCD_BL, 1);    /* open illumination */
	        
            /*TODO: other modules? gauge? */
	        ECU_AnalogueInput_Init();
	        ECU_StepMotor_Init();
	        
    	    APPL_NotifyModeChange(BSP_MODE_NORMAL);
    	}
        else        
        {
            APPL_NotifyModeChange(BSP_MODE_NORMAL);
        }
    }
    else
    {
    	 ;    /* dummy */
    }
}




