/*****************************************************************/
/*                                                               */
/* FILE        : IODefine.h                                      */
/* DATE        : 2015-05-14                                      */
/* AUTHOR      : Cao ChenGuang                                   */
/* DESCRIPTION : Connect peripheral Pins to MCU Pins, except for */
/*               dedicated MCU pins.                             */
/* NOTE        : All Rights reserved by DETC, Technology Center  */
/*                                                               */
/*****************************************************************/
#ifndef IO_DEFINE_H
#define IO_DEFINE_H

/*      peripheral pin     MCU pin */
/* Indicator */
#define LED_SDI            PK11
#define LED_SDI2           PB9
#define LED_SDI3           PB8
#define LED_CK             PB7
#define LED_LE             PJ4
#define LED_OE             PJ5

#define ACTIVE_BUZZER      PB12
#define IGN_VOLTAGE_IN     PB2
#define IGN_ON             PJ13
#define CAN0_STB           PJ11
#define CAN1_STB           PH4

#define LCD_BL             PK1
#define GAUGE_BL           PF0
#define VCC_CTL            PF8
/* ADC */
#define DI_CLK             PF1
#define P_SOUT     		   PF3
#define SH_LD      	       PF4
#define DI_CE              PF5
#define PORT_A             PJ2
#define PORT_B             PJ1
#define PORT_C             PJ0

#endif
