/*                                                               */
/* FILE        : Discrete.h                                      */
/* DATE        : 2015-06-09                                      */
/* AUTHOR      : Cao Wei                                         */
/* DESCRIPTION : source file of discrete module                  */
/* HISTORY     : 1.0.0 release for the first time.               */
/*                                                               */
/* NOTE        : All Rights reserved by DETC, Technology Centre  */
/*               Last Modified By Cao wei on 2015-06-09          */
/*                                                               */
/*****************************************************************/
#ifndef DISCRETE_H
#define DISCRETE_H

#define DI_INPUT_NUM          32u

#if DI_INPUT_NUM > 32
#define DI_INPUT_NUM          32u
#endif


/*
 * Name        : ECU_DiscreteInput_Init
 * Description : Initialize discrete input
 * Arguments   : none
 * Return      : none
 */
extern void ECU_DiscreteInput_Init(void);

/*
 * Name        : ECU_DiscreteInput_Runnable
 * Description : get discrete input at every 50ms
 * Arguments   : none
 * Return      : none
 */
extern void ECU_DiscreteInput_Runnable(void);

#endif
