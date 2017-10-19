/*****************************************************************/
/*                                                               */
/* FILE        : Display.c                                       */
/* DATE        : 2015-08-25                                      */
/* AUTHOR      : Zhu Mingqi                                      */
/* DESCRIPTION : source file of LCD display module               */
/* NOTE        : All Rights reserved by DETC, Technology Center  */
/*                                                               */
/*****************************************************************/
#include "Platform_Types.h"
#include "jdp.h"
#include "LCD.h"
#include "SIUL.h"

#include "IODefine.h"
#include "BSP.h"
#include "Display.h"

static uint8 u8s_DispReverse[256]={
//0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f,
  0x00,0x80,0x40,0xc0,0x20,0xa0,0x60,0xe0,0x10,0x90,0x50,0xd0,0x30,0xb0,0x70,0xf0,
//0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1a,0x1b,0x1c,0x1d,0x1e,0x1f,
  0x08,0x88,0x48,0xc8,0x28,0xa8,0x68,0xe8,0x18,0x98,0x58,0xd8,0x38,0xb8,0x78,0xf8,
//0x20,0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x2a,0x2b,0x2c,0x2d,0x2e,0x2f,
  0x04,0x84,0x44,0xc4,0x24,0xa4,0x64,0xe4,0x14,0x94,0x54,0xd4,0x34,0xb4,0x74,0xf4,
//0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x3a,0x3b,0x3c,0x3d,0x3e,0x3f,
  0x0c,0x8c,0x4c,0xcc,0x2c,0xac,0x6c,0xec,0x1c,0x9c,0x5c,0xdc,0x3c,0xbc,0x7c,0xfc,
//0x40,0x41,0x42,0x43,0x44,0x45,0x46,0x47,0x48,0x49,0x4a,0x4b,0x4c,0x4d,0x4e,0x4f,
  0x02,0x82,0x42,0xc2,0x22,0xa2,0x62,0xe2,0x12,0x92,0x52,0xd2,0x32,0xb2,0x72,0xf2,
//0x50,0x51,0x52,0x53,0x54,0x55,0x56,0x57,0x58,0x59,0x5a,0x5b,0x5c,0x5d,0x5e,0x5f,
  0x0a,0x8a,0x4a,0xca,0x2a,0xaa,0x6a,0xea,0x1a,0x9a,0x5a,0xda,0x3a,0xba,0x7a,0xfa,
//0x60,0x61,0x62,0x63,0x64,0x65,0x66,0x67,0x68,0x69,0x6a,0x6b,0x6c,0x6d,0x6e,0x6f,
  0x06,0x86,0x46,0xc6,0x26,0xa6,0x66,0xe6,0x16,0x96,0x56,0xd6,0x36,0xb6,0x76,0xf6,
//0x70,0x71,0x72,0x73,0x74,0x75,0x76,0x77,0x78,0x79,0x7a,0x7b,0x7c,0x7d,0x7e,0x7f,
  0x0e,0x8e,0x4e,0xce,0x2e,0xae,0x6e,0xee,0x1e,0x9e,0x5e,0xde,0x3e,0xbe,0x7e,0xfe,
//0x80,0x81,0x82,0x83,0x84,0x85,0x86,0x87,0x88,0x89,0x8a,0x8b,0x8c,0x8d,0x8e,0x8f,
  0x01,0x81,0x41,0xc1,0x21,0xa1,0x61,0xe1,0x11,0x91,0x51,0xd1,0x31,0xb1,0x71,0xf1,
//0x90,0x91,0x92,0x93,0x94,0x95,0x96,0x97,0x98,0x99,0x9a,0x9b,0x9c,0x9d,0x9e,0x9f,
  0x09,0x89,0x49,0xc9,0x29,0xa9,0x69,0xe9,0x19,0x99,0x59,0xd9,0x39,0xb9,0x79,0xf9,
//0xa0,0xa1,0xa2,0xa3,0xa4,0xa5,0xa6,0xa7,0xa8,0xa9,0xaa,0xab,0xac,0xad,0xae,0xaf,
  0x05,0x85,0x45,0xc5,0x25,0xa5,0x65,0xe5,0x15,0x95,0x55,0xd5,0x35,0xb5,0x75,0xf5,
//0xb0,0xb1,0xb2,0xb3,0xb4,0xb5,0xb6,0xb7,0xb8,0xb9,0xba,0xbb,0xbc,0xbd,0xbe,0xbf,
  0x0d,0x8d,0x4d,0xcd,0x2d,0xad,0x6d,0xed,0x1d,0x9d,0x5d,0xdd,0x3d,0xbd,0x7d,0xfd,
//0xc0,0xc1,0xc2,0xc3,0xc4,0xc5,0xc6,0xc7,0xc8,0xc9,0xca,0xcb,0xcc,0xcd,0xce,0xcf,
  0x03,0x83,0x43,0xc3,0x23,0xa3,0x63,0xe3,0x13,0x93,0x53,0xd3,0x33,0xb3,0x73,0xf3,
//0xd0,0xd1,0xd2,0xd3,0xd4,0xd5,0xd6,0xd7,0xd8,0xd9,0xda,0xdb,0xdc,0xdd,0xde,0xdf,
  0x0b,0x8b,0x4b,0xcb,0x2b,0xab,0x6b,0xeb,0x1b,0x9b,0x5b,0xdb,0x3b,0xbb,0x7b,0xfb,
//0xe0,0xe1,0xe2,0xe3,0xe4,0xe5,0xe6,0xe7,0xe8,0xe9,0xea,0xeb,0xec,0xed,0xee,0xef,
  0x07,0x87,0x47,0xc7,0x27,0xa7,0x67,0xe7,0x17,0x97,0x57,0xd7,0x37,0xb7,0x77,0xf7,
//0xf0,0xf1,0xf2,0xf3,0xf4,0xf5,0xf6,0xf7,0xf8,0xf9,0xfa,0xfb,0xfc,0xfd,0xfe,0xff
  0x0f,0x8f,0x4f,0xcf,0x2f,0xaf,0x6f,0xef,0x1f,0x9f,0x5f,0xdf,0x3f,0xbf,0x7f,0xff
};

