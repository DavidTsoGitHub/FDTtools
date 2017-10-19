#include <stdlib.h>
#include <check.h>
#include "../inc/UreaLevelMeter.h"
#include "GCL_Stub.h"

#define SEGMENT_ON      0x01
#define SEGMENT_OFF     0x00


#define TANKEMPTY       0x00
#define TANKLEVEL20     (20/0.4)
#define TANKLEVEL40     (40/0.4)
#define TANKLEVEL60     (60/0.4)
#define TANKLEVEL80     (80/0.4)
#define TANKFULL        0xFF

#define GCL_SIGNAL_OK       (0x00)
#define GCL_SIGNAL_TIMEOUT  (1 << 0)

void AssertGCLReadTankLevelCalls(uint8 expectedCalls);
void AssertSegmentStatus(uint8 segment0, uint8 segment1, uint8 segment2, uint8 segment3);

START_TEST (InitializeFunctionBlock_AllSegmentsSetToZero)
{
	// Act
	UreaLevelMeter_Init();
	
	// Assert
    AssertSegmentStatus(SEGMENT_OFF, SEGMENT_OFF, SEGMENT_OFF, SEGMENT_OFF);
}
END_TEST

START_TEST (CatalystTankEmpty_AllSegmentsSetToZero)
{
    // Arrange
    GCLStub_SetCatalystTankLevel(TANKEMPTY, GCL_SIGNAL_OK);
    
    // Act
    UreaLevelMeter_Init();
    UreaLevelMeter_Run();

    // Assert
    AssertSegmentStatus(SEGMENT_OFF, SEGMENT_OFF, SEGMENT_OFF, SEGMENT_OFF);
}
END_TEST

START_TEST (PowerOffCatalystTankNotEmpty_NoTankLevelReadAndAllSegmentsSetToZero)
{
    // Arrange
    GCLStub_SetPowerMode(POWER_IGNITION_OFF, GCL_SIGNAL_OK);
    GCLStub_SetCatalystTankLevel(TANKFULL, GCL_SIGNAL_OK);
    
    // Act
    UreaLevelMeter_Init();
    UreaLevelMeter_Run();
    
    // Assert
    AssertGCLReadTankLevelCalls(0);
    AssertSegmentStatus(SEGMENT_OFF, SEGMENT_OFF, SEGMENT_OFF, SEGMENT_OFF);
}
END_TEST

START_TEST (PowerLowCatalystTankNotEmpty_NoTankLevelReadAndAllSegmentsSetToZero)
{
    // Arrange
    GCLStub_SetPowerMode(POWER_LOW, GCL_SIGNAL_OK);
    GCLStub_SetCatalystTankLevel(TANKFULL, GCL_SIGNAL_OK);
    
    // Act
    UreaLevelMeter_Init();
    UreaLevelMeter_Run();
    
    // Assert
    AssertGCLReadTankLevelCalls(0);
    AssertSegmentStatus(SEGMENT_OFF, SEGMENT_OFF, SEGMENT_OFF, SEGMENT_OFF);
}
END_TEST

START_TEST (PowerOnCatalystTankLevelFullSweep_SegmentsLightUpAtDefinedIntervals)
{
    // Arrange
    GCLStub_SetPowerMode(POWER_IGNITION_ON, GCL_SIGNAL_OK);
    UreaLevelMeter_Init();
    
    // Act & Assert
    for (int i=TANKEMPTY; i<=TANKFULL; ++i)
    {
        GCLStub_SetCatalystTankLevel(i, GCL_SIGNAL_OK);
        UreaLevelMeter_Run();
        if (i >= TANKLEVEL80) AssertSegmentStatus(SEGMENT_ON, SEGMENT_ON, SEGMENT_ON, SEGMENT_ON);
        else if (i>=TANKLEVEL60) AssertSegmentStatus(SEGMENT_ON, SEGMENT_ON, SEGMENT_ON, SEGMENT_OFF);
        else if (i>=TANKLEVEL40) AssertSegmentStatus(SEGMENT_ON, SEGMENT_ON, SEGMENT_OFF, SEGMENT_OFF);
        else if (i>=TANKLEVEL20) AssertSegmentStatus(SEGMENT_ON, SEGMENT_OFF, SEGMENT_OFF, SEGMENT_OFF);
        else AssertSegmentStatus(SEGMENT_OFF, SEGMENT_OFF, SEGMENT_OFF, SEGMENT_OFF);
    }
}
END_TEST

