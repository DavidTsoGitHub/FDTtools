/* -------------------------------- Arctic Core ------------------------------
 * Copyright (C) 2009-2011 ArcCore AB <contact@arccore.com>
 * Licensed under ArcCore Embedded Software License Agreement.
 * -------------------------------- Arctic Core ------------------------------*/

/*
 * DESCRIPTION
 *   QSPI for Freescale MPC5645S
 *
 *   The implementation assumes that a Winbond W25Q64CV is used.
 *
 */

/* ----------------------------[includes]------------------------------------*/
#include "config.h"
#include "Std_Types.h"
#include "Arc_Types.h"
#include "Mcu.h"
#include "MPC5645S.h"
#include "QuadSPI.h"
#include "Board.h"
/* ----------------------------[private define]------------------------------*/

#define	CR_QUAD					0x2u
#define SR_WEL					0x2u
#define SR_WIP                  0x1u
#define WIP_TIMEOUT_LOOPS       0x10000uL

#define WINBOND 				1
#define SPANSION 				2
#define FLASH_MANUFACTURER 		WINBOND

/* ----------------------------[private macro]-------------------------------*/
#define PAGE_SIZE                   256
#define FLASH_ERASE_SECTOR_SIZE		64*1024

#define QSPI_SFMSR_BUSY				0x1

#define QUADSPI_ARDB (*(vuint32_t *)(0x87FFFFFC));

/*****************************************************************************/
/* Local function prototypes ('static')                                      */
/*****************************************************************************/
static void SpiSendByte(en_flashID flash_ID, uint8 u8data);
static void QspiSendByte(en_flashID flash_ID, uint8 u8data);
static void SpiRevByte(en_flashID flash_ID, uint8 *u8data);

static void SetSoftwareQspiMode(void);
static void SetHardwareQspiMode(void);
static void SetSoftwareSpi(void);
static void SetSoftwareQspi(void);

static en_exflashStateType FL_eraseChip(en_flashID flash_ID, flashCbType cb);
static en_exflashStateType FL_writeEnableLatch(en_flashID flash_ID, en_w25qInstruction cmd, flashCbType cb);
static en_exflashStateType FL_writeStatusRegister(en_flashID flash_ID, uint8 u8R1, uint8 u8R2, flashCbType cb);
static en_exflashStateType FL_readStatusRegister(en_flashID flash_ID, en_w25qInstruction cmd, uint8 *u8data);
static en_exflashStateType FL_QpageProgram(en_flashID flash_ID, uint32 u32addr, uint8 *u8data, uint32 u32len, flashCbType cb);
static en_exflashStateType FL_PageProgram(en_flashID flash_ID, uint32 u32addr, uint8 *u8data, uint32 u32len, flashCbType cb);
static en_exflashStateType FL_waitFlashIdel(en_flashID flash_ID, flashCbType cb);
/* ----------------------------[private typedef]-----------------------------*/
/* ----------------------------[private function prototypes]-----------------*/
/* ----------------------------[private variables]---------------------------*/
/* ----------------------------[private functions]---------------------------*/
/**
 * Error in the QuadSPI flash communication, reset board
 */
static void QuadSPI_Error() {
	Board_Reset();
}

/**
 * Set flash in write enable mode
 */
static void QuadSPI_FlashWriteEnable(void) {
	QUADSPI_0.ICR.R = 0x00000006; /* WREN */
	while (QUADSPI_0.SFMSR.B.BUSY) {
	};
}


/**
 * Read the first 24-bits of RDID.
 *
 * @param[out] id The 24 first bits of RDID
 */
static void QuadSPI_GetRDID(uint32_t *id) {
	uint32_t data;

	/* Read status register */
	QUADSPI_0.ICR.R = 0x0003009f;
	while (QUADSPI_0.SFMSR.B.BUSY) {};
	while (QUADSPI_0.SFMSR.B.RXNE == 0) {};

	data = QUADSPI_0.RBDR[0].R;
	QUADSPI_0.SFMFR.R = 0x10000; /* read complete */

	*id = (data >> 8u);
}


