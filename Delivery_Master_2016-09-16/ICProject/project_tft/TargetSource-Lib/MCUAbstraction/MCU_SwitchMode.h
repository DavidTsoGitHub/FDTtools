/***************************************************************************************/
/*                                                                                     */
/* FILE        : MCU_SwitchMode.h                                                      */
/* DATE        : 2015-12-15                                                            */
/* AUTHOR      : Cao chenguang                                                         */
/* DESCRIPTION : header file of wakup unit                                             */
/* NOTE        : All Rights reserved by DETC, Technology Center                        */
/*                                                                                     */
/***************************************************************************************/
#ifndef MCU_SWITCHMODE_H
#define MCU_SWITCHMODE_H

/* Wakeup source according to hardware schematic */
#define MCU_WAKEUP_IGN    4u//7u    /* WKPU7 */
//#define MCU_WAKEUP_VECU   4u    /* WKPU2 */
#define MCU_WAKEUP_CAN0   1u    /* WKPU1 */

#define MCU_NORMAL_MODE       0x4u
#define MCU_LOWPOWER_MODE     0x5u
#define MCU_CUR_MODE          ME.GS.B.S_CURRENTMODE

/* TODO: move the following 4 function prototypes to a proper place */
extern void mode_entry_drun(void);
extern void mode_entry_run1(void);
extern void mode_entry_stop(void);
extern void mode_entry_run0(void);

/*
 *Name         : MCU_LowPowerModeEnter 
 *Description  : switch RUN0 to RUN1
 *Arguments    : none
 *Return       : none
 */
extern void MCU_LowPowerModeEnter(void);

/*
 *Name        : MCU_NormalModeEnter
 *Description : Switch RUN1 to RUN0
 *Arguments   : none
 *Return      : none
 */
extern void MCU_NormalModeEnter(void);

/*
 * Name        : MCU_StopModeEnter
 * Description : switch to STOP
 * Arguments   : none
 * Return      : none
 * 
 */
extern void MCU_StopModeEnter(void);

/* Name        : MCU_Set_AllowedWakeupSource
 * Description : Set Wake-Up source
 * Arguments   : u8_WakeupMask - wake up mask
 * Return      : None
 */
extern void MCU_SetAllowedWakeupSource(uint8 u8_WakeupMask); 

/*
 * Name        : MCU_Clear_AllowedWakeupSource
 * Description : Clear Wake-Up source
 * Arguments   : u8_WakeupMask - wake up mask
 * Return      : None
 */
extern void MCU_ClearAllowedWakeupSource(uint8 u8_WakeupMask);

#endif
