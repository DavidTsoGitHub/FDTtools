#ifndef CANDRV_5606S_H_
#define CANDRV_5606S_H_

#include "StandardTypes.h"

#define CODE_MS_BUF_RX_INACTIVE		0x0u
#define CODE_MS_BUF_RX_EMPTY		0x4u
#define CODE_MS_BUF_TX_INACTIVE		0x8u
#define CODE_MS_BUF_TX_TRANSMIT		0xCu

#define MPC_FREEZE_ENABLE_BUFFERS	0x5000003F // Set Freeze Mode & Halt. Enable all 64 msg buffers , [MPC5510RM p.25-11]

#define CAN_MAX_MB                  64u

void CAN_ISR(uint8 u8ControllerId, uint8 u8Mb);

#endif /* CANDRV_5606S_H_ */
