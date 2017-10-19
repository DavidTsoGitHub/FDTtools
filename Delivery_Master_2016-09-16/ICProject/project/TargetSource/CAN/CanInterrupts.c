//lint -save
//lint -e923 [Supression of conversion from platform register to uint32]
//lint -e9078 [Supression of conversion from platform register to uint32]

#include "CanInterrupts.h"
#include "Platform_Types.h"
#include "CanDrv5606S.h"
#include "jdp.h"

#define BIT_SCAN_LEFT(out,in) __asm ("cntlzw %0,%1" : "=r" (out) : "r" (in))

void canA_MB00_03int(void)
{
	uint8 u8Mb;
	uint32 u32Flags1 = (CAN_0.IFRL.R) & 0x0000000Fu;
	uint32 u32TmpFlags = u32Flags1;
	uint32 u32Lz = 0u; //Number of leading zeros
	while(u32TmpFlags > 0u)
	{
		BIT_SCAN_LEFT(u32Lz,u32TmpFlags); //detect which bit is set
		if(u32Lz > 31u) 
		{
			break;
		}
		u8Mb = (uint8)(31u-u32Lz); //get the mailbox number
		CAN_ISR(0, u8Mb);
		u32TmpFlags &= ~((uint32)0x00000001u << u8Mb); //consume the bit
	}
	CAN_0.IFRL.R = u32Flags1; // Write 1s to clear interrupt flag register
}

void canA_MB04_07int(void)
{
	uint8 u8Mb;
	uint32 u32Flags1 = (CAN_0.IFRL.R) & 0x000000F0u;
	uint32 u32TmpFlags = u32Flags1;
	uint32 u32Lz = 0u; //Number of leading zeros
	while(u32TmpFlags > 0u)
	{
		BIT_SCAN_LEFT(u32Lz,u32TmpFlags); //detect which bit is set
		if(u32Lz > 27u) 
		{
			break;
		}
		u8Mb = (uint8)(31u-u32Lz); //get the bit number (16-31)
		CAN_ISR(0, u8Mb);
		u32TmpFlags &= ~((uint32)0x00000001u << u8Mb); //consume the bit
	}
	CAN_0.IFRL.R = u32Flags1; // Write 1s to clear interrupt flag register
}

void canA_MB08_11int(void)
{
	uint8 u8Mb;
	uint32 u32Flags1 = (CAN_0.IFRL.R) & 0x00000F00u;
	uint32 u32TmpFlags = u32Flags1;
	uint32 u32Lz = 0u; //Number of leading zeros
	while(u32TmpFlags > 0u)
	{
		BIT_SCAN_LEFT(u32Lz,u32TmpFlags); //detect which bit is set
		if(u32Lz > 23u) 
		{
			break;
		}
		u8Mb = (uint8)(31u-u32Lz); //get the bit number (16-31)
		CAN_ISR(0, u8Mb);
		u32TmpFlags &= ~((uint32)0x00000001u << u8Mb); //consume the bit
	}
	CAN_0.IFRL.R = u32Flags1; // Write 1s to clear interrupt flag register
}

void canA_MB12_15int(void)
{
	uint8 u8Mb;
	uint32 u32Flags1 = (CAN_0.IFRL.R) & 0x0000F000u;
	uint32 u32TmpFlags = u32Flags1;
	uint32 u32Lz = 0; //Number of leading zeros
	while(u32TmpFlags > 0u)
	{
		BIT_SCAN_LEFT(u32Lz,u32TmpFlags); //detect which bit is set
		if(u32Lz > 19u) 
		{
			break;
		}
		u8Mb = (uint8)(31u-u32Lz); //get the bit number (16-31)
		CAN_ISR(0, u8Mb);
		u32TmpFlags &= ~((uint32)0x00000001u << u8Mb); //consume the bit
	}
	CAN_0.IFRL.R = u32Flags1; // Write 1s to clear interrupt flag register
}

void canA_MB16_31int(void)
{
	uint8 u8Mb;
	uint32 u32Flags1 = (CAN_0.IFRL.R) & 0xFFFF0000u; //We are not interested in MB0-MB15
	uint32 u32TmpFlags = u32Flags1;
	uint32 u32Lz = 0u; //Number of leading zeros
	while(u32TmpFlags > 0u)
	{
		BIT_SCAN_LEFT(u32Lz,u32TmpFlags); //detect which bit is set
		if(u32Lz > 15u) 
		{
			break;
		}
		u8Mb = (uint8)(31u-u32Lz); //get the bit number (16-31)
		CAN_ISR(0, u8Mb);
		u32TmpFlags &= ~((uint32)0x00000001u << u8Mb); //consume the bit
	}
	CAN_0.IFRL.R = u32Flags1; // Write 1s to clear interrupt flag register
}
void canA_MB32_63int(void)
{
	uint8 u8Mb;
	uint32 u32Flags2 = CAN_0.IFRH.R;
	uint32 u32TmpFlags = u32Flags2;
	uint32 u32Lz = 0u; //Number of leading zeros
	while(u32TmpFlags > 0u)
	{
		BIT_SCAN_LEFT(u32Lz,u32TmpFlags); //detect which bit is set
		if(u32Lz > 31u) 
		{
			break;
		}
		u8Mb = (uint8)(63u-u32Lz);
		CAN_ISR(0, u8Mb);
		u32TmpFlags &= ~((uint32)0x00000001u << (u8Mb-32u)); //consume the bit
	}
	CAN_0.IFRH.R = u32Flags2; // Write 1s to clear interrupt flag register
}

void canA_ERR_ISR(void)
{
	//Error handling done in CAN hardware
    CAN_0.ESR.B.BOFFINT = 1u;
}

//lint -restore
