#ifndef _BASE64_H_
#define _BASE64_H_

#include "Platform_Types.h"

char *base64_encode(const uint8 *data, uint32 input_length, uint32 *output_length);

#endif // _BASE64_H_