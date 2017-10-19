#include "StandardTypes.h"
#include <assert.h>
#include "FilterLib.h"

#define SHIFT_CONSTANT 8U
#define SHIFT_CONSTANT2 16U

/******************************************************************************/
/**
* This function uses a 2nd Order Lagfilter on the input data
* 
* Y[n] = c1 * Y[n-2] + c2 * Y[n-1] + c3 * X[n-2] + c4 * X[n-1] + c5 * X[n]
*
* c1+c2+c3+c4+c5=256
*
* @param     indata   Structure that contains filterdata
*
* @retval    None
*/
/*******************************************************************************/
uint16 SecOrderLagFilterLoRes16( SecOrdFilterInput *indata )
{

  uint32 u32ScaledInput = (((uint32) indata->Xn) << SHIFT_CONSTANT);
  uint32 Yn2 = indata->Yn1;
  indata->Yn1 = (indata->Yn);
  indata->Yn =((( (uint32) ( indata->c1 * Yn2 )          + 128U ) >> SHIFT_CONSTANT ) +
               (( (uint32) ( indata->c2 * indata->Yn1 )  + 128U ) >> SHIFT_CONSTANT ) +
                ( (uint32) indata->c3 * indata->Xn2 ) +
                ( (uint32) indata->c4 * indata->Xn1 ) +
                ( (uint32) indata->c5 * indata->Xn  )  );

  if( (indata->Yn1 < u32ScaledInput) && (indata->Yn > u32ScaledInput))
  {
     indata->Yn = u32ScaledInput;
  }


  indata->Xn2 = indata->Xn1;
  indata->Xn1 = indata->Xn;

  return (uint16) ( (indata->Yn + 128U) >> SHIFT_CONSTANT);
}

/******************************************************************************/
/**
* First order lag filter - 16-bit I/O, 8-bit coefficient (0-255).
* Y[n] = Y[n-1] + c*(X[n] - Y[n-1]), where
*   Y[n] is the new filtered outdata (Ynf).
*   Y[n-1] is the old outdata (Ynf)
*   X[n] is current input (Xnf)
*   c is the filter constant (cPos when increasing cNeg when decreasing)
*/
/******************************************************************************/
uint16 FirstOrderLagfilterPosNegLoRes16( volatile FirstOrderFilterInpuPosNegLoRes16 *pFilterData)
{

  uint32 u32Delta =0;
  uint16 u16ret;
  uint32 u32Input =  (((uint32) pFilterData->Xnf) << SHIFT_CONSTANT );

  //decreasing
  if( pFilterData->Ynf > u32Input )
  {
    u32Delta = ( ( ( pFilterData->Ynf * pFilterData->cNeg ) >> SHIFT_CONSTANT ) - ( pFilterData->Xnf * (uint32) pFilterData->cNeg ) );

    if( u32Delta == 0 && pFilterData->Ynf > 0 )
    {
      u32Delta = 1;
    }

    pFilterData->Ynf =  pFilterData->Ynf - u32Delta;

    if( pFilterData->Ynf < u32Input )
    {
      pFilterData->Ynf = u32Input;
    }
  }

  //increasing
  else if ( u32Input > pFilterData->Ynf )
  {
    u32Delta = ( ( pFilterData->Xnf * (uint32) pFilterData->cPos ) - ( ( pFilterData->Ynf * pFilterData->cPos ) >> SHIFT_CONSTANT) );

    if( u32Delta == 0 )
    {
      u32Delta = 1;
    }
    
    pFilterData->Ynf = ( pFilterData->Ynf + u32Delta );

    if( pFilterData->Ynf > u32Input )
    {
      pFilterData->Ynf = u32Input;
    }
  }
  else
  {
    pFilterData->Ynf = u32Input;
  }
  
  u16ret = (pFilterData->Ynf + 128 ) >> SHIFT_CONSTANT;
  pFilterData->u16RetValue = u16ret;
  return u16ret;
  
}