/**
 * Waits for the flash to finish program or erase
 *
 * @param[in] cb callback function
 */
static uint32_t QuadSPI_WaitWhileFlashBusy( flashCbType cb)
{
	uint32_t timeout = 0;
	volatile vuint32_t read_data = 0x01000000;

	while((read_data & 0x01000000) == 0x01000000) /* loop while BUSY bit is set */
	{
		QUADSPI_0.ICR.R = 0x00010005; /* Query status register */
		while(QUADSPI_0.SFMSR.B.BUSY == 1) {}
		while(QUADSPI_0.SFMSR.B.RXNE == 0) {}
		read_data = QUADSPI_ARDB;
		QUADSPI_0.SFMFR.R = 0x10000; /* read complete */

		/* Call the callback function if any */
		if (cb != NULL) {
			cb();
		}
        if( timeout > WIP_TIMEOUT_LOOPS) {
            return QSPI_ERR_TIMEOUT;
        }
        timeout++;
	}
	return 0;
}

/* ----------------------------[public functions]----------------------------*/

/**
 * Reset flash
 */
void QuadSPI_Reset(void) {
	QUADSPI_0.ICR.R = 0x000000F0; /* Reset */
	while (QUADSPI_0.SFMSR.B.BUSY) {
	};
}

/**
 * Initiate the QuadSPI
 *
 * @param none
 */
uint32_t QuadSPI_Init(void)
{
	SetSoftwareQspiMode();
	FL_writeStatusRegister(flashA, 0u, 0x02u, NULL);
	FL_writeStatusRegister(flashB, 0u, 0x02u, NULL);
	SetHardwareQspiMode();
}

static void SetSoftwareQspiMode(void)
{
	SIU.PCR[FLA_CK].R = 0x020cu;
	SIU.PCR[FLA_CS].R = 0x020cu;
	SIU.PCR[FLA_DO].R = 0x020cu;
	SIU.PCR[FLA_DI].R = 0x010cu;
	SIU.PCR[FLA_WP].R = 0x020cu;
	SIU.PCR[FLA_HD].R = 0x020cu;
	
	SIU.GPDO[FLA_CK].B.PDO = 1u;
	SIU.GPDO[FLA_CS].B.PDO = 1u;
	SIU.GPDO[FLA_DO].B.PDO = 0u;
	SIU.GPDO[FLA_WP].B.PDO = 1u;
	SIU.GPDO[FLA_HD].B.PDO = 1u;
	
	
	SIU.PCR[FLB_CK].R = 0x020cu;
	SIU.PCR[FLB_CS].R = 0x020cu;
	SIU.PCR[FLB_DO].R = 0x020cu;
	SIU.PCR[FLB_DI].R = 0x010cu;
	SIU.PCR[FLB_WP].R = 0x020cu;
	SIU.PCR[FLB_HD].R = 0x020cu;
	
	SIU.GPDO[FLB_CK].B.PDO = 1u;
	SIU.GPDO[FLB_CS].B.PDO = 1u;
	SIU.GPDO[FLB_DO].B.PDO = 0u;
	SIU.GPDO[FLB_WP].B.PDO = 1u;
	SIU.GPDO[FLB_HD].B.PDO = 1u;
}

static void SetSoftwareSpi(void)
{
	SIU.PCR[FLA_DI].R  = 0x010c;
	SIU.PCR[FLB_DI].R  = 0x010c;
}

static void SetSoftwareQspi(void)
{
	SIU.PCR[FLA_DI].R  = 0x020c;
	SIU.PCR[FLB_DI].R  = 0x020c;
}

