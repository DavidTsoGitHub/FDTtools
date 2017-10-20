#ifndef MCU_CFG_H
#define MCU_CFG_H

/*=======[M A C R O S]========================================================*/

/* Define 4MHZ,8MHZ,12MHZ and 16MHZ crystal freq for different projects select */
/* @type:define range:NONE note:auto generate */
#define CLOCK_CRYSTAL_FREQ_4MHZ     4000000uL

#define CLOCK_CRYSTAL_FREQ_8MHZ     8000000uL

#define CLOCK_CRYSTAL_FREQ_12MHZ    12000000uL

#define CLOCK_CRYSTAL_FREQ_16MHZ    16000000uL

/**  Crystal Frequency , called by Can_Init */
/* @type:define range:CLOCK_CRYSTAL_FREQ_4MHZ~CLOCK_CRYSTAL_FREQ_16MHZ note:NONE */
#define CLOCK_CRYSTAL_FREQ_UESD  CLOCK_CRYSTAL_FREQ_8MHZ

#endif



