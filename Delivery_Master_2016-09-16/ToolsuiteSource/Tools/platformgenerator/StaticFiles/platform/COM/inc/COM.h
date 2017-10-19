#ifndef COM_H
#define COM_H

/********************************* Includes **********************************/
#include "Platform_Types.h"

/***************************** Exported Functions ****************************/
void COM_Init(void);
void COM_Run(void);
void COM_Reset(void);

void COM_Read_Signal(uint32 canId, uint8 startBit, uint8 length, uint32* value, uint8* status);
void COM_Write_Signal(uint32 canId, uint8 startBit, uint8 length, uint32 value, boolean event);

void COM_TxConfirmation(uint32 u32PduId);
void COM_RxIndication(const uint32 u32PduId,const uint8* const u8Data,const uint8 u8DataLength);

boolean COM_Get_CANTimeout(void);

#endif //COM_H
