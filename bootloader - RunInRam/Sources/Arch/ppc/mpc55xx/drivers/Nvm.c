#define BLFLASH_C_

/*=======[I N C L U D E S]====================================================*/
#include "config.h"
#include "Nvm.h"
#include "Fls.h"
#include "FlashLoader.h"
#include "QuadSPI.h"

#define QUAD_SPI_ADDRESS 0x80000000

const tFlashInfo BLFlash_Info = 
{
	BLFLASH_DRIVER_VERSION_MCUTYPE,
	BLFLASH_DRIVER_VERSION_MASKTYPE,
	0x00,
	BLFLASH_DRIVER_VERSION_INTERFACE,
	BlFlash_Init,
	BlFlash_Deinit,
	BlFlash_Erase,
	BlFlash_Write,
	0xFFFFFFFFuL,
};

void BlFlash_Init( tFlashParam* flashParam )
{
	(void) flashParam;
#if defined(CFG_QSPI)
	QuadSPI_Init();
#endif
	Fls_Init( FlsConfigSet );
}

void BlFlash_Deinit ( tFlashParam* flashParam )
{
	(void)flashParam;
#if defined(CFG_QSPI)
	QuadSPI_Deinit();
#endif
}

void BlFlash_Erase( tFlashParam* flashParam )
{
	Fls_AddressType TargetAddress=flashParam->address;
    Fls_LengthType Length=flashParam->length;

    if (TargetAddress >= QUAD_SPI_ADDRESS) 
    {
    	QuadSPI_Erase(TargetAddress, Length, flashParam->wdTriggerFct);

    } 
    else 
    {
		Fls_Erase(TargetAddress, Length, flashParam->wdTriggerFct);
		do 
		{
			Fls_MainFunction();
			flashParam->wdTriggerFct();
		} while (Fls_GetStatus() == MEMIF_BUSY);
    }

    /* Get result */
    flashParam->errorCode = (Fls_GetJobResult() == MEMIF_JOB_OK) ? FL_OK : FL_FAILED;
}
void WdgKick(void);
void BlFlash_Write ( tFlashParam* flashParam )
{
	Fls_AddressType TargetAddress=flashParam->address;
    uint8 *SourceAddressPtr=(uint8*)flashParam->data;
    Fls_LengthType Length=flashParam->length;

#if defined(CFG_QSPI)
    if (TargetAddress >= QUAD_SPI_ADDRESS) {
    	QuadSPI_Write(SourceAddressPtr, TargetAddress, Length, flashParam->wdTriggerFct);
    } else {
#else
    {
#endif
    	Fls_Write(TargetAddress, SourceAddressPtr, Length);

		/* Wait until operation done! */
		do {
			Fls_MainFunction();
			flashParam->wdTriggerFct();
		} while (Fls_GetStatus() == MEMIF_BUSY);
    }

    /* Get result */
    flashParam->errorCode = (Fls_GetJobResult() == MEMIF_JOB_OK) ? FL_OK : FL_FAILED;
    
}
