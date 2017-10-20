/*-------------------------------- Arctic Core ------------------------------
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

/*
 * flash_h7f_c90.c
 *
 *  Created on: 29 aug 2011
 *      Author: mahi
 *
 * Interface for the low level flash written by freescale (flash_ll_h7f_c90.c )
 *
 * This file aims to support support all mpc55xx as well as mpc56xx.
 */


/** @tagSettings DEFAULT_ARCHITECTURE=NOT_USED */

/*lint -e923 Cast in Freescale header file */


/* ----------------------------[includes]------------------------------------*/
#include "config.h"
#include <stdint.h>
#include <string.h>
//#include <assert.h>
#include "flash_ll_h7f_c90.h"
#include "typedefs.h"
#include "io.h"
#include "MPC5645S.h"
#include "Fls_Cfg.h"

//#define ASSERT(_x)  assert(_x)

/* ----------------------------[private define]------------------------------*/

#define PFLASH_CTRL_BASE 	0xFFE8_8000
#define PFCR0				(PFLASH_CTRL_BASE + 0x1c)
#define PFCR1				(PFLASH_CTRL_BASE + 0x20)
#define PFAPR				(PFLASH_CTRL_BASE + 0x24)

#define NVLML_LME			(1uL<<31u)


//#define FLASH_BANK_CNT				3
#define PASSWORD_LOW_MID  			0xA1A11111UL
#define PASSWORD_HIGH				0xB2B22222UL
#define PASSWORD_SECONDARY_LOW_MID	0xC3C33333UL

/* ----------------------------[private macro]-------------------------------*/

/* Check if two ranges overlap (_a0->_a1 is first range ) */
#define OVERLAP(_a0,_a1, _b0, _b1 ) ( ( ((_a0) <= (_b0)) && ((_b0) <= (_a1)) ) || \
									  ( ((_b0) <= (_a0)) && ((_a0) <= (_b1)) ) )

/* ----------------------------[private typedef]-----------------------------*/
/* ----------------------------[private function prototypes]-----------------*/
/* ----------------------------[private variables]---------------------------*/

/* ----------------------------[private functions]---------------------------*/
/* ----------------------------[public functions]----------------------------*/

/**
 * Convert address to strange block format that freescale likes.
 *
 * @param addr
 * @param size
 * @param fb
 * @return
 */
static boolean getAffectedBlocks( const FlashType *bPtr, const Fls_AddressType addr, uint32 size, uint32 (*fb)[ADDR_SPACE_CNT] ) {
	uint32 addrSpace;
	boolean anyAffected = FALSE;
	uint8 i;

	if (fb == NULL) {
	    return anyAffected;
	}

	for (i=0; i<sizeof(*fb)/sizeof(uint32); i++) {
		(*fb)[i] = 0;
	}

	/* Check if sector range overlaps */
	for (uint32 sector = 0; sector < bPtr->sectCnt; sector++)
	{
	    /*lint -e{639, 638} */
		if (OVERLAP( addr, addr+size-1uL,
				     bPtr->sectAddr[sector], bPtr->sectAddr[sector+1uL]-1uL))
		{
			addrSpace = bPtr->addrSpace[sector];
			(*fb)[ADDR_SPACE_GET(addrSpace)] |= (1uL << ADDR_SPACE_GET_SECTOR(addrSpace));
			anyAffected = TRUE;
		}
	}
	return anyAffected;
}


/**
 * Setup the flash
 */

void Flash_Init(void) {

}

