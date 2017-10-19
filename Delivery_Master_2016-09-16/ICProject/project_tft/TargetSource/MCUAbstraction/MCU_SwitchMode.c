/***************************************************************************************/
/*                                                                                     */
/* FILE        : SwitchMode.c                                                          */
/* DATE        : 2015-12-15                                                            */
/* AUTHOR      : Cao chenguang                                                         */
/* DESCRIPTION : implementation file of wakup unit                                     */
/* NOTE        : All Rights reserved by DETC, Technology Center                        */
/*                                                                                     */
/***************************************************************************************/
#include "jdp.h"
#include "Platform_Types.h"
#include "MCU_SwitchMode.h"
#include "ModeManager.h"

/*
 * Name         : MCU_LowPowerModeEnter 
 * Description  : switch running-mode to low power mode
 * Arguments    : none
 * Return       : none
 * Notes        : This function will be automatically generated
 */
void MCU_LowPowerModeEnter(void)
{
    ME.PCTL[16].R = 0x02u;     /* FlexCAN0 in mode peripherals configuration 2, which runs in RUN1 */
	ME.PCTL[62].R = 0x02u;     /* SGL      in mode peripherals configuration 2, which runs in RUN1 */
	ME.PCTL[68].R = 0x02u;     /* SIUL     in mode peripherals configuration 2, which runs in RUN1 */     
    ME.PCTL[72].R = 0x02u;     /* eMIOS0   in mode peripherals configuration 2, which runs in RUN1 */
	ME.PCTL[73].R = 0x02u;     /* eMIOS1   in mode peripherals configuration 2, which runs in RUN1 */
	ME.PCTL[91].R = 0x02u;     /* RTC/API  in mode peripherals configuration 2, which runs in RUN1 */ 
	ME.PCTL[92].R = 0x02u;     /* PIT/RTI  in mode peripherals configuration 2, which runs in RUN1 */
	mode_entry_run1();         /* entry peripherals configuration 2, which runs in RUN1 */
	mode_entry_run1();
}

/*
 * Name        : MCU_NormalModeEnter
 * Description : Switch mode run1 to run0 (normal mode )
 * Arguments   : none
 * Return      : none
 * Notes       : This function will be automatically generated
 */
void MCU_NormalModeEnter(void)
{
    ME.PCTL[16].R = 0x01;        /* FlexCAN0 in mode peripherals configuration 1, which runs in RUN0 */  
	ME.PCTL[62].R = 0x01;        /* SGL      in mode peripherals configuration 1, which runs in RUN0 */
    ME.PCTL[68].R = 0x01;        /* SIUL     in mode peripherals configuration 1, which runs in RUN0 */  
	ME.PCTL[72].R = 0x01;        /* eMIOS0   in mode peripherals configuration 1, which runs in RUN0 */
    ME.PCTL[73].R = 0x01;        /* eMIOS1   in mode peripherals configuration 1, which runs in RUN0 */
	ME.PCTL[91].R = 0x01;        /* RTC/API  in mode peripherals configuration 1, which runs in RUN0 */ 
	ME.PCTL[92].R = 0x01;        /* PIT/RTI  in mode peripherals configuration 1, which runs in RUN0 */
    mode_entry_run0();           /* switch to normal mode run0 */
}

/*
 * Name        : MCU_StopEnter
 * Description : switch running-mode to stop mode
 * Arguments   : none
 * Return      : none
 * Notes       : This function will be automatically generated
 */
void MCU_StopModeEnter(void) 
{	    	   	
    mode_entry_stop();
}

/* Name        : MCU_Set_AllowedWakeupSource
 * Description : Set Wake-Up source
 * Arguments   : None
 * Return      : None
 */
void MCU_SetAllowedWakeupSource(uint8 u8_WakeupMask) 
{
	uint32 u32t_WakeUpSourceValue = u8_WakeupMask;
	u32t_WakeUpSourceValue = 1u << u32t_WakeUpSourceValue;
	WKUP.WIREER.R |= u32t_WakeUpSourceValue;               /* Enable rising edge events on WakeupMask*/ 
    WKUP.WIFER.R  |= u32t_WakeUpSourceValue;               /* Enable analogue filters - WakeupMask */  
    WKUP.WRER.R   |= u32t_WakeUpSourceValue;               /* Enable wake-up events for WakeupMask */
//    WKUP.IRER.R   |= u32t_WakeUpSourceValue;               /* Enable interrupt events for WakeupMask */
    WKUP.WISR.R   |= u32t_WakeUpSourceValue;               /* Clear wake up flag WakeupMask*/
}

/*
 * Name        : MCU_Clear_AllowedWakeupSource
 * Description : Clear Wake-Up source
 * Arguments   : None
 * Return      : None
 */
void MCU_ClearAllowedWakeupSource(uint8 u8_WakeupMask)
{
    uint32  u32t_WakeUpSourceValue = u8_WakeupMask;
    u32t_WakeUpSourceValue = 1u << u32t_WakeUpSourceValue;
    WKUP.WIREER.R &= ~u32t_WakeUpSourceValue;
    WKUP.WIFER.R  &= ~u32t_WakeUpSourceValue;
    WKUP.WRER.R   &= ~u32t_WakeUpSourceValue;
//    WKUP.IRER.R   &= ~u32t_WakeUpSourceValue;
}