static void SetHardwareQspiMode(void)
{
	/* I/O configration */
#if(MODE_SELCET == SECLECT_AB || MODE_SELCET == SECLECT_A)
	/* QSPI A */
	SIU.PCR[PF13].R = 0x070c;
	SIU.PCR[PF14].R = 0x070c;
	SIU.PCR[PF11].R = 0x070c;
	SIU.PCR[PF12].R = 0x070c;
	SIU.PCR[PF10].R = 0x070f;
	SIU.PCR[PF15].R = 0x0600;
#endif
#if(MODE_SELCET == SECLECT_AB || MODE_SELCET == SECLECT_B)
	/* QSPI B */
	SIU.PCR[PF6].R  = 0x070c;
	SIU.PCR[PF5].R  = 0x070c;
	SIU.PCR[PK1].R  = 0x070c;
	SIU.PCR[PJ15].R = 0x070c;
	SIU.PCR[PJ13].R = 0x070f;
	SIU.PCR[PJ14].R = 0x0600;
#endif
	
	QUADSPI_0.MCR.B.SCLKCFG = 0;
	QUADSPI_0.MCR.B.MDIS = 0;
	QUADSPI_0.MCR.B.VMID = FLASH_MANUFACTURER;
	QUADSPI_0.LCR.B.NDC = 4;
	QUADSPI_0.ACR.B.ARMB = 0;
	QUADSPI_0.ACR.B.ARSZ = 8;
	QUADSPI_0.ACR.B.ARIC = 0Xeb;
#if 0
#if (FLASH_MANUFACTURER == WINBOND)
  #if (MODE_SELCET==SECLECT_A || MODE_SELCET==SECLECT_AB)
	/* configure Quad enable bit on flash 0 */
	QUADSPI_0.SFAR.R = 0x70000000; /* select flash A */
	QUADSPI_0.ICR.R = 0x00000006;  /* enable write */
	while(QUADSPI_0.SFMSR.B.BUSY){};
	/* set flash to quad spi */
	QUADSPI_0.TBDR.R = 0x00020000; 
	QUADSPI_0.ICR.R = 0x00020001;
	while(QUADSPI_0.SFMSR.B.BUSY){};
  #endif
  #if (MODE_SELCET==SECLECT_B || MODE_SELCET==SECLECT_AB)
	/* configure Quad enable bit on flash 1 */
	QUADSPI_0.SFAR.R = 0x78000000; /* select flash B */
	QUADSPI_0.ICR.R = 0x00000006;  /* enable write */
	while(QUADSPI_0.SFMSR.B.BUSY){};
	/* set flash to quad spi */
	QUADSPI_0.TBDR.R = 0x00020000;
	QUADSPI_0.ICR.R = 0x00020001;
	while(QUADSPI_0.SFMSR.B.BUSY){};
  #endif
#elif (FLASH_MANUFACTURER == SPANSION)

#endif
#endif
}

en_exflashStateType FL_read(uint32 u32addr, uint8 *u8readBuffer, uint32 u32len)
{
	uint32 u32i;
	uint32 u32readAddr;
	en_exflashStateType ret=flash_OK;
	
	u32readAddr = u32addr;
	
	if( (u32readAddr<FLASH_VIRTAUL_ADDR_MIN) || (u32readAddr>FLASH_VIRTAUL_ADDR_MAX) )
	{
		ret = flash_AddrError;
	}
	
	if(FLASH_VIRTAUL_ADDR_MAX < (u32readAddr + u32len -1) )
	{
		ret = flash_LenError;
	}
	
	if(ret == flash_OK)
	{
		for(u32i=0; u32i<u32len; u32i++)
		{
			u8readBuffer[u32i] = *(uint8*)(u32readAddr + u32i);
		}
	}
	
	return ret;
}