START_TEST (PowerOnCatalystTankNotEmptyButSignalTimeout_AllSegmentsSetToZero)
{
    // Arrange
    GCLStub_SetPowerMode(POWER_IGNITION_ON, GCL_SIGNAL_OK);
    UreaLevelMeter_Init();
    GCLStub_SetCatalystTankLevel(TANKFULL, GCL_SIGNAL_OK);
    UreaLevelMeter_Run();
    AssertSegmentStatus(SEGMENT_ON, SEGMENT_ON, SEGMENT_ON, SEGMENT_ON);
    
    // Act 
    GCLStub_SetCatalystTankLevel(TANKFULL, GCL_SIGNAL_TIMEOUT);
    UreaLevelMeter_Run();
    
    // Assert
    AssertSegmentStatus(SEGMENT_OFF, SEGMENT_OFF, SEGMENT_OFF, SEGMENT_OFF);
}
END_TEST

START_TEST (PowerLowCatalystTankFull_AllSegmentsSetToZero)
{
    // Arrange
    GCLStub_SetPowerMode(POWER_IGNITION_ON, GCL_SIGNAL_OK);
    UreaLevelMeter_Init();
    GCLStub_SetCatalystTankLevel(TANKFULL, GCL_SIGNAL_OK);
    UreaLevelMeter_Run();
    AssertSegmentStatus(SEGMENT_ON, SEGMENT_ON, SEGMENT_ON, SEGMENT_ON);
    
    // Act 
    GCLStub_SetPowerMode(POWER_LOW, GCL_SIGNAL_OK);
    UreaLevelMeter_Run();
    
    // Assert
    AssertSegmentStatus(SEGMENT_OFF, SEGMENT_OFF, SEGMENT_OFF, SEGMENT_OFF);
}
END_TEST


void setup(void)
{
    GCLStub_Reset();
}

void teardown(void)
{}

Suite* testSuite_create()
{
    Suite* suite = suite_create("UreaLevelMeterSuite");

    TCase* testCase = tcase_create("UreaLevelMeterTests");
    tcase_add_checked_fixture(testCase, setup, teardown);
    tcase_add_test(testCase, InitializeFunctionBlock_AllSegmentsSetToZero);
    tcase_add_test(testCase, CatalystTankEmpty_AllSegmentsSetToZero);
    tcase_add_test(testCase, PowerOnCatalystTankLevelFullSweep_SegmentsLightUpAtDefinedIntervals);
    tcase_add_test(testCase, PowerLowCatalystTankNotEmpty_NoTankLevelReadAndAllSegmentsSetToZero);
    tcase_add_test(testCase, PowerOffCatalystTankNotEmpty_NoTankLevelReadAndAllSegmentsSetToZero);
    tcase_add_test(testCase, PowerOnCatalystTankNotEmptyButSignalTimeout_AllSegmentsSetToZero);
    tcase_add_test(testCase, PowerLowCatalystTankFull_AllSegmentsSetToZero);

    suite_add_tcase(suite, testCase);

    return suite;
}

int main (void)
{
    Suite* testSuite = testSuite_create();
    SRunner* suiteRunner = srunner_create(testSuite);

    srunner_run_all(suiteRunner, CK_NORMAL);
    int failedTestCount = srunner_ntests_failed(suiteRunner);
    srunner_free(suiteRunner);

    return (failedTestCount == 0 ? EXIT_SUCCESS : EXIT_FAILURE);
}

void AssertGCLReadTankLevelCalls(uint8 expectedCalls)
{
    uint8 actualCalls = GCLStub_GetReadCatalystTankLevelCallCount();
    ck_assert_msg(expectedCalls == actualCalls, "ReadCatalystTankLevelCallCount - Expected: %d   Actual: %d", expectedCalls, actualCalls);
}

void AssertSegmentStatus(uint8 segment0Expected, uint8 segment1Expected, uint8 segment2Expected, uint8 segment3Expected)
{
    uint8 segment0Actual = GCLStub_GetSegment0();
    uint8 segment1Actual = GCLStub_GetSegment1();
    uint8 segment2Actual = GCLStub_GetSegment2();
    uint8 segment3Actual = GCLStub_GetSegment3();

    ck_assert_msg(segment0Actual == segment0Expected, "Segment0 - Expected: %d   Actual: %d", segment0Expected, segment0Actual);
    ck_assert_msg(segment1Actual == segment1Expected, "Segment1 - Expected: %d   Actual: %d", segment1Expected, segment1Actual);
    ck_assert_msg(segment2Actual == segment2Expected, "Segment2 - Expected: %d   Actual: %d", segment2Expected, segment2Actual);
    ck_assert_msg(segment3Actual == segment3Expected, "Segment3 - Expected: %d   Actual: %d", segment3Expected, segment3Actual);
}