/*
 * FILE : MPC564xS_HWInit.c
 *    
 * COPYRIGHT (c) 2007-2012 Freescale Semiconductor, Inc. All rights reserved.
 *     
 * DESCRIPTION: This file contains all MPC564xS derivative basic initializations,
 *  and initialisations for the MPC564xS boards which are supported.
 *  This includes setting up the MMU, SRAM and WatchDog.
 *  
 *  For more on Qorivva MCUs module initializations please consult the Qorivva cookbook AN2865.  
 *
 * VERSION: 1.2
 */

/*--------------------------------------------------------------*/
/* Includes                                                     */
/*--------------------------------------------------------------*/
#include "MPC5645S.h"  /* platform development header */
#include "MPC5645S_HWInit.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifndef INIT_USED_BOARD
#pragma error INIT_USED_BOARD should be defined !
#endif

/*------------------------------------------------------------------*/
/* Symbol L2SRAM_LOCATION is defined in the application linker      */
/* command file (.lcf). Its value should reflect SRAM start address.*/ 
/*------------------------------------------------------------------*/
/*lint -esym(752, L2SRAM_LOCATION) */
extern unsigned long L2SRAM_LOCATION;  

/*------------------------------------------------------------------*/
/* Symbol L2SRAM_CNT is defined in the application linker command   */
/* file (.lcf). It represents the how many writes with stmw         */
/* (128 bytes each) are needed to cover the whole L2SRAM.           */
/*------------------------------------------------------------------*/
extern unsigned long L2SRAM_CNT;

/*----------------------------------------------------------------*/
/* Function implementations                                       */
/*----------------------------------------------------------------*/

MAKE_HLI_COMPATIBLE(SR_WSC_1, 0xc520) /*50464*/
MAKE_HLI_COMPATIBLE(SR_WSC_2, 0xd928) /*55592*/
MAKE_HLI_COMPATIBLE(MCR_VALUE, 0x8000010A)
MAKE_HLI_COMPATIBLE(SWT_SR, &SWT.SR.R) /* Address of the SWT SR */
MAKE_HLI_COMPATIBLE(SWT_MCR, &SWT.CR.R) /* Address of the SWT MCR */

/*------------------------------------------------------------------*/
/* FUNCTION     : INIT_Derivative                                   */
/* PURPOSE      : This function initializes the derivative.         */
/* SEQUENCE:                                                        */
/* - initializes the MMU                                            */
/* - disables system WDT and Core WDT                               */
/* - initializes the internal SRAM for ROM targets                  */
/*------------------------------------------------------------------*/
__asm void INIT_Derivative(void)
{
nofralloc

    /* Configure the MMU in order to access the MCU memory and peripherals
    because when the MCU came out of reset there is only one entry configured
    with a 4k window - where this code is running.*/

    /* Don't have a stack yet, save the return address in a register */
    mflr     r26
    bl __initMMU
    mtlr r26

    /* Clear the soft lock bit SWT_CR.SLKSWT_CR: */
    /* SR --> 0x0000c520 */
    lis r4, 0
    ori r4, r4, SR_WSC_1@l
    lis r3, SWT_SR@ha
    stw r4, SWT_SR@l(r3)
    /* SR --> 0x0000d928 */
    lis r4, 0
    ori r4, r4, SR_WSC_2@l
    stw r4, SWT_SR@l(r3)

    /* Disable watchdog, SWT.CR.WEN = 0*/
    lis r4, MCR_VALUE@h
    ori r4, r4, MCR_VALUE@l
    lis r3, SWT_MCR@ha
    stw r4, SWT_MCR@l(r3)

#if defined(ROM_VERSION) && (ROM_VERSION == 1)
    /* enable SPE if the compiler isn't called with -spe_vector */

    /* SRAM initialization code*/
    lis r11,L2SRAM_LOCATION@h
    ori r11,r11,L2SRAM_LOCATION@l

    /* Loops to cover L2SRAM, stmw allows 128 bytes (32 GPRS x 4 bytes) writes */
    lis r12,L2SRAM_CNT@h
    ori r12,r12,L2SRAM_CNT@l
    mtctr r12

    init_l2sram_loop:
        stmw r0, 0(r11)        /* Write 32 GPRs to SRAM*/
        addi r11,r11,128       /* Inc the ram ptr; 32 GPRs * 4 bytes = 128B */
        bdnz init_l2sram_loop  /* Loop for L2SRAM_CNT times */
#endif

    blr
}