/******************************************************************************/
/**
* First order lag filter - 8-bit I/O, 8-bit coefficient (0-255).
*/
/******************************************************************************/
uint8 FirstOrderLagfilterPosNegLoRes8( FirstOrderFilterInpuPosNegLoRes8 *pFilterData)
{

  uint16 u16Delta;
  uint16 u16Input = (uint16) (((uint16) pFilterData->Xnf) << SHIFT_CONSTANT );

  //decreasing
  if( pFilterData->Ynf > u16Input )
  {
    u16Delta = ( ( ( pFilterData->Ynf * pFilterData->cNeg ) >> SHIFT_CONSTANT ) - ( pFilterData->Xnf * (uint16) pFilterData->cNeg ) );

    if( u16Delta == 0 && pFilterData->Ynf > 0 )
    {
      u16Delta = 1;
    }

    pFilterData->Ynf =  pFilterData->Ynf - u16Delta;

    if( pFilterData->Ynf < u16Input )
    {
      pFilterData->Ynf = u16Input;
    }
  }

  //increasing
  else if ( u16Input > pFilterData->Ynf )
  {
    u16Delta = ( ( pFilterData->Xnf * (uint16) pFilterData->cPos ) - ( ( pFilterData->Ynf * pFilterData->cPos ) >> SHIFT_CONSTANT) );

    if( u16Delta == 0 )
    {
      u16Delta = 1;
    }
    
    pFilterData->Ynf = ( pFilterData->Ynf + u16Delta );

    if( pFilterData->Ynf > u16Input )
    {
      pFilterData->Ynf = u16Input;
    }
  }
  else
  {
    pFilterData->Ynf = u16Input;
  }
  return (uint8) ((pFilterData->Ynf + 128 ) >> SHIFT_CONSTANT);
}

/******************************************************************************/
/**
* First order lag filter - 8-bit I/O, 16-bit coefficient (0-65535).
*/
/******************************************************************************/
uint8 FirstOrderLagfilterPosNegHiRes8( FirstOrderFilterInpuPosNegHiRes8 *pFilterData)
{

  uint16 u16Delta;
  uint16 u16Input = (uint16) (((uint16) pFilterData->Xnf) << SHIFT_CONSTANT );

  //decreasing
  if( pFilterData->Ynf > u16Input )
  {        
    u16Delta = ( ( ( pFilterData->Ynf * pFilterData->cNeg ) >> SHIFT_CONSTANT2 ) - (
      ( pFilterData->Xnf * (uint16) pFilterData->cNeg ) >> SHIFT_CONSTANT ) );

    if( u16Delta == 0 && pFilterData->Ynf > 0 )
    {
      u16Delta = 1;
    }

    pFilterData->Ynf =  pFilterData->Ynf - u16Delta;

    if( pFilterData->Ynf < u16Input )
    {
      pFilterData->Ynf = u16Input;
    }
  }

  //increasing
  else if ( u16Input > pFilterData->Ynf )
  {
    
    u16Delta = ( ( ( pFilterData->Xnf * pFilterData->cPos ) >> SHIFT_CONSTANT )
      - ( ( pFilterData->Ynf * pFilterData->cPos ) >> SHIFT_CONSTANT2) );

    if( u16Delta == 0 )
    {
      u16Delta = 1;
    }
    
    pFilterData->Ynf = ( pFilterData->Ynf + u16Delta );

    if( pFilterData->Ynf > u16Input )
    {
      pFilterData->Ynf = u16Input;
    }
 
  }
  else
  {
    pFilterData->Ynf = u16Input;
  }
  return (uint8) ((pFilterData->Ynf + 128 ) >> SHIFT_CONSTANT);
}


/******************************************************************************/
/**
* Reset lag filter values
*
* @param      indata    SecOrdFilterInput
*/
/******************************************************************************/
void ResetSecOrdLagFilter( SecOrdFilterInput *indata )
{
  indata->Xn  = 0U;
  indata->Xn1 = 0U;
  indata->Xn2 = 0U;
  indata->Yn  = 0U;
  indata->Yn1 = 0U;
}

