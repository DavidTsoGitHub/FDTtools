#ifndef __IC_FILTERLIB_H_
#define __IC_FILTERLIB_H_

#include "StandardTypes.h"


// Contains second order filter data.
typedef struct
{
  uint8 c1;        // Constant 1.
  uint8 c2;        // Constant 2.
  uint8 c3;        // Constant 3.
  uint8 c4;        // Constant 4.
  uint8 c5;        // Constant 5.
  uint16 Xn;       // X[n]
  uint16 Xn1;      // X[n-1]
  uint16 Xn2;      // X[n-2]
  uint32 Yn;       // Y[n]
  uint32 Yn1;      // Y[n-1]
} SecOrdFilterInput;


typedef struct
{
  uint8  cPos;        // First order lagfilter positive constant (0-255)
  uint8  cNeg;        // First order lagfilter negative constant (0-255)
  uint16 Xnf;         // First order lagfilter current input value
  uint32 Ynf;         // First order lagfilter current/previous output value
  uint16 u16RetValue; //Current outValue
} FirstOrderFilterInpuPosNegLoRes16;

typedef struct
{
  uint8  cPos;       // First order lagfilter positive constant (0-255)
  uint8  cNeg;       // First order lagfilter negative constant (0-255)
  uint8  Xnf;        // First order lagfilter current input value
  uint16 Ynf;        // First order lagfilter current/previous output value
} FirstOrderFilterInpuPosNegLoRes8;

typedef struct
{
  uint16 cPos;      // First order lagfilter positive constant (0-65535)
  uint16 cNeg;      // First order lagfilter negative constant (0-65535)
  uint8  Xnf;       // First order lagfilter current input value
  uint16 Ynf;       // First order lagfilter current/previous output value
} FirstOrderFilterInpuPosNegHiRes8;


// Function Definitions
uint16 SecOrderLagFilterLoRes16( SecOrdFilterInput *indata );
uint16 FirstOrderLagfilterPosNegLoRes16(volatile FirstOrderFilterInpuPosNegLoRes16 *indataFirstOrd );
uint8 FirstOrderLagfilterPosNegLoRes8( FirstOrderFilterInpuPosNegLoRes8 *pFilterData );
uint8 FirstOrderLagfilterPosNegHiRes8( FirstOrderFilterInpuPosNegHiRes8 *pFilterData );
void ResetSecOrdLagFilter( SecOrdFilterInput *indata );


uint16 interpolate16( uint16 u16Value, const uint16 *u16X_point, const uint16 *u16Y_point,uint8 u8Size, uint8 *u8LastIndex );
uint8 interpolate8( uint8 u8Value, const uint8 *u8X_point,const uint8 *u8Y_point, uint8 u8Size, uint8 *u8LastIndex );
uint16 interpolate8_16( uint8 u8Value, const uint8 *u8X_point, const uint16 *u16Y_point, uint8 u8Size, uint8 *u8LastIndex );


#endif
