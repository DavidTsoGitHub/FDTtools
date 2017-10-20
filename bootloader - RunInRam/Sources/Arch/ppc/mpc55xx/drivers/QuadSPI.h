/* -------------------------------- Arctic Core ------------------------------
 * Copyright (C) 2009-2011 ArcCore AB <contact@arccore.com>
 * Licensed under ArcCore Embedded Software License Agreement.
 * -------------------------------- Arctic Core ------------------------------*/

#ifndef QSPI_H_
#define QSPI_H_
#include "Fls.h"
#include "SIUL.h"

#define FLA_CK		PF15
#define FLA_CS  	PF10
#define FLA_D0		PF13
#define FLA_D1		PF14
#define FLA_D2		PF11
#define FLA_D3		PF12
#define FLA_DO		PF13
#define FLA_DI		PF14
#define FLA_WP		PF11
#define FLA_HD		PF12

#define FLB_CK 		PJ14
#define FLB_CS		PJ13
#define FLB_D0		PF6
#define FLB_D1		PF5
#define FLB_D2		PK1
#define FLB_D3		PJ15
#define FLB_DO		PF6
#define FLB_DI		PF5
#define FLB_WP		PK1
#define FLB_HD		PJ15

typedef enum
{
	flash_OK = 0,
	flash_LenError,
	flash_AddrError,
	flash_AddrOverFlow,
	flash_timeout,
}en_exflashStateType;


typedef enum
{
	flashA = 0,
	flashB = 1
}en_flashID;

typedef enum
{
	WR_EN		= 0x06u,	/* wtite enable */
	WR_EnVSRQ	= 0x50u,	/* write enable for volatile status reg */
	WR_DIS		= 0x04u,	/* write disable */
	WR_SR		= 0x01u,	/* wtite status reg */
	
	RD_SR1		= 0x05u,	/* read status reg 1 */
	RD_SR2		= 0x35u,	/* read status reg 2 */
	
	PG_PAGE		= 0x02u,	/* page progame */
	PG_QPAGE	= 0x32u,	/* quad page progame */
	
	ER_SECTOR4	= 0x20u,	/* sector erase 4kb */
	ER_BLOCK32	= 0x52u, 	/* block erase 32kb */
	ER_BLOCK64	= 0xd8u, 	/* block erase 64kb */
	ER_CHIP1	= 0xc7u,	/* chip erase */
	ER_CHIP2	= 0x60u,	/* chip erase */
	EP_SUSPEND	= 0x75u,	/* Erase / Program Suspend */
	EP_RESUME 	= 0x7au,	/* Erase / Program Resume */

	POWER_DOWN	= 0xb9u,	/* Power-down */
	CRMR		= 0xffu, 	/* Continuous Read Mode Reset */
	
	RD_DATA		= 0x03u,	/* read data, only for SPI*/
	RD_FAST		= 0x0bu,	/* fast read */
	RD_FDO		= 0x3bu,	/* fast read dual output */
	RD_FQO		= 0x6bu,	/* fast read quad output */
	RD_FDIO		= 0xbbu,	/* fast read dual I/O */
	RD_FQIO		= 0xebu,	/* fast read quad I/O */
	RD_WQIO		= 0xe7u,	/* word read quad I/O */
	RD_OWQIO	= 0xe3u,	/* octal word read quad I/O */
	
	SBW			= 0x77u,	/* set burst with wrap */
	
}en_w25qInstruction;

#define FLASH_A_ADDR 0x70000000
#define FLASH_B_ADDR 0x78000000
#define SET_FLASH_A QUADSPI_0.SFAR.R = FLASH_A_ADDR
#define SET_FLASH_B QUADSPI_0.SFAR.R = FLASH_B_ADDR

#define WINBOND 1
#define SPANSION 2
#define FLASH_MANUFACTURER WINBOND

#define SECLECT_A  1
#define SECLECT_B  2
#define SECLECT_AB 3
#define MODE_SELCET SECLECT_AB

#define W25Q64_END_ADDR 0x007fffffuL
#define FLASH_VIRTAUL_ADDR_MAX 0x80ffffffuL
#define FLASH_VIRTAUL_ADDR_MIN 0x80000000uL

#define ADDR_FLASH_A                 0x70000000uL
#define ADDR_FLASH_B                 0x78000000uL

/* error reading the ID of the QSPI devices */
#define QSPI_ERR_ID					1
/* error setting CR  */
#define QSPI_ERR_CR					2
/* error reading SR  */
#define QSPI_ERR_SR					3
/* Timeout reading from chip */
#define QSPI_ERR_TIMEOUT			4

void QuadSPI_Reset(void);
uint32_t QuadSPI_Init(void);
uint32_t QuadSPI_Deinit(void);
en_exflashStateType QuadSPI_Erase(uint32_t start, uint32_t size, flashCbType cb);
en_exflashStateType QuadSPI_Write(uint8_t *from, uint32_t to, uint32_t size, flashCbType cb);

#endif