en_exflashStateType QuadSPI_Write(uint8_t *from, uint32_t to, uint32_t size, flashCbType cb)
{
	en_exflashStateType ret=flash_OK;
	uint8 u8writeStatus;
	uint8 *pu8inData;
	uint32 u32phyAddr,u32phyLen,u32phyAddrEnd;
	uint8 u8txData[256];
	uint32 u32txLen;
	uint32 u32i;
	
	if(to%2 != 0)
	{
		ret = flash_AddrError;
	}
	else
	{}
	
	if(size%2 != 0)
	{
		ret = flash_LenError;
	}
	else
	{}
	
	SetSoftwareQspiMode();

	if(ret == flash_OK)
	{
		pu8inData = from;
		/*calculate address and length in a single flash*/
		u32phyAddr = (to&0x00ffffff)/2;
		u32phyLen = size/2;
		u32phyAddrEnd = u32phyAddr + u32phyLen - 1;
		if(u32phyAddrEnd > W25Q64_END_ADDR)
		{
			ret = flash_AddrOverFlow;
		}
		else
		{}
	}
	else
	{}

	
	while( (ret == flash_OK) && (u32phyAddr <= u32phyAddrEnd) )
	{
		u32txLen = (uint32)256 - (u32phyAddr&0x000000ff);
		if( u32txLen > (u32phyAddrEnd - u32phyAddr + 1) )
		{
			u32txLen = (u32phyAddrEnd - u32phyAddr + 1);
		}
		else
		{}
		
		for(u32i=0; u32i<u32txLen; u32i++)
		{
			u8txData[u32i] = 0;
			u8txData[u32i] |= (pu8inData[u32i*2] & 0xf0);
			u8txData[u32i] |= (pu8inData[u32i*2+1] >> 4);
		}
		u8writeStatus = FL_QpageProgram(flashA, u32phyAddr, u8txData, u32txLen, cb);
		if(u8writeStatus == 1)
		{
			ret = flash_timeout;
		}
		else
		{}
		
		for(u32i=0; u32i<u32txLen; u32i++)
		{
			u8txData[u32i] = 0;
			u8txData[u32i] |= (pu8inData[u32i*2] << 4);
			u8txData[u32i] |= (pu8inData[u32i*2+1] & 0x0f);
		}
		u8writeStatus = FL_QpageProgram(flashB, u32phyAddr, u8txData, u32txLen, cb);
		if(u8writeStatus == 1)
		{
			ret = flash_timeout;
		}
		else
		{}
		
		pu8inData += u32txLen*2;
		u32phyAddr += u32txLen;
	}	
	
	SetHardwareQspiMode();
	return ret;
}

static en_exflashStateType FL_writeEnableLatch(en_flashID flash_ID, en_w25qInstruction cmd, flashCbType cb)
{
	en_exflashStateType ret=flash_OK;
	
	if(flash_ID == flashA)
	{
		SIUL_Output(FLA_CS, 0);
		SpiSendByte(flashA, cmd);
		SIUL_Output(FLA_CS, 1);
	}
	else if(flash_ID == flashB)
	{
		SIUL_Output(FLB_CS, 0);
		SpiSendByte(flashB, cmd);
		SIUL_Output(FLB_CS, 1);
	}
	else
	{}
	
	ret = FL_waitFlashIdel(flash_ID, cb);
	return ret;
}

static en_exflashStateType FL_writeStatusRegister(en_flashID flash_ID, uint8 u8R1, uint8 u8R2, flashCbType cb)
{
	en_exflashStateType ret=flash_OK;
	
	FL_writeEnableLatch(flash_ID, WR_EN, cb);
	if(flash_ID == flashA)
	{		
		SIUL_Output(FLA_CS, 0);
		SpiSendByte(flashA, WR_SR);
		SpiSendByte(flashA, u8R1);
		SpiSendByte(flashA, u8R2);
		SIUL_Output(FLA_CS, 1);
	}
	else if(flash_ID == flashB)
	{
		SIUL_Output(FLB_CS, 0);
		SpiSendByte(flashB, WR_SR);
		SpiSendByte(flashB, u8R1);
		SpiSendByte(flashB, u8R2);
		SIUL_Output(FLB_CS, 1);
	}
	else
	{}
	
	ret = FL_waitFlashIdel(flash_ID, cb);
	return ret;
}

