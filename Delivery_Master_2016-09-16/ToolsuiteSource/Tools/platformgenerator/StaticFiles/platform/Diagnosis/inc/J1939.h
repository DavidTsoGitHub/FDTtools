#ifndef DM1_MANAGER_H
#define DM1_MANAGER_H

/********************************* Includes **********************************/
#include "Platform_Types.h"

/**************************** Constants and Types ****************************/


/***************************** Exported Functions ****************************/

void J1939_Init(void);
void J1939_Run(void);

void J1939_TxConfirmation(uint32 u32PduId);
void J1939_RxIndication(uint32 u32PduId, uint8* u8Data, uint8 u8DataLength);

/*************************** J1939 Adapation Interfaces **********************/
void J1939_Adapt_SendData(uint8 length, uint8* data);
void J1939_Adapt_GetData(uint8 *length, uint8* data);


#endif //DM1_MANAGER_H
