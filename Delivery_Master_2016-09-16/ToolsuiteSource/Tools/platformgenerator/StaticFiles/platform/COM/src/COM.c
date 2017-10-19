//lint -save
/********************************* Includes **********************************/
#include "COM.h"
#include "COM_GCL_Relay.h"
#include "COM_Types.h"
#include "CAN_Message_Data.h"
#include "PDU_Router.h"
#include "GCL_Types.h"
#include "Logger.h"
#include "BSP.h"        //Critical section

#include "SysTime.h"

/**************************** Constants and Types ****************************/
#define RX_TIMEOUT_MS   30000u

/********************************* Variables *********************************/
static uint32 u32LastRx;
static boolean bCANTimeout;

/************************* Local Function Prototypes *************************/
static inline uint8 min(uint8 a, uint8 b);
static void pack(uint32 value, uint8 startbit, uint8 length, uint8* array, endianness_t endianness);
static uint32 unpack(const uint8* array, uint8 startbit, uint8 length, endianness_t endianness);

/***************************** Exported Functions ****************************/
void COM_Init(void)
{
    Logger_Info("COM", "COM_Init");
    COM_Reset();
 }

void COM_Run(void)
{
    uint16 i;
    uint16 count;

    uint32 t = SysTime_GetTimeMs();

    if (t > (u32LastRx + RX_TIMEOUT_MS))
    {
        Logger_Info("COM", "Timeout after no CAN Rx for %d ms", RX_TIMEOUT_MS);
        //ModeManager_CAN_Timeout();
        bCANTimeout = TRUE;
        u32LastRx = t;      //Indicate timout every RX_TIMEOUT/1000 seconds
    }

    // monitor the RX list and set the timeout status flag if necessary:
    rx_message_t* rx_msgs = Get_CAN_Rx_Messages(&count);

    for (i = 0u; i < count; i++)
    {
        if ((rx_msgs[i].TimeOut > 0u) && ((t - rx_msgs[i].LastRx) > rx_msgs[i].TimeOut))
        {
            // set the timeout flag
            rx_msgs[i].Status |= ((uint8) GCL_SIGNAL_TIMEOUT);
        }
    }

    // send pending messages:
    tx_message_t* tx_msgs = Get_CAN_Tx_Messages(&count);

    for (i = 0u; i < count; i++)
    {

        if (((tx_msgs[i].Status & COM_TX_EVENT_PENDING) >= 1U) && (t >= (tx_msgs[i].LastTx + tx_msgs[i].Delay)))
        {
            Logger_Info("COM", "Sending pending event");
            tx_msgs[i].Status |= ((uint8) COM_TX_EVENT);
            tx_msgs[i].Status &= ((uint8) ~COM_TX_EVENT_PENDING);
            PDU_Router_ComTransmit(tx_msgs[i].CanId, tx_msgs[i].Data, tx_msgs[i].Dlc);
        }
        else if ((tx_msgs[i].SendType != Event) && (t >= tx_msgs[i].NextTx) && (t >= (tx_msgs[i].LastTx + tx_msgs[i].Delay)))
        {
            tx_msgs[i].Status |= ((uint8) COM_TX_CYCLIC);
            PDU_Router_ComTransmit(tx_msgs[i].CanId, tx_msgs[i].Data, tx_msgs[i].Dlc);
        }
        else
        {

            ; //Intentially left blank
        }
    }
}

void COM_Reset(void)
{
    uint16 i;
    uint16 count;

    bCANTimeout = FALSE;

    uint32 t = SysTime_GetTimeMs();

    // initialize next tx times for outbound messages:
    tx_message_t* tx_msgs = Get_CAN_Tx_Messages(&count);
    for (i = 0u; i < count; i++)
    {
        tx_msgs[i].NextTx = tx_msgs[i].Periodicity;
        tx_msgs[i].LastTx = 0;
    }

    // Set last rx time to now, so we don't start with a timeout indication
    u32LastRx = t;
}

