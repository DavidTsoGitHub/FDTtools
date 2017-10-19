#ifndef _CAL_STUB_H_
#define _CAL_STUB_H_

#include "CAL.h"

extern const uint8 DisplayIlluminationLevel1;
extern const uint8 DisplayIlluminationLevel2;
extern const uint8 DisplayIlluminationLevel3;
extern const uint8 DisplayIlluminationLevel4;
extern const uint8 DisplayIlluminationLevel5;

extern const uint8 GaugeIlluminationLevel1;
extern const uint8 GaugeIlluminationLevel2;
extern const uint8 GaugeIlluminationLevel3;
extern const uint8 GaugeIlluminationLevel4;
extern const uint8 GaugeIlluminationLevel5;

void CALStub_Init(void);
void CALStub_SetIlluminationLevel(uint8 level);
uint8 CALStub_GetIlluminationLevel(void);
uint8 CALStub_GetCalWriteCount(void);

#endif //_CAL_STUB_H_