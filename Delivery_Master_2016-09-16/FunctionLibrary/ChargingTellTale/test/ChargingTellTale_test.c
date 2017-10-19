#include <stdlib.h>
#include <check.h>
#include "../inc/ChargingTellTale.h"
#include "GCL_Stub.h"

#define CHARGING_SIGNAL_OFF     0U
#define CHARGING_SIGNAL_ON      1U

#define CHARGINGTELLTALE_OFF    0U
#define CHARGINGTELLTALE_ON     1U

void RunTest(PowerMode powerMode, uint8 chargingSignal, uint8 expectedChargingTellTaleValue)
{
    // Arrange
    ChargingTellTale_Init();
    GCLStub_SetPowerMode(powerMode);
    GCLStub_SetChargingSignal(chargingSignal);

    // Act
    ChargingTellTale_Run();
    uint8 chargingTellTaleValue = GCLStub_GetChargingTellTaleValue();

    // Assert
    ck_assert(chargingTellTaleValue == expectedChargingTellTaleValue);
}

START_TEST (PowerModeOffAndChargingSignalOff_ChargingTellTaleOff)
{
    RunTest(POWER_IGNITION_OFF, CHARGING_SIGNAL_OFF, CHARGINGTELLTALE_OFF);
}
END_TEST

START_TEST (PowerModeOffAndChargingSignalOn_ChargingTellTaleOff)
{
    RunTest(POWER_IGNITION_OFF, CHARGING_SIGNAL_ON, CHARGINGTELLTALE_OFF);
}
END_TEST

START_TEST (PowerModeOnAndChargingSignalOff_ChargingTellTaleOff)
{
    RunTest(POWER_IGNITION_ON, CHARGING_SIGNAL_OFF, CHARGINGTELLTALE_OFF);
}
END_TEST

START_TEST (PowerModeOnAndChargingSignalOn_ChargingTellTaleOn)
{
    RunTest(POWER_IGNITION_ON, CHARGING_SIGNAL_ON, CHARGINGTELLTALE_ON);
}
END_TEST

START_TEST (PowerModeLowAndChargingSignalOff_ChargingTellTaleOff)
{
    RunTest(POWER_LOW, CHARGING_SIGNAL_OFF, CHARGINGTELLTALE_OFF);
}
END_TEST

START_TEST (PowerModeLowAndChargingSignalOn_ChargingTellTaleOff)
{
    RunTest(POWER_LOW, CHARGING_SIGNAL_ON, CHARGINGTELLTALE_OFF);
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
    Suite* suite = suite_create("ChargingTellTaleSuite");

    TCase* testCase = tcase_create("ChargingTellTaleTests");
    tcase_add_checked_fixture(testCase, setup, teardown);
    tcase_add_test(testCase, PowerModeOffAndChargingSignalOff_ChargingTellTaleOff);
	tcase_add_test(testCase, PowerModeOffAndChargingSignalOn_ChargingTellTaleOff);
	tcase_add_test(testCase, PowerModeOnAndChargingSignalOff_ChargingTellTaleOff);
	tcase_add_test(testCase, PowerModeOnAndChargingSignalOn_ChargingTellTaleOn);
	tcase_add_test(testCase, PowerModeLowAndChargingSignalOff_ChargingTellTaleOff);
	tcase_add_test(testCase, PowerModeLowAndChargingSignalOn_ChargingTellTaleOff);

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