/*----------------------------------------------------------------------*/
/* FUNCTION:WriteMMUTableEntryS                                         */
/* PURPOSE: Creates a new TLB entry with synchronization.It ensures that*/
/*          the TLB context change doesn't affect the core by execution */
/*          before and after tlbwe a CSI. This should be called when    */
/*          creating SRAM or FLASH TLB entries.                         */
/* SEQUENCE: write GPR to MAS, execute CSI, tlbwe, CSI                  */
/*----------------------------------------------------------------------*/
__asm void WriteMMUTableEntryS( void )
{ 
nofralloc
    /* Write MMU Assist Register 0 (MAS0); SPR 624 */
    mtspr   624, r3
    /* Write MMU Assist Register 1 (MAS1); SPR 625 */
    mtspr   625, r4
    /* Write MMU Assist Register 2 (MAS2); SPR 626 */
    mtspr   626, r5
    /* Write MMU Assist Register 3 (MAS3); SPR 627 */
    mtspr   627, r6
    /* Write the table entry */

    /* All instruction will complete here in current context. */
    msync
    tlbwe
    /* synchronize instruction fetches and data accesses in respect
     with the new created TLB entry. */
    msync
    isync
    blr
}

/*--------------------------------------------------*/
/* FUNCTION:WriteMMUTableEntry                      */
/* PURPOSE: Creates a new TLB entry with            */
/* SEQUENCE: write GPR to MAS, execute tlbwe        */
/*--------------------------------------------------*/
__asm void WriteMMUTableEntry( void )
{ 
nofralloc
    /* Write MMU Assist Register 0 (MAS0); SPR 624 */
    mtspr   624, r3
    /* Write MMU Assist Register 1 (MAS1); SPR 625 */
    mtspr   625, r4
    /* Write MMU Assist Register 2 (MAS2); SPR 626 */
    mtspr   626, r5
    /* Write MMU Assist Register 3 (MAS3); SPR 627 */
    mtspr   627, r6
    /* Write the table entry */
    tlbwe
    blr
}

/*
   Device MMU table:
   ---------------------------------------------------------
   Name:            TLB entry  Start            Length
   ---------------------------------------------------------
   FLASH            0         0x0000_0000       2MB
   SRAM             1         0x4000_0000       64KB
   SHADOW           2         0x00FF_C000       16KB 
   G_SRAM           3         0x6000_0000       1MB
   QuadSPI          4         0x8000_0000       256MB
   PBRIDGE1         5         0xC3F8_8000       512KB
   PBRIDGE0 + BAM   6         0xFFE0_0000       2MB
   ---------------------------------------------------------
 */

/* Run MMU init:TLB 15, 0x0000_0000, 4GB, TS=1, not guarded, big endian, cache inhibited, all access, VLE/BOOKE */
MAKE_HLI_COMPATIBLE(TLB_Entry_15_MAS0, MAS0_VALUE(15))
MAKE_HLI_COMPATIBLE(TLB_Entry_15_MAS1, MAS1_VALUE(V_VALID, IPROT_NOTPROTECTED, TID_GLOBAL, TS_ON, TSIZE_4GB))
MAKE_HLI_COMPATIBLE(TLB_Entry_15_MAS1_INVALID, MAS1_VALUE(V_INVALID, IPROT_NOTPROTECTED, TID_GLOBAL, TS_ON, TSIZE_4GB))
#if __option(vle)
MAKE_HLI_COMPATIBLE(TLB_Entry_15_MAS2, MAS2_VALUE(0, VLE_MODE, WRITE_BACK, CACHE_INHIBIT, MEM_COHERENCE_NREQ, NOT_GUARDED, BIG_ENDIAN))
#else
MAKE_HLI_COMPATIBLE(TLB_Entry_15_MAS2, MAS2_VALUE(0, BOOK_E_MODE, WRITE_BACK, CACHE_INHIBIT, MEM_COHERENCE_NREQ, NOT_GUARDED, BIG_ENDIAN))
#endif
MAKE_HLI_COMPATIBLE(TLB_Entry_15_MAS3, MAS3_VALUE(0, READ_WRITE_EXECUTE))

