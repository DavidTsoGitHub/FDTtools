#ifndef _J1939ADAPT_H_
#define _J1939ADAPT_H_

#include "StandardTypes.h"
/**************** Scheduler interfaces **************/
void J1939Adapt_Init(void);
void J1939Adapt_Run(void);


/************* Customiziable adaptation interfaces ********/
void J1939Adapt_Client_GetData(uint8* length, uint8* data);
void J1939Adapt_Client_SendData(uint8 length,const uint8* data);



#endif