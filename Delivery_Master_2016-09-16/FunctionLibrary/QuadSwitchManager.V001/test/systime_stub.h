#ifndef SYSTIME_STUB_H
#define SYSTIME_STUB_H

#include "StandardTypes.h"

void SysTimeStub_SetTime(uint32 newTime);
void SysTimeStub_FastForward(uint32 timeToFastForward);

#endif // SYSTIME_STUB_H