static en_exflashStateType FL_readStatusRegister(en_flashID flash_ID, en_w25qInstruction cmd, uint8 *u8data)
{	
	en_exflashStateType ret=flash_OK;
	
	if(flash_ID == flashA)
	{
		SIUL_Output(FLA_CS, 0);
		SpiSendByte(flashA, cmd);
		SpiRevByte(flashA, u8data);
		SIUL_Output(FLA_CK, 1);
		SIUL_Output(FLA_CS, 1);
	}
	else if(flash_ID == flashB)
	{
		SIUL_Output(FLB_CS, 0);
		SpiSendByte(flashB, cmd);
		SpiRevByte(flashB, u8data);
		SIUL_Output(FLB_CK, 1);
		SIUL_Output(FLB_CS, 1);
	}
	else
	{}
	
	return ret;
}

static en_exflashStateType FL_PageProgram(en_flashID flash_ID, uint32 u32addr, uint8 *u8data, uint32 u32len, flashCbType cb)
{
	en_exflashStateType ret;
	uint32 u32i;
	
	FL_writeEnableLatch(flash_ID, WR_EN, cb);
	if(flash_ID == flashA)
	{
		SIUL_Output(FLA_CS, 0u);
		SpiSendByte(flashA, PG_PAGE);
		SpiSendByte(flashA, (uint8)(u32addr>>16));
		SpiSendByte(flashA, (uint8)(u32addr>>8));
		SpiSendByte(flashA, (uint8)(u32addr));
		for(u32i=0; u32i<u32len; u32i++)
		{
			SpiSendByte(flashA, *(u8data+u32i));
		}
		SIUL_Output(FLA_CS, 1u);
	}
	else if(flash_ID == flashB)
	{
		SIUL_Output(FLB_CS, 0u);
		SpiSendByte(flashB, PG_PAGE);
		SpiSendByte(flashB, (uint8)(u32addr>>16));
		SpiSendByte(flashB, (uint8)(u32addr>>8));
		SpiSendByte(flashB, (uint8)(u32addr));
		for(u32i=0; u32i<u32len; u32i++)
		{
			SpiSendByte(flashB, *(u8data+u32i));
		}
		SIUL_Output(FLB_CS, 1u);
	}
	else
	{}
	
	for(u32i=0; u32i<10; u32i++)
	{
		asm("nop");
	}
	ret = FL_waitFlashIdel(flash_ID, cb);
	return ret;
}


static en_exflashStateType FL_QpageProgram(en_flashID flash_ID, uint32 u32addr, uint8 *u8data, uint32 u32len, flashCbType cb)
{
	en_exflashStateType ret=flash_OK;
	uint32 u32i;
	
	SetSoftwareQspi();
	FL_writeEnableLatch(flash_ID, WR_EN, cb);
	if(flash_ID == flashA)
	{
		SIUL_Output(FLA_CS, 0u);
		SpiSendByte(flashA, PG_QPAGE);
		SpiSendByte(flashA, (uint8)(u32addr>>16));
		SpiSendByte(flashA, (uint8)(u32addr>>8));
		SpiSendByte(flashA, (uint8)(u32addr));
		for(u32i=0; u32i<u32len; u32i++)
		{
			QspiSendByte(flashA, *(u8data+u32i));
		}
		SIUL_Output(FLA_CS, 1u);
	}
	else if(flash_ID == flashB)
	{	
		SIUL_Output(FLB_CS, 0u);
		SpiSendByte(flashB, PG_QPAGE);
		SpiSendByte(flashB, (uint8)(u32addr>>16));
		SpiSendByte(flashB, (uint8)(u32addr>>8));
		SpiSendByte(flashB, (uint8)(u32addr));
		for(u32i=0; u32i<u32len; u32i++)
		{
			QspiSendByte(flashB, *(u8data+u32i));
		}
		SIUL_Output(FLB_CS, 1u);
	}
	else
	{}
	SetSoftwareSpi();
	
	for(u32i=0; u32i<10; u32i++)
	{
		asm("nop");
	}
	ret = FL_waitFlashIdel(flash_ID, cb);
	return ret;
}

