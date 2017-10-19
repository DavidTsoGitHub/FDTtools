/*****************************************************************/
/*                                                               */
/* FILE        : Discrete.c                                      */
/* DATE        : 2015-06-09                                      */
/* AUTHOR      : Cao Chenguang                                   */
/* DESCRIPTION : source file of discrete module                  */
/* NOTE        : All Rights reserved by DETC, Technology Center  */
/*                                                               */
/*****************************************************************/
#include "jdp.h"
#include "BSP.h"
#include "IODefine.h" 
#include "SIUL.h"
#include "Discrete.h"  
#include "MCU_SwitchMode.h"

static uint32 u32g_DIVector;
static BSP_DIStatusType u32g_IgnStatus;

static void delay(uint32_t counter)    /* this function need to be removed and unified */
{
    uint32_t i,j;
    for(i=0;i<counter;i++)
    {
        for(j=0;j<counter;j++)
        {
            asm(nop);
        }
    }
}

/* the following mapping varies from different IC boards */
const static uint8 u8g_DIPinMapping_Tbl[32] = 
{
    0x1Fu, 0x1Eu, 0x1Du, 0x1Cu, 0x1Au, 0x19u, 0x1Bu, 0x18u,
    0x02u, 0x03u, 0x04u, 0x16u, 0x10u, 0x00u, 0x15u, 0x0Au,
    0x0Bu, 0x0Cu, 0x0Du, 0x0Eu, 0x0Fu, 0x11u, 0x12u, 0x13u,
    0x01u, 0x06u, 0x06u, 0x06u, 0x06u, 0x06u, 0x06u, 0x06u
};
static const uint32 u32g_DIVectorMask = 0xF0FFFFFFu;

/*
 * Name        : ECU_DiscreteInput_Init
 * Description : Initialize discrete input
 * Arguments   : none
 * Return      : none
 */
void ECU_DiscreteInput_Init(void)
{
    u32g_DIVector = u32g_DIVectorMask;
    u32g_IgnStatus = BSP_DI_OFF;
    SIUL_Output(SH_LD,1u);
    SIUL_Output(DI_CLK, 0u);
    SIUL_Output(DI_CE,1u);
}


/*
 * Name        : BSP_Get_DiscreteInput
 * Description : Returns the state of the requested discrete input signal
 * Arguments   : DInputId - The discrete input ID
 *               result   - pointer to status of the discrete input
 * Return      : BSP standard return type
 */
BSP_StdReturnType BSP_Get_DiscreteInput(uint8 DInputId, BSP_DIStatusType *result)
{
    uint8 u8t_BitPos;
    uint8 u8t_value;
    uint8 u8t_mask;
    
    /* get corresponding bit according to DInputId */
    u8t_BitPos = u8g_DIPinMapping_Tbl[DInputId];
    u8t_value = (uint8)((u32g_DIVector >> u8t_BitPos) & 0x00000001u);
    u8t_mask  = (uint8)((u32g_DIVectorMask >> u8t_BitPos) & 0x00000001u);
    u8t_value = u8t_value ^ u8t_mask;
    if(u8t_value == 0u)
    {
        *result = BSP_DI_OFF;
    }
    else
    {
        *result = BSP_DI_ON;
    }
    
    return BSP_OK;
}

/*
 * Name        : BSP_Set_DiscreteOutput
 * Description : get ignition status
 * Arguments   : DOutputId - The discrete output ID
 *               value   - The new status that the output shall be set to
 * Return      : BSP standard return type
 */
BSP_StdReturnType BSP_Get_IgnitionStatus(BSP_DIStatusType* result)
{
    *result = u32g_IgnStatus;
    return BSP_OK;
}

/*
 * Name        : BSP_Set_DiscreteOutput
 * Description : Sets the values of the discrete output to a specific value
 * Arguments   : DOutputId - The discrete output ID
 *               value   - The new status that the output shall be set to
 * Return      : BSP standard return type
 */