static uint8 u8g_FrameBuffer[30u * 320u]= {0x00u};
static uint32 u32s_RunCnt;

static void ECU_Matrix_TxCmd(uint8 u8t_Cmd);
static void ECU_Matrix_TxData(uint8 u8t_Data);
static void ECU_Matrix_SetAddr(uint8 u8_StartAddrH, uint8 u8_StartAddrL, uint8 u8_EndAddrH, uint8 u8_EndAddrL);
static void ECU_Matrix_SetPage(uint8 u8_PageStartAddr, uint8 u8_PageEndAddr);

static void delayms(uint16 value);


void delayms(uint16 value)
{
	uint16 i;
	for (i=0u;i<value;i++)
	{
		__asm(" nop");
	}
}

/*
 * Name         : ECU_SegmentLcd_Open
 * Description  : Open segment display
 * Arguments    : void
 * Return       : void                  
 */
void ECU_SegmentLcd_Open(void) 
{
	DISPLAY_ON();
}

/*
 * Name         : ECU_SegmentLcd_Close
 * Description  : close segment display
 * Arguments    : void
 * Return       : void         
 */
void ECU_SegmentLcd_Close(void) 
{
	DISPLAY_OFF();
}

/*
 * Name         : ECU_SegmentLcd_Clear
 * Description  : clear segment display
 * Arguments    : void
 * Return       : void                  
 */
void ECU_SegmentLcd_Clear(void) 
{
	MCU_SegmentLcd_Clear();
}

/*
 * Name         : ECU_SegmentLcd_Fill 
 * Description  : display all segment characters
 * Arguments    : void
 * Return       : void                  
 */
void ECU_SegmentLcd_Fill(void)
{
	MCU_SegmentLcd_Fill();
}

