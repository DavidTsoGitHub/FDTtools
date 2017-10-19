#ifndef __CAN_DRIVER_H__
#define __CAN_DRIVER_H__

/********************************* Includes **********************************/
#include "Platform_Types.h"

/**************************** Constants and Types ****************************/
typedef enum
{
	CAN_MODE_DISABLED,
	CAN_MODE_RX,
	CAN_MODE_TX
}CAN_MODES;

/***************************** Exported Functions ****************************/
void CAN_Init(void);
void CAN_InitController(uint8 u8ControllerId, uint32 u32TimingConfig);
void CAN_Run(void);

void CAN_ConfigMB(uint8 controllerId, uint8 mailBox, CAN_MODES mode, uint32 canID, uint32 canMask);
//void CAN_SetRXCallback((void(*rxCallback)(const char*,int));
//void CAN_ISR(uint8 controllerId, uint8 mailBox);

void CAN_Start(uint8 controllerId);
void CAN_Stop(uint8 controllerId);

void CAN_RxOnly();
void CAN_RxAndTx();

void CAN_Write(uint8 controllerId, uint8 mailBox, uint32 canID, const uint8* message, uint8 length);

void CAN_DisableControllerInterrupts(uint8 controllerId);
void CAN_EnableControllerInterrupts(uint8 controllerId);
void CAN_BusOff(uint8 controllerId);

#endif //__CAN_DRIVER_H__
