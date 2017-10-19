/*
 * ECU_DataFlash.c
 *
 *  Created on: Sep 29, 2015
 *      Author: Administrator
 */
#include "ee_emulation.h"
#include "ECU_DataFlash.h"

/* cache configuration */
UINT32 table[EE_CACHE_SIZE/4];
CACHE_TABLE cTable =
{
    (UINT32)table,      /* cache start address */
    sizeof(table)       /* cache size */
};

/* block 0 configuration */
BLOCK_CONFIG blockConf0 =
{
    BLOCK0_MASK,                           /* the block bit map in specific space */
    MAIN_ARRAY_BASE + BLOCK0_OFFSET,       /* the block start address */
    BLOCK0_SIZE,                           /* the block size */
    0x00000000,                            /* the address pointer to the blank space */
    BLOCK0_ADD_SPACE,                      /* the space (low, middle or high) for the block */
    BLOCK0_PARTITION                       /* Partition number of selelcted blocks */
};

/* block 2 configuration */
BLOCK_CONFIG blockConf1 =
{
    BLOCK1_MASK,                           /* the block bit map in specific space */
    MAIN_ARRAY_BASE + BLOCK1_OFFSET,       /* the block start address */
    BLOCK1_SIZE,                           /* the block size */
    0x00000000,                            /* the address pointer to the blank space */
    BLOCK1_ADD_SPACE,                      /* the space (low, middle or high) for the block */
    BLOCK1_PARTITION                       /* Partition number of selelcted blocks */
};

/* block 2 configuration */
BLOCK_CONFIG blockConf2 =
{
    BLOCK2_MASK,                           /* the block bit map in specific space */
    MAIN_ARRAY_BASE + BLOCK2_OFFSET,       /* the block start address */
    BLOCK2_SIZE,                           /* the block size */
    0x00000000,                            /* the address pointer to the blank space */
    BLOCK2_ADD_SPACE,                      /* the space (low, middle or high) for the block */
    BLOCK2_PARTITION                       /* Partition number of selelcted blocks */
};

/* block 3 configuration */
BLOCK_CONFIG blockConf3 =
{
    BLOCK3_MASK,                           /* the block bit map in specific space */
    MAIN_ARRAY_BASE + BLOCK3_OFFSET,       /* the block start address */
    BLOCK3_SIZE,                           /* the block size */
    0x00000000,                            /* the address pointer to the blank space */
    BLOCK3_ADD_SPACE,                      /* the space (low, middle or high) for the block */
    BLOCK3_PARTITION                       /* Partition number of selelcted blocks */
};
/* block configuration array */
BLOCK_CONFIG* blockConfig[4] =
{
    &blockConf0,        /* block 0 structure address */
    &blockConf1,        /* block 2 structure address */
    &blockConf2         /* block 3 structure address */
};
/* EEPROM configuration */
EEPROM_CONFIG eepromConf =
{
    NUMBER_OF_BLOCK,  /* the number of blocks being used emulation */
    0,                /* the number of dead block */
    0,                /* the active block index */
    FALSE,            /* the writing flag of the blocks */
    &cTable,          /* the cache table structure */
    blockConfig,      /* block configuration array */
    FALSE              /* the flag to enable/disable the cache table */
};

/*****************************************************************
Static function prototype
******************************************************************/
static void ErrorTrap(UINT32 ret);
void CallBack(void);
static void InitGlobalVars(void);
static UINT32 swap_num = 0;

#if DCACHE_AVAILABLE
    extern void dcache_disable(void);
    extern void dcache_enable(void);
#endif

UINT32 buffer[(BUFFER_SIZE - 0x0020)/4];
/**************************************************************
*                          Disable Flash Cache                  *
***************************************************************/
extern void DisableFlashControllerCache(UINT32 *origin_pflash_pfcr1, UINT32 *origin_pflash_pfcr2);

/*****************************************************************
*               Restore configuration register of FCM              *
******************************************************************/
extern void RestoreFlashControllerCache(UINT32 pflash_pfcr1, UINT32 pflash_pfcr2);

/*****************************************************************
ErrorTrap function
******************************************************************/
static void ErrorTrap(UINT32 ret)
{
    VUINT32 failReason;

    failReason = ret;
    while(1)
    {

    }
}
/*****************************************************************
CallBack function
******************************************************************/
void CallBack(void)
{
	;
}


