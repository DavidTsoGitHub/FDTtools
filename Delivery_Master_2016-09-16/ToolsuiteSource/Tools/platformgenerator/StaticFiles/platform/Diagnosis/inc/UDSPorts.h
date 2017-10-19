/*****************************************************************/
/*                                                               */
/* FILE        : UDSPorts.h                                      */
/* DATE        : 2015-11-30                                      */
/* AUTHOR      : Cao Chenguang                                   */
/* DESCRIPTION : functions relates to  target MCU.               */
/* NOTE        : All Rights reserved by DETC, Technology Center  */
/*                                                               */
/*****************************************************************/
#ifndef UDS_PORT_H
#define UDS_PORT_H

#define SEED_LEN                         4u
#define KEY_LEN                          4u  /* KEY_LEN is equal to SEED_LEN temporarily */
#define MAX_ACCESS_FAILURE_NUM           2u
#define SECURITY_ACCESS_DELAY_TIMEOUT 1000u  /* number of period for counting security access delay */

#define AVAILABILITY_MASK             0x09u
#define DTC_NUM                         12u  /* total number of DTCs */

#define TEST_PRESENT_TIMEOUT           500u  /* number of period for counting test present service */
#define DID_MAX_NUM                     10u  /* maximum number of DIDs which can be simultaneously requested */

/* functions or statments */
#define   TRANSITION_SESS(Session)                           Diag_CheckTransitionCondition(Session)

#ifndef _WIN32
#include "jdp.h"
#define   SET_REBOOT_FLAG()                                  asm("mtsprg0 %0"::"r"(0x2A));asm("mtsprg1 %0"::"r"(0xFFFFFFD5))
#define   RESET_ECU()                                        {int i = 0; while(i < 3000) i++;}\
                                                                ME.MCTL.R = 0x00005AF0u;ME.MCTL.R = 0x0000A50Fu
#else
#define   SET_REBOOT_FLAG()                                  {;}
#define   RESET_ECU()                                        {;}
#endif

#define   RESUME_DTC_SET()                                   {;}
#define   STOP_DTC_SET()                                     {;}
#define   ENABLE_RX_TX()                                     {;}
#define   ENABLE_RX_DISABLE_TX()                             {;}
#define   DISABLE_RX_ENABLE_TX()                             {;}
#define   DISABLE_RX_TX()                                    {;}
#define   CLEAR_DTC_CHECK()                                  Diag_CheckClearDTC()
#define   GEN_SEED(u8_Seed)                                  Diag_GenSeed(u8_Seed)
#define   ENCRYPTION(u8_Seed, u8_Key)                        Diag_Encryption(u8_Seed, u8_Key)
#endif
