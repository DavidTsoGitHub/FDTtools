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
 * file                   : stm_init.h
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
 * Brief Description      : File contains declaration for System Timer Module (STM)
 *                          initialization function.
 *
 *
 *######################################################################
*/

#ifndef  _STM_INIT_H
#define  _STM_INIT_H
/********************  Dependent Include files here **********************/

#include "jdp.h"

/**********************  Function Prototype here *************************/

void stm_init_fnc (void);

void STM_Isr0 (void);
void STM_Isr1 (void);

#endif  /*_STM_INIT_H*/

/*
 *######################################################################
 *                           End of File
 *######################################################################
*/