/******************************************************************************/
/**
* Interpolates a point on a continous linear function
* 
*
* @param      u16Value      Value to be interpolated
* @param      u16X_point    An array of points on the x-axis 
* @param      u16Y_point    An array of points on the y_axis
* @param      u8Size        number of elements on x and y arrays
* @param      u8LastIndex   array index last time this function was called 
*                           (pointer contains new index after return)
*
* Working conditions: 
* - Considers only functions with positive x and positive y.
* - list of x-points must be in strictly increasing order.
* - list of y-points must be in either increasing or decreasing order 
*   (piecewise constant sections allowed).
*
*/
/******************************************************************************/
uint16 interpolate16( uint16 u16Value, const uint16 *u16X_point, const uint16 *u16Y_point, uint8 u8Size, uint8 *u8LastIndex )
{
  register uint8 i = *u8LastIndex;

  assert(u16X_point && u16Y_point && u8LastIndex);
  assert(i<u8Size && u8Size>1); 

  if( u16Value <= u16X_point[0] ) return u16Y_point[0];		                  // freeze when out of range
  else if( u16Value >= u16X_point[u8Size-1] ) return u16Y_point[u8Size-1];	// freeze when out of range

  if(u16Value > u16X_point[i]) // i==0 will be catched here
  {
    do ++i; while(u16Value > (u16X_point[i]) && i<u8Size);
  }
  else if(u16Value < u16X_point[i-1])
  {
    do --i; while(u16Value < (u16X_point[i-1]) && i>1);
  }

  *u8LastIndex = i;

  // increasing slope
  if((u16Y_point[i]) > (u16Y_point[i-1]))                                   
  {
    return (uint16) (u16Y_point[i-1]+(( (uint32) (u16Y_point[i]) - (u16Y_point[i-1])) * (u16Value-(u16X_point[i-1]))) / ((u16X_point[i]) - (u16X_point[i-1])));
  }
  //decreasing slope
  return (uint16) (u16Y_point[i-1]-((( (uint32) (u16Y_point[i-1]-u16Y_point[i]))) * (u16Value-(u16X_point[i-1]))) / ((u16X_point[i]) - (u16X_point[i-1])));
}

/******************************************************************************/
/**
*
* Same as interpolate16 but works with 8-bit values.
*
*/
/******************************************************************************/
uint8 interpolate8( uint8 u8Value, const uint8 *u8X_point, 
                    const uint8 *u8Y_point, uint8 u8Size, uint8 *u8LastIndex )
{
  
	register uint8 i = *u8LastIndex;

  assert(u8X_point && u8Y_point && u8LastIndex);
  assert(i<u8Size && u8Size>1); 

  if( u8Value <= u8X_point[0] ) return u8Y_point[0];		                  // freeze when out of range
	else if( u8Value >= u8X_point[u8Size-1] ) return u8Y_point[u8Size-1];	// freeze when out of range

  if(u8Value > u8X_point[i]) // i==0 will be catched here
  {
    do ++i; while(u8Value > (u8X_point[i]) && i<u8Size);
  }
  else if(u8Value < u8X_point[i-1])
  {
    do --i; while(u8Value < (u8X_point[i-1]) && i>1);
  }

  *u8LastIndex = i;

  // increasing slope
  if((u8Y_point[i]) > (u8Y_point[i-1]))                                   
  {
    return (uint8) (u8Y_point[i-1]+(( (uint32) (u8Y_point[i]) - (u8Y_point[i-1])) * (u8Value-(u8X_point[i-1]))) / ((u8X_point[i]) - (u8X_point[i-1])));
  }
  //decreasing slope
  return (uint8) (u8Y_point[i-1]-((( (uint32) (u8Y_point[i-1]-u8Y_point[i]))) * (u8Value-(u8X_point[i-1]))) / ((u8X_point[i]) - (u8X_point[i-1])));
  
}

/******************************************************************************/
/**
*
* Same as interpolate8 but outputs a 16-bit value (higher resolution)
*
*/
/******************************************************************************/
uint16 interpolate8_16( uint8 u8Value, const uint8 *u8X_point, 
                    const uint16 *u16Y_point, uint8 u8Size, uint8 *u8LastIndex )
{
  
	register uint8 i = *u8LastIndex;

  assert(u8X_point && u16Y_point && u8LastIndex);
  assert(i<u8Size && u8Size>1); 

  if( u8Value <= u8X_point[0] ) return u16Y_point[0];		                  // freeze when out of range
	else if( u8Value >= u8X_point[u8Size-1] ) return u16Y_point[u8Size-1];	// freeze when out of range

  if(u8Value > u8X_point[i]) // i==0 will be catched here
  {
    do ++i; while(u8Value > (u8X_point[i]) && i<u8Size);
  }
  else if(u8Value < u8X_point[i-1])
  {
    do --i; while(u8Value < (u8X_point[i-1]) && i>1);
  }

  *u8LastIndex = i;

  // increasing slope
  if((u16Y_point[i]) > (u16Y_point[i-1]))                                   
  {
    return (uint16) ((u16Y_point[i-1])+(( (uint32) (u16Y_point[i]) - (u16Y_point[i-1])) * (u8Value-(u8X_point[i-1])) ) / ((u8X_point[i]) - (u8X_point[i-1])));
  }
  //decreasing slope
  return (uint16) ((u16Y_point[i-1])-((( (uint32) (u16Y_point[i-1]-u16Y_point[i]))) * (u8Value-(u8X_point[i-1])) ) / ((u8X_point[i]) - (u8X_point[i-1])));
  
}

/******************************************************************************/