void COM_Read_Signal(uint32 canId, uint8 startBit, uint8 length, uint32* value, uint8* status)
{
    uint16 count;
    uint16 i;
    const rx_message_t* rx_msgs = Get_CAN_Rx_Messages(&count);

    // find the requested message. No error checking here; the caller is generated code and there should never
    // be a request for a CAN id that we don't know about.
    for (i = 0u; i < count; i++)
    {
        if (rx_msgs[i].CanId == canId)
        {
            // This operation requires critical section since CAN interrupt might modify the value during an unpack opertation
            // resulting in a situation where some data is new and some old.
            BSP_EnterCriticalSection();
            *value = unpack(rx_msgs[i].Data, startBit, length, LITTLE_ENDIAN);
            *status = rx_msgs[i].Status;
            BSP_LeaveCriticalSection();

            // found the message; break out of the loop
            break;
        }
    }
}

void COM_Write_Signal(uint32 canId, uint8 startBit, uint8 length, uint32 value, boolean event)
{
    uint16 count;
    uint16 i;
    tx_message_t* tx_msgs = Get_CAN_Tx_Messages(&count);
    uint32 t; // = SysTime_GetTimeMs();


    // find the correct message. No error checking here; the caller is generated code and there should never
    // be a request for writing a CAN id that we don't know about.
    for (i = 0; i < count; i++)
    {
        if (tx_msgs[i].CanId == canId)
        {
            pack(value, startBit, length, tx_msgs[i].Data, LITTLE_ENDIAN);
            //tx_msgs[i].Event |= event;
            if(event == TRUE)
            {
                t = SysTime_GetTimeMsDelta(tx_msgs[i].LastTx);
                if(t > tx_msgs[i].Delay)
                {
                    Logger_Info("COMEVENT","t: %d    Last: %d    Delay: %d", SysTime_GetTimeMs(), tx_msgs[i].LastTx,tx_msgs[i].Delay);
                    tx_msgs[i].Status |= ((uint8) COM_TX_EVENT);
                    PDU_Router_ComTransmit(tx_msgs[i].CanId, tx_msgs[i].Data, tx_msgs[i].Dlc);
                }
                else
                {
                    Logger_Info("COM", "Delayed event");
                    tx_msgs[i].Status |= ((uint8) COM_TX_EVENT_PENDING);
                }
            }
            break;
        }
    }
}

void COM_TxConfirmation(uint32 u32PduId)
{
    uint16 i;
    uint16 count;

    tx_message_t* tx_msgs = Get_CAN_Tx_Messages(&count);

    for (i = 0u; i < count; i++)
    {
        if((tx_msgs[i].CanId == u32PduId))
        {
            uint32 t = SysTime_GetTimeMs();
            tx_msgs[i].LastTx = t;

            if ((tx_msgs[i].Status & COM_TX_CYCLIC) >= 1U)
            {
                uint32 nextTx = tx_msgs[i].NextTx + tx_msgs[i].Periodicity;

                // If the previous assignment causes an overrun, nextTx will be less than the periodicity
                // If drift has caused us to miss one event, we will increase the nextTx by the periodicity
                // until we have passed the current time.
                while ((nextTx < t) && (nextTx > tx_msgs[i].Periodicity))
                {
                    nextTx += tx_msgs[i].Periodicity;
                }

                tx_msgs[i].NextTx = nextTx;
                tx_msgs[i].Status &= ((uint8) ~COM_TX_CYCLIC);

                // found the message; break out of the loop
                break;
            }
            else if ((tx_msgs[i].Status & COM_TX_EVENT) >= 1U)
            {
                if((t + tx_msgs[i].Delay ) >= tx_msgs[i].NextTx)
                {
                    uint32 nextTx = tx_msgs[i].NextTx + tx_msgs[i].Periodicity;

                    // If the previous assignment causes an overrun, nextTx will be less than the periodicity
                    // If drift has caused us to miss one event, we will increase the nextTx by the periodicity
                    // until we have passed the current time.
                    while ((nextTx < t) && (nextTx > tx_msgs[i].Periodicity))
                    {
                        nextTx += tx_msgs[i].Periodicity;
                    }

                    tx_msgs[i].NextTx = nextTx;
                    //tx_msgs[i].Event = TRUE;
                }
                else
                {
                    //tx_msgs[i].Event = FALSE;
                }

                tx_msgs[i].Status &= ((uint8) ~(COM_TX_EVENT));

            }
            else
            {
                ; //Intentially left blank
            }
        }
    }
}

