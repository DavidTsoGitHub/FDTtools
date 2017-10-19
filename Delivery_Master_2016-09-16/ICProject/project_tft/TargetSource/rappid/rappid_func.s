# rappid_func.s - INTC hardware vector mode function
# Description: This assembly routine/function initializes the IVPR. 
# It initializes the IVPR with baseaddress of Interrupt Vector table 
# for Core z0.
# IVPRInitialize() for software vector mode

  .globl        IVPRInitialize
  .section .text_vle   
  .extern IVORxHandlers

IVPRInitialize:

  e_lis       r3,  IVORxHandlers@h
  e_or2i   r3,  IVORxHandlers@l
  mtivpr   r3        # move to Interrupt vector table address to IVPR register

  se_blr
#end
