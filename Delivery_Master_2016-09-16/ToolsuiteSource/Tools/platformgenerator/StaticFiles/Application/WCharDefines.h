#ifndef WCHARDEFINES_H_
#define WCHARDEFINES_H_

#include "StandardTypes.h"

#define WCHAR_EOL               (wchar)(0x0000u)
#define WCHAR_BACKSLASH         (wchar)(0x005Cu)
#define WCHAR_DECIMAL_POINT     (wchar)(0x002Eu)

#define WCHAR_CAPITAL_I         (wchar)(0x0049u)
#define WCHAR_CAPITAL_L         (wchar)(0x004Cu)
#define WCHAR_CAPITAL_N         (wchar)(0x004Eu)
#define WCHAR_CAPITAL_T         (wchar)(0x0054u)

#define WCHAR_K                 (wchar)(0x006Bu)
#define WCHAR_M                 (wchar)(0x006Du)

#define WCHAR_N                 (wchar)(0x006Eu)

#define WCHAR_ZERO              (wchar)(0x0030u)

#define WCHAR_SLASH             (wchar)(0x002Fu)
#define WCHAR_DASH              (wchar)(0x002Du)
#define WCHAR_HASH              (wchar)(0x0023u)

wchar DigitToWchar(uint8 digit);

#endif // WCHARDEFINES_H_
