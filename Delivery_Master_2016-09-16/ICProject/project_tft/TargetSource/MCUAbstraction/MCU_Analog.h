/*****************************************************************/
/*                                                               */
/* FILE        : MCU_Analog.h                                    */
/* DATE        : 2015-06-15                                      */
/* AUTHOR      : Cao Wei                                         */
/* DESCRIPTION : header file of Analog module in MCU             */
/* HISTORY     : 1.0.0 release for the first time.               */
/*                                                               */
/* NOTE        : All Rights reserved by DETC, Technology Centre  */
/*               Last Modified By Cao ChenGuang on 2015-06-17    */
/*                                                               */
/*****************************************************************/
#ifndef MCU_ANALOG_H
#define MCU_ANALOG_H

/* AD channel basic address */
#define BASIC_ADDR              32u

#define START_ADC()             ADC_0.MCR.B.NSTART=1
#define WAIT_FOR_ADC_DONE()     while(ADC_0.MSR.B.ADCSTATUS == 0x6){;}
#define DATA_VALID(num)         while(ADC_0.CDR[num].B.VALID == 0){;}

#define AD_GetInput(num)        ADC_0.CDR[num].B.CDATA


extern void MCU_OpenADSwitchPort(uint8 u8_port1,uint8 u8_port2,uint8 u8_port3);

#endif