#if 0
/*
 * Name         : BSP_SetSegmentDigit
 * Description  : send the specified characters to LCD
 * Arguments    : u8digitIndex - indicate which digit that shall be updated
 * 				  u8bitArray   - will hold the correct bit value for the digit.
 * Return       : void
 * Notes        : void
 */
void BSP_SetSegmentDigit(uint8 u8digitIndex, uint8 u8bitArray)
{
	uint8 u8t_Data0, u8t_Data1;
	
	switch(u8digitIndex)
	{
	case 1u:
		/*first char*/
		u8t_Data0 = (u8bitArray & 0xf0uL) >> 4;
		u8t_Data1 = (u8bitArray & 0x0fuL);
		
		SetSegmentLCD3(0, u8t_Data1);
		SetSegmentLCD2(0, u8t_Data0);
		break;
	case 2u:
		/*second char*/
		u8t_Data0 = (u8bitArray & 0xf0uL) >> 4;
		u8t_Data1 = (u8bitArray & 0x0fuL);
		
		SetSegmentLCD1(0, u8t_Data1);
		SetSegmentLCD0(0, u8t_Data0);

		break;
	case 3u:
		/*third char*/
		u8t_Data0 = (u8bitArray & 0xf0uL) >> 4;
		u8t_Data1 = (u8bitArray & 0x0fuL);
		
		SetSegmentLCD3(1, u8t_Data1);
		SetSegmentLCD2(1, u8t_Data0);
		break;
	case 4u:
		/*forth char*/
		u8t_Data0 = (u8bitArray & 0xf0uL) >> 4;
		u8t_Data1 = (u8bitArray & 0x0fuL);
		
		SetSegmentLCD1(1, u8t_Data1);
		SetSegmentLCD0(1, u8t_Data0);
		break;
	case 5u:
		/*fifth char*/
		u8t_Data0 = (u8bitArray & 0xf0uL) >> 4;
		u8t_Data1 = (u8bitArray & 0x0fuL);
		
		SetSegmentLCD3(2, u8t_Data1);
		SetSegmentLCD2(2, u8t_Data0);
		break;
	case 6u:
		/*sixth char*/
		u8t_Data0 = (u8bitArray & 0xf0uL) >> 4;
		u8t_Data1 = (u8bitArray & 0x0fuL);
		
		SetSegmentLCD1(2, u8t_Data1);
		SetSegmentLCD0(2, u8t_Data0);
		break;
	default:
		break;
	}
}
#endif
/*
 * Name         : BSP_Set_DisplaySegments
 * Description  : send the specified characters to LCD
 * Arguments    : Segment - the pointer to characters
 * Return       : void
 * Notes        : The function needs to be optimised
 */
void BSP_Set_DisplaySegments(const uint8* Segments, uint8 ArrayLength)
{
	uint8 temp;
	uint8 u8t_Data0, u8t_Data1, u8t_Data2, u8t_Data3;
    uint8 i;

    i = 0;

	/*first char*/
	temp = Segments[0];
	u8t_Data2 = (temp & 0xf0uL) >> 4;
	u8t_Data3 = (temp & 0x0fuL);
	
	/*second char*/
	temp = Segments[1];
	u8t_Data0 = (temp & 0xf0uL) >> 4;
	u8t_Data1 = (temp & 0x0fuL);
	
	SetSegmentLCD3(0, u8t_Data3);
	SetSegmentLCD2(0, u8t_Data2);
	SetSegmentLCD1(0, u8t_Data1);
	SetSegmentLCD0(0, u8t_Data0);
	
	/*third char*/
	temp = Segments[2];
	u8t_Data2 = (temp & 0xf0uL) >> 4;
	u8t_Data3 = (temp & 0x0fuL);
	
	/*forth char*/
	temp = Segments[3];
	u8t_Data0 = (temp & 0xf0uL) >> 4;
	u8t_Data1 = (temp & 0x0fuL);
	
	SetSegmentLCD3(1, u8t_Data3);
	SetSegmentLCD2(1, u8t_Data2);
	SetSegmentLCD1(1, u8t_Data1);
	SetSegmentLCD0(1, u8t_Data0);
	
	/*fifth char*/
	temp = Segments[4];
	u8t_Data2 = (temp & 0xf0uL) >> 4;
	u8t_Data3 = (temp & 0x0fuL);
	
	/*sixth char*/
	temp = Segments[5];
	u8t_Data0 = (temp & 0xf0uL) >> 4;
	u8t_Data1 = (temp & 0x0fuL);
	
	SetSegmentLCD3(2, u8t_Data3);
	SetSegmentLCD2(2, u8t_Data2);
	SetSegmentLCD1(2, u8t_Data1);
	SetSegmentLCD0(2, u8t_Data0);
}

