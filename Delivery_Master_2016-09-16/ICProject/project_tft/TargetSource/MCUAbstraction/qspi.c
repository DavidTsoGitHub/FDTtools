
#include "jdp.h"
#include "SIUL.h"
#include "qspi.h"

void init_qspi(void)
{
  uint32_t qspi_rx_data=0;
  
  /* QSPI Mux Mem 0 */
  SIU.PCR[PC2].R = 0x0200;	// EN QSPI Memory 0
  SIU.GPDO[PC2].R = 0;		// 0 for U2, 1 for U3
  
  SIU.PCR[PF10].R = 0x0A00;
  SIU.PCR[PF11].R = 0x0B00;
  SIU.PCR[PF12].R = 0x0200;
  SIU.GPDO[PF12].B.PDO = 1;
  SIU.PCR[PF13].R = 0x0700;
  SIU.PCR[PF14].R = 0x0700;
  SIU.PCR[PF15].R = 0x0600;
  
  //CGM.AC3SC.B.SELCTL = 1; 	 //set QSPI FMPLL 2 at 48MHz


  QUADSPI_0.MCR.R = 0x00000088;


  QUADSPI_0.ICR.R = 0x00010035;
  qspi_wait_for_ICR_complete();
  qspi_rx_data = qspi_get_receive_data();

  if ((qspi_rx_data & 0xFF000000uL) != 0x02000000uL)
  {

    SIU.PCR[PF11].R = 0x0203;
    SIU.GPDO[PF11].B.PDO = 1;


    QUADSPI_0.ICR.R = 0x00000006;
    qspi_wait_for_ICR_complete();


    QUADSPI_0.TBDR = 0x00020000;
    QUADSPI_0.ICR.R = 0x00020001;
    qspi_wait_for_ICR_complete();


    SIU.PCR[PF11].R = 0x0B03;
  }


  SIU.PCR[PF12].R = 0x0B03;

  QUADSPI_0.ACR.R = 0x000040EB;
	
}
