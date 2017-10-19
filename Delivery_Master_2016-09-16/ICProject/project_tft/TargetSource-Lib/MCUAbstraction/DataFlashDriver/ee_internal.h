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
* $FileName: ee_internal.h$
* $Version : 1.2.0$
* $Date    : Jul-22-2015$
*
* Comments:
*
*   - This file consitutes of defines, structures and declarations 
*     of global variables as extern.
*   - This file is used internally by the driver
*
*END************************************************************************/

#ifndef _EE_INTERNAL_H_
#define _EE_INTERNAL_H_

#include "ee_emulation.h"

#ifndef MAX_REERASE
#define MAX_REERASE                         10U
#endif

#ifndef MAX_REPGM_BLK_IND
#define MAX_REPGM_BLK_IND                   10U
#endif

#ifndef EER_OPTION
#error "EER_OPTION is not defined"
#endif

/* Maximum programmable size in byte */
#if (FLASH_MODULE == C55)
    #define PGM_SIZE                        128U
#elif (FLASH_MODULE == C90FL)
    #define PGM_SIZE                        16U
#elif (FLASH_MODULE == H7Fx)
    #define PGM_SIZE                        16U
#elif (FLASH_MODULE == C90LC)
    #define PGM_SIZE                        8U
#elif (FLASH_MODULE == C90LC_DFO)
    #define PGM_SIZE                        4U
#else
    #error "Incorrect flash module"
#endif

#if (FLASH_MODULE == C90LC_DFO)
    #define MIN_PGM_SIZE                    WORD_SIZE
#else
    #define MIN_PGM_SIZE                    DWORD_SIZE
#endif

/* Determine macors which depend on scheme selection */
#if (ECC4_FIXLENGTH == SCHEME_SELECT)
    #define ECC_SIZE                4U               /* Size in byte being ECC*/
    #define MIN_RECORD_SIZE         (ECC_SIZE * 2U)  /* The section including record status,IDSize and must be ECC_SIZE aligned */
    #define SIZE_FIELD              0U               /* Size in byte of SIZE field */
#elif (ECC4_VARLENGTH == SCHEME_SELECT)
    #define ECC_SIZE                4U               /* Size in byte being ECC*/
    #define MIN_RECORD_SIZE         (ECC_SIZE * 2U)  /* The section including record status,IDSize and must be ECC_SIZE aligned */
    #define SIZE_FIELD              2U               /* Size in byte of SIZE field */
#elif (ECC8_FIXLENGTH == SCHEME_SELECT)
    #define ECC_SIZE                8U               /* Size in byte being ECC*/
    #define MIN_RECORD_SIZE         (ECC_SIZE * 2U)  /* The section including record status,IDSize and must be ECC_SIZE aligned */
    #define SIZE_FIELD              0U               /* Size in byte of SIZE field */
#elif (ECC8_VARLENGTH == SCHEME_SELECT)
    #define ECC_SIZE                8U               /* Size in byte being ECC*/
    #define MIN_RECORD_SIZE         (ECC_SIZE * 2U)  /* The section including record status,IDSize and must be ECC_SIZE aligned */
    #define SIZE_FIELD              2U               /* Size in byte of SIZE field */
#elif (ECC16_FIXLENGTH == SCHEME_SELECT)
    #define ECC_SIZE                16U              /* Size in byte being ECC*/
    #define MIN_RECORD_SIZE         ECC_SIZE         /* The section including record status,IDSize and must be ECC_SIZE aligned */
    #define SIZE_FIELD              0U               /* Size in byte of SIZE field */
#elif (ECC16_VARLENGTH == SCHEME_SELECT)
    #define ECC_SIZE                16U              /* Size in byte being ECC*/
    #define MIN_RECORD_SIZE         (ECC_SIZE * 2U)  /* The section including record status,IDSize and must be ECC_SIZE aligned */
    #define SIZE_FIELD              2U               /* Size in byte of SIZE field */
#elif (ECC32_FIXLENGTH == SCHEME_SELECT)
    #define ECC_SIZE                32U              /* Size in byte being ECC*/
    #define MIN_RECORD_SIZE         ECC_SIZE         /* The section including record status,IDSize and must be ECC_SIZE aligned */
    #define SIZE_FIELD              0U               /* Size in byte of SIZE field */
#elif (ECC32_VARLENGTH == SCHEME_SELECT)
    #define ECC_SIZE                32U              /* Size in byte being ECC*/
    #define MIN_RECORD_SIZE         (ECC_SIZE * 2U)  /* The section including record status,IDSize and must be ECC_SIZE aligned */
    #define SIZE_FIELD              2U               /* Size in byte of SIZE field */
#else
    #error "Incorrect selected scheme"