void Flash_Lock(const FlashType *fPtr, uint32 op, const Fls_AddressType from, uint32 size) {
	uint32 flashBlocks[ADDR_SPACE_CNT];
	uint32 bank;
	const FlashType *bPtr;
	uint32 regAddr;
	uint32 lock;

//	LDEBUG_PRINTF("Flash_lock from:%p size:%ul\n", from, size);

	for (bank = 0; bank < FLASH_BANK_CNT; bank++) {
		bPtr = &fPtr[bank];

		/*lint -e{545, 934} Ok to take address of flashBlocks */
		(void)getAffectedBlocks(bPtr, from, size, &flashBlocks);

		/* ---------- Low/Mid ---------- */
	    lock = (flashBlocks[ADDR_SPACE_MID]<<16uL) | flashBlocks[ADDR_SPACE_LOW];
	    if( lock != 0u ) {
			regAddr = bPtr->regBase + C90FL_LML;

			/* Unlock LML (enable LME bit) */
			if ( (READ32(regAddr) & NVLML_LME) == 0 ) {
				WRITE32(regAddr,PASSWORD_LOW_MID);
			}
			/* lock/unlock */
			if( (op & FLASH_OP_UNLOCK) > 0u) {
				WRITE32(regAddr,(~lock) & READ32(regAddr) );
			} else {
				WRITE32(regAddr,(lock) | READ32(regAddr) );
			}

			regAddr = bPtr->regBase + C90FL_SLL;
			/* Unlock secondary, SLL (enable LME bit) */
			if ( (READ32(regAddr) & NVLML_LME) == 0 ) {
				WRITE32(regAddr,PASSWORD_SECONDARY_LOW_MID);
			}
			/* lock/unlock */
			if( (op & FLASH_OP_UNLOCK) > 0u) {
				WRITE32(regAddr,(~lock) & READ32(regAddr) );
			} else {
				WRITE32(regAddr,(lock) | READ32(regAddr) );
			}

	    }

		/* ---------- high ----------*/
	    lock = flashBlocks[ADDR_SPACE_HIGH];
#if defined(CFG_MPC5644A)
	    // For MPC5644A both flash controller A and B will be required when writing or erasing to the
	    // high address space.
	    if( lock != 0 ) {
            /* Unlock LML (enable LME bit) */
            if ( (READ32(&FLASH_A.HLR) & NVLML_LME) == 0 ) {
                WRITE32(&FLASH_A.HLR,PASSWORD_HIGH);
            }

            if ( (READ32(&FLASH_B.HLR) & NVLML_LME) == 0 ) {
                WRITE32(&FLASH_B.HLR,PASSWORD_HIGH);
            }

            /* lock/unlock */
            if( op & FLASH_OP_UNLOCK != 0u) {
                WRITE32(&FLASH_A.HLR,(~lock) & READ32(&FLASH_A.HLR) );
                WRITE32(&FLASH_B.HLR,(~lock) & READ32(&FLASH_B.HLR) );
            } else {
                WRITE32(&FLASH_A.HLR,(lock) | READ32(&FLASH_A.HLR) );
                WRITE32(&FLASH_B.HLR,(lock) | READ32(&FLASH_B.HLR) );
            }
        }
#else
	    if( lock != 0 ) {
	    	regAddr = bPtr->regBase + C90FL_HBL;
	    	/* Unlock LML (enable LME bit) */
			if ( (READ32(regAddr) & NVLML_LME) == 0 ) {
				WRITE32(regAddr,PASSWORD_HIGH);
			}

			/* lock/unlock */
			if( (op & FLASH_OP_UNLOCK) > 0u) {
				WRITE32(regAddr,(~lock) & READ32(regAddr) );
			} else {
				WRITE32(regAddr,(lock) | READ32(regAddr) );
			}
	    }
#endif
	}
}

/**
 *
 * @param fPtr
 * @param dest
 * @param size
 * @return
 */
