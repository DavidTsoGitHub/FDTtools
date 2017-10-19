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
* $FileName: user_cfg.h$
* $Version : 1.10$
* $Date    : Feb-25-2014$
*
* Comments:
*
*   Contains user defined options for the driver
*
*END************************************************************************/

#ifndef _USER_CFG_H_
#define _USER_CFG_H_
#include "ee_emulation.h"
#include "ee_blocks.h"

#define SCHEME_SELECT  ECC16_VARLENGTH 

#define MAX_REERASE                 1

#define MAX_REPGM_BLK_IND           1
    
#define SWAP_CACHE_SIZE             0x30

#define EER_OPTION                  IVOR_EXCEPTION

#define FLASH_MODULE                C90LC

#define FLASH_REG_BASE              0xC3F8C000

/* Size of eeprom cache byte*/
#define EE_CACHE_SIZE 				0x80
/* Number of eeprom blocks*/
#define NUMBER_OF_BLOCK				3
/* Maximum number of active blocks */
#define NUMBER_OF_ACTIVE_BLOCKS     2
/* Size of buffer */
#define BUFFER_SIZE                 500//0x50
/* Size of data if a fixed length record schem is seleted */
#define DATA_SIZE                   500//0x40   

#endif /* _USER_CFG_H_ */
