/********************************* Includes **********************************/
#include "CANTestBlock.h"
#include "GCL_CANTestBlock.h"
#include "StandardTypes.h"

/**************************** Constants and Types ****************************/
/********************************* Variables *********************************/

/************************* Local Function Prototypes *************************/

/***************************** Exported Functions ****************************/
/* Sample code is only generated for the function block named task functions
   If you have defined more tasks, they have to be added manually */

void CANTestBlock_Init(void)
{
    // Add initialization routines here
}

void CANTestBlock_Run(void)
{
    static uint8 CyclicCounter = 0u;
    
    /** Input signal reading samples **/
    uint8 u8SignalStatus;
    uint8 TestInput1;
    uint8 TestInput2;

    static uint8 u8OldInput1 = 0u;
    static uint8 u8OldInput2 = 0u;

    GCL_Read_CANTestBlock_TestInput1(&TestInput1, &u8SignalStatus);
    GCL_Read_CANTestBlock_TestInput2(&TestInput2, &u8SignalStatus);

    if ( u8OldInput1 != TestInput1)
    {
        GCL_Write_CANTestBlock_OutEvent16((uint16)TestInput1);    

    }
    if ( u8OldInput2 != TestInput2)
    {
        GCL_Write_CANTestBlock_OutEvent8(TestInput2);    

    }

    if (CyclicCounter > 50u)
    {
        GCL_Write_CANTestBlock_OutCyclic8(CyclicCounter);
        GCL_Write_CANTestBlock_OutCyclic16(CyclicCounter);     
        CyclicCounter = 0u;
    }
    else
    {
        CyclicCounter++;
    }

    u8OldInput1 = TestInput1;
    u8OldInput2 = TestInput2;
}

/****************************** Local Functions ******************************/
