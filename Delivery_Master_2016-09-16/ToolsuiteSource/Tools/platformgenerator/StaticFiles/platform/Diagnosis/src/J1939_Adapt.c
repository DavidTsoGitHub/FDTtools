#include "J1939_Adapt.h"
#include "StandardTypes.h"

//Used by the application client to get data from the platform layer
void J1939Adapt_Client_GetData(uint8* length, uint8* data)
{

}

/******* Interface for the customer adapation module to send data */
void J1939Adapt_Client_SendData(uint8 length,const uint8* data)
{
 //   J1939_Adapt_SendData(length,data);
}

/****************** Scheduler interfaces*************/
void J1939Adapt_Init(void)
{

}

void J1939Adapt_Run(void)
{

}

