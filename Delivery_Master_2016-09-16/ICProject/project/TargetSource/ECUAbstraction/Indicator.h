/*****************************************************************/
/*                                                               */
/* FILE        : Indicator.h                                     */
/* DATE        : 2015-05-14                                      */
/* AUTHOR      : Cao ChenGuang                                   */
/* DESCRIPTION : header file of Indicator module                 */
/* HISTORY     : 1.0.0 release for the first time.               */
/*               1.1.0 merge 1HZ runnable and 2HZ runnable,  all */
/*                    of the indicators are set in same Runnable */
/* NOTE        : All Rights reserved by DETC, Technology Centre  */
/*               Last Modified By Cao ChenGuang on 2015-08-27    */
/*                                                               */
/*****************************************************************/
#ifndef INDICATOR_H
#define INDICATOR_H

#define INDICATOR_NUM      83
#define INDICATOR_GRP_NUM   3
#define FLASH1HZ_IND_NUM    3
#define FLASH2HZ_IND_NUM    2     

/*
 * Name        : ECU_IndicatorInit
 * Description : initialize indicators
 * Arguments   : none
 * Return      : none
 */
extern void ECU_Indicator_Init(void);

/*
 * Name        : ECU_IndicatorInit
 * Description : initialize indicators
 * Arguments   : none
 * Return      : none
 */
extern void ECU_Telltale_Init(void);

/*
 * Name        : ECU_LED1HzFlash_Runnable
 * Description : LED twinkles at 1 Hz.
 * Arguments   : none
 * Return      : none
 * Notes       : This runnable shall be called every 10ms.
 */
void ECU_LEDFlash_Runnable(void);

#endif
