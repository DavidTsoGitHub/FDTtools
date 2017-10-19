 
 
#
# Assumes IVPR contains address of teh section mwvects
# STACK FRAME DESIGN: Depth: 20 words (0x50, or 80 bytes)

#            ************* ______________
#   0x4C     *  GPR12    *    ^
#   0x48     *  GPR11    *    |
#   0x44     *  GPR10    *  GPRs (32 bit)
#   0x40     *  GPR9     *    |
#   0x3C     *  GPR8     *    |
#   0x38     *  GPR7     *    |
#   0x34     *  GPR6     *    |
#   0x30     *  GPR5     *    |
#   0x2C     *  GPR4     *    |
#   0x28     *  GPR3     *    |
#   0x24     *  GPR0     * ___v__________
#   0x20     *  CR       * __CR__________
#   0x1C     *  XER      *    ^
#   0x18     *  CTR      *    |
#   0x14     *  LR       * locals & padding for 16 B alignment
#   0x10     *  CSRR1    *    |
#   0x0C     *  CSRR0    *    |
#   0x08     *  padding  * ___v__________
#   0x04     * resvd- LR * Reserved for calling function
#   0x00     *  SP       * Backchain (same as gpr1 in GPRs)
#            *************
#
#  To ensure the servicing is recognized the EISR.NMI0 (Z1) or NMI1 (Z0) must be set to clear the request.
#
    .global IVOR0Handler
    .global IVOR1Handler
    .global IVOR2Handler
    .global IVOR3Handler
    .extern IVOR4Handler
    .global IVOR5Handler
    .global IVOR6Handler
    .global IVOR8Handler
    .global IVOR15Handler
    .globl IVORxHandlers
    .global default_except_handler  # Ensure access to the C Interrupt Service Routine for Handlers

    .section .exec_vect_Vle,text_vle  # Fixed location of IVPR used by Core Interrupts (exceptions)



    .align 16             # Use 16 for Metrowerks, use .align 4 for Diab and GreenHills

IVORxHandlers: 

.org 0x0000
     e_b               IVOR0Handler          #Critical Input

.org 0x0010
     e_b               IVOR1Handler          #Machine Check

.org 0x0020
     e_b               IVOR2Handler          #Data Storage

.org 0x0030
     e_b               IVOR3Handler          #Instruction Storage

.org 0x0040
     e_b               IVOR4Handler          #External Input

.org 0x0050
     e_b               IVOR5Handler          #Alignment

.org 0x0060
     e_b               IVOR6Handler          #Program

.org 0x0080
     e_b               IVOR8Handler          #System call

.org 0x00f0
     e_b               IVOR15Handler          #Debug

   .section .text_vle

IVOR0Handler:  

# PROLOGUE
     e_stwu   r1,  -0x50(r1)     # Create stack frame and store back chain (4 * 20) = 0x50

     e_add16i r1,r1,0x50     # Delete stack frame
     se_rfi # Return 

IVOR1Handler:  

# PROLOGUE
     e_stwu   r1,  -0x50(r1)     # Create stack frame and store back chain (4 * 20) = 0x50

     e_add16i r1,r1,0x50     # Delete stack frame
     se_rfi # Return 

IVOR2Handler:  

# PROLOGUE
     e_stwu   r1,  -0x50(r1)     # Create stack frame and store back chain (4 * 20) = 0x50

     e_add16i r1,r1,0x50     # Delete stack frame
     se_rfi # Return 

IVOR3Handler:  

# PROLOGUE
     e_stwu   r1,  -0x50(r1)     # Create stack frame and store back chain (4 * 20) = 0x50

     e_add16i r1,r1,0x50     # Delete stack frame
     se_rfi # Return 

IVOR5Handler:  

# PROLOGUE
     e_stwu   r1,  -0x50(r1)     # Create stack frame and store back chain (4 * 20) = 0x50

     e_add16i r1,r1,0x50     # Delete stack frame
     se_rfi # Return 

IVOR6Handler:  

# PROLOGUE
     e_stwu   r1,  -0x50(r1)     # Create stack frame and store back chain (4 * 20) = 0x50

     e_add16i r1,r1,0x50     # Delete stack frame
     se_rfi # Return 

IVOR8Handler:  

# PROLOGUE
     e_stwu   r1,  -0x50(r1)     # Create stack frame and store back chain (4 * 20) = 0x50

     e_add16i r1,r1,0x50     # Delete stack frame
     se_rfi # Return 

IVOR15Handler:  

# PROLOGUE
     e_stwu   r1,  -0x50(r1)     # Create stack frame and store back chain (4 * 20) = 0x50

     e_add16i r1,r1,0x50     # Delete stack frame
     se_rfi # Return 



default_except_handler:     
