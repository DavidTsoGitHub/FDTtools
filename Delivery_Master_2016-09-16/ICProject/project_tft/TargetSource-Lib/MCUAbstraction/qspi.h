#ifndef _QSPI_H_
#define _QSPI_H_

#define qspi_wait_for_ICR_complete() 	while(QUADSPI_0.SFMSR.B.BUSY)
#define qspi_get_receive_data() 	(*(vuint32_t *)(0x87FFFFFC));


void init_qspi(void);

#endif