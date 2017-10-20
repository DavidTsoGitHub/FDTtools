/**
 * FILE : MPC564xS_HWInit.h
 * 
 * COPYRIGHT (c) 2007-2012 Freescale Semiconductor, Inc. All rights reserved.
 *
 * DESCRIPTION: This file contains defines and declaration of utility functions needed
 * to initialize several modules: core MMU and Flash.  
 *  
 *  For more on Qorivva MCUs module initializations please consult the Qorivva cookbook AN2865.
 *  
 * VERSION: 1.2
 */

#ifndef _MPC5645S_HWINIT_H_
#define _MPC5645S_HWINIT_H_

#ifdef __cplusplus
extern "C" {
#endif

/* This macro allows to use C defined address with the inline assembler */
#define MAKE_HLI_COMPATIBLE(hli_name, c_expr) enum { hli_name=/*lint -e30*/((int)(c_expr)) };

/* Supported Boards */
#define NO_EXTERNAL_MEMORY      0

/* Used board */
#define INIT_USED_BOARD NO_EXTERNAL_MEMORY

/*------------------------------------------------------*/
/* MCU derivative specific hardware initialization      */
/*------------------------------------------------------*/
__declspec(section ".init") __asm void INIT_Derivative(void); 

/*-------------------------------------------------------*/
/* Initialize the needed MMU Table entries.              */
/*-------------------------------------------------------*/
__declspec(section ".init") __asm void __initMMU(void);

/* Write one MMU Table Entry */
__declspec(section ".init") __asm void WriteMMUTableEntry( void );

/* Write one MMU Table Entry with context synchronization */
__declspec(section ".init") __asm void WriteMMUTableEntryS( void );

/*-------------------------------------------------------*/
/*  Call this function to setup the external bus         */ 
/*  chip selects and MMU entries for the selected board. */
/*-------------------------------------------------------*/
__asm void INIT_ExternalBusAndMemory( void );

/*------------------------------------------------------*/
/* MMU Table Entries Defines                            */
/*------------------------------------------------------*/

/*-----------------------------------------------------------------------*/
/* Generate MMU Assist 0 value from the parameters provided.             */
/* In accordance with the PowerPC Zen core specification the TLBSEL value*/ 
/* is always set to 01b to maintain future compatibility.                */
/*-----------------------------------------------------------------------*/
#define MAS0_VALUE(eselcam) ((unsigned long)(0x10000000 | (eselcam << 16)))

/*--------------------------------------------------------------------------*/
/* Generate MMU Assist 1 value from the parameters provided                 */
/* parameter valid:   1 if the MMU entry is valid, otherwise \c 0 (invalid).*/
/* parameter iprot:   Invalidation protection value                         */
/* parameter tid:     the translation ID                                    */
/* parameter ts:      the translation space value                           */
/* parameter tsize:   the translation size                                  */
/*--------------------------------------------------------------------------*/
#define MAS1_VALUE(valid, iprot, tid, ts, tsize) \
        ((unsigned long)((valid << 31) | (iprot << 30) | (tid << 16) | (ts << 12) | (tsize << 7)))


/** Translation address space. This bit is compared with the IS or DS fields
    of the MSR (depending on the type of access) to determine if this TLB
    entry may be used for translation.
*/
#define TS_ON  1
#define TS_OFF 0

/** V TLB valid bit */
#define V_INVALID 0
#define V_VALID   1

/** IPROT TLB Invalidate protect bit */
#define IPROT_NOTPROTECTED 0
#define IPROT_PROTECTED    1

/** Translation ID defines the TID as global and matches all process IDs */
#define TID_GLOBAL          0

/** Translation size */
#define TSIZE_1KB           0
#define TSIZE_2KB           1
#define TSIZE_4KB           2
#define TSIZE_8KB           3
#define TSIZE_16KB          4
#define TSIZE_32KB          5
#define TSIZE_64KB          6
#define TSIZE_128KB         7
#define TSIZE_256KB         8
#define TSIZE_512KB         9
#define TSIZE_1MB           10
#define TSIZE_2MB           11
#define TSIZE_4MB           12
#define TSIZE_8MB           13
#define TSIZE_16MB          14
#define TSIZE_32MB          15
#define TSIZE_64MB          16
#define TSIZE_128MB         17
#define TSIZE_256MB         18
#define TSIZE_512MB         19
#define TSIZE_1GB           20
#define TSIZE_2GB           21
#define TSIZE_4GB           22

/*----------------------------------------------------------------------*/
/* Generate MMU Assist 2 value from the parameters provided             */
/*                                                                      */
/* Effective Page Number (start address of logical memory region)       */
/* must be computed directly in the assembly code.                      */
/*                                                                      */
/* parameter   epn: effective page number                               */
/* parameter   vle: VLE flag                                            */
/* parameter   w:   Write-through Required                              */
/* parameter   i:   Cache Inhibited                                     */
/* parameter   m:   Memory Coherence Required                           */
/* parameter   g:   Guarded                                             */
/* parameter   e:   Endianness                                          */
/*----------------------------------------------------------------------*/
#define MAS2_VALUE(epn, vle, w, i, m, g, e) \
  ((unsigned long)(epn | (vle << 5) | (w << 4) | (i << 3) | (m << 2) | (g << 1) | (e)))

/** MAS2[VLE]: Book E mode */
#define BOOK_E_MODE         0
/** MAS2[VLE]: VLE mode */
#define VLE_MODE            1

/** MAS2[W]: Update data in the cache only */
#define WRITE_BACK          0
/** MAS2[W]: All stores performed are written through to memory */
#define WRITE_THROUGH       1

/** MAS2[I]: The page is considered cacheable */
#define CACHEABLE           0
/** MAS2[I]: The page is cache-inhibited */
#define CACHE_INHIBIT       1

/** MAS2[M]: Memory Coherence is not-required */
#define MEM_COHERENCE_NREQ  0
/** MAS2[M]: Memory Coherence is required */
#define MEM_COHENRECE_REQ   1

/** MAS2[G]: Access to page is not guarded */
#define NOT_GUARDED         0
/** MAS2[G]: All loads and stores are performed without speculation */
#define GUARDED             1

/** MAS2[E]: Page is accessed in big-endian order */
#define BIG_ENDIAN          0
/** MAS2[E]: Page is accessed in little-endian order */
#define LITTLE_ENDIAN       1

/*--------------------------------------------------------------*/
/* Generate MMU Assist 3 flags from the parameters provided     */
/* Real Page Number (start address of physical memory region)   */
/* must be computed directly in the assembly code               */
/* parameter rpn: real page number                              */
/* parameter permissions:  Permission bits                      */
/*--------------------------------------------------------------*/
#define MAS3_VALUE(rpn, permissions) ((unsigned long)(rpn | permissions))

/** MAS3[U/S{XWR}]: Read. Write and Execute permission */
#define READ_WRITE_EXECUTE  0x3f
/** MAS3[U/S{XWR}]: Read and Execute permission */
#define READ_EXECUTE        0x33

/** MAS3[U/S{XWR}]: Read and Write permission */
#define READ_WRITE          0xF

/** MAS3[U/S{XWR}]: Read permission */
#define READ                0x3

#ifdef __cplusplus
}
#endif

#endif
