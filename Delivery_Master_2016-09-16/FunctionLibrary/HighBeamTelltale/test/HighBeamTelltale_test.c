#include <stdlib.h>
#include <check.h>
#include "../inc/HighBeamTelltale.h"
#include "GCL_Stub.h"

#define HIGHBEAM_SIGNAL_OFF     0U
#define HIGHBEAM_SIGNAL_ON      1U

#define HIGHBEAMTELLTALE_OFF    0U
#define HIGHBEAMTELLTALE_ON     1U

void RunTest(PowerMode powerMode, uint8 highBeamSignal, uint8 expectedHighBeamTellTaleValue)
{
    // Arrange
    HighBeamTelltale_Init();
    GCLStub_SetPowerMode(powerMode);
    GCLStub_SetHighBeamSignal(highBeamSignal);

    // Act
    HighBeamTelltale_Run();
    uint8 highBeamTellTaleValue = GCLStub_GetHighBeamTellTaleValue();

    // Assert
    ck_assert(highBeamTellTaleValue == expectedHighBeamTellTaleValue);
}

START_TEST (IgnitionOffAndHighBeamSignalOff_HighBeamTellTaleOff)
{
    RunTest(POWER_IGNITION_OFF, HIGHBEAM_SIGNAL_OFF, HIGHBEAMTELLTALE_OFF);
}
END_TEST

START_TEST (IgnitionOffAndHighBeamSignalOn_HighBeamTellTaleOff)
{
    RunTest(POWER_IGNITION_OFF, HIGHBEAM_SIGNAL_ON, HIGHBEAMTELLTALE_OFF);
}
END_TEST

START_TEST (LowPowerModeAndHighBeamSignalOff_HighBeamTellTaleOff)
{
    RunTest(POWER_LOW, HIGHBEAM_SIGNAL_OFF, HIGHBEAMTELLTALE_OFF);
}
END_TEST

START_TEST (LowPowerModeAndHighBeamSignalOn_HighBeamTellTaleOff)
{
    RunTest(POWER_LOW, HIGHBEAM_SIGNAL_ON, HIGHBEAMTELLTALE_OFF);
}
END_TEST

START_TEST (IgnitionOnAndHighBeamSignalOff_HighBeamTellTaleOff)
{
    RunTest(POWER_IGNITION_ON, HIGHBEAM_SIGNAL_OFF, HIGHBEAMTELLTALE_OFF);
}
END_TEST

START_TEST (IgnitionOnAndHighBeamSignalOn_HighBeamTellTaleOn)
{
    RunTest(POWER_IGNITION_ON, HIGHBEAM_SIGNAL_ON, HIGHBEAMTELLTALE_ON);
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
    Suite* suite = suite_create("HighBeamTelltaleSuite");

    TCase* testCase = tcase_create("HighBeamTelltaleTests");
    tcase_add_checked_fixture(testCase, setup, teardown);
    tcase_add_test(testCase, IgnitionOffAndHighBeamSignalOff_HighBeamTellTaleOff);
    tcase_add_test(testCase, IgnitionOffAndHighBeamSignalOn_HighBeamTellTaleOff);
    tcase_add_test(testCase, LowPowerModeAndHighBeamSignalOff_HighBeamTellTaleOff);
    tcase_add_test(testCase, LowPowerModeAndHighBeamSignalOn_HighBeamTellTaleOff);
    tcase_add_test(testCase, IgnitionOnAndHighBeamSignalOff_HighBeamTellTaleOff);
    tcase_add_test(testCase, IgnitionOnAndHighBeamSignalOn_HighBeamTellTaleOn);

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


