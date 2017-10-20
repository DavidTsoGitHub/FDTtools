#ifndef MCU_H_
#define MCU_H_

/*=======[I N C L U D E S]====================================================*/
#include "Std_Types.h"
#include "Mcu_Cfg.h"

typedef uint32   Mcu_RamSectionType;
typedef uint32   Mcu_ModeType;
typedef uint32   Mcu_ClockType;

typedef struct Mcu_ClockCfgType
{
  uint32 dummy;
} Mcu_ClockCfgType;

typedef struct Mcu_StandbyModeType
{
  uint32 PMSWCR0;                  /* PMSWCR0 register contents */
  uint8 CrcCheckEnable;            /* CRC check to be done or not */
} Mcu_StandbyModeType;

typedef struct Mcu_ConfigType
{
  const Mcu_ClockCfgType *ClockCfgPtr;  /* Ptr to Clock Setting Config Struct*/

  uint32 ResetCfg;  /* Reset request configuration */

  Mcu_ClockType NoOfClockCfg;  /* Total number of Clock Settings */

  Mcu_RamSectionType NoOfRamCfg;  /* Total number of RAM Sectors */
  Mcu_ModeType MaxMode;  /* The maximum mode that is supported
                            by this module instance */
  const Mcu_StandbyModeType *StandbyCfgPtr; /* Ptr to Standby configuration */
} Mcu_ConfigType;

/*=======[M A C R O S]========================================================*/

/**  PLL Frequency,called by  Gpt_Init*/
#define CLOCK_PLL_FREQ  80000000uL

/*=======[E X T E R N A L   F U N C T I O N   D E C L A R A T I O N S]========*/
void Mcu_Init(const Mcu_ConfigType* ConfigPtr);
void Mcu_Deinit(void);
void Mcu_InitClock( void );
void Mcu_Arc_GetECCError( uint8 *err );
void MCU_ModeEnterDRUN(void);

#endif


