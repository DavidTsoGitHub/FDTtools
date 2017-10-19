/**************************
Copyright Swedspot AB 2014
***************************/

#ifndef PDU_ROUTER_H_
#define PDU_ROUTER_H_

#include "Platform_Types.h"

// *********** Exported functions ***********//

// *********** Scheduler functions ***********//

// *********** Mode management functions ***********//

// *********** Exposed to upper/Side layers (PDUR) ***********//

void PDU_Router_ComTransmit( uint32 u32PduId, const uint8 *Data, uint8 u8DataLength);
void PDU_Router_DiagTransmit( uint32 u32PduId, const uint8 *Data, uint8 u8DataLength);
void PDU_Router_J1939Transmit( uint32 u32PduId, const uint8 *Data, uint8 u8DataLength);




// *********** Exposed to lower layers (CAN driver) ***********//
void PDU_Router_CAN_IF_TxConfirmation(uint32 u32PduId);
void PDU_Router_CAN_IF_RxIndication(uint32 u32PduId, uint8 *u8Data, uint8 u8DataLength);
void PDU_Router_CAN_IF_J1939_RxIndication(uint32 u32PduId, uint8 *u8Data, uint8 u8DataLength);

#endif /* PDU_ROUTER_H_ */