#endif /* END OF SCHEME_SELECT */

#define ACT_IND_OFFSET              (0U)
#define ERASE_CYCLE_OFFSET          (ECC_SIZE)       /* offset of erase cycle */
#define DEAD_IND_OFFSET             (ECC_SIZE * 2U)
#define COPY_DONE_OFFSET            (ECC_SIZE * 3U)
#define RECORD_OFFSET               (ECC_SIZE * 4U)  /* Address of the first record*/

/* Record head config*/
#define ID_FIELD_SIZE                2U              /* size in byte of ID field*/


/* Size of data in record head */
#if (SIZE_FIELD != 0U)
    #define ID_OFFSET                               ECC_SIZE            /* the offset of ID field by record status*/
    #if (ECC4_VARLENGTH == SCHEME_SELECT)
        #define SMALL_DATA_AFTER_STATUS_SIZE        0U
    #else
        #define SMALL_DATA_AFTER_STATUS_SIZE        (ECC_SIZE - DWORD_SIZE)
    #endif
    #define SMALL_DATA_AFTER_ID_SIZE                (ECC_SIZE - WORD_SIZE)
#else
    #define ID_OFFSET                               MIN_PGM_SIZE        /* the offset of ID field after record status*/
    #define SMALL_DATA_AFTER_STATUS_SIZE            (0U)
    #if ((ECC4_FIXLENGTH == SCHEME_SELECT)|| (ECC8_FIXLENGTH == SCHEME_SELECT))
        #define SMALL_DATA_AFTER_ID_SIZE            (ECC_SIZE - ID_FIELD_SIZE)
    #else
        #define SMALL_DATA_AFTER_ID_SIZE            (ECC_SIZE - ID_FIELD_SIZE-ID_OFFSET)
    #endif
#endif

#define DATA_HEAD_SIZE                              (SMALL_DATA_AFTER_STATUS_SIZE + SMALL_DATA_AFTER_ID_SIZE)

/* Maximum record number */
#define NUMBER_OF_SEARCHING_RECORD_IN_BLOCK         0x2000U

/* Offsets of FLASH Control Registers*/
#define FLASH_MCR                 0x00000000U    /* Module Configuration Register */

#if (FLASH_MODULE == C55)
    #define FLASH_SEL0            0x00000038U    /* Block selection register for Low and Mid space */
    #define FLASH_SEL1            0x0000003CU    /* Block selection register for High space */
    #define FLASH_ADR             0x00000050U    /* Address register */
    /* Address register bit mapping */
    #define FLASH_ADR_aH          0x40000000U    /* Address high region */
    #define FLASH_ADR_aM          0x20000000U    /* Address mid region */
    #define FLASH_ADR_aL          0x10000000U    /* Address low region */
    #define FLASH_ADR_a256K       0x08000000U    /* Address 256K region */
    #define FLASH_ADR_a64K        0x04000000U    /* Address 64K region */
    #define FLASH_ADR_a32K        0x02000000U    /* Address 32K region */
    #define FLASH_ADR_a16K        0x01000000U    /* Address 16K region */
    #define FLASH_ADR_ADDR        0x00FFFFF8U    /* offset address bit mapping */
#else
    #define FLASH_SEL0            0x00000010U    /* Low/Mid Address Space Block Select Register */
    #define FLASH_SEL1            0x00000014U    /* High Address Space Block Select Register */
    #define PFB_CR                0x0000001CU    /* PFBIU Configuration Register for port 0 */
    #define PFB_CR_BFEN           0x00000001U    /* PFBIU Line Read Buffers Enable */
#endif

/* Module Configuration Register */
#define FLASH_MCR_EER             0x00008000U    /* ECC Event Error */
#define FLASH_MCR_RWE             0x00004000U    /* Read While Write Event Error */
#define FLASH_MCR_SBC             0x00002000U    /* Single Bit Correction Error */
#define FLASH_MCR_PEAS            0x00000800U    /* Program/Erase Access Space */
#define FLASH_MCR_DONE            0x00000400U    /* State Machine Status */
#define FLASH_MCR_PEG             0x00000200U    /* Program/Erase Good */
#define FLASH_MCR_FERS            0x00000080U    /* Factory Erase */
#define FLASH_MCR_PGM             0x00000010U    /* Program */
#define FLASH_MCR_PSUS            0x00000008U    /* Program Suspend */
#define FLASH_MCR_ERS             0x00000004U    /* Erase */
#define FLASH_MCR_ESUS            0x00000002U    /* Erase Suspend */
#define FLASH_MCR_EHV             0x00000001U    /* Enable High Voltage */

#define ENABLE_MSR                0x00008000U

