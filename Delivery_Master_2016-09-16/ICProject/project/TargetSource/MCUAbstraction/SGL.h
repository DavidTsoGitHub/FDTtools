/*****************************************************************/
/*                                                               */
/* FILE        : SGL.h                                           */
/* DATE        : 2015-04-30                                      */
/* AUTHOR      : Cao ChenGuang                                   */
/* DESCRIPTION : header file of SGL module                       */
/* HISTORY     : 1.0.0 release for the first time.               */
/*                                                               */
/* NOTE        : All Rights reserved by DETC, Technology Center  */
/*               Last Modified By Cao ChenGuang on 2015-05-30    */
/*                                                               */
/*****************************************************************/
#ifndef SGL_H
#define SGL_H

#define RESET_SGL_MODE()           SGL.MODE_SEL.B.SN_CTRL=0
#define CLEAR_SGL_INTC_FLAG()      SGL.SGL_STATUS.B.SDCIFC=1

#define SGL_SET_HIGH(high)         SGL.HIGH_PER.R=high
#define SGL_SET_LOW(low)           SGL.LOW_PER.R=low
#define SGL_SET_DURATION(dur)      SGL.SOUND_DUR.R=dur

/* 5 modes: 0 - 5, see "BSP design specification" for more details. */
#define SGL_SET_MODE(mode)         SGL.MODE_SEL.B.SN_CTRL=mode

/*
 * Name        : SGL_Init
 * Description : initialize sound
 * Arguments   : none
 * Return      : none
 */
extern void SGL_Init(void);

/*
 * Name        : MCU_ToneSet
 * Description : set sound tone
 * Arguments   : u32_tone- tone to be set
 * Return      : none
 */
extern void MCU_ToneSet(uint32 u32_tone);

#endif
