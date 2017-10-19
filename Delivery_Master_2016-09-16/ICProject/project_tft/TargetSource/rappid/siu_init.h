/*
 *######################################################################
 *                                RAppIDJDP
 *           Rapid Application Initialization and Documentation Tool
 *                         Freescale Semiconductor Inc.
 *
 *######################################################################
 *
 * Project Name           : detc_ecu
 *
 * Project File           : detc_ecu.rsp
 *
 * Revision Number        : 1.0
 *
 * Tool Version           : 1.0.0.3
 *
 * file                   : siu_init.h
 *
 * Target Compiler        : Codewarrior
 *
 * Target Part            : MPC5606S
 *
 * Part Errata Fixes      : none
 *
 * Project Last Save Date : 04-Sep-2014 15:25:01
 *
 * Created on Date        : 04-Sep-2014 15:25:02
 *
 * Brief Description      : This file contains functions declaration of SIU code file
 *
 *
 *######################################################################
*/

#ifndef  _SIU_INIT_H
#define  _SIU_INIT_H
/********************  Dependent Include files here **********************/

#include "jdp.h"

extern void siu_portA_init_fnc(void);
extern void siu_portB_init_fnc(void);
extern void siu_portC_init_fnc(void);
extern void siu_portD_init_fnc(void);
extern void siu_portE_init_fnc(void);
extern void siu_portF_init_fnc(void);
extern void siu_portG_init_fnc(void);
extern void siu_portH_init_fnc(void);
extern void siu_portJ_init_fnc(void);
extern void siu_portK_init_fnc(void);
extern void siu_general_init_fnc(void);
extern void siu_parallel_port_init_fnc(void);
extern void siu_unused_pins_init_fnc(void);
extern void lcd_sfr_init_fnc(void);
extern void siu_psmi_init_fnc(void);



#endif  /*_SIU_INIT_H*/

/*
 *######################################################################
 *                           End of File
 *######################################################################
*/