/* Mask bit to detect immediate data */
#define IMMEDIATE_MASK            0x8000U

/* data record status state */
#define R_PROGRAM_COMPLETE              0xFFFF0000U
#define R_DELETE                        0x00000000U

#define ACT_INDICATOR_ACT               0x0000FFFFU
#define DEAD_INDICATOR_DEAD             0x0000FFFFU
#define COPY_DONE                       0x0000FFFFU

/* block status state */
#define B_ERASED                        0U
#define B_ALT                           1U
#define B_ACT                           2U
#define B_UPDATE                        3U
#define B_INVALID                       4U
#define B_DEAD                          5U
#define B_COPY_DONE                     6U

/* Last job status from brownout */
#define EE_LAST_JOB_NONE                0U
#define EE_LAST_JOB_FIRST_TIME          1U
#define EE_LAST_JOB_NORMAL              2U
#define EE_LAST_JOB_UPDATE              3U
#define EE_LAST_JOB_COPY_DONE           4U
#define EE_LAST_JOB_ERASE               5U

/* Write eeprom options: normal, swap or not enough space */
#define EE_WRITE_NORMAL                 0U
#define EE_WRITE_ON_NEW_ACTIVE          1U
#define EE_WRITE_ON_COPYDONE            2U
#define EE_WRITE_SWAP                   3U

/* Immediate request type */
#define EE_IMMEDIATE_NONE               0U
#define EE_IMMEDIATE_READ               1U
#define EE_IMMEDIATE_WRITE              2U

#define FLASH_RESUME_WAIT               15U

/* Constant definition */
#define CTABLE_ITEM_SIZE                4U

#define FLASH_RESUME_WAIT               15U


/* Macros for Flash suspend and resume*/
#define FLASH_MCR_BIT_SET(Addr, mask)      \
    WRITE32(Addr, ((mask | READ32(Addr)) & (~(FLASH_MCR_EER | FLASH_MCR_RWE))))

#define FLASH_MCR_BIT_CLEAR(Addr, mask)    \
    WRITE32(Addr, (((~mask) & READ32(Addr)) & (~(FLASH_MCR_EER | FLASH_MCR_RWE))))


/* Variable to store erasing cycles */
extern UINT32 erasingCycleInternal;

/* Variable to store index of the erasing block while swapping */
extern UINT32 sourceBlockIndexInternal;

/* Flag to keep track of ECC Error Status */
extern VUINT8 eccErrorStatus_Flag;

/* Flag to call FlashDepletionRecover() or not */
#if (FLASH_MODULE == C90FL)
extern BOOL depletion_Flag;
#endif

/* DATA_RECORD_HEAD structure is to read the data from record status to to IDSIZE field */
/* It does not read data which is right after IDSIZE field */
typedef struct
{
    UINT32  dataStatus;                 /* 8 byte record status */
#if (MIN_PGM_SIZE > WORD_SIZE)
    UINT32 unused;                      /* padding bytes */
#endif
#if (SMALL_DATA_AFTER_STATUS_SIZE != 0U)
    UINT32  wordData[(SMALL_DATA_AFTER_STATUS_SIZE)/WORD_SIZE];
#endif
    UINT16  dataID;                     /* the unique data ID */
    UINT16  dataSize;                   /* the data size */
}DATA_RECORD_HEAD;

/* Reading options */
typedef enum
{
    BLANK_CHECK =0U,
    VERIFY = 1U,
    READ = 2U
}EE_ReadCode;


/*******************************************************************
| function prototypes (scope: module-local)
|------------------------------------------------------------------*/
extern void CallBack(void);

/* flash driver functions */
UINT32 FSL_FlashProgramStart(UINT32 dest, UINT16 size, UINT32 source);

UINT32 FSL_FlashEraseStart(UINT32 interlockWriteAddress, UINT32 lowEnabledBlock,\
                           UINT32 midEnabledBlock, UINT32 highEnabledBlock);

UINT32 FSL_FlashCheckStatus(void);

UINT32 FSL_FlashSuspend(void);

void FSL_FlashResume(UINT32 suspendState);

UINT32 FSL_FlashAbortErase(void);


/*************************
 Low level functions
*************************/
UINT32 FSL_ReadBlockStatus(BLOCK_CONFIG* blockConf, void (*CallBack) (void));

UINT32 FSL_GetRecordLength (UINT16 dataSize);

#if (SIZE_FIELD == 0U)
UINT32 FSL_SearchRecordFromBottom(BLOCK_CONFIG * blockConf, UINT16 recordID, \
                                  UINT32 * recordAddr, BOOL *deleted, void (*CallBack) (void));
#endif

