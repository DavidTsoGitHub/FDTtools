/* -------------------------------- Arctic Core ------------------------------
 * Copyright (C) 2013, ArcCore AB, Sweden, www.arccore.com.
 * Contact: <contact@arccore.com>
 *
 * You may ONLY use this file:
 * 1)if you have a valid commercial ArcCore license and then in accordance with  
 * the terms contained in the written license agreement between you and ArcCore, 
 * or alternatively
 * 2)if you follow the terms found in GNU General Public License version 2 as 
 * published by the Free Software Foundation and appearing in the file 
 * LICENSE.GPL included in the packaging of this file or here 
 * <http://www.gnu.org/licenses/old-licenses/gpl-2.0.txt>
 *-------------------------------- Arctic Core -----------------------------*/

#include "Fls.h"
#include "flash.h"
#include <stdlib.h>


/*lint -e940 -e785 -e835 -e9027 -e845 -e651 To increase readability  */
const FlashType flashInfo[] = {

	/* NO RWW */

	/* Bank 0, Array 0 */
	{
	.sectCnt = 18,
	.bankSize = 0x200000,
	.regBase = 0xC3F88000UL,
	.sectAddr[0] = 0, 	   /* 0, blockL0, LOW  */
	.addrSpace[0] = ADDR_SPACE_SET(ADDR_SPACE_LOW) + 0u,
	.sectAddr[1] = 0x04000, /* 1, blockL1, LOW */
	.addrSpace[1] = ADDR_SPACE_SET(ADDR_SPACE_LOW) + 1u,
	.sectAddr[2] = 0x08000, /* 2, blockL2, LOW */
	.addrSpace[2] = ADDR_SPACE_SET(ADDR_SPACE_LOW) + 2u,
	.sectAddr[3] = 0x0c000, /* 3, blockL3, LOW */
	.addrSpace[3] = ADDR_SPACE_SET(ADDR_SPACE_LOW) + 3u,
	.sectAddr[4] = 0x10000, /* 4, blockL4, LOW */
	.addrSpace[4] = ADDR_SPACE_SET(ADDR_SPACE_LOW) + 4u,
	.sectAddr[5] = 0x14000, /* 5, blockL5, LOW */
	.addrSpace[5] = ADDR_SPACE_SET(ADDR_SPACE_LOW) + 5u,
	.sectAddr[6] = 0x18000, /* 6, blockL6, LOW */
	.addrSpace[6] = ADDR_SPACE_SET(ADDR_SPACE_LOW) + 6u,
	.sectAddr[7] = 0x1c000, /* 7, blockL7, LOW */
	.addrSpace[7] = ADDR_SPACE_SET(ADDR_SPACE_LOW) + 7u,
	.sectAddr[8] = 0x20000, /* 8, blockL8, LOW */
	.addrSpace[8] = ADDR_SPACE_SET(ADDR_SPACE_LOW) + 8u,
	.sectAddr[9] = 0x30000, /* 9, blockL9, LOW */
	.addrSpace[9] = ADDR_SPACE_SET(ADDR_SPACE_LOW) + 9u,
		
	.sectAddr[10] = 0x40000, /* 10, blockM0, MID */
	.addrSpace[10] = ADDR_SPACE_SET(ADDR_SPACE_MID) + 0u,
	.sectAddr[11] = 0x60000, /* 11, blockM1, MID */
	.addrSpace[11] = ADDR_SPACE_SET(ADDR_SPACE_MID) + 1u,
	
	.sectAddr[12] = 0x080000, /* 12, blockH0, HIGH */
	.addrSpace[12] = ADDR_SPACE_SET(ADDR_SPACE_HIGH) + 0u,
	.sectAddr[13] = 0x0C0000, /* 13, blockH1, HIGH */
	.addrSpace[13] = ADDR_SPACE_SET(ADDR_SPACE_HIGH) + 1u,
	.sectAddr[14] = 0x100000, /* 14, blockH2, HIGH */
	.addrSpace[14] = ADDR_SPACE_SET(ADDR_SPACE_HIGH) + 2u,
	.sectAddr[15] = 0x14000, /* 15, blockH3, HIGH */
	.addrSpace[15] = ADDR_SPACE_SET(ADDR_SPACE_HIGH) + 3u,
	.sectAddr[16] = 0x180000, /* 16, blockH4, HIGH */
	.addrSpace[16] = ADDR_SPACE_SET(ADDR_SPACE_HIGH) + 4u,
	.sectAddr[17] = 0x1C0000, /* 17, blockH5, HIGH */
	.addrSpace[17] = ADDR_SPACE_SET(ADDR_SPACE_HIGH) + 5u,
	
	.sectAddr[18] = 0x200000,  /* End, NOT a sector */
	},

};


const Fls_ConfigType FlsConfigSet[]=
{
  {
#if ( FLS_AC_LOAD_ON_JOB_START == STD_ON)
    .FlsAcWrite = __FLS_ERASE_RAM__,
    .FlsAcErase = __FLS_WRITE_RAM__,
#else
    .FlsAcWrite = NULL,
    .FlsAcErase = NULL,
#endif
    .FlsJobEndNotification = NULL,
    .FlsJobErrorNotification = NULL,

	.FlsMaxReadFastMode = 32,
	.FlsMaxReadNormalMode = 8,
	.FlsMaxWriteFastMode = 32,
	.FlsMaxWriteNormalMode = 8,
	.FlsProtection = 0,

    .FlsInfo = flashInfo,
  }
};
