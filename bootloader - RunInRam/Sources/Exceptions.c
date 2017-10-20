/** 
 * FILE: Exceptions.c
 *
 *  DESCRIPTION: Setup of IVPR to point to the EXCEPTION_HANDLERS memory area 
 *               defined in the linker command file.
 *               Default setup of the IVORxx registers. 
 *
 * VERSION: 1.1
*/


/*----------------------------------------------------------------------------*/
/* Includes                                                                   */
/*----------------------------------------------------------------------------*/

#include "Exceptions.h" /* Implement functions from this file */

/*----------------------------------------------------------------------------*/
/* Function Implementations                                                   */
/*----------------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" {
#endif

#pragma push /* Save the current state */
/* Symbol EXCEPTION_HANDLERS is defined in the application linker command file (.lcf) 
   It is defined to the start of the code memory area used for the .__exception_handlers section. 
*/
/*lint -esym(752, EXCEPTION_HANDLERS) */
__declspec (section ".__exception_handlers") extern long EXCEPTION_HANDLERS;  
#pragma force_active on
#pragma function_align 16 /* We use 16 bytes alignment for Exception handlers */
__declspec(interrupt)
__declspec (section ".__exception_handlers")
void EXCEP_DefaultExceptionHandler(void)
{

}
#pragma force_active off
#pragma pop

__asm void EXCEP_InitExceptionHandlers(void)
{
nofralloc
    // Set the IVPR to the exception handlers address defined in the lcf file.
    // The IVPR[0-15] bits Vector Base and IVPR[16-31] bits are ignored.
    lis     r0, EXCEPTION_HANDLERS@h
    mtivpr  r0

    li		r0, EXCEPTION_HANDLERS@l
    mtivor0 r0
    
    li		r0, (EXCEPTION_HANDLERS + 0x10)@l
    mtivor1 r0
    
    li		r0, (EXCEPTION_HANDLERS + 0x20)@l
    mtivor2 r0
       	
    li		r0, (EXCEPTION_HANDLERS + 0x30)@l
    mtivor3 r0
       	
    li		r0, (EXCEPTION_HANDLERS + 0x40)@l
    mtivor4 r0
     	
    li		r0, (EXCEPTION_HANDLERS + 0x50)@l
    mtivor5 r0
    
    li		r0, (EXCEPTION_HANDLERS + 0x60)@l
    mtivor6 r0
    
    li		r0, (EXCEPTION_HANDLERS + 0x70)@l
    mtivor7 r0
    
    li		r0, (EXCEPTION_HANDLERS + 0x80)@l
    mtivor8 r0
    
    li		r0, (EXCEPTION_HANDLERS + 0x90)@l
    mtivor9 r0
    
    li		r0, (EXCEPTION_HANDLERS + 0xA0)@l
    mtivor10 r0
    
    li		r0, (EXCEPTION_HANDLERS + 0xB0)@l
    mtivor11 r0
    
    li		r0, (EXCEPTION_HANDLERS + 0xC0)@l
    mtivor12 r0
    
    li		r0, (EXCEPTION_HANDLERS + 0xD0)@l
    mtivor13 r0
    
    li		r0, (EXCEPTION_HANDLERS + 0xE0)@l
    mtivor14 r0
    
    li		r0, (EXCEPTION_HANDLERS + 0xF0)@l
    mtivor15 r0

    blr
}

#ifdef __cplusplus
}
#endif

