/*****************************************************************/
/*                                                               */
/* FILE        : emios.h                                         */
/* DATE        : 2015-06-16                                      */
/* AUTHOR      : Yuan Jia                                        */
/* DESCRIPTION : header file of BSP module                       */
/* HISTORY     : 1.0.0 release for the first time.               */
/*                                                               */
/* NOTE        : All Rights reserved by DETC, Technology Centre  */
/*               Last Modified By Cao ChenGuang on 2015-06-16    */
/*                                                               */
/*****************************************************************/
#ifndef  EMIOS_H
#define  EMIOS_H

/* Be Converted from Frequency(868Hz),corresponding PWM_DURATION is 1150*/
#define HALF_PWM_DURATION   575u

/*
 * Name        : MCU_SetEmios0Channel
 * Description : Set Register value of Emios0 Channel , to output PWM waveform
 * Arguments   : u8_emios_channel - emios channel
 *               u16_HDuration    - high duration
 * Return      : none
 */
extern void MCU_SetEmios0Channel(uint8 u8_emiosChannel, uint16 u16_HDuration);
#endif
