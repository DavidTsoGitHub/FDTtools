//lint -save
/********************************* Includes **********************************/
#include "COM_Stack.h"
#include "COM.h"
#include "CAN_IF.h"
#include "CAN_Driver.h"

/***************************** Exported Functions ****************************/
void COM_Stack_Init(void)
{
    COM_Init();
    CAN_IF_Init();
}

void COM_Stack_Run(void)
{
    // The CAN stack is interrupt driven therefore we need to suppress MISRA warning since nothing is done in the _Run tasks.
    COM_Run();
    //lint --e(522) [Working as intended, no operations in called function]
    CAN_IF_Run();
    //lint --e(522) [Working as intended, no operations in called function]
    CAN_Run();
}
//lint -restore
