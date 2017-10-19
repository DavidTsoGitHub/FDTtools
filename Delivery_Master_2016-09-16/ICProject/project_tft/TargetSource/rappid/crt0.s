    #*************************************************************************/              
    #* FILE NAME: ctr0.s                                                     */           
    #* This initializes stack pointer, SDA_BASE and SDA2_BASE	register     */             
    #*                                                                       */
    #*************************************************************************/      

    .globl    __start  
    .extern   __SP_INIT 
    .extern   _SDA_BASE_
    .extern   _SDA2_BASE_  
    .extern   romcopy   
 
    .section .text_vle,text_vle   	   # The "text" generates symbols for debug

    __start:
		.include "crt0_include.inc"
# Disable watchdog
        e_lis     r6,0xfff3
        e_or2i    r6,0x8010
        e_li      r7,0xC520
        se_stw    r7,0x0(r6)
        e_li      r7,0xD928
        se_stw    r7,0x0(r6)           # Disengage soft-lock

        e_lis     r6,0xfff3
        e_or2i    r6,0x8000
        e_lis     r7,0xff00
        e_or2i    r7,0x10A
        se_stw    r7,0x0(r6)           # WEN = 0
# end of disabling watchdog

		e_lis      r1, __SP_INIT@h     # Initialize stack pointer r1 to             
		e_or2i     r1, __SP_INIT@l     # value in linker command file.              
		e_lis      r13, _SDA_BASE_@h   # Initialize r13 to sdata base             
		e_or2i     r13, _SDA_BASE_@l   #              

		e_lis      r2, _SDA2_BASE_@h   # Initialize r2 to sdata2 base              
		e_or2i     r2, _SDA2_BASE_@l   # (provided by linker).             
		e_addi     r0,r0,0             # Clear r0.             
		e_stwu     r1,-64(r1)          # Terminate stack.             

#------------------------------------------------------------             
# Start of code              

        e_bl          romcopy                                                            
        #e_b           exit      	   # Never returns.             

######################################################################
#                           End of File                        
######################################################################
