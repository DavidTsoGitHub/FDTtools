/********************************* Includes **********************************/
#include "FaultBlockDummy.h"
#include "FaultBlockDummy_cfg.h"
#include "GCL_FaultBlockDummy.h"
#include "StandardTypes.h"
#include "DTCManager.h"
#include "DiagManager.h"

/**************************** Constants and Types ****************************/
//extern const uint32 DTC_Fault1; // A small fault
//extern const uint32 DTC_Fault2; // A larger fault

/********************************* Variables *********************************/
static uint8 Trigger1State;
static uint8 Trigger2State;
static uint8 Trigger1Position;
static uint8 Trigger2Position;
static uint8 DTCIndicated;

static LabelMappings ActiveDTCs =
{
    .Count = 0,
    .Mappings = 
        {
            { .LabelID = 0, .Value = 0 },
            { .LabelID = 0, .Value = 1 },
            { .LabelID = 0, .Value = 2 },
            { .LabelID = 0, .Value = 3 },
            { .LabelID = 0, .Value = 4 },
            { .LabelID = 0, .Value = 5 },
            { .LabelID = 0, .Value = 6 },
            { .LabelID = 0, .Value = 7 },
            { .LabelID = 0, .Value = 8 },
            { .LabelID = 0, .Value = 9 },
            { .LabelID = 0, .Value = 10 },
            { .LabelID = 0, .Value = 11 }
        }
};

/************************* Local Function Prototypes *************************/

/***************************** Exported Functions ****************************/
void FaultBlockDummy_Init(void)
{
    Trigger1State = 0;
    Trigger2State = 0;
    Trigger1Position = 0;
    Trigger2Position = 0;
    DTCIndicated = 0;

    GCL_Write_FaultBlockDummy_ActiveDTCs(&ActiveDTCs); // Initial write of labelmappings for DTCs to GCL
    GCL_Write_FaultBlockDummy_DTCIndication(0);
}

void FaultBlockDummy_Run(void)
{
    if((Trigger1State == 1U) && (Trigger1Position == 0U))
    {
        ActiveDTCs.Mappings[0].LabelID = DTCManager_GetDTCLabelID(DTC_Fault1);
        ActiveDTCs.Count += 1U;
        Trigger1Position = 1;
        DTCIndicated = 0;
    }
    else if((Trigger1State == 0U) && (Trigger1Position > 0U))
    {
        ActiveDTCs.Mappings[0].LabelID = 0;
        ActiveDTCs.Count -= 1U;
        Trigger1Position = 0;
        DTCIndicated = 0;
    }
    else
    {
        // Do nothing
    }

    if((Trigger2State == 1U) && (Trigger2Position == 0U))
    {
        ActiveDTCs.Mappings[1].LabelID = DTCManager_GetDTCLabelID(DTC_Fault2);
        ActiveDTCs.Count += 1U;
        Trigger2Position = 1;
        DTCIndicated = 0;
    }
    else if((Trigger2State == 0U) && (Trigger2Position > 0U))
    {
        ActiveDTCs.Mappings[1].LabelID = 0;
        ActiveDTCs.Count -= 1U;
        Trigger2Position = 0;
        DTCIndicated = 0;
    }
    else
    {
        // Do nothing
    }

    if((((Trigger1State == 1U) && (Trigger1Position == 1U)) || ((Trigger2State == 1U) && (Trigger2Position == 1U))) && (DTCIndicated == 0U))
    {
        GCL_Write_FaultBlockDummy_DTCIndication(1);
        DTCIndicated = 1;
    }
    else if(((((Trigger1State == 0U) && (Trigger1Position == 0U)) && (Trigger2State == 0U)) || (((Trigger2State == 0U) && (Trigger2Position == 0U)) && (Trigger1State == 0U))) && (DTCIndicated == 0U))
    {
        GCL_Write_FaultBlockDummy_DTCIndication(0);
        DTCIndicated = 1;
    }
    else
    {
        // Do nothing
    }
}

void FaultBlockDummy_Trigger1_Indication()
{
    uint8 u8SignalStatus;
    uint8 Trigger1;

    GCL_Read_FaultBlockDummy_Trigger1(&Trigger1, &u8SignalStatus);

    if((Trigger1 == 1U) && (Trigger1State == 0U))
    {
        Trigger1State = 1;
        tDiagReport errorReport;
        errorReport.u32DTCNumber = DTC_Fault1;
        errorReport.Status = DTC_ACTIVE;
        DIAG_ReportError(errorReport);
    }else if ((Trigger1 == 0U) && (Trigger1State == 1U))
    {
        Trigger1State = 0;
        tDiagReport errorReport;
        errorReport.u32DTCNumber = DTC_Fault1;
        errorReport.Status = DTC_INACTIVE;
        DIAG_ReportError(errorReport);
    }
    else
    {
        // Do nothing
    }
}

void FaultBlockDummy_Trigger2_Indication()
{
    uint8 u8SignalStatus;
    uint8 Trigger2;

    GCL_Read_FaultBlockDummy_Trigger2(&Trigger2, &u8SignalStatus);

    if((Trigger2 == 1U) && (Trigger2State == 0U))
    {
        Trigger2State = 1;
        tDiagReport errorReport;
        errorReport.u32DTCNumber = DTC_Fault2;
        errorReport.Status = DTC_ACTIVE;
        DIAG_ReportError(errorReport);
    }
    else if ((Trigger2 == 0U) && (Trigger2State == 1U))
    {
        Trigger2State = 0;
        tDiagReport errorReport;
        errorReport.u32DTCNumber = DTC_Fault2;
        errorReport.Status = DTC_INACTIVE;
        DIAG_ReportError(errorReport);
    }
    else
    {
        // Do nothing
    }
}

/****************************** Local Functions ******************************/