/* FLASH: TLB 0, 0x0000_0000, 2MB, not guarded, protected, cachable, all access */
MAKE_HLI_COMPATIBLE(TLB_Entry_0_MAS0, MAS0_VALUE(0))
MAKE_HLI_COMPATIBLE(TLB_Entry_0_MAS1, MAS1_VALUE(V_VALID, IPROT_PROTECTED, TID_GLOBAL, TS_OFF, TSIZE_2MB))
#if __option(vle)
MAKE_HLI_COMPATIBLE(TLB_Entry_0_MAS2, MAS2_VALUE(0x0, VLE_MODE, WRITE_BACK, CACHEABLE, MEM_COHERENCE_NREQ, NOT_GUARDED, BIG_ENDIAN))
#else
MAKE_HLI_COMPATIBLE(TLB_Entry_0_MAS2, MAS2_VALUE(0x0, BOOK_E_MODE, WRITE_BACK, CACHEABLE, MEM_COHERENCE_NREQ, NOT_GUARDED, BIG_ENDIAN))
#endif
MAKE_HLI_COMPATIBLE(TLB_Entry_0_MAS3, MAS3_VALUE(0x0, READ_WRITE_EXECUTE))

/* SRAM: TLB 1, 0x4000_0000, 64KB, not protected, not guarded, cache off, big-endian, all access */
MAKE_HLI_COMPATIBLE(TLB_Entry_1_MAS0, MAS0_VALUE(1))
MAKE_HLI_COMPATIBLE(TLB_Entry_1_MAS1, MAS1_VALUE(V_VALID, IPROT_NOTPROTECTED, TID_GLOBAL, TS_OFF, TSIZE_64KB))
#if __option(vle)
MAKE_HLI_COMPATIBLE(TLB_Entry_1_MAS2, MAS2_VALUE(0x40000000, VLE_MODE, WRITE_BACK, CACHE_INHIBIT, MEM_COHERENCE_NREQ, NOT_GUARDED, BIG_ENDIAN))
#else
MAKE_HLI_COMPATIBLE(TLB_Entry_1_MAS2, MAS2_VALUE(0x40000000, BOOK_E_MODE, WRITE_BACK, CACHE_INHIBIT, MEM_COHERENCE_NREQ, NOT_GUARDED, BIG_ENDIAN))
#endif
MAKE_HLI_COMPATIBLE(TLB_Entry_1_MAS3, MAS3_VALUE(0x40000000, READ_WRITE_EXECUTE))

/* SHADOW FLASH: TLB 2, 0x00FF_C000, 16KB, not guarded, protected, cachable, all access */
MAKE_HLI_COMPATIBLE(TLB_Entry_2_MAS0, MAS0_VALUE(2))
MAKE_HLI_COMPATIBLE(TLB_Entry_2_MAS1, MAS1_VALUE(V_VALID, IPROT_PROTECTED, TID_GLOBAL, TS_OFF, TSIZE_16KB))
MAKE_HLI_COMPATIBLE(TLB_Entry_2_MAS2, MAS2_VALUE(0x00FFC000, BOOK_E_MODE, WRITE_BACK, CACHEABLE, MEM_COHERENCE_NREQ, NOT_GUARDED, BIG_ENDIAN))
MAKE_HLI_COMPATIBLE(TLB_Entry_2_MAS3, MAS3_VALUE(0x00FFC000, READ_WRITE))

/* G_SRAM: TLB 3, 0x6000_0000, 1MB, not protected, not guarded, cache off, big-endian, all access */
MAKE_HLI_COMPATIBLE(TLB_Entry_3_MAS0, MAS0_VALUE(3))
MAKE_HLI_COMPATIBLE(TLB_Entry_3_MAS1, MAS1_VALUE(V_VALID, IPROT_NOTPROTECTED, TID_GLOBAL, TS_OFF, TSIZE_1MB))
MAKE_HLI_COMPATIBLE(TLB_Entry_3_MAS2, MAS2_VALUE(0x60000000, BOOK_E_MODE, WRITE_BACK, CACHE_INHIBIT, MEM_COHERENCE_NREQ, NOT_GUARDED, BIG_ENDIAN))
MAKE_HLI_COMPATIBLE(TLB_Entry_3_MAS3, MAS3_VALUE(0x60000000, READ_WRITE_EXECUTE))

/* QuadSPI: TLB 4, 0x8000_0000, 256MB, not protected, not guarded, cache off, big-endian, all access */
MAKE_HLI_COMPATIBLE(TLB_Entry_4_MAS0, MAS0_VALUE(4))
MAKE_HLI_COMPATIBLE(TLB_Entry_4_MAS1, MAS1_VALUE(V_VALID, IPROT_NOTPROTECTED, TID_GLOBAL, TS_OFF, TSIZE_256MB))
MAKE_HLI_COMPATIBLE(TLB_Entry_4_MAS2, MAS2_VALUE(0x80000000, BOOK_E_MODE, WRITE_BACK, CACHE_INHIBIT, MEM_COHERENCE_NREQ, NOT_GUARDED, BIG_ENDIAN))
MAKE_HLI_COMPATIBLE(TLB_Entry_4_MAS3, MAS3_VALUE(0x80000000, READ_WRITE_EXECUTE))