void COM_RxIndication(const uint32 u32PduId, const uint8* const u8Data, const uint8 u8DataLength)
{
    uint16 count;
    uint16 i;
    boolean valueUpdated;

    rx_message_t* rx_msgs = Get_CAN_Rx_Messages(&count);

    for (i = 0u; i < count; i++)
    {
        if (rx_msgs[i].CanId == u32PduId)
        {
            uint8 j;
            valueUpdated = FALSE;

            uint32 t = SysTime_GetTimeMs();
            for (j = 0u; j < 8u; j++)
            {
                if ( j < u8DataLength )
                {
                    if (rx_msgs[i].Data[j] != u8Data[j])
                    {
                        valueUpdated = TRUE;
                    }
                    rx_msgs[i].Data[j] = u8Data[j];
                }
                else
                {
                    rx_msgs[i].Data[j] = 0u;
                }
            }

            rx_msgs[i].LastRx = t;

            // clear the timeout flag on message
            rx_msgs[i].Status &= ((uint8) ~GCL_SIGNAL_TIMEOUT);

            if ((rx_msgs[i].NotificationCallback != NULL) && (valueUpdated == TRUE))
            {
                rx_msgs[i].NotificationCallback();
            }

            // clear timeout status on COM and set last RX
            u32LastRx = t;

            // found the message; break out of the loop
            break;
        }
    }
}

boolean COM_Get_CANTimeout(void)
{
    return bCANTimeout;
}

/****************************** Local Functions ******************************/
static inline uint8 min(uint8 a, uint8 b)
{
    return (a > b) ? b : a;
}

static void pack(uint32 value, uint8 startbit, uint8 length, uint8* array, endianness_t endianness)
{
    uint8 startByte = startbit / 8U;

    uint8 remainingLength = length;
    uint8 currentByte = startByte;
    uint8 startBitInCurrentByte = startbit % 8U;
    uint32 u32tempValue = value;

    while (remainingLength > 0U)
    {
        uint8 mask = 0xFFu >> startBitInCurrentByte;
        uint8 valueToInsert = (uint8)((u32tempValue & mask) << startBitInCurrentByte);
        //lint --e(701) [Function intentionally designed for left shifting]
        array[currentByte] &= ~(uint8)((uint8)(mask) << (uint8)startBitInCurrentByte);
        array[currentByte] |= valueToInsert;

        if (endianness == BIG_ENDIAN)
        {
            --currentByte;
        }
        else
        {
            ++currentByte;
        }

        uint8 bitsInserted = min(remainingLength, (8U - startBitInCurrentByte));
        remainingLength -= bitsInserted;
        u32tempValue = u32tempValue >> bitsInserted;

        // Bits of interest in subsequent bytes will always start at index 0
        startBitInCurrentByte = 0u;
    }

}

static uint32 unpack(const uint8* array, uint8 startbit, uint8 length, endianness_t endianness)
{
    uint32 value = 0u;
    uint8 startByte = startbit / 8U;

    uint8 remainingLength = length;
    uint8 currentByte = startByte;
    uint8 startBitInCurrentByte = startbit % 8U;

    while (remainingLength > 0U)
    {
        uint8 mask = 0xFFU >> (8U - (min(8U, startBitInCurrentByte + remainingLength)));
        //lint --e(701) --e(9032) [Function intentionally designed for left shifting] [Operation is intentionally only done on part of value]
        value |= (uint8)((uint8)(array[currentByte] & mask) >> startBitInCurrentByte) << (uint8)(length - remainingLength);

        if (endianness == BIG_ENDIAN)
        {
            --currentByte;
        }
        else
        {
            ++currentByte;
        }

        remainingLength -= min(remainingLength, (8U - startBitInCurrentByte));

        // Bits of interest in subsequent bytes will always start at index 0
        startBitInCurrentByte = 0u;
    }

    return value;
}

//lint -restore
