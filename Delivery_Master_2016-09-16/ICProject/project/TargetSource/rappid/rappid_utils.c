/********************  Dependent Include files here **********************/
#include "rappid_utils.h"

/****************** Enable/Disable Interrupt Interrupts *******************/

void EnableExternalInterrupts (void)
{
    asm(" wrteei 1");
}


void DisableExternalInterrupts (void)
{
    asm(" wrteei 0");
}



/*  ###################### Function for clearing BUFFERs ######################  */

void fs_clearmem(vuint8_t* address , uint8_t size, uint8_t fillbyte)
{
    while(size > 0)
    {
        *address = fillbyte;
        size-- ;
        address++ ;
    }
}