uint32 Flash_Erase( const FlashType *fPtr, Fls_AddressType dest, uint32 size, flashCbType cb) {
	uint32 rv;
	uint32 flashBlocks[ADDR_SPACE_CNT];
	uint32 write_add = (uint32)dest;
	const FlashType *bPtr;
	boolean affected;
	(void)cb;  /*lint !e920 Unused argument */

	/* FSL functions are for each bank, so loop over banks */
	for (uint32 bank = 0; bank < FLASH_BANK_CNT; bank++) {
		bPtr = &fPtr[bank];

		/*lint -e{545, 934} Ok to take address of flashBlocks */
		affected = getAffectedBlocks(bPtr, dest, size, &flashBlocks);
		if( !affected) {
			continue;
		}

#if defined(CFG_MPC5644A)
		//The addressed used for the interlock must be aligned to the block.
        if ((((bank == FLASH_B_HIGH_ADDR_BANK) && !(dest & FLASH_CTRL_ALIGNMENT_MASK)) ||
            ((bank == FLASH_A_HIGH_ADDR_BANK) && (dest & FLASH_CTRL_ALIGNMENT_MASK))) &&
            (FLASH_ALIGNMENT_LENGTH <= size))
        {
            write_add += (uint32)FLASH_ALIGNMENT_LENGTH;
        }
#endif

		rv = FSL_FlashEraseStart(bPtr->regBase, bPtr->sectAddr[0], flashBlocks[0],
				flashBlocks[1], flashBlocks[2]);

		if (rv != EE_OK) {
			return EE_ERROR_PE_OPT;
		}

		/* Poll status */
		while ((rv = FSL_FlashCheckStatus(bPtr->regBase)) != EE_OK) {

			if (rv == EE_ERROR_PE_OPT) {
				return rv;
			}

			if( cb != NULL ) {
				cb();
			}

		}
	}

	return EE_OK;
}


/**
 *
 * @param to     Flash address to write. Updated to next address to write for each call
 * @param from   RAM address to read from. Updated with next address to read from for each call
 * @param size   The size to program. Updated for each for each call.
 * @return
 */
uint32 Flash_ProgramPageStart( const FlashType *fPtr, uint32 *to, uint32 * from, uint32* size, flashCbType cb) {
    const FlashType *bPtr;
    uint32 bank;
    (void)cb; /*lint !e920 Unused argument */

    if (size == NULL) {
        return EE_ARC_ERROR_PARAMETER;
    }

    /* Check double word alignment */
//    ASSERT( (*size % 8uL) == 0uL );

    /* FSL functions are for each bank, so loop over banks */
    for (bank = 0; bank < FLASH_BANK_CNT; bank++) {
        bPtr = &fPtr[bank];

        if(!OVERLAP( *to,
                *to + *size - 1,
                bPtr->sectAddr[0],
                bPtr->sectAddr[0] + bPtr->bankSize-1)) {
            /* This bank was not affected */
            continue;
        }
        
        return FSL_FlashProgramStart(bPtr->regBase, to, size, from);
    }

    return EE_OK;
}

/**
 * Check status of programming/erase
 *
 * @param fPtr
 * @param to
 * @return
 */
uint32 Flash_CheckStatus( const FlashType *fPtr, const uint32 *to, uint32 size ) {
    const FlashType *bPtr;
    uint32 rv = E_NOT_OK;
    uint32 flAddr = (uint32)to;

    for (uint32 bank = 0; bank < FLASH_BANK_CNT; bank++) {
        bPtr = &fPtr[bank];

        /* We only need to figure out what bank is used, note that multiple banks
         * must be handled at the same time here */

        if(!OVERLAP(flAddr, flAddr+ size - 1uL,
                    bPtr->sectAddr[0], bPtr->sectAddr[0] + bPtr->bankSize - 1uL)) {
            /* This bank was not affected */
            continue;
        }
#if defined(CFG_MPC5644A)
        //Both flash controllers can be used at the same time on MPC5644A.
        rv = FSL_FlashCheckStatus(FLASH_A_MCR);

        if( rv != EE_OK) {
                    return rv;
        }

        rv = FSL_FlashCheckStatus(FLASH_B_MCR);
#else
        rv = FSL_FlashCheckStatus(bPtr->regBase);
#endif
        if( rv != EE_OK) {
            return rv;
        }

    }

    return rv;
}


