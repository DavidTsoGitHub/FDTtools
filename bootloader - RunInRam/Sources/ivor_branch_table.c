/*
 * FILE: ivor_branch_table.c
 *   
 * COPYRIGHT (c) 2007-2012 Freescale Semiconductor, Inc. All rights reserved.
 *
 * DESCRIPTION: IVOR branch table interrupts. 
 *
 *  Rev 1.0 Jul  6 2007 S.M. - Initial version 
 *  Rev 1.1 May 13 2008 D.F. - Adapted 551x version for e200z0h processors
 *  Rev 1.2 Sept 2012 H.C.   - add IVOR comments 
 *  
 * VERSION: 1.1 
*/

#ifdef __cplusplus
extern "C" {
#endif

#pragma push
#define SIXTEEN_BYTES 16

/* IVOR4 will call this handler */
extern void INTC_INTCInterruptHandler(void);

/* Set interrupt handlers */
#pragma section code_type ".ivor_branch_table"
#pragma force_active on
#pragma function_align 16
#pragma require_prototypes off

asm void ivor_branch_table(void) {
  nofralloc
 
				.align SIXTEEN_BYTES       
IVOR0trap:  b   IVOR0trap	 // Critical Input interrupt handler

				.align SIXTEEN_BYTES       
IVOR1trap:  b	IVOR1trap	 // Machine check / (NMI) interrupt handler
                             // ISI, ITLB Error on first instruction fetch for an exception handler
                             // Parity Error signaled on cache access
                             // External bus error
				.align SIXTEEN_BYTES       
IVOR2trap:  b	IVOR2trap	 // Data Storage interrupt handler
                             //  Access control.
                             // Byte ordering due to misaligned access across page boundary to pages with mismatched E bits
                             // Cache locking exception
				.align SIXTEEN_BYTES       
IVOR3trap:  b	IVOR3trap    // Instruction Storage interrupt handler
                             // Access control.
                             // Byte ordering due to misaligned instruction across page boundary to
                             //   pages with mismatched VLE bits, or access to page with VLE set, and E
                             //   indicating little-endian.
                             // Misaligned Instruction fetch due to a change of flow to an odd half-word
                             //   instruction boundary on a Power ISA (non-VLE) instruction page
				.align SIXTEEN_BYTES       
IVOR4trap:  b   INTC_INTCInterruptHandler // External Interrupt interrupt handler

				.align SIXTEEN_BYTES       
IVOR5trap:  b	IVOR5trap    //Alignment interrupt handler
                             // lmw, stmw not word aligned
                             // lwarx or stwcx. not word aligned, lharx or sthcx. not half-word aligned
                             // dcbz with disabled cache, or to W or I storage
                             // SPE ld and st instructions not properly aligned
				.align SIXTEEN_BYTES       
IVOR6trap:  b	IVOR6trap	 // Program interrupt handler
                             // Illegal, Privileged, Trap, FP enabled, AP enabled, Unimplemented Operation.
                             
				.align SIXTEEN_BYTES       
IVOR7trap:  b	IVOR7trap	 // Floating-point unavailable interrupt handler
                             // MSRFP = 0 and attempt to execute a Book E floating point operation
				.align SIXTEEN_BYTES       
IVOR8trap:  b	IVOR8trap	 // System call interrupt handler, Execution of the System Call (sc, se_sc) instruction

				.align SIXTEEN_BYTES       
IVOR9trap:  b	IVOR9trap	 // AP unavailable interrupt handler

				.align SIXTEEN_BYTES       
IVOR10trap: b   IVOR10trap   // Decrementer interrupt handler

				.align SIXTEEN_BYTES       
IVOR11trap:  b	IVOR11trap	 // Fixed Interval Timer interrupt handler

				.align SIXTEEN_BYTES       
IVOR12trap:  b	IVOR12trap	 // Watchdog Timer interrupt handler

				.align SIXTEEN_BYTES       
IVOR13trap:  b	IVOR13trap	 // Data TLB Error interrupt handler
                             // Data translation lookup did not match a valid entry in the TLB

				.align SIXTEEN_BYTES       
IVOR14trap:  b	IVOR14trap	 // Instruction translation lookup did not match a valid entry in the TLB

				.align SIXTEEN_BYTES       
IVOR15trap:  b  IVOR15trap   // Trap, instruction address compare, data address compare, instruction
                             // complete, branch taken, return from interrupt, interrupt taken, debug
                             //counter, external debug event, unconditional debug event
				.align SIXTEEN_BYTES
IVOR32trap:  b    IVOR32trap // SPE Unavailable Exception interrupt handler

				.align SIXTEEN_BYTES
IVOR33trap:  b	IVOR33trap	 // EFP Data Exception interrupt handler

                .align SIXTEEN_BYTES       
IVOR34trap:  b	IVOR34trap	 // EFP Round Exception interrupt handler
}

#pragma pop

#ifdef __cplusplus
}
#endif
