/**HEADER********************************************************************
*
* Copyright (c) 2014 Freescale Semiconductor;
* All Rights Reserved
*
***************************************************************************
*
* THIS SOFTWARE IS PROVIDED BY FREESCALE "AS IS" AND ANY EXPRESSED OR
* IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
* OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
* IN NO EVENT SHALL FREESCALE OR ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
* INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
* SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
* HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
* STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
* IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
* THE POSSIBILITY OF SUCH DAMAGE.
*
**************************************************************************
*
* $FileName: ee_blocks.h$
* $Version : 1.10$
* $Date    : Feb-25-2014$
*
* Comments:
*
*   Provide information about blocks used for emualtion
*
*END************************************************************************/

#ifndef _EE_BLOCKS_H_
#define _EE_BLOCKS_H_
#include "ee_emulation.h"

#define C90LC_LML                       0x0004         /* Low/Mid Address Sapce Block Locking Register */
#define C90LC_SLL                       0x000C         /* Secondary Low/Mid Address Space Block Locking Register */


#define MAIN_ARRAY_BASE                 0x00800000
/* Low space block 0 */
#define BLOCK0_OFFSET                    0x00000000        /* Offset of low block 0 */
#define BLOCK0_SIZE                        0x4000            /* 16KB size */
#define BLOCK0_MASK                        0x00000001
#define BLOCK0_PARTITION                (0x1<<0)
#define BLOCK0_ADD_SPACE                LOW_ADDRESS_SPACE

/* Low space block 1 */
#define BLOCK1_OFFSET                    0x00004000        /* Offset of low block 1 */
#define BLOCK1_SIZE                        0x4000             /* 16KB size */
#define BLOCK1_MASK                        0x00000002
#define BLOCK1_PARTITION                (0x1<<0)
#define BLOCK1_ADD_SPACE                LOW_ADDRESS_SPACE

/* Low space block 2 */
#define BLOCK2_OFFSET                    0x00008000        /* Offset of low block 2 */
#define BLOCK2_SIZE                        0x4000             /* 16KB size */
#define BLOCK2_MASK                        0x00000004
#define BLOCK2_PARTITION                (0x1<<0)
#define BLOCK2_ADD_SPACE                LOW_ADDRESS_SPACE

/* Low space block 3 */
#define BLOCK3_OFFSET                    0x0000C000        /* Offset of low block 2 */
#define BLOCK3_SIZE                        0x4000             /* 16KB size */
#define BLOCK3_MASK                        0x00000008
#define BLOCK3_PARTITION                (0x1<<0)
#define BLOCK3_ADD_SPACE                LOW_ADDRESS_SPACE

#define UNLOCK_EBLOCKS                  WRITE32(FLASH_REG_BASE + C90LC_LML, 0xA1A11111);\
                                        WRITE32(FLASH_REG_BASE + C90LC_SLL, 0xC3C33333);\
                                        WRITE32(FLASH_REG_BASE + C90LC_LML, 0xFFFFFFF0);\
                                        WRITE32(FLASH_REG_BASE + C90LC_SLL, 0xFFFFFFF0);
#endif /* _EE_BLOCKS_H_ */
