/*****************************************************************/
/*                                                               */
/* FILE        : MCR_RTC.h                                       */
/* DATE        : 2015-06-20                                      */
/* AUTHOR      : Cao ChenGuang                                   */
/* DESCRIPTION : header file of BSP module                       */
/* HISTORY     : 1.0.0 release for the first time.               */
/*                                                               */
/* NOTE        : All Rights reserved by DETC, Technology Centre  */
/*               Last Modified By Cao ChenGuang on 2015-06-20    */
/*                                                               */
/*****************************************************************/
#ifndef MCU_RTC_H
#define MCU_RTC_H
#define CLEAR_RTC_API_INT_FLAG()  RTC.RTCS.B.APIF=1
#define RTC_COUNTER               RTC.RTCCNT.R

#endif
