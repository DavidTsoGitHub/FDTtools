/*                                                               */
/* FILE        : ECU_RTC.h                                       */
/* DATE        : 2015-06-18                                      */
/* AUTHOR      : Zhu Mingqi                                      */
/* DESCRIPTION : header file of RTC module                       */
/* HISTORY     : 1.0.0 release for the first time.               */
/*                                                               */
/* NOTE        : All Rights reserved by DETC, Technology Centre  */
/*               Last Modified By Zhu Mingqi on 2015-06-18       */
/*                                                               */
/*****************************************************************/
#ifndef ECU_RTC_H
#define ECU_RTC_H

#define RTC_MAXYEAR 2200u

/*
 * Name        : ECU_Rtc_Init
 * Description : ECU Interface to initialize RTC variables
 * Arguments   : none
 * Return      : none
 */
extern void ECU_Rtc_Init(void);

extern void MCU_Api_ISR(void);

#endif
