/********************************* Includes **********************************/
#include "OdoRuntimeHandler.h"
#include "GCL_OdoRuntimeHandler.h"
//#include "StandardTypes.h"

/**************************** Constants and Types ****************************/
/********************************* Variables *********************************/
static boolean ShowNextIndicated = FALSE;
static boolean ShowOdo;
static uint32 LastOdoValue;
static uint16 LastRuntimeValue;
static boolean DisplayChanged;


/************************* Local Function Prototypes *************************/

/***************************** Exported Functions ****************************/

void OdoRuntimeHandler_Init(void)
{
    ShowOdo = TRUE;
    LastOdoValue = 0;
    LastRuntimeValue = 0;
    DisplayChanged = TRUE;
}

void OdoRuntimeHandler_Run(void)
{
    uint8 u8SignalStatusOdo;
    uint32 OdoValue;
    uint8 u8SignalStatusRuntime;
    uint16 RuntimeValue;

    if (ShowNextIndicated == TRUE)
    {
        ShowOdo = (ShowOdo == TRUE) ? FALSE : TRUE;
        ShowNextIndicated = FALSE;
        DisplayChanged = TRUE;
    }
    if(ShowOdo == TRUE)
    {
        GCL_Read_OdoRuntimeHandler_OdoValue(&OdoValue, &u8SignalStatusOdo);
        if(u8SignalStatusOdo == GCL_SIGNAL_OK)
        {
            if((LastOdoValue != OdoValue) || (DisplayChanged == TRUE))
            {
                GCL_Write_OdoRuntimeHandler_Value(OdoValue);
                GCL_Write_OdoRuntimeHandler_ShowDot(FALSE);
                GCL_Write_OdoRuntimeHandler_ShowHourGlass(FALSE);
                LastOdoValue = OdoValue;
                DisplayChanged = FALSE;
            }
        }
    }
    else
    {
        GCL_Read_OdoRuntimeHandler_RuntimeValue(&RuntimeValue, &u8SignalStatusRuntime);
        if(u8SignalStatusRuntime == GCL_SIGNAL_OK)
        {
            if((LastRuntimeValue != RuntimeValue) || (DisplayChanged == TRUE))
            {
                GCL_Write_OdoRuntimeHandler_Value(RuntimeValue);
                GCL_Write_OdoRuntimeHandler_ShowDot(TRUE);
                GCL_Write_OdoRuntimeHandler_ShowHourGlass(TRUE);                   
                LastRuntimeValue = RuntimeValue;
                DisplayChanged = FALSE;
            }
        }
    }
}

void OdoRuntimeHandler_ShowNext_Indication()
{
    ShowNextIndicated = TRUE;
}

/****************************** Local Functions ******************************/