/*
 * Name        : ECU_Matrix_TxCmd
 * Description : send command to Matrix display
 * Arguments   : u8t_Cmd : command
 * Return      : none
 */
static void ECU_Matrix_TxCmd(uint8 u8t_Cmd)
{

	SIUL_Output(LCD_CS_OUTPUT, OFF);  /* pull down CS to enable input/output for data/command */
	SIUL_Output(LCD_A0_OUTPUT, OFF);  /* Set 0 to A0 : PA0-PA7 is command output */							
	SIUL_Output(LCD_RD_OUTPUT, ON);				
	SIUL_Output(LCD_WR_OUTPUT, OFF);

	/* send out command bit by bit, PA0 - PA7 */
	SIUL_POutput0(0, u8t_Cmd);

	SIUL_Output(LCD_WR_OUTPUT,ON);
	SIUL_Output(LCD_CS_OUTPUT,ON);    /* set 1 to CS, the data bus 表示数据总线高阻抗*/
						
}

/*
 * Name        : ECU_Matrix_TxData
 * Description : send data to Matrix display
 * Arguments   : u8t_Cmd : command
 * Return      : none
 */
static void ECU_Matrix_TxData(uint8 u8t_Data)
{
	SIUL_Output(LCD_CS_OUTPUT, OFF);   /* pull down CS to enable input/output for data/command */
	SIUL_Output(LCD_A0_OUTPUT,  ON);   /* Set 1 to A0 : PA0-PA7 is data or command parameter output */
	SIUL_Output(LCD_RD_OUTPUT,  ON);					
	SIUL_Output(LCD_WR_OUTPUT, OFF);

	/* send out data bit by bit, PA0 - PA7 */
	SIUL_POutput0(0, u8t_Data);

    SIUL_Output(LCD_WR_OUTPUT, ON);
}
 
/*
 * Name        : ECU_Matrix_SetAddr
 * Description : set column address
 * Arguments   : u8_StartAddrH - high 8 bit of start address
 *               u8_StartAddrL - low  8 bit of start address
 *               u8_EndAddrH   - high 8 bit of end address
 *               u8_EndAddrL   - low  8 bit of end address
 * Return      : none
 */
static void ECU_Matrix_SetAddr(uint8 u8_StartAddrH, uint8 u8_StartAddrL, uint8 u8_EndAddrH, uint8 u8_EndAddrL)
{
	ECU_Matrix_TxCmd(0xc4u);           /* send 0x23 command, i.e column setting command */
	ECU_Matrix_TxData(u8_StartAddrH);  /* high 8 bit of start address */
	ECU_Matrix_TxData(u8_StartAddrL);  /* low  8 bit of start address */
	ECU_Matrix_TxData(u8_EndAddrH);    /* high 8 bit of end address */
	ECU_Matrix_TxData(u8_EndAddrL);    /* low  8 bit of end address */     
}

/*
 * Name        : ECU_Matrix_SetAddr
 * Description : set page
 * Arguments   : u8_StartAddrH - high 8 bit of start address
 *               u8_StartAddrL - low  8 bit of start address
 *               u8_EndAddrH   - high 8 bit of end address
 *               u8_EndAddrL   - low  8 bit of end address
 * Return      : none
 */