static en_exflashStateType FL_eraseChip(en_flashID flash_ID, flashCbType cb)
{
	uint8 u8i;
	en_exflashStateType ret=flash_OK;
	
	SetSoftwareQspiMode();
	FL_writeEnableLatch(flash_ID, WR_EN, cb);
	if(flash_ID == flashA)
	{
		SIUL_Output(FLA_CS, 0);
		SpiSendByte(flashA, ER_CHIP1);
		SIUL_Output(FLA_CS, 1);
	}
	else if(flash_ID == flashB)
	{
		SIUL_Output(FLB_CS, 0);
		SpiSendByte(flashB, ER_CHIP1);
		SIUL_Output(FLB_CS, 1);
	}
	else
	{}
	
	for(u8i=0; u8i<10; u8i++)
	{
		asm("nop");
	}
	ret = FL_waitFlashIdel(flash_ID, cb);
	SetHardwareQspiMode();
	return ret;
}

static en_exflashStateType FL_waitFlashIdel(en_flashID flash_ID, flashCbType cb)
{
	uint8 u8status;
	en_exflashStateType ret=flash_OK;
	uint32 u32cnt=0;
	
	u8status = 0x01;
	while( (u8status & 0x01u) == 0x01u )
	{
		if(flash_ID == flashA)
		{		
			FL_readStatusRegister(flashA, RD_SR1, &u8status);
		}
		else if(flash_ID == flashB)
		{
			FL_readStatusRegister(flashB, RD_SR1, &u8status);
		}
		else
		{}
		
		if(cb != NULL)
		{
			cb();
		}
		
		u32cnt++;
		if(u32cnt > 0xfffffffe)
		{
			u8status = 0x0;
			ret = flash_timeout;
		}
	}
	return ret;
}


static void SpiSendByte(en_flashID flash_ID, uint8 u8data)
{
	uint8 u8temp;
	uint32 u32i;

	u8temp = u8data;
	if(flash_ID == flashA)
	{
		SIU.GPDO[FLA_CK].B.PDO = 1u;
		for(u32i=0; u32i<8; u32i++)
		{
			SIU.GPDO[FLA_CK].B.PDO = 0u;
			SIU.GPDO[FLA_DO].B.PDO = ((u8temp & 0x80) == 0x80 )?1 : 0;
			u8temp <<= 1u;
			SIU.GPDO[FLA_CK].B.PDO = 1u;
		}
	}
	else if(flash_ID == flashB)
	{
		SIU.GPDO[FLB_CK].B.PDO = 1u;
		for(u32i=0; u32i<8; u32i++)
		{
			SIU.GPDO[FLB_CK].B.PDO = 0u;
			SIU.GPDO[FLB_DO].B.PDO = ((u8temp & 0x80) == 0x80 )?1 : 0;
			u8temp <<= 1u;
			SIU.GPDO[FLB_CK].B.PDO = 1u;
		}
	}
	else
	{}
}

