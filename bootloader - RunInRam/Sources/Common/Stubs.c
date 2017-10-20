/*
 * Stubs.c
 *
 *  Created on: 29 feb 2016
 *      Author: mafo
 */
#include "config.h"
#include "Std_Types.h"
#include "Arc_Types.h"
#include "Nvm.h"

uint8 InitializeUART(uint32 baudRate)
{
   (void)baudRate;
   return 0;
}

uint8 ReadUARTN(void* bytes, uint32 length)
{
   (void)bytes;
   (void)length;
   return 0;
}

uint8 WriteUARTN(const void* buf, uint32 cnt)
{
   (void)buf;
   (void)cnt;
   return 0;
}

int FLS_memcpy(char *s1, char *s2, int  n)
{
    if (n != 0) {
        unsigned char *p1 = (unsigned char *)s1,
                  *p2 = (unsigned char *)s2;
        do {
            *p1++ = *p2++;
        } while (--n != 0);
        return(1);
    }
    return (0);
}

#if defined(CFG_BOOT_SBL)
asm void __init_hardware(void)
{
	fralloc
	frfree
	blr
}

asm void __flush_cache(register void *address, register unsigned int size)
{
	fralloc
	frfree
	blr
}

asm void __init_user(void)
{
	fralloc
	frfree
	blr
}

void main(void)
{
	BLFlash_Info.flashInitFct(NULL_PTR);
}

void exit(void)
{
}
#endif
