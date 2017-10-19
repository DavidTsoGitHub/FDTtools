/********************************* Includes **********************************/
#include "UreaLevelMeter.h"
#include "GCL_UreaLevelMeter.h"
#include "Platform_Types.h"
#include "Logger.h"

#define SEGMENT_INIT 0xFF
#define SEGMENT_ON 0x01
#define SEGMENT_OFF 0x00

#define TANKLEVEL20 0x32U // 20/0.4 = 50
#define TANKLEVEL40 0x64U // 40/0.4 = 100
#define TANKLEVEL60 0x96U // 60/0.4 = 150
#define TANKLEVEL80 0xC8U // 80/0.4 = 200

static void SetSegmentStatus(uint8 segment0, uint8 segment1, uint8 segment2, uint8 segment3);
static uint8 oldSegment0Status;
static uint8 oldSegment1Status;
static uint8 oldSegment2Status;
static uint8 oldSegment3Status;

void UreaLevelMeter_Init(void)
{
    Logger_Info("UreaLevelMeter", "UreaLevelMeter_Init");
    oldSegment0Status = SEGMENT_INIT;
    oldSegment1Status = SEGMENT_INIT;
    oldSegment2Status = SEGMENT_INIT;
    oldSegment3Status = SEGMENT_INIT;
    SetSegmentStatus(SEGMENT_INIT, SEGMENT_INIT, SEGMENT_INIT, SEGMENT_INIT);
}

void UreaLevelMeter_Run(void)
{
    uint8 u8SignalStatus;
    uint8 u8CatalystTankLevel;
    PowerMode powerMode;
    
    GCL_Read_UreaLevelMeter_PowerMode(&powerMode, &u8SignalStatus);
    
    switch (powerMode)
    {
        case POWER_IGNITION_ON:
            GCL_Read_UreaLevelMeter_CatalystTankLevel(&u8CatalystTankLevel, &u8SignalStatus);
    
            if ((u8SignalStatus & GCL_SIGNAL_TIMEOUT) > 0U)
            {
                SetSegmentStatus(SEGMENT_OFF, SEGMENT_OFF, SEGMENT_OFF, SEGMENT_OFF);
            }
            else if (u8CatalystTankLevel >= TANKLEVEL80)
            {
                SetSegmentStatus(SEGMENT_ON, SEGMENT_ON, SEGMENT_ON, SEGMENT_ON);
            }
            else if (u8CatalystTankLevel>=TANKLEVEL60)
            {
                SetSegmentStatus(SEGMENT_ON, SEGMENT_ON, SEGMENT_ON, SEGMENT_OFF);
            }
            else if (u8CatalystTankLevel>=TANKLEVEL40)
            {
                SetSegmentStatus(SEGMENT_ON, SEGMENT_ON, SEGMENT_OFF, SEGMENT_OFF);
            }
            else if (u8CatalystTankLevel>=TANKLEVEL20)
            {
                SetSegmentStatus(SEGMENT_ON, SEGMENT_OFF, SEGMENT_OFF, SEGMENT_OFF);
            }
            else
            {
                SetSegmentStatus(SEGMENT_OFF, SEGMENT_OFF, SEGMENT_OFF, SEGMENT_OFF);
            }
        break;
        case POWER_IGNITION_OFF: // All other power modes (including unknown) shall turn off all segments
        case POWER_LOW:
        case POWER_INVALID:
        default: 
            SetSegmentStatus(SEGMENT_OFF, SEGMENT_OFF, SEGMENT_OFF, SEGMENT_OFF);
        break;
    }
}

static void SetSegmentStatus(uint8 segment0, uint8 segment1, uint8 segment2, uint8 segment3)
{
    if (oldSegment0Status != segment0)
    {
        GCL_Write_UreaLevelMeter_Segment0(segment0);
    }
    
    if (oldSegment1Status != segment1)
    {
        GCL_Write_UreaLevelMeter_Segment1(segment1);
    }
    
    if (oldSegment2Status != segment2)
    {
        GCL_Write_UreaLevelMeter_Segment2(segment2);
    }
    
    if (oldSegment3Status != segment3)
    {
        GCL_Write_UreaLevelMeter_Segment3(segment3);
    }
    
    oldSegment0Status = segment0;
    oldSegment1Status = segment1;
    oldSegment2Status = segment2;
    oldSegment3Status = segment3;
}
