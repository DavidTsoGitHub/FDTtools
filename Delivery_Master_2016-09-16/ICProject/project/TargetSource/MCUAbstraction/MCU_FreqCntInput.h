/*                                                                */
/* FILE        : MCU_FreqCntInput.h                               */
/* DATA        : 2015-08-21                                       */
/* AUTHOR      : Zhu Mingqi                                       */
/* DESCRIPTION : header file of freq input capture module in MCU  */
/* HISTORY     : 1.0.0 release for the first time                 */
/*                                                                */
/* NOTE        : All Rights reserved by DETC , Technology Centre  */
/*               Last Modified By Zhu Mingqi on 2015-08-21        */
/*                                                                */
/******************************************************************/
#include "Platform_Types.h"

#ifndef  FREQCNTINPUT_H
#define  FREQCNTINPUT_H
#define  FREQINPUTDATABUFFERSIZE   15//5//Modified by Zhu Mingqi 2015-08-21
#define  FREQCNTSHIFTING           2
#define  COUNTINGMETHOD            0
#define  PERIODICMETHOD            1

/*
 * Name         : ECU_FreqInput_Init
 * Description  : Initialize frequency counter input
 * Arguments    : void
 * Return       : void                  
 */
extern void MCU_FreqInput_Init(uint8 CurMethod);

/*
 * Name        : FreqInputISR
 * Description : frequency signal input interrupt handler
 * Arguments   : void
 * Return      : void
 */
extern void FreqInputISR (void);

/*
 * Name        : FreqInputCntOverflowISR
 * Description : counting overflow ISR while in PeriodMethod 
 * Arguments   : void
 * Return      : void
 */
extern void FreqInputCntOverflowISR (void);

/*
 * Name        : FreqInputCapTimerISR
 * Description : system timer in 1s, which is used to count the counting-value when in CountingMethod
 * Arguments   : void
 * Return      : void
 */
extern void FreqInputCapTimerISR (void);

extern uint32 u32g_FreqCntValueInCounting;                  /* Freq Signal Input Counter value in Counting method after Filter */
extern uint32  u32g_FreqCntValueInPeriod;                   /* Frequency Signal Input Counter Value in period method after Filter */
#endif

