/* -------------------------------- Arctic Core ------------------------------
 * Copyright (C) 2009-2011 ArcCore AB <contact@arccore.com>
 * Licensed under ArcCore Embedded Software License Agreement.
 * -------------------------------- Arctic Core ------------------------------*/


#ifndef CONFIG_H_
#define CONFIG_H_


#define CFG_ARCH_FAM 		ppc
#define CFG_ARCH			mpc55xx

#include "config_defines.h"

/*----------------------------[ BOOT BEHAVIOUR ]--------------------------*/

#define CFG_FL_MODE_STAY_TIME	10	/* in mseconds, if 0, no wait */

/*----------------------------[ MODULES TO USE ]--------------------------*/
#define CFG_DCM 				1
#define CFG_CAL 				1
#define CFG_SECM 				1
#define CFG_FLASH				1
#define CFG_RAMLOG				0
#define CFG_ISO15765			1
#define CFG_CAN					1
#define CFG_FLASH_H7F_C90		1
#define CFG_CONSOLE				0
#define CFG_SERIAL_T32			0
#define CFG_SERIAL_SCI			0
#define CFG_PRINTF				0
#define CFG_CRT0				1
#define CFG_WATCHDOG 			1

#define CFG_PPC					1
//#define CFG_MPC5606S            0
#define CFG_MPC560X             0
#define CFG_MPC5645S            1
#define CFG_MPC55XX				0
#define CFG_MPC5XXX				1
#define CFG_VLE					1
#define CFG_WDG_BOOKE           0
#define CFG_E200Z4D             1
//#define CFG_E200Z0H             1
#define CFG_WDG_SWT				1
#define CFG_QSPI                1
#define CFG_CAL_HEADER_IN_DATA_FLASH    0

#define CFG_CLIB_NEWLIB			1
#define CFG_CLIB_NATIVE			0
#define CFG_CLIB_CW_MSL			0
#define CFG_CLIB_WRC			0

//#define CFG_DECOMPRESSION 1
//#define CFG_COMPLETE_COMPATIBLE 1

//#define CFG_DID_FLASH_BLOCK     1

//#define PROG_END_SIG_MSB (uint32)0x456e6453uL
//#define PROG_END_SIG_LSB (uint32)0x69676e61uL

//#define PROG_START_SIG_MSB (uint32)0x53746172uL
//#define PROG_START_SIG_LSB (uint32)0x74536967uL

//#define OEM_SEED_SIZE       3
//#define OEM_KEY_SIZE        OEM_SEED_SIZE


//#define OEM_SECACC_K1   (uint8_t)0x54u
//#define OEM_SECACC_K2   (uint8_t)0x44u
//#define OEM_SECACC_K3   (uint8_t)0x52u
//#define OEM_SECACC_K4   (uint8_t)0x30u
//#define OEM_SECACC_K5   (uint8_t)0x31u
/*---------------------------[ CONSOLE ]---------------------------------*/

/* Default console device */
#if (CFG_CONSOLE == 1 )
#define CONSOLE_DEVICE      "serial_t32"
#define CMD_BOARD			1
#endif

/*----------------------------[ MISC ]------------------------------------*/

#define CPU_FREQ		        80000000		/* 80Mhz */
#define TIMER_FREQ		        8000000UL		/* True? */
#define STACK_STARTUP_SIZE      (32*4*2)        /* 32 regs, 4 bytes each, 1 iterations */
#define FMPLL_IDF(_x)		    ((_x)<<(31-5))
#define FMPLL_ODF(_x)			((_x)<<(31-7))
#define FMPLL_NDIV(_x)			((_x)<<(31-15))
#define FMPLL_EN_PLL_SW			(1<<(31-23))
#define FMPLL_S_LOCK			(1<<(31-28))

/* IVPR is 20 bits */
#define IVOR_ALIGNMENT			0x1000