/*****************************************************************
CallBack InitGlobalVars
******************************************************************/
static void InitGlobalVars(void)
{
    blockConf0.enabledBlock = BLOCK0_MASK;
    blockConf0.blockStartAddr = MAIN_ARRAY_BASE + BLOCK0_OFFSET;
    blockConf0.blockSize = BLOCK0_SIZE;
    blockConf0.blankSpace = 0x00000000;
    blockConf0.blockSpace = BLOCK0_ADD_SPACE;
    blockConf0.partSelect = BLOCK0_PARTITION;


    blockConf1.enabledBlock = BLOCK1_MASK;
    blockConf1.blockStartAddr = MAIN_ARRAY_BASE + BLOCK1_OFFSET;
    blockConf1.blockSize = BLOCK1_SIZE;
    blockConf1.blankSpace = 0x00000000;
    blockConf1.blockSpace = BLOCK1_ADD_SPACE;
    blockConf1.partSelect = BLOCK1_PARTITION;

    blockConf2.enabledBlock = BLOCK2_MASK;
    blockConf2.blockStartAddr = MAIN_ARRAY_BASE + BLOCK2_OFFSET;
    blockConf2.blockSize = BLOCK2_SIZE;
    blockConf2.blankSpace = 0x00000000;
    blockConf2.blockSpace = BLOCK2_ADD_SPACE;
    blockConf2.partSelect = BLOCK3_PARTITION;

    blockConf3.enabledBlock = BLOCK3_MASK;
    blockConf3.blockStartAddr = MAIN_ARRAY_BASE + BLOCK3_OFFSET;
    blockConf3.blockSize = BLOCK3_SIZE;
    blockConf3.blankSpace = 0x00000000;
    blockConf3.blockSpace = BLOCK3_ADD_SPACE;
    blockConf3.partSelect = BLOCK3_PARTITION;

    blockConfig[0]= &blockConf0;
    blockConfig[1]= &blockConf1;
    blockConfig[2]= &blockConf2;

    cTable.startAddress = (UINT32)table;
    cTable.size = sizeof(table);


    eepromConf.numberOfBlock = NUMBER_OF_BLOCK;
    eepromConf.activeBlockIndex = 0;
    eepromConf.blockWriteFlag = 0;
    eepromConf.cacheEnable = FALSE;
    eepromConf.cTable = &cTable;
    eepromConf.flashBlocks = blockConfig;
}

/*
 * Name        : ECU_Init_Eeprom
 * Description : Init eeprom cache
 * Arguments   : none
 * Return      : ret
 */
UINT32  ECU_Init_Eeprom()
{
	UINT32 i;                                                   /* index vairable */
	UINT32 ret;                                                 /* store return code */
	//UINT32 pflash_pfcr1, pflash_pfcr2;
	  
    UNLOCK_EBLOCKS                                              /* Unlock all eeprom blocks */
    InitGlobalVars();                                           /* Init all globle variable. If startup code can initialize all variables with                                                         the thier values specified at declaration, user can skip calling this function  */
#if DCACHE_AVAILABLE                                            /* Disable Data cache if present */
    dcache_disable();
#endif
    //DisableFlashControllerCache(&pflash_pfcr1,&pflash_pfcr2);   /* Invalidate flash controller cache */
    for(i = 0; i < EE_CACHE_SIZE; i = i+4)                      /* Init eeprom cache */
    {
	    WRITE32(cTable.startAddress + i, 0xFFFFFFFF);
	} 
#if 0
    /* Erase all eeprom blocks*/
	ret = FSL_RemoveEeprom(&eepromConf,&CallBack);
	if(ret != EE_OK)
	{
		ErrorTrap(ret);
	}
#endif   
       
	i = 0;                                                     /* Init eeprom, will try 5 times if failed */
	ret = !EE_OK;
	while ((i <5) && (ret != EE_OK) && (ret != EE_ERROR_CANNOT_IDENTIFY_VALID_BLOCK))
	{
	    ret = FSL_InitEeprom(&eepromConf,&CallBack);
	    i++;
	}
	if (ret == EE_ERROR_CANNOT_IDENTIFY_VALID_BLOCK)
	{
	   ;                                                       /* Specify invalid block and call to FSL_RecoverEmulationSystem  */
	}
	else if(ret != EE_OK)
	{
	    return ret;
	}
	swap_num = 0;
}