static void ECU_Matrix_SetPage(uint8 u8_PageStartAddr, uint8 u8_PageEndAddr)
{
	ECU_Matrix_TxCmd(0x44u);             /* page set command */
	ECU_Matrix_TxData(u8_PageStartAddr); /* page1 start address*/
	ECU_Matrix_TxData(u8_PageEndAddr);   /* page2 end address*/	
	ECU_Matrix_TxData(0x00u);            /* P3 frame address P30-31*/
	ECU_Matrix_TxData(0xA5u);            /* P4 dummy */
}

/*
 * Name        : ECU_MatrixLcd_Clear
 * Description : erase all the matrix display, the whole display is white
 * Arguments   : none
 * Return      : none
 */
void ECU_MatrixLcd_Clear(void)
{
	uint32 i;

	ECU_Matrix_SetPage(0u, 0xb8u);
	ECU_Matrix_SetAddr(0x00u, 0x00u, 0x80u, 0xfcu);
	ECU_Matrix_TxCmd(0x5cu);
	ECU_Matrix_TxData(0xA5u);

	for(i = 0u; i < 9600u; i++)   /* 9600 = (240/8) * 320 */
	{
		ECU_Matrix_TxData(0x00u);
	}
}

/*
 * Name        : ECU_MatrixLcd_Fill
 * Description : set all the matrix display, the whole display is black
 * Arguments   : none
 * Return      : none
 */
void ECU_MatrixLcd_Fill(void)
{
	uint32 i;

	ECU_Matrix_SetPage(0u, 0xb8u);
	ECU_Matrix_SetAddr(0x00u, 0x00u, 0x80u, 0xfcu);
	ECU_Matrix_TxCmd(0x5cu);	
	ECU_Matrix_TxData(0xA5u);
	for(i = 0u; i < 9600u; i++)   /* 9600 = (240/8) * 320 */
	{
		ECU_Matrix_TxData(0xFFu);
	}
}

/*
 * Name        : ECU_MatrixLcd_Open
 * Description : open matrix display
 * Arguments   : none
 * Return      : none
 */
void ECU_MatrixLcd_Open(void) 
{
	ECU_Matrix_TxCmd(0x48u);    /* Display ON */
	ECU_Matrix_TxData(0xA5u);
}

/*
 * Name        : ECU_MatrixLcd_Close
 * Description : close matrix display
 * Arguments   : none
 * Return      : none
 */
void ECU_MatrixLcd_Close(void) 
{
   ECU_Matrix_TxCmd(0x88u);	/* Display OFF */
    ECU_Matrix_TxData(0xA5u);	
}

/*
 * Name        : ECU_MatrixLcd_Init
 * Description : Initialize matrix display
 * Arguments   : none
 * Return      : none
 */
