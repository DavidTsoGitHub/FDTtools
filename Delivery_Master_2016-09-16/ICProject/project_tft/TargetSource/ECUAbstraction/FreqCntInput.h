/*****************************************************************/
/*                                                               */
/* FILE        : FreqCntInput.h                                  */
/* DATE        : 2015-06-20                                      */
/* AUTHOR      : Yuan Jia                                        */
/* DESCRIPTION : header file of frequent input capture module    */
/* HISTORY     : 1.0.0 release for the first time.               */
/*                                                               */
/* NOTE        : All Rights reserved by DETC, Technology Centre  */
/*               Last Modified By Yuan Jia on 2015-07-16         */
/*                                                               */
/*****************************************************************/
#ifndef  ECU_FREQCNTINPUT_H
#define  ECU_FREQCNTINPUT_H

/*
 * Name         : ECU_FreqInput_Init
 * Description  : Initialize frequency counter input capture
 * Arguments    : void
 * Return       : void                  
 */
extern void ECU_FreqInput_Init(void); 

extern BSP_StdReturnType BSP_Get_PulseCounter(uint8 FInputId, uint32 *Value);
#endif