/* PBRIDGE1: TLB 5, 0xC3F8_8000, 512KB, not protected, not guarded, cache off, big-endian, all access */
MAKE_HLI_COMPATIBLE(TLB_Entry_5_MAS0, MAS0_VALUE(5))
MAKE_HLI_COMPATIBLE(TLB_Entry_5_MAS1, MAS1_VALUE(V_VALID, IPROT_NOTPROTECTED, TID_GLOBAL, TS_OFF, TSIZE_512KB))
MAKE_HLI_COMPATIBLE(TLB_Entry_5_MAS2, MAS2_VALUE(0xC3F88000, BOOK_E_MODE, WRITE_BACK, CACHE_INHIBIT, MEM_COHERENCE_NREQ, NOT_GUARDED, BIG_ENDIAN))
MAKE_HLI_COMPATIBLE(TLB_Entry_5_MAS3, MAS3_VALUE(0xC3F88000, READ_WRITE))

/* PBRIDGE0 + BAM: TLB 6, 0xFFE0_0000, 2MB, not protected, guarded, cache off, big-endian, all access, VLE */
MAKE_HLI_COMPATIBLE(TLB_Entry_6_MAS0, MAS0_VALUE(6))
MAKE_HLI_COMPATIBLE(TLB_Entry_6_MAS1, MAS1_VALUE(V_VALID, IPROT_NOTPROTECTED, TID_GLOBAL, TS_OFF, TSIZE_2MB))
MAKE_HLI_COMPATIBLE(TLB_Entry_6_MAS2, MAS2_VALUE(0xFFE00000, VLE_MODE, WRITE_BACK, CACHE_INHIBIT, MEM_COHERENCE_NREQ, GUARDED, BIG_ENDIAN))
MAKE_HLI_COMPATIBLE(TLB_Entry_6_MAS3, MAS3_VALUE(0xFFE00000, READ_WRITE_EXECUTE))