BSP_StdReturnType BSP_Set_DiscreteOutput(uint8 DOutputId, BSP_DOStatusType value)
{
    SIUL_Output(DOutputId, value);
    return BSP_OK;
}

/*
 * Name        : BSP_ReportError_DiscreteInput
 * Description : Check for errors on the requested discrete input.
 * Arguments   : DInputId  - The discrete input ID
               : FTB       - pointer failure type byte
 * Return      : BSP standard return type
 */
BSP_StdReturnType BSP_ReportError_DiscreteInput(uint8 DInputId, uint8* FTB)
{
    DInputId = DInputId;
    *FTB = 0;
    return BSP_OK;
}

/*
 * Name        : BSP_ReportError_DiscreteOutput
 * Description : Check for errors on the requested discrete output.
 * Arguments   : DOutputId - The discrete output ID
               : FTB       - pointer to failure type byte
 * Return      : BSP standard return type
 */
BSP_StdReturnType BSP_ReportError_DiscreteOutput(uint8 DOutputId, uint8* FTB)
{
    DOutputId = DOutputId;
    *FTB = 0;
    return BSP_OK;
}

void ECU_DiscreteInput_Runnable(void)
{
    uint32 i, j;
    uint8 u8t_Sum;
    uint8 u8t_IgnSum;
    uint16 BatteryVoltage;
    static uint32 u32s_CurDIVector = 0;
    static uint8 u8s_DIBuffer[DI_INPUT_NUM][4] = {0};
    static uint8 u8s_IgnDIBuffer[4] = {0};
    static uint8 u8s_RunCnt = 0;    /* record running times */

    /*loading data*/
    SIUL_Output(SH_LD, 0);
    delay(4);
    SIUL_Output(SH_LD, 1);  
        
    /*start clocking*/
    delay(3); 
    SIUL_Output(DI_CE, 0);
        
    /* read current data */
    for(i = 0; i < DI_INPUT_NUM; i++)
    {
        SIUL_Output(DI_CLK, 1);
        u8s_DIBuffer[i][u8s_RunCnt] = SIUL_GetInput(P_SOUT); 
        SIUL_Output(DI_CLK, 0);
    }
    u8s_IgnDIBuffer[u8s_RunCnt] = SIUL_GetInput(IGN_ON);

    u8s_RunCnt++;

    /* de-bounce the discrete input at the 4th calling */
    if(u8s_RunCnt > 3u)
    {
        u8s_RunCnt = 0;
        for(i = 0; i < DI_INPUT_NUM; i++)
        {
            u8t_Sum = 0u;
            for(j = 0; j < 4u; j++)    /* get the sum of inputs */
            {
                u8t_Sum = u8t_Sum + u8s_DIBuffer[i][j];
            }
            if(u8t_Sum == 0u)     /* all of the inputs is 0, set 0 to the corresponding bit */
            {
                u32s_CurDIVector = u32s_CurDIVector & (~(1uL << i));
            }
            else if(u8t_Sum == 4u) /* all of the inputs is 1, set 1 to the corresponding bit*/
            {
                u32s_CurDIVector = u32s_CurDIVector | (1uL << i);
            }
            else
            {
                ;    /* remain previous value */
            }
        }

        /* check whether ignition is on */
        u8t_IgnSum = 0;
        for(j = 0; j < 4; j++)
        {
            u8t_IgnSum = u8t_IgnSum + u8s_IgnDIBuffer[j];
        }
        if(u8t_IgnSum == 0u)
        {
            u32g_IgnStatus = BSP_DI_OFF;
        }
        else if(u8t_IgnSum == 4u)
        {
            u32g_IgnStatus = BSP_DI_ON;
        }
        else
        {
            ;    /* remain previous value */
        }

        if(u32g_IgnStatus == 1u)
        {
        	u32g_DIVector = u32s_CurDIVector;    /* update DI vector */
        }
        else
        {
        	u32g_DIVector = u32g_DIVectorMask;   /* default DI vector */
        }
    }
}

