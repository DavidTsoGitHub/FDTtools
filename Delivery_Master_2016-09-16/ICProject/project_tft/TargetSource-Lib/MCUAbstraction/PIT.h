/*****************************************************************/
/*                                                               */
/* FILE        : PIT.h                                           */
/* DATE        : 2015-04-28                                      */
/* AUTHOR      : Cao ChenGuang                                   */
/* DESCRIPTION : header file of Periodic Interrupt Timer module  */
/* HISTORY     : 1.0.0 release for the first time.               */
/*                                                               */
/* NOTE        : All Rights reserved by DETC, Technology Center  */
/*               Last Modified By Cao ChenGuang on 2015-04-28    */
/*                                                               */
/*****************************************************************/
#ifndef PIT_H
#define PIT_H

/* macros with parameters */
#define CLEAR_PIT_INT_FLAG(Channel)    PIT.CH[Channel].TFLG.R=1
extern void PIT_ISR0(void);
#endif
