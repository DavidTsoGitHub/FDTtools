/*****************************************************************/
/*                                                               */
/* FILE        : Watchdog.h                                      */
/* DATE        : 2015-04-20                                      */
/* AUTHOR      : Cao ChenGuang                                   */
/* DESCRIPTION : header file of watchdog module                  */
/* HISTORY     : 1.0.0 release for the first time.               */
/*                                                               */
/* NOTE        : All Rights reserved by DETC, Technology Center  */
/*               Last Modified By Cao ChenGuang on 2015-04-20    */
/*                                                               */
/*****************************************************************/
#ifndef  WATCHDOG_H
#define  WATCHDOG_H

/*
 * Name        : MCU_EnableWatchdog
 * Description : disable watchdog
 * Arguments   : none
 * Return      : none
 */
extern void MCU_DisableWatchdog(void); 

/*
 * Name        : MCU_EnableWatchdog
 * Description : enable watchdog
 * Arguments   : none
 * Return      : none
 */
extern void MCU_EnableWatchdog(void);

/*
 * Name        : MCU_ClearWatchdog
 * Description : clear watchdog
 * Arguments   : none
 * Return      : none
 */
extern void MCU_ClearWatchdog(void);

#endif