static void QspiSendByte(en_flashID flash_ID, uint8 u8data)
{
	uint8 u8temp;

	u8temp = u8data;
	if(flash_ID == flashA)
	{
		SIU.GPDO[FLA_CK].B.PDO = 1u;
		SIU.GPDO[FLA_CK].B.PDO = 0u;
		SIU.GPDO[FLA_D3].B.PDO = ((u8temp & 0x80) == 0x80 )?1 : 0;
		SIU.GPDO[FLA_D2].B.PDO = ((u8temp & 0x40) == 0x40 )?1 : 0;
		SIU.GPDO[FLA_D1].B.PDO = ((u8temp & 0x20) == 0x20 )?1 : 0;
		SIU.GPDO[FLA_D0].B.PDO = ((u8temp & 0x10) == 0x10 )?1 : 0;
		asm("nop");
		SIU.GPDO[FLA_CK].B.PDO = 1u;
		SIU.GPDO[FLA_CK].B.PDO = 0u;
		SIU.GPDO[FLA_D3].B.PDO = ((u8temp & 0x08) == 0x08 )?1 : 0;
		SIU.GPDO[FLA_D2].B.PDO = ((u8temp & 0x04) == 0x04 )?1 : 0;
		SIU.GPDO[FLA_D1].B.PDO = ((u8temp & 0x02) == 0x02 )?1 : 0;
		SIU.GPDO[FLA_D0].B.PDO = ((u8temp & 0x01) == 0x01 )?1 : 0;
		asm("nop");
		SIUL_Output(FLA_CK, 1u);
	}
	else if(flash_ID == flashB)
	{
		SIU.GPDO[FLB_CK].B.PDO = 1u;
		SIU.GPDO[FLB_CK].B.PDO = 0u;
		SIU.GPDO[FLB_D3].B.PDO = ((u8temp & 0x80) == 0x80 )?1 : 0;
		SIU.GPDO[FLB_D2].B.PDO = ((u8temp & 0x40) == 0x40 )?1 : 0;
		SIU.GPDO[FLB_D1].B.PDO = ((u8temp & 0x20) == 0x20 )?1 : 0;
		SIU.GPDO[FLB_D0].B.PDO = ((u8temp & 0x10) == 0x10 )?1 : 0;
		asm("nop");
		SIUL_Output(FLB_CK, 1u);
		SIUL_Output(FLB_CK, 0u);
		SIU.GPDO[FLB_D3].B.PDO = ((u8temp & 0x08) == 0x08 )?1 : 0;
		SIU.GPDO[FLB_D2].B.PDO = ((u8temp & 0x04) == 0x04 )?1 : 0;
		SIU.GPDO[FLB_D1].B.PDO = ((u8temp & 0x02) == 0x02 )?1 : 0;
		SIU.GPDO[FLB_D0].B.PDO = ((u8temp & 0x01) == 0x01 )?1 : 0;
		asm("nop");
		SIUL_Output(FLB_CK, 1u);
	}
	else
	{}
}

static void SpiRevByte(en_flashID flash_ID, uint8 *u8data)
{
	uint8 u8rev,u8temp;;
	uint32 u32i;
	
	u8rev = 0u;
	if(flash_ID == flashA)
	{
		SIU.GPDO[FLA_CK].B.PDO = 0u;
		for(u32i=0; u32i<8; u32i++)
		{
			SIU.GPDO[FLA_CK].B.PDO = 1u;
			asm("nop");
			u8temp = SIU.GPDI[FLA_DI].B.PDI;
			u8rev |= u8temp<<(7-u32i);
			SIU.GPDO[FLA_CK].B.PDO = 0u;
		}
		*u8data = u8rev;
	}
	else if(flash_ID == flashB)
	{
		SIU.GPDO[FLB_CK].B.PDO = 0u;
		for(u32i=0; u32i<8; u32i++)
		{
			SIU.GPDO[FLB_CK].B.PDO = 1u;
			asm("nop");
			u8temp = SIU.GPDI[FLB_DI].B.PDI;
			u8rev |= u8temp<<(7-u32i);
			SIU.GPDO[FLB_CK].B.PDO = 0u;
		}
		*u8data = u8rev;
	}
	else
	{}
}


/**
 * Deinitialize the QuadSPI
 *
 * @param none
 */
uint32_t QuadSPI_Deinit(void)
{

	/* Remove non-volatile quad bit, Write status register  */
	QUADSPI_0.SFAR.R = SET_FLASH_A;
	QuadSPI_FlashWriteEnable();
	QUADSPI_0.TBDR.R = (uint32_t)SR_WEL << 24u;
	QUADSPI_0.ICR.R = 0x00020001;
	while (QUADSPI_0.SFMSR.B.BUSY) {};

	/* Disable QSPI */
	QUADSPI_0.MCR.R = 0x000f4000uL;

	/* PCR[80] -> PCR[85] */
	SIU.PCR[80].R = 0x001; /* restore to reset state */
	SIU.PCR[81].R = 0x001; /* restore to reset state */
	SIU.PCR[82].R = 0x001; /* restore to reset state */
	SIU.PCR[83].R = 0x001; /* restore to reset state */
	SIU.PCR[84].R = 0x001; /* restore to reset state */
	SIU.PCR[85].R = 0x001; /* restore to reset state */
	
	SIU.PCR[76].R  = 0x001;  
	SIU.PCR[75].R  = 0x001; 
	SIU.PCR[122].R = 0x001; 
	SIU.PCR[120].R = 0x001; 
	SIU.PCR[118].R = 0x001; 
	SIU.PCR[119].R = 0x001; 

	return 0;
}