void ECU_MatrixLcd_Init(void)
{
	uint16 i;

	/*initial all the used pins*/
	SIUL_Output(LCD_BL,         OFF);    /* close illumination */
	SIUL_Output(LCD_RES_OUTPUT, ON);
	SIUL_Output(LCD_CS_OUTPUT,  ON);
	SIUL_Output(LCD_A0_OUTPUT,  OFF);
	SIUL_Output(LCD_RD_OUTPUT,  ON);
	SIUL_Output(LCD_WR_OUTPUT,  OFF);

	/* delay */
	for(i=0;i<50000u;i++)
	{
		__asm(" nop");
	}
	for(i=0;i<50000u;i++)
	{
		__asm(" nop");
	}

	/* Hardware reset */		
	SIUL_Output(LCD_RES_OUTPUT,OFF);
	delayms(50);	
	SIUL_Output(LCD_RES_OUTPUT,ON);
	delayms(100);
	
	/* Software reset */
	ECU_Matrix_TxCmd(0x5Eu);
	ECU_Matrix_TxData(0xA5u);	
	delayms(50);
	
	/* DISAR (Display Area: 91h) */
	ECU_Matrix_TxCmd(0x89u);
	ECU_Matrix_TxData(0x00u); /* Parameter1: number of display lines (MSB) */
	ECU_Matrix_TxData(0xf7u); /* Parameter2: number of display lines (LSB) 120 lines */
	ECU_Matrix_TxData(0x80u); /* Parameter3: number of display columns (MSB) */
	ECU_Matrix_TxData(0xfcu); /* Parameter4: number of display columns (LSB)  276 column */
	
	/* DISSET1 (Display Set 1: 92h) */
	ECU_Matrix_TxCmd(0x49u);
	ECU_Matrix_TxData(0Xa2u); /* Parameter1: number of clocks in 1H, 45H = 70clocks */
	ECU_Matrix_TxData(0x40u); /* Parameter2: number of back porch set “number of back porch” – 1.2 porch*/
	ECU_Matrix_TxData(0x00u); /* Parameter3: LCD type setting, OSCO output setting, number of front porch (MSB) */
	ECU_Matrix_TxData(0x80u); /* Parameter4: number of front porch(LSB)set number of front porch lines -1.1porch */

	/* DISSET2 (Display Set 2: 93h) */
	ECU_Matrix_TxCmd(0xc9u);	
	ECU_Matrix_TxData(0x50u); /* Parameter1: Source output ON timing  source output at the 10th clock */
	ECU_Matrix_TxData(0x9cu); /* Parameter2: Source output OFF timing source output stop at the 57th clock */
	ECU_Matrix_TxData(0x30u); /* Parameter3: Gate output ON timing, Gate output at the 12th clock */
	ECU_Matrix_TxData(0xecu); /* Parameter4: Gate output OFF timing   Gate output stop at the 55th clock */

	/* (Memory Address Control: 21h) */
	ECU_Matrix_TxCmd(0x84u);
	ECU_Matrix_TxData(0x60u); /* Parameter1: Display data RAM control Increment column address direction */
	ECU_Matrix_TxData(0xA5u); /* Parameter2: reserved */
	ECU_Matrix_TxData(0xA5u); /* Parameter3: reserved */
	ECU_Matrix_TxData(0xA5u); /* Parameter4: reserved */

	/* STFRAME (Start Frame Address: 24h) */
	ECU_Matrix_TxCmd(0x24u);	
	ECU_Matrix_TxData(0x00u); /* Parameter1: start frame address - 0 */
	ECU_Matrix_TxData(0xA5u); /* Parameter2: reserved */
	ECU_Matrix_TxData(0xA5u); /* Parameter3: reserved */
	ECU_Matrix_TxData(0xA5u); /* Parameter4: reserved */

	/* BPPSEL (Bpp Sel: 25h) */
	ECU_Matrix_TxCmd(0xa4u);
	ECU_Matrix_TxData(0x00u); /* Parameter 1: Data format */
	ECU_Matrix_TxData(0xA5u); /* Parameter2: reserved */
	ECU_Matrix_TxData(0xA5u); /* Parameter3: reserved */
	ECU_Matrix_TxData(0xA5u); /* Parameter4: reserved */

	/* MDCTL (Memory Data Control: 26h) */
	ECU_Matrix_TxCmd(0x64u);	
	ECU_Matrix_TxData(0x00u); /* Parameter1: Data mask option no bit operation */
	ECU_Matrix_TxData(0x00u); /* Parameter2: Mask at start page address */
	ECU_Matrix_TxData(0x00u); /* Parameter3: Mask at end page address */
	ECU_Matrix_TxData(0xA5u); /* Parameter4: Dummy */	

	/* Command GATESET */
	ECU_Matrix_TxCmd(0x45u);
	ECU_Matrix_TxData(0x00u); /* Parameter1: Scan mode 2side Interlace driving 1, Normal scan direction */
	ECU_Matrix_TxData(0xA5u); /* Parameter2: reserved */
	ECU_Matrix_TxData(0xA5u); /* Parameter3: reserved */
	ECU_Matrix_TxData(0xA5u); /* Parameter4: reserved */

	/* ACSET */
	ECU_Matrix_TxCmd(0x85u);
	ECU_Matrix_TxData(0x00u); /* Parameter1: 1 line inversion driving */
	ECU_Matrix_TxData(0xA5u); /* Parameter2: reserved */
	ECU_Matrix_TxData(0xA5u); /* Parameter3: reserved */
	ECU_Matrix_TxData(0xA5u); /* Parameter4: reserved */

	/* GAMSET1 (Gamma Set 1bpp: 86h) */
	ECU_Matrix_TxCmd(0x61u);
	ECU_Matrix_TxData(0x00u); /*Parameter1: V0 voltage setting (positive polarity) V0 */
	ECU_Matrix_TxData(0xfcu); /*Parameter2: V1 voltage setting (positive polarity) V1 */
	ECU_Matrix_TxData(0x00u); /*Parameter3: V0 voltage setting (Negative polarity) V0 */	
	ECU_Matrix_TxData(0xfcu); /*Parameter4: V1 voltage setting (Negative polarity) V1 */

	/* Electronic Volume Set 1 */
	ECU_Matrix_TxCmd(0x8eu);
	ECU_Matrix_TxData(0x70u); /* Parameter1: VDDHS P10-P13 0-FH 4.1-5.6V Step 0.1V 0x0E is 5.5V (VDDHS) */
	ECU_Matrix_TxData(0X7Eu); /* Parameter2: VCOMH P20-P27 00-98H 2.46-5.5V Step 0.02V 0x71 is 4.98V  */
	ECU_Matrix_TxData(0x68u); /* Parameter3: VCA P30-P34  00-1EH 1.50-3.00V Step is 0.05V 0x16 is 2.6V */
	ECU_Matrix_TxData(0x28u); /* Parameter4: VDDRH P40-P44 00-19H  3.00-5.50V Step is 0.1V 0x11 is 5.0V(VDDRH) */	

	/* Electronic Volume Set 2 */
	ECU_Matrix_TxCmd(0x4eu);
	ECU_Matrix_TxData(0xd0u); /* Parameter1: VOFREG P10-P14 00-1FH 2.4-5.5V Step 0.1V 0x0B is 3.5V(VOFREG) */
	ECU_Matrix_TxData(0x44u); /* Parameter2: VONREG P20-P25 01-24H 1.5-4.5V Step 0.1V 0x22 is 4.8V */
	ECU_Matrix_TxData(0xA5u); /* Parameter3: reserved */
	ECU_Matrix_TxData(0xA5u); /* Parameter4: reserved */

	/* Command PWRCTL */
	ECU_Matrix_TxCmd( 0x86);	
	ECU_Matrix_TxData( 0xfa); /* Parameter1: Booster circuit control, VCOMH and VCOML control 3rd booster *(-2)*/
	ECU_Matrix_TxData( 0x90); /* Parameter2: Source amp, setting, display wait & VCOMH regulator setting */
	ECU_Matrix_TxData( 0x00); /* Parameter3: Booster clock frequency setting in normal display */
	ECU_Matrix_TxData( 0x00); /* Parameter4: Booster clock frequency setting in partial display */

	/* VCOMH Offset Data */
	ECU_Matrix_TxCmd(0x92u);
	ECU_Matrix_TxData(0x00u); /* No offset adjustment of VCOMH */
	ECU_Matrix_TxData(0x00u);
	ECU_Matrix_TxData(0x00u);
	ECU_Matrix_TxData(0xA5u);

	/*delay more than 1ms*/
	 delayms(10);

	/* SLEEP OUT */
	ECU_Matrix_TxCmd(0xc8u);
	ECU_Matrix_TxData(0xA5u); 
	delayms(10);
		
	/* Column Address Set SOURCE66-SOURCE255 0-0x13F */
	ECU_Matrix_TxCmd(0xc4u);
	ECU_Matrix_TxData(0x00u); /* Parameter1: Start address (MSB) P10 */
	ECU_Matrix_TxData(0x00u); /* Parameter2: Start address (LSB)P20-P27 */
	ECU_Matrix_TxData(0x80u); /* Parameter3: End address (MSB)P30 */
	ECU_Matrix_TxData(0xfcu); /* Parameter4: End address (LSB)P40-P47 */

	/* PASET (Page Address Set: 22h) Gate1-Gate94 1BIT/PIXEL */
	ECU_Matrix_TxCmd(0x44u);
	ECU_Matrix_TxData(0u);    /* Parameter 1(P1) start address */
	ECU_Matrix_TxData(0xb8u);   /* Parameter 2(P2) end address */
	ECU_Matrix_TxData(0x00u); /* Parameter 3(P3) frame address P30-31 */
	ECU_Matrix_TxData(0xA5u); /* Parameter 4(P4) dummy */

	/* Display ON */
	ECU_Matrix_TxCmd(0x48u);	
	ECU_Matrix_TxData(0xA5u);

	ECU_Matrix_TxCmd(0x5cu);
	ECU_Matrix_TxData(0xA5u);

	/* Display OFF */
	ECU_Matrix_TxCmd(0x88u);
	ECU_Matrix_TxData(0xA5u);

	ECU_MatrixLcd_Clear();
}

