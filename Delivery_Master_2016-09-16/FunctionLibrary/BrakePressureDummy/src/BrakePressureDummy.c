/********************************* Includes **********************************/
#include "BrakePressureDummy.h"
#include "GCL_BrakePressureDummy.h"
#include "Platform_Types.h"

/**************************** Constants and Types ****************************/

/********************************* Variables *********************************/
static uint16 currentValue = 0;
static boolean countDown = FALSE;

/************************* Local Function Prototypes *************************/

/***************************** Exported Functions ****************************/
void BrakePressureDummy_Init(void)
{
    currentValue = 0;
    countDown = FALSE;
}

void BrakePressureDummy_Run(void)
{
    GCL_Write_BrakePressureDummy_BrakePressure(currentValue);

    if(countDown == TRUE)
    {
        --currentValue;
    }
    else
    {
        ++currentValue;
    }

    if ((currentValue < 1U) || (currentValue > 1200U))
    {
        countDown = (countDown == TRUE) ? FALSE : TRUE;
    }
}

/****************************** Local Functions ******************************/
