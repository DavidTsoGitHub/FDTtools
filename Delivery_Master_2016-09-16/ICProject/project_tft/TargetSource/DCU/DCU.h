/*                                                               */
/* FILE        : DCU.h                                           */
/* DATE        : 2015-12-24                                      */
/* AUTHOR      : Cao Chenguang                                   */
/* DESCRIPTION : header file of DCU module                       */
/* NOTE        : All Rights reserved by DETC, Technology Center  */
/*               Last Modified By Cao Wei on 2015-06-24          */
/*                                                               */
/*****************************************************************/
#ifndef DCU_H
#define DCU_H
#include "Platform_Types.h"

typedef  void  (*DCU_CallbackType)(void);

extern void DCU_Init(void);

void DCU_ISR(void);
void DCU_RegisterLSBFVSCallback(DCU_CallbackType callback);
void DCU_Modeswitch(uint8 mode);

#endif