__asm void __initMMU(void)
{
nofralloc
    /* Use a translation address space page in order to run the MMU initialization
        regardless of the current executing env. and MMU setup. */
    lis r3, TLB_Entry_15_MAS0@h
    ori r3, r3, TLB_Entry_15_MAS0@l
    mtspr 624, r3
    lis r4, TLB_Entry_15_MAS1@h
    ori r4, r4, TLB_Entry_15_MAS1@l
    mtspr 625, r4
    xor r5, r5, r5
    mr r6, r5
    ori r5, r5, TLB_Entry_15_MAS2@l
    ori r6, r6, TLB_Entry_15_MAS3@l
    mtspr 626, r5
    mtspr 627, r6
    tlbwe

    /* force this TLB entry to be used for translation */
    mfmsr r10
    /* save state */
    mr r3, r10
    /* set IS=1, DS=1 */
    ori r3, r3, 0x20

    msync
    /* mtmsr does execution synchronization.*/
    mtmsr r3
    /* Required after changing MSR.IS, and MSR.DS so the prefetched instructions
       will be discarded and all subsequent instructions will use the TLB 15 context.*/
    isync
    msync

     /* FLASH,TLB0,0x0000_0000,2MB */
     lis r3, TLB_Entry_0_MAS0@h
     ori r3, r3, TLB_Entry_0_MAS0@l
     lis r4, TLB_Entry_0_MAS1@h
     ori r4, r4, TLB_Entry_0_MAS1@l
     lis r5, TLB_Entry_0_MAS2@h
     ori r5, r5, TLB_Entry_0_MAS2@l
     lis r6, TLB_Entry_0_MAS3@h
     ori r6, r6, TLB_Entry_0_MAS3@l
     /* Synchronize in case running from FLASH */
     mflr r28
     bl WriteMMUTableEntryS
     mtlr r28

     /* SRAM,TLB1,0x4000_0000,64KB */
     lis r3, TLB_Entry_1_MAS0@h
     ori r3, r3, TLB_Entry_1_MAS0@l
     lis r4, TLB_Entry_1_MAS1@h
     ori r4, r4, TLB_Entry_1_MAS1@l
     lis r5, TLB_Entry_1_MAS2@h
     ori r5, r5, TLB_Entry_1_MAS2@l
     lis r6, TLB_Entry_1_MAS3@h
     ori r6, r6, TLB_Entry_1_MAS3@l
     /* Synchronize in case running from SRAM */
     mflr r28
     bl WriteMMUTableEntryS
     mtlr r28

     /* SHADOW,TLB2,0x00FF_C000,16KB */
     lis r3, TLB_Entry_2_MAS0@h
     ori r3, r3, TLB_Entry_2_MAS0@l
     lis r4, TLB_Entry_2_MAS1@h
     ori r4, r4, TLB_Entry_2_MAS1@l
     lis r5, TLB_Entry_2_MAS2@h
     ori r5, r5, TLB_Entry_2_MAS2@l
     lis r6, TLB_Entry_2_MAS3@h
     ori r6, r6, TLB_Entry_2_MAS3@l
     mflr r28
     bl WriteMMUTableEntry
     mtlr r28

     /* G_SRAM,TLB3,0x6000_0000,1MB */
     lis r3, TLB_Entry_3_MAS0@h
     ori r3, r3, TLB_Entry_3_MAS0@l
     lis r4, TLB_Entry_3_MAS1@h
     ori r4, r4, TLB_Entry_3_MAS1@l
     lis r5, TLB_Entry_3_MAS2@h
     ori r5, r5, TLB_Entry_3_MAS2@l
     lis r6, TLB_Entry_3_MAS3@h
     ori r6, r6, TLB_Entry_3_MAS3@l
     mflr r28
     bl WriteMMUTableEntry
     mtlr r28

     /* QuadSPI,TLB4,0x8000_0000,256MB*/
     lis r3, TLB_Entry_4_MAS0@h
     ori r3, r3, TLB_Entry_4_MAS0@l
     lis r4, TLB_Entry_4_MAS1@h
     ori r4, r4, TLB_Entry_4_MAS1@l
     lis r5, TLB_Entry_4_MAS2@h
     ori r5, r5, TLB_Entry_4_MAS2@l
     lis r6, TLB_Entry_4_MAS3@h
     ori r6, r6, TLB_Entry_4_MAS3@l
     mflr r28
     bl WriteMMUTableEntry
     mtlr r28

     /* PBRIDGE1,TLB5,0xC3F8_8000,512KB */
     lis r3, TLB_Entry_5_MAS0@h
     ori r3, r3, TLB_Entry_5_MAS0@l
     lis r4, TLB_Entry_5_MAS1@h
     ori r4, r4, TLB_Entry_5_MAS1@l
     lis r5, TLB_Entry_5_MAS2@h
     ori r5, r5, TLB_Entry_5_MAS2@l
     lis r6, TLB_Entry_5_MAS3@h
     ori r6, r6, TLB_Entry_5_MAS3@l
     mflr r28
     bl WriteMMUTableEntry
     mtlr r28

     /* PBRIDGE0 + BAM,TLB6,0xFFE0_0000,2MB */
     lis r3, TLB_Entry_6_MAS0@h
     ori r3, r3, TLB_Entry_6_MAS0@l
     lis r4, TLB_Entry_6_MAS1@h
     ori r4, r4, TLB_Entry_6_MAS1@l
     lis r5, TLB_Entry_6_MAS2@h
     ori r5, r5, TLB_Entry_6_MAS2@l
     lis r6, TLB_Entry_6_MAS3@h
     ori r6, r6, TLB_Entry_6_MAS3@l
     mflr r28
     bl WriteMMUTableEntry
     mtlr r28

     /* restore msr */
     mtmsr r10
     /* dicard instruction & data prefetch */
     msync

     /* invalidated initialization TLB entry 15 */
     lis r3, TLB_Entry_15_MAS0@h
     ori r3, r3, TLB_Entry_15_MAS0@l
     lis r4, TLB_Entry_15_MAS1_INVALID@h
     ori r4, r4, TLB_Entry_15_MAS1_INVALID@l
     mtspr 624, r3
     mtspr 625, r4
     tlbwe
     /* make sure isntructions and data are fetched from the new context. */
     isync
     msync

     blr
}

/*--------------------------------------------------------------------------*/
/* SIU External Bus Interface                                               */
/*--------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------*/
/* FUNCTION     : INIT_ExternalBusAndMemory                                 */
/* PURPOSE      : This function configures EBI for the external memory      */
/*--------------------------------------------------------------------------*/
__asm void INIT_ExternalBusAndMemory(void) 
{
nofralloc
    /* TODO */
    blr
}

#ifdef __cplusplus
}
#endif