/*
 * Name        : BSP_Get_Framebuffer
 * Description : get frame buffer from matrix display module
 * Arguments   : none
 * Return      : none
 */
void BSP_Get_Framebuffer(uint8 **FrameBuffer)  
{
	*FrameBuffer = u8g_FrameBuffer;
}

/*
 * Name        : BSP_Get_Framebuffer
 * Description : update frame buffer 
 * Arguments   : none
 * Return      : none
 */
void BSP_Service_FramebufferUpdated(void)              
{
#if 0
	uint32 i;
	/* location */
	ECU_Matrix_SetPage(0, 0xb8u);
	ECU_Matrix_SetAddr(0u, 0u,0x80u, 0xfcu);
	ECU_Matrix_TxCmd(0x5cu);	
	ECU_Matrix_TxData(0xA5u);

	for(i = 0u; i < 9600u; i++)    /* 9600 = (240/8) * 320*/
	{
		ECU_Matrix_TxData(u8s_DispReverse[u8g_FrameBuffer[i]]);
	}
#endif
}

void ECU_FramebufferUpdated_Init(void)
{
	u32s_RunCnt = 0;
}

void ECU_FramebufferUpdated_Runnable(void)
{
	uint32 i, u32t_StartAddr, u32t_OverAddr;
	u32t_StartAddr = u32s_RunCnt * 0x20u;
	u32t_OverAddr = (u32s_RunCnt + 1u) * 0x20u - 1u;
	
	ECU_Matrix_SetPage(0, 0xb8u);
	ECU_Matrix_SetAddr(u8s_DispReverse[u32t_StartAddr / 0x100u],
	u8s_DispReverse[u32t_StartAddr % 0x100u], 
	u8s_DispReverse[u32t_OverAddr / 0x100u], 
	u8s_DispReverse[u32t_OverAddr % 0x100u]);
	
	ECU_Matrix_TxCmd(0x5cu);	
	ECU_Matrix_TxData(0xA5u);
	
	
	for(i = (u32s_RunCnt * 960u); i < ((u32s_RunCnt + 1u) *960u); i++)
	{
		ECU_Matrix_TxData(u8s_DispReverse[u8g_FrameBuffer[i]]);
	}
	
	u32s_RunCnt++;
	if (u32s_RunCnt == 10)
	{
		u32s_RunCnt = 0;
	}
}

BSP_StdReturnType BSP_Service_DisplayTest(BSP_TestCommand Command)
{
    /* TODO: to be implemented */
}