/* Number of bytes to checksum before kicking watchdog */
#define CRC_KICK_LENGTH     50

#define APP_START_BEHAVIOUR	APP_START_WITH_RESET


/* 16Mhz crystal connected to XTAL, EXTAL (FXOSC)
 * Calculation for MPC56xxB/P/S
 *  FMPLL1 - IDF
 *  FMPLL2 - NDIV
 *  FMPLL3 - ODF
 *
 *  Fpll = Ffxosc * NDIV / (( IDF + 1) * 2^(ODF+1))
 *
 *  So, with FXOSC = 16Mhz
 *  Fpll  = 16000000 * 64 / ((0+1)*2^(2+1)) = 64Mhz...
 */
#if (CPU_FREQ==80000000)
#define MCU_XTAL						8000000UL
#define MCU_FMPLL1						0
#define MCU_FMPLL2						40
#define MCU_FMPLL3						0
#define MCU_FMPLL4						0		/* NOT USED */
#define MCU_FMPLL_EN_PLL_SW			    (1<<(31-23))
#undef MCU_ENABLE_CLKOUT
#else
#error CPU_FREQ not defined
#endif

/* Check settings */
//#if !(MCU_IDF <= 15) || !(MCU_NDIV >=32 && MCU_NDIV <= 96) || !(MCU_ODF <= 3)
//#error BAD config
//#endif


/* Number of bytes to verify before calling callback */
#define FLASH_VERIFY_CALLBACK_BYTE_COUNT         50


/*----------------------------[ MEMORY ]------------------------------------*/

#define SRAM_START				    0x40000000		/* Start SRAM */
//#define SRAM_SIZE				    0xC000		    /* 48Kbyte */ //todo adto it has to  be clarified

/* Code flash  */
#define FLASH_START				    0x0
//#define FLASH_SIZE				    0x00200000		/* 2Mbyte */

/* Valid flash regions */
#define FLASH_VALID_ERASE_START	    0x8000          //todo adto it has to  be clarified
#define FLASH_VALID_ERASE_SIZE	    0x20000         //todo adto it has to  be clarified

#define CFG_SBL_LOAD_ADDR_ROM       0xa000          //todo adto it has to  be clarified
#define CFG_SBL_LOAD_ADDR_RAM		0x4000a000      //todo adto it has to  be clarified

#define SBL_BUFFER_SIZE			    0x1000

#define APP_IMAGE_START				0x10000         //todo adto it has to  be clarified
#define APP_HEADER_START		    0x10020         //todo adto it has to  be clarified

#define PBL_DID_BLOCK_START_ADDR    0x100uL         //todo adto it has to  be clarified

#define FLASH_SERIAL_NBR_ADDR                   0x0000C000uL //todo adto this must be refined according to new reqs
#define FLASH_DID_ECU_DELIVERY_ASSEMBLY_ADDR    0x0000C008uL //todo adto this must be refined according to new reqs
#define FLASH_DID_ECU_CORE_ASSEMBLY_ADDR        0x0000C010uL //todo adto this must be refined according to new reqs

#if defined(CFG_DECOMPRESSION)
#define DECOMPRESSION_OVERLAP       2u
#endif

#define COM_BUFFER_SIZE				4096


/*----------------------------[ SERIAL ]------------------------------------*/
/*----------------------------[ TIMERS ]------------------------------------*/
/*----------------------------[ FLASH ]-------------------------------------*/


/*----------------------------[ ISO15765-X ]---------------------------------*/
#define ISO15765_CAN_DEVICE             0

#define ISO15765_SERVICE_PERIOD 		5 // ms

#define ISO15765_maxNumberOfBlockLength	0xFFA

/*
 * ISO15765-3, Application, Server timing, Table 2
 */
#define ISO15765_P2_TIMEOUT 			25      		/* ms, MIN:0, MAX: 50ms */
#define ISO15765_P2o_TIMEOUT 			5000 	        /* ms, MIN:0, MAX: 5000ms */

/*
 * ISO15765-2, Network, Server timing, Table 15

#define ISO15765_stMin 					0               //see iso15765 for definition
#define ISO15765_N_As 					ISO15765_N_Bs   // same time as N_Bs
#define ISO15765_N_Ar 					ISO15765_N_Bs   //same time as N_Bs
#define ISO15765_N_Bs 					1000            //ms, default value from ISO15765-2, table 16
#define ISO15765_N_Cr 					1000            //ms, default value from ISO15765-2, table 16
*/
/*-----------------------------[ DEVICES ]------------------------------------*/

/*
 * CANID assignments
 */

#define CANID_SIZE                     CANID_11BIT

/* Tester Requests */
#define CANID_PHYS_REQ                  0x752uL
#define CANID_FUNC_REQ                  0x7FFuL
#define CANID_RX_PHYS_MASK				0x7FFuL
#define CANID_RX_FUNC_MASK				0x7FFuL

/* Our Responses */
#define CANID_PHYS_RSP                  0x652uL
#define CANID_FUNC_RSP                  0x652uL

/* On XPC56XX , CAN(H) is CAN_0 = CAN_A */
#if (ISO15765_CAN_DEVICE==0)
#define  USE_CAN_A			1
#elif (ISO15765_CAN_DEVICE==1)
#define  USE_CAN_B          1
#elif (ISO15765_CAN_DEVICE==2)
#define  USE_CAN_C          1
#elif (ISO15765_CAN_DEVICE==3)
#define  USE_CAN_D          1
#elif (ISO15765_CAN_DEVICE==4)
#define  USE_CAN_E          1
#elif (ISO15765_CAN_DEVICE==5)
#define  USE_CAN_F          1
#endif



#if defined(USE_CAN_A)
#undef CAN_A_CLK_SRC_CRYSTAL
#define CAN_A_NAME				"CAN_A"
#define CAN_A_BAUDRATE			500		/* kbit/s */

#if (CAN_A_BAUDRATE==500)
/* PRESDIV = (Fclk) / (500*1000 *tq ) = 80000000 / (500000*16 ) = 10 */
#define CAN_A_PROPSEG			2
#define CAN_A_PSEG1				6
#define CAN_A_PSEG2				4
#elif (CAN_A_BAUDRATE==125)
/* PRESDIV = (Fclk) / (125*1000 *tq ) = 80000000 / (125000*16 ) = 40 */
#define CAN_A_PROPSEG           2
#define CAN_A_PSEG1             6
#define CAN_A_PSEG2             4
#else
#error No prop/pseg defined for baudrate
#endif
#define CAN_A_FLAGS				(CAN_FLG_CLK_SRC_BUS) // CAN_FLG_LOOPBACK
#endif

#if defined(USE_CAN_B)
#undef CAN_B_CLK_SRC_CRYSTAL
#define CAN_B_NAME              "CAN_B"
#define CAN_B_BAUDRATE          500     /* kbit/s */

#if (CAN_B_BAUDRATE==500)
/* PRESDIV = (Fclk) / (500*1000 *tq ) = 80000000 / (500000*16 ) = 10 */
#define CAN_B_PROPSEG           2
#define CAN_B_PSEG1             6
#define CAN_B_PSEG2             4
#elif (CAN_B_BAUDRATE==125)
/* PRESDIV = (Fclk) / (125*1000 *tq ) = 80000000 / (125000*16 ) = 40 */
#define CAN_B_PROPSEG           2
#define CAN_B_PSEG1             6
#define CAN_B_PSEG2             4
#else
#error No prop/pseg defined for baudrate
#endif

#define CAN_B_FLAGS             (CAN_FLG_CLK_SRC_BUS) // CAN_FLG_LOOPBACK
#endif

#define NULL_PTR	((void *)0)

#endif /* CONFIG_H_ */