/**
 * Erase an area of the SPI flash
 *
 * @param start   start address of the erase
 * @param size    size of area to erase
 * @param cb      Callback function
 */
en_exflashStateType QuadSPI_Erase(uint32_t start, uint32_t size, flashCbType cb)
{
	en_exflashStateType ret=flash_OK;
	
	ret = FL_eraseChip(flashA, cb);
	if(ret == flash_OK)
	{
		ret = FL_eraseChip(flashB, cb);
	}
	else
	{}
	
	return ret;
}

#if 0
/**
 * Write up to one page in SPI flash
 *
 * @param from
 * @param to    Offset from 0
 * @param size
 */
static uint32_t QuadSPI_WritePage(uint32_t *from, uint32_t to, uint32_t size, flashCbType cb)
{
	uint32_t i = 0;
	uint32_t writeWords;
	uint32_t fromIndex = 0;

	QUADSPI_0.SFAR.R = to;

	QuadSPI_FlashWriteEnable();

	/* We have a 15 32bit word TX buffer, write up to 15 words into buffer */
	writeWords = MIN((size+3)/4, 15);

	for (i = 0; i < writeWords; i++) { /*16*/
		QUADSPI_0.TBDR.R = from[fromIndex++];
	}

	/* Execute write */
	QUADSPI_0.ICR.R = 0x00000002 | (size << 16u);

	/* Calculate rest to be sent */
	writeWords = (size + 3)/4 - writeWords;

	/* Continue to fill up to a page (page size can be 1 to 256 bytes) */
	for (i = 0; i < writeWords; i++) { /* rest of page or till end */
		while (QUADSPI_0.SFMSR.B.TXFULL) {}; /* busy wait */
		QUADSPI_0.TBDR.R = from[fromIndex++];
	}

	/* Wait for transmission to finish */
	do {
		/* Call the callback function if any */
		if (cb != NULL) {
			cb();
		}
	} while (QUADSPI_0.SFMSR.B.BUSY);

	/* Wait for the flash to finish operation */
	return QuadSPI_WaitWhileFlashBusy(cb);

}


/**
 * Write to SPI flash.
 *
 * @param from  Address to data to program
 * @param to    Program to this address.
 * @param size  Size in bytes. Must be a multiple of 4.
 * @param cb    Callback function
 */

uint32_t QuadSPI_Write(uint8_t *from, uint32_t to, uint32_t size, flashCbType cb)
{
	uint32_t res=0;
	uint32_t bytesLeft;
	uint32_t progBytes;
	uint32_t *fromPtr = (uint32_t*)from;

	QUADSPI_0.SFAR.R = SET_FLASH_A;
	bytesLeft = size;

	do {
		/* Program up to page boundary */
		if (bytesLeft > PAGE_SIZE) {
			progBytes = PAGE_SIZE;
		} else {
			progBytes = bytesLeft;
		}
		progBytes = MIN(bytesLeft, progBytes);

		/* Program a page at a time */
		res = QuadSPI_WritePage(fromPtr, to, progBytes, cb);
		if (res == 0) {
			to += progBytes;
			bytesLeft -= progBytes;
			fromPtr += progBytes/4; /* move number of 32bit words forward */

			/* Call the callback function if any */
			if (cb != NULL) {
				cb();
			}
		} else {
			return res;
		}
	} while (bytesLeft != 0uL);

	/* We are done, flush buffers */
	QUADSPI_0.MCR.R = QUADSPI_0.MCR.R | 0x00000c00UL;

	return 0;
}
#endif
