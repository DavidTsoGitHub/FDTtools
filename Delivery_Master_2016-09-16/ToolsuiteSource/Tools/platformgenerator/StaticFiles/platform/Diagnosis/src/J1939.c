/********************************* Includes **********************************/
#include "StandardTypes.h"
#include "Logger.h"
#include "J1939.h"
#include "J1939_Adapt.h"    //Scheduler functions

/**************************** Constants and Types ****************************/
#define J1939ID "J1939"

/********************************* Variables *********************************/

/************************* Local Function Prototypes *************************/

/***************************** Exported Functions ****************************/
void J1939_Init(void)
{
    Logger_Info(J1939ID, "J1939_Init");
    J1939Adapt_Init();
    return;
}

void J1939_Run(void)
{
    Logger_Info(J1939ID, "J1939_Run");
    J1939Adapt_Run();
      
}

void J1939_TxConfirmation(uint32 u32PduId)
{
    Logger_Info(J1939ID, "J1939_TxConfirmation");
    (void)u32PduId;
    return;
}

void J1939_RxIndication(uint32 u32PduId, uint8* u8Data,  uint8 u8DataLength)
{
    Logger_Info(J1939ID, "J1939_RxIndication");
    (void)u32PduId;
    (void)u8Data;
    (void)u8DataLength;
    return;
}

//Access function for the adaptation layers to get current J1939 data
void J1939_Adapt_GetData(uint8 *length, uint8* data)
{
   
}

//used by the J1939 customer module to send data
//PDUID == CANId which must be present in the CAN xml file in order to allocate CAN MB
void J1939_Adapt_SendData(uint8 length, uint8* data)
{
    
}





/****************************** Local Functions ******************************/
