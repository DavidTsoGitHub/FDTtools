#ifndef CAN_IF_HEADER
#define CAN_IF_HEADER

#include "CAN_IF_Cfg.h"

#define MAX_NUMBER_MB			64		//Supported by MPC5606S

// *********** Exported functions ***********//

//Scheduler functions
void CAN_IF_Init(void);
void CAN_IF_Run(void);


// *********** Modemanagement functions ***********//
void CAN_IF_Start(void);

// *********** Exposed to upper layers (PDUR) ***********//
void CAN_IF_Transmit(uint32 u32PduId, const uint8 *Data, uint8 u8DataLength);


// *********** Exposed to lower layers (CAN driver) ***********//
void CAN_IF_TxConfirmation( uint32 u32PduId, uint8 u8Mb, uint8 u8ControllerId);
void CAN_IF_RxIndication( uint8 u8ControllerId, uint8 u8Mb, uint32 u32PduId, uint8 *u8Data, uint8 u8DataLength);

#endif
