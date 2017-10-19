#ifndef CAN_MESSAGE_DATA_H
#define CAN_MESSAGE_DATA_H

/********************************* Includes **********************************/
#include "Platform_Types.h"

/**************************** Constants and Types ****************************/
#define DLC 8

#define COM_TX_WAITING          (1U << 3U)
#define COM_TX_EVENT            (1U << 4U)
#define COM_TX_EVENT_PENDING    (1U << 5U)
#define COM_TX_CYCLIC           (1U << 6U)
#define COM_TX_CYCLIC_PENDING   (1U << 7U)

typedef enum
{
    Cyclic,
    Event,
    CyclicAndEvent
} can_message_type_e;

typedef struct
{
    uint32 CanId;
    uint8 Data[DLC];
    uint8 Status;
    uint16 TimeOut;
    uint32 LastRx;
    void (*NotificationCallback)(void);
} rx_message_t;

typedef struct
{
    uint32 CanId;
    uint8 Data[DLC];
    uint16 Periodicity;
    uint16 Delay;
    uint8 Status;
    boolean Event;
    uint32 NextTx;
    uint32 LastTx;
    uint8 Dlc;
    can_message_type_e SendType;
} tx_message_t;


/***************************** Exported Functions ****************************/
rx_message_t* Get_CAN_Rx_Messages(uint16* count);
tx_message_t* Get_CAN_Tx_Messages(uint16* count);

#endif //CAN_MESSAGE_DATA_H
