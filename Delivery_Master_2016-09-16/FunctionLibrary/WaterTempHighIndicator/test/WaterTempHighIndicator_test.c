#include <stdlib.h>
#include <check.h>
#include "../inc/WaterTempHighIndicator.h"
#include "SoundManager.h"
#include "GCL_Stub.h"

/**** TEST SPECIFIC DEFINES ****/
#define WATERTEMP_SIGNAL_OFF	0U
#define WATERTEMP_SIGNAL_ON		1U

#define NO_SOUND_EXPECTED   0xFF

#define WATERTEMPHIGHTELLTALE_OFF	0U
#define WATERTEMPHIGHTELLTALE_ON	1U

void RunTest(PowerMode powerMode, uint8 waterTempSignal, uint8 expectedWaterTempHighTellTaleValue, uint8 expectedBuzzerSound)
{
    // Arrange
    WaterTempHighIndicator_Init();
	SoundManager_Init();
	GCLStub_SetPowerMode(powerMode);
    GCLStub_SetPowerModeSignalStatus(GCL_SIGNAL_OK);
    GCLStub_SetWaterTempHighIndication(waterTempSignal, GCL_SIGNAL_OK);

    // Act
    WaterTempHighIndicator_Run();

    // Assert
    uint8 waterTempHighTellTaleValue = GCLStub_GetWaterTempHighTellTaleValue();
    ck_assert_msg(waterTempHighTellTaleValue == expectedWaterTempHighTellTaleValue, "current telltale value: %d, expected telltale value: %d", waterTempHighTellTaleValue, expectedWaterTempHighTellTaleValue);
    
    if (expectedBuzzerSound == NO_SOUND_EXPECTED)
    {
        int callcount = SoundManagerStub_GetSoundRequestCallCount();
        ck_assert_msg(callcount == 0, "no sound was expected! (sound requests triggered:%d)", callcount);
    }
    else
    {
        BSP_BuzzerSound currentBuzzerSound = SoundManagerStub_GetCurrentActiveBuzzerSound();
        ck_assert_msg(currentBuzzerSound == expectedBuzzerSound, "current sound: %d, expected sound: %d", currentBuzzerSound, expectedBuzzerSound);        
    }
}

START_TEST (IgnitionOnWaterTempSignalOff_WaterTempHighTellTaleOff)
{
    RunTest(POWER_IGNITION_ON, WATERTEMP_SIGNAL_OFF, WATERTEMPHIGHTELLTALE_OFF, NO_SOUND_EXPECTED);
}
END_TEST

START_TEST (IgnitionOnWaterTempSignalOn_WaterTempHighTellTaleOn)
{
    RunTest(POWER_IGNITION_ON, WATERTEMP_SIGNAL_ON, WATERTEMPHIGHTELLTALE_ON, BSP_SOUND_ON);
}
END_TEST

START_TEST (PowerLowWaterTempSignalOff_WaterTempHighTellTaleOff)
{
    RunTest(POWER_LOW, WATERTEMP_SIGNAL_OFF, WATERTEMPHIGHTELLTALE_OFF, NO_SOUND_EXPECTED);
}
END_TEST

START_TEST (PowerLowWaterTempSignalOn_WaterTempHighTellTaleOff)
{
    RunTest(POWER_LOW, WATERTEMP_SIGNAL_ON, WATERTEMPHIGHTELLTALE_OFF, NO_SOUND_EXPECTED);
}
END_TEST

START_TEST (IgnitionOffWaterTempSignalOff_WaterTempHighTellTaleOff)
{
    RunTest(POWER_IGNITION_OFF, WATERTEMP_SIGNAL_OFF, WATERTEMPHIGHTELLTALE_OFF, NO_SOUND_EXPECTED);
}
END_TEST

START_TEST (IgnitionOffWaterTempSignalOn_WaterTempHighTellTaleOff)
{
    RunTest(POWER_IGNITION_OFF, WATERTEMP_SIGNAL_ON, WATERTEMPHIGHTELLTALE_OFF, NO_SOUND_EXPECTED);
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
    Suite* suite = suite_create("WaterTempHighIndicatorSuite");

    TCase* testCase = tcase_create("WaterTempHighIndicatorTests");
    tcase_add_checked_fixture(testCase, setup, teardown);
    tcase_add_test(testCase, IgnitionOnWaterTempSignalOff_WaterTempHighTellTaleOff);
	tcase_add_test(testCase, IgnitionOnWaterTempSignalOn_WaterTempHighTellTaleOn);
	tcase_add_test(testCase, PowerLowWaterTempSignalOff_WaterTempHighTellTaleOff);
	tcase_add_test(testCase, PowerLowWaterTempSignalOn_WaterTempHighTellTaleOff);
	tcase_add_test(testCase, IgnitionOffWaterTempSignalOff_WaterTempHighTellTaleOff);
	tcase_add_test(testCase, IgnitionOffWaterTempSignalOn_WaterTempHighTellTaleOff);

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


