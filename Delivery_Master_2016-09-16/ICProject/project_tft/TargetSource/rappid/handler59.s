# handler.s - INTC hardware vector mode example
# Description:  Creates prolog, epilog for C ISR and enables nested interrupts
# Compiled with CodeWarrior 2.3, Diab 5.5.1, Green Hills 5.0.3
# Assumes IVPR contains address of HWvecTable
# STACK FRAME DESIGN: Depth: 20 words (0xA0, or 80 bytes)
#            ************* ______________
#   0x4C     *  GPR12    *    ^
#   0x48     *  GPR11    *    |
#   0x44     *  GPR10    *    |
#   0x40     *  GPR9     *    |
#   0x3C     *  GPR8     *    |
#   0x38     *  GPR7     *  GPRs (32 bit)
#   0x34     *  GPR6     *    |
#   0x30     *  GPR5     *    |
#   0x2C     *  GPR4     *    |
#   0x28     *  GPR3     *    |
#   0x24     *  GPR0     * ___v__________
#   0x20     *  CR       * __CR__________
#   0x1C     *  XER      *    ^
#   0x18     *  CTR      *    |
#   0x14     *  LR       * locals & padding for 16 B alignment
#   0x10     *  SRR1     *    |
#   0x0C     *  SRR0     *    |
#   0x08     *  padding  * ___v__________
#   0x04     * resvd- LR * Reserved for calling function 
#   0x00     *  SP       * Backchain (same as gpr1 in GPRs)
#

  .section .text_vle
  .global handler59
  .equ  INTC_EOIR,  0xfff48018  # End Of Interrupt Register address

  .extern PIT_ISR0

handler59:

# PROLOGUE
 e_stwu   r1,  -0x50(r1)      # Create stack frame and store back chain
 se_stw   r3,  0x28(r1)       # Store a working register
 mfsrr0   r3          # Store SRR0:1 (must be done before enabling EE)
 se_stw   r3,  0x0C(r1)
 mfsrr1   r3
 se_stw   r3,  0x10(r1)

 wrteei   1           # Set MSR[EE]=1 (must wait a couple clocks after reading IACKR)
 e_stw    r12, 0x4C(r1)       # Store rest of gprs
 e_stw    r11, 0x48(r1)
 e_stw    r10, 0x44(r1)
 e_stw    r9,  0x40(r1)
 e_stw    r8,  0x3C(r1)
 se_stw   r7,  0x38(r1)
 se_stw   r6,  0x34(r1)
 se_stw   r5,  0x30(r1)
 se_stw   r4,  0x2C(r1)
 se_stw   r0,  0x24(r1)
 mfcr     r3          # Store CR
 se_stw   r3,  0x20(r1)
 mfxer    r3          # Store XER
 se_stw   r3,  0x1C(r1)
 se_mfctr r3          # Store CTR
 se_stw   r3,  0x18(r1)
 se_mflr  r3
 se_stw   r3,  0x14(r1)       # Store LR

 e_bl PIT_ISR0

# EPILOGUE
 se_lwz   r3,  0x14(r1)       # Restore LR
 se_mtlr  r3
 se_lwz   r3,  0x18(r1)       # Restore CTR
 se_mtctr r3
 se_lwz   r3,  0x1C(r1)       # Restore XER
 mtxer    r3
 se_lwz   r3,  0x20(r1)       # Restore CR
 mtcrf    0xff,r3
 se_lwz   r0,  0x24(r1)       # Restore other gprs except working registers
 se_lwz   r5,  0x30(r1)
 se_lwz   r6,  0x34(r1)
 se_lwz   r7,  0x38(r1)
 e_lwz    r8,  0x3C(r1)
 e_lwz    r9,  0x40(r1)
 e_lwz    r10, 0x44(r1)
 e_lwz    r11, 0x48(r1)
 e_lwz    r12, 0x4C(r1)

 mbar     0           # Ensure store to clear interrupts flag bit has completed
 e_lis    r3,INTC_EOIR@h     # Load upper half of EOIR address to r3
 se_li    r4,0
 wrteei   0           # Disable interrupts for rest of handler
 e_or2i      r3, INTC_EOIR@l
 e_stw     r4, 0x0(r3) # Write 0 to INTC_EOIR (address 0xFFF4 8018)
 se_lwz   r3,  0x0C(r1)       # Restore SRR0
 mtsrr0   r3
 se_lwz   r3,  0x10(r1)       # Restore SRR1
 mtsrr1   r3
 se_lwz   r4,  0x2C(r1)       # Restore working registers
 se_lwz   r3,  0x28(r1)
 e_add16i r1,r1,0x50      # Delete stack frame
 se_rfi               # End of Interrupt
##########################################################################################

