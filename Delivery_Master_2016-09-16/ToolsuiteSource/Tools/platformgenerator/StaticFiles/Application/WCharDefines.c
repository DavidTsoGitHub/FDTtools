#include "WCharDefines.h"

wchar DigitToWchar(uint8 digit)
{
    return (wchar)0x0030 + (wchar)digit;
}