void FSL_SearchRecordFromTop (BLOCK_CONFIG *blockConf, UINT32 bufferAddress, \
                              UINT32 bufferSize, UINT16 startID, UINT32 *startAddrInBlock, \
                              UINT16 *nextStartID, BOOL *deleted, void(*CallBack)(void));

UINT32 FSL_FlashRead(EE_ReadCode func, UINT32 dest, UINT32 size, UINT32 buffer, void(*CallBack)(void));

void FSL_ReadRecordHead(UINT32 address, DATA_RECORD_HEAD *pLocRecHead,void(*CallBack)(void));

#if (EER_OPTION == IVOR_EXCEPTION)
void cRestoreEei(UINT32);
UINT32 cReadAndClearEei(void);
#else
static UINT32 FSL_GetFailedAddr(void);
#endif  /* EER_OPTION */

UINT32 FSL_ProcessImmediateRequest(EEPROM_CONFIG *eepromConfig, BLOCK_CONFIG *blockConf, UINT8 iReq);

UINT32 FSL_MakeBlock2Dead(EEPROM_CONFIG *pEepromConfig, BLOCK_CONFIG *pBlockConf, void (*CallBack)(void));

UINT32 FSL_ProgramBlockIndicator(UINT32 dest, UINT32 source, void (*CallBack) (void));

#if (FLASH_MODULE == C90FL)
/* function to recover from over-erase after brownout */

typedef UINT32 (*pFLASHDEPLETIONRECOVER)(UINT32 flashRegBase, UINT32 interlockWriteAddress, UINT32 lowEnabledBlock, \
                                         UINT32 midEnabledBlock, UINT32 highEnabledBlock,void (*CallBack)(void));
extern const unsigned char FSL_FlashDepletionRecover_C[];
#define pFlashDepletionRecover          ((pFLASHDEPLETIONRECOVER)FSL_FlashDepletionRecover_C)
#endif


/*************************
 Middle level functions
*************************/
UINT32 FSL_ValidateInvalidAndDeadBlocks(EEPROM_CONFIG* eepromConfig, void (*CallBack)(void));

UINT32 FSL_GetLastJobStatus(EEPROM_CONFIG *eepromConfig, void (*CallBack)(void));

UINT32 FSL_GetEraseStatus(EEPROM_CONFIG* eepromConfig, void (*CallBack) (void));

void FSL_ReadRecordAtAddr(UINT32 recordAddr, UINT16 dataSize, UINT32 bufferAddr, void (*CallBack)(void));

UINT32 FSL_GetWriteRecordOption(EEPROM_CONFIG* eepromConfig, UINT32 recordLength, void (*CallBack)(void));

UINT32 FSL_SearchInTable(CACHE_TABLE* cTable, UINT16 dataID, UINT32* expDataAddress);

UINT32 FSL_SearchEntireBlockFromTop(BLOCK_CONFIG* blockConf, UINT16 dataID, \
                                    UINT32* expDataInBlock, BOOL *deleted, void (*CallBack)(void));

UINT32 FSL_SearchEntireBlock(BLOCK_CONFIG* blockConf, UINT16 dataID, UINT32* recordAddr, BOOL *deleted, void(*CallBack)(void));

UINT32 FSL_SearchInAllBlocks(EEPROM_CONFIG* eepromConfig, UINT16 dataID, UINT8 iReq, UINT32* recordAddr, 
                             UINT32 *suspendState, void (*CallBack) (void));

void FSL_RecordBuffering(BLOCK_CONFIG* blockConf, UINT32 bufferAddress, UINT32 bufferSize,
                         UINT16* startID, void (*CallBack)(void));

UINT32 FSL_WriteDataRecord(BLOCK_CONFIG* blockConf, BOOL backup_flag, UINT16 dataID, UINT16 dataSize, \
                           UINT32 source, void(*CallBack)(void));

UINT32 FSL_CopyDataRecord(EEPROM_CONFIG* eepromConfig, BLOCK_CONFIG * blockConf,\
                          BOOL backup_flag, UINT16 dataID, UINT16 dataSize, \
                          UINT32 source, void (*CallBack)(void));

BOOL FSL_UpdateCacheTable(CACHE_TABLE* cTable, UINT16 dataID, UINT32 newValue);

UINT32 FSL_BlockSwapping(EEPROM_CONFIG* eepromConfig,  BOOL syncErase, void (*CallBack)(void));

UINT32 FSL_EraseEEBlock(EEPROM_CONFIG* eepromConfig, UINT32 blockIndex, BOOL syncErase, void (*CallBack)(void));

UINT32 FSL_SyncProgram(UINT32 dest, UINT16 size, UINT32 source, void (*CallBack)(void));
    
#endif /*_EE_INTERNAL_H_*/
/* EOF */
