/**HEADER********************************************************************
*
* Copyright (c) 2015 Freescale Semiconductor
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
* $FileName: ee_emulation.h$
* $Version : 1.2.0$
* $Date    : Jul-22-2015$
*
* Comments:
*
*   This file consitutes of defines, structures and declarations of global variables as extern.
*
*END************************************************************************/

#ifndef _EE_EMULATION_H_
#define _EE_EMULATION_H_

#include "ee_types.h"
#include "user_cfg.h"

/* Return code definition */
#define EE_OK                                       0x00000000U
#define EE_INFO_HVOP_INPROGRESS                     0x00000001U
#define EE_INFO_PROGRAM_SUSPEND                     0x00000002U
#define EE_INFO_ERASE_SUSPEND                       0x00000004U
#define EE_ERROR_WRITE_IN_PROGRESS                  0x00000008U
#define EE_ERROR_PE_OPT                             0x00000010U
#define EE_ERROR_MISMATCH                           0x00000020U
#define EE_ERROR_BLOCK_STATUS                       0x00000040U
#define EE_ERROR_BLOCK_CONFIG                       0x00000080U
#define EE_ERROR_DATA_NOT_FOUND                     0x00000100U
#define EE_ERROR_NOT_IN_CACHE                       0x00000200U
#define EE_ERROR_NO_ENOUGH_SPACE                    0x00000400U
#define EE_ERROR_PROGRAM_BLOCK_INDICATOR            0x00000800U
#define EE_ERROR_CANNOT_IDENTIFY_VALID_BLOCK        0x00001000U
#define EE_ERROR_PROGRAM_ERASE_CYCLE                0x00002000U
#define EE_ERROR_NOT_ENOUGH_BLOCK_FOR_ROUND_ROBIN   0x00004000U
#define EE_ERROR_PROGRAM_BLOCK_INDICATOR_FOR_DEAD   0x00008000U
#define EE_MAKE_DEAD_OK                             0x00010000U


/* Macros for CallBack period controlling */
#define CALLBACK_READ_BYTES_NUMBER                  0x0020U
#define CALLBACK_SEARCHING_BYTES_NUMBER             0x001AU
#define CALLBACK_VERIFY_BYTES_NUMBER                0x0020U


/* Address space macros */
#define LOW_ADDRESS_SPACE         0x01U
#define MID_ADDRESS_SPACE         0x02U
#define HIGH_ADDRESS_SPACE        0x03U


/* Define Block Config type */
typedef struct
{
    UINT32  enabledBlock;               /* the block bit map in specific space */
    UINT32  blockStartAddr;             /* the block start address */
    UINT32  blockSize;                  /* the block size */
    UINT32  blankSpace;                 /* the address pointer to the blank space */
    UINT8   blockSpace;                 /* the space (low, middle or high) for the block */
    UINT32  partSelect;                 /* Partition number of selelcted blocks */
}BLOCK_CONFIG;


/* Define Cache Table type */
typedef struct
{
    UINT32  startAddress;               /* the start address of cache table */
    UINT32  size;                       /* the size of cache table in byte */
}CACHE_TABLE;


/* Define Eeprom Config type */
typedef struct
{
    UINT32              numberOfBlock;      /* the number of blocks being used emulation */
    UINT32              numberOfDeadBlock;  /* the number of dead block */
    UINT32              activeBlockIndex;   /* the active block index */
    VUINT32             blockWriteFlag;     /* the writing flag of the blocks */
    CACHE_TABLE*        cTable;             /* the cache table structure */
    BLOCK_CONFIG**      flashBlocks;        /* the block configuration array pointer */
    BOOL                cacheEnable;        /* the flag to enable/disable the cache table */
}EEPROM_CONFIG;


/* Modules for ECC exception handler */
typedef enum
{
    NONE = 0U,
    EE_MODULE = 1U,
    OTHER_MODULE = 2U
}EE_ModuleType;


/* Status of Erasing */
typedef enum
{
    ERASE_NOTSTARTED=0U,
    ERASE_DONE = 1U,
    ERASE_FAIL = 2U,
    ERASE_INPROGRESS = 3U,
    ERASE_SWAPERROR = 4U
}EE_EraseType;


/* Callback function */
typedef void (*PCALLBACK)(void);

/* Use this value if you don't use Callback function */
#define NULL_CALLBACK               (PCALLBACK)0xFFFFFFFFU


/* Flag to keep track of Erase State */
extern EE_EraseType eraseStatus_Flag;

/* Read and write macros */
#define WRITE8(address, value)      (*(VUINT8*)(address) = (value))
#define READ8(address)              ((UINT8)(*(VUINT8*)(address)))
#define SET8(address, value)        (*(VUINT8*)(address) |= (value))
#define CLEAR8(address, value)      (*(VUINT8*)(address) &= ~(value))

#define WRITE16(address, value)     (*(VUINT16*)(address) = (value))
#define READ16(address)             ((UINT16)(*(VUINT16*)(address)))
#define SET16(address, value)       (*(VUINT16*)(address) |= (value))
#define CLEAR16(address, value)     (*(VUINT16*)(address) &= ~(value))

#define WRITE32(address, value)     (*(VUINT32*)(address) = (value))
#define READ32(address)             ((UINT32)(*(VUINT32*)(address)))
#define SET32(address, value)       (*(VUINT32*)(address) |= (value))
#define CLEAR32(address, value)     (*(VUINT32*)(address) &= ~(value))


/*****************************************************************************
 User APIs
******************************************************************************/
UINT32 FSL_InitEeprom(EEPROM_CONFIG* eepromConfig, void(*CallBack)(void));

UINT32 FSL_ReadEeprom(EEPROM_CONFIG* eepromConfig, UINT16 dataID, UINT16 dataSize, UINT32 source, void(*CallBack)(void));

UINT32 FSL_WriteEeprom(EEPROM_CONFIG* eepromConfig, UINT16 dataID, UINT16 dataSize, UINT32 source, void(*CallBack)(void));

UINT32 FSL_DeleteRecord(EEPROM_CONFIG* eepromConfig, UINT16 dataID, void(*CallBack)(void));

UINT32 FSL_RemoveEeprom(EEPROM_CONFIG* eepromConfig, void(*CallBack)(void));

UINT32 FSL_ReportEepromStatus( EEPROM_CONFIG* eepromConfig, UINT32* erasingCycles);

UINT32 FSL_MainFunction(EEPROM_CONFIG* eepromConfig, void(*CallBack)(void));

UINT32 FSL_RecoverEmulationSystem(EEPROM_CONFIG * eepromConfig,UINT8 invalidBlockIndex,void (*CallBack) (void));

/* ECC exception handler */
#if (EER_OPTION == IVOR_EXCEPTION)
UINT32 EER_exception_handler(UINT32 return_address, UINT16 instruction);
#endif  /* EER_OPTION */

#endif /*_EE_EMULATION_H_*/
/* EOF */
