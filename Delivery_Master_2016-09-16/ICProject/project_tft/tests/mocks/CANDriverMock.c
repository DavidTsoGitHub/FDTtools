#include "CANDriverMock.h"
#include "can_driver.h"

static boolean isStarted;

void CANDriverMock_Reset()
{
    isStarted = FALSE;
}

boolean CANDriverMock_IsStarted()
{
    return isStarted;
}

void CAN_Start(uint8 controllerId)
{
    isStarted = TRUE;
}

void CAN_Stop(uint8 controllerId)
{
    isStarted = FALSE;
}

void CAN_RxOnly()
{
}

void CAN_RxAndTx()
{
}


// void CAN_InitController(uint8 u8ControllerId, uint32 u32TimingConfig);
// void CAN_ConfigMB(uint8 controllerId, uint8 mailBox, CAN_MODES mode, uint32 canID, uint32 canMask);
// void CAN_SetRXCallback((void(*rxCallback)(const char*,int));
// void CAN_ISR(uint8 controllerId, uint8 mailBox);
// void CAN_Write(uint8 controllerId, uint8 mailBox, uint32 canID, uint8* message, uint8 length);
// void CAN_DisableControllerInterrupts(uint8 controllerId);
// void CAN_EnableControllerInterrupts(uint8 controllerId);
// void CAN_BusOff(uint8 controllerId);
// void CAN_Init();