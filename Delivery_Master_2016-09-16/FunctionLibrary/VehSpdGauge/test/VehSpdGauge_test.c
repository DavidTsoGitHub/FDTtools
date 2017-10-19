#include <stdlib.h>
#include <check.h>
#include "../inc/VehSpdGauge.h"
#include "GCL_Stub.h"

uint16 u16NeedleRestPos = 0;
uint16 u16NeedleMaxPos = 140;
uint16 u16VehSpdBias = 3;

static void assertValue(uint16 currentValue, uint16 expectedValue);
static void stabilizeGaugeValue(void);
static uint32 SpeedToFrequency(uint16 speed);


START_TEST (IgnitionOff_NeedleInRestPosition)
{
    // Arrange
    const uint32 expectedValue = 0U;
    GCLStub_SetSystemPowerMode(POWER_IGNITION_OFF);

    // Act
    VehSpdGauge_Run();
    uint32 displayValue = GCLStub_GetVehSpd_dsp();

    // Assert
    ck_assert_msg(displayValue == expectedValue, "Expected: %d  Actual: %d", expectedValue, displayValue);
}
END_TEST

START_TEST (IgnitionOnNoVehSpd_NeedleInZeroPos)
{
    // Arrange
    const uint32 expectedValue = u16VehSpdBias;
    GCLStub_SetSystemPowerMode(POWER_IGNITION_ON);
    GCLStub_SetInputFrequency(0U);

    // Act
    stabilizeGaugeValue();
    uint32 displayValue = GCLStub_GetVehSpd_dsp();

    // Assert
    ck_assert_msg(displayValue == expectedValue, "Expected: %d  Actual: %d", expectedValue, displayValue);
}
END_TEST

START_TEST (IgnitionOnAndInMotion_NeedleInCurrentSpeedPos)
{
    // Arrange
    const uint16 currentSpeed = 50U;
    const uint32 expectedValue = currentSpeed + u16VehSpdBias - 1;
    const uint32 currentFrequency = SpeedToFrequency(currentSpeed);

    GCLStub_SetSystemPowerMode(POWER_IGNITION_ON);
    GCLStub_SetInputFrequency(currentFrequency);

    // Act
    stabilizeGaugeValue();
    uint32 displayValue = GCLStub_GetVehSpd_dsp();

    // Assert
    ck_assert_msg(displayValue == expectedValue, "Expected: %d  Actual: %d", expectedValue, displayValue);
}
END_TEST

START_TEST (IgnitionOnNoVehSpd_ChangePowerModeToLow_NeedleInRest)
{
    // Arrange
    const uint32 expectedValue = 0U;
    GCLStub_SetSystemPowerMode(POWER_IGNITION_ON);
    GCLStub_SetInputFrequency(0U);
    VehSpdGauge_Run();

    // Act
    GCLStub_SetSystemPowerMode(POWER_LOW);
    VehSpdGauge_Run();
    uint32 displayValue = GCLStub_GetVehSpd_dsp();

    // Assert
    ck_assert_msg(displayValue == expectedValue, "Expected: %d  Actual: %d", expectedValue, displayValue);
}
END_TEST

START_TEST (IgnitionOnVehSpdOverMax_NeedleAtMax)
{
    // Arrange
    const uint32 expectedValue = u16NeedleMaxPos;
    GCLStub_SetSystemPowerMode(POWER_IGNITION_ON);
    GCLStub_SetInputFrequency(SpeedToFrequency(180));

    // Act
    stabilizeGaugeValue();
    uint32 displayValue = GCLStub_GetVehSpd_dsp();

    // Assert
    ck_assert_msg(displayValue == expectedValue, "Expected: %d  Actual: %d", expectedValue, displayValue);
}
END_TEST

START_TEST (IgnitionOnVehSpdChangeTo100kph_FilteredIncreasingValue)
{
    // Arrange
    GCLStub_SetSystemPowerMode(POWER_IGNITION_ON);
    GCLStub_SetInputFrequency(SpeedToFrequency(100));

    // Act
    VehSpdGauge_Run();
    uint32 displayValue = GCLStub_GetVehSpd_dsp();
    assertValue(displayValue, 0);
    VehSpdGauge_Run();
    displayValue = GCLStub_GetVehSpd_dsp();
    assertValue(displayValue, 80);
    VehSpdGauge_Run();
    displayValue = GCLStub_GetVehSpd_dsp();
    assertValue(displayValue, 97);
    VehSpdGauge_Run();
    displayValue = GCLStub_GetVehSpd_dsp();
    assertValue(displayValue, 101);
}
END_TEST

START_TEST (IgnitionOnVehSpdChangeto100kphThenRestartAndRepeat_ShouldFollowSameCurve)
{
    // Arrange
    GCLStub_SetSystemPowerMode(POWER_IGNITION_ON);
    GCLStub_SetInputFrequency(SpeedToFrequency(100));

    // Act
    VehSpdGauge_Run();
    uint32 displayValue = GCLStub_GetVehSpd_dsp();
    assertValue(displayValue, 0);
    VehSpdGauge_Run();
    displayValue = GCLStub_GetVehSpd_dsp();
    assertValue(displayValue, 80);
    VehSpdGauge_Run();
    displayValue = GCLStub_GetVehSpd_dsp();
    assertValue(displayValue, 97);
    VehSpdGauge_Run();
    displayValue = GCLStub_GetVehSpd_dsp();
    assertValue(displayValue, 101);

    GCLStub_SetSystemPowerMode(POWER_IGNITION_OFF);
    VehSpdGauge_Run();
    GCLStub_SetSystemPowerMode(POWER_IGNITION_ON);
    VehSpdGauge_Run();
    displayValue = GCLStub_GetVehSpd_dsp();
    assertValue(displayValue, 0);
    VehSpdGauge_Run();
    displayValue = GCLStub_GetVehSpd_dsp();
    assertValue(displayValue, 80);
    VehSpdGauge_Run();
    displayValue = GCLStub_GetVehSpd_dsp();
    assertValue(displayValue, 97);
    VehSpdGauge_Run();
    displayValue = GCLStub_GetVehSpd_dsp();
    assertValue(displayValue, 101);
}
END_TEST

START_TEST (LowPowerAndSpeedNotZero_NeedleInRest)
{
    // Arrange
    const uint32 currentFrequency = SpeedToFrequency(50);
    GCLStub_SetSystemPowerMode(POWER_IGNITION_ON);
    GCLStub_SetInputFrequency(currentFrequency);
    stabilizeGaugeValue();
    
    // Act
    GCLStub_SetSystemPowerMode(POWER_LOW);
    VehSpdGauge_Run();
    
    // Assert
    uint32 displayValue = GCLStub_GetVehSpd_dsp();
    ck_assert_msg(displayValue == 0, "Expected: %d  Actual: %d", 0, displayValue);
}
END_TEST


void setup(void)
{
    GCLStub_Reset();
    VehSpdGauge_Init();
}

void teardown(void)
{}

Suite* testSuite_create()
{
    Suite* suite = suite_create("VehSpdGaugeSuite");

    TCase* testCase = tcase_create("VehSpdGaugeTests");
    tcase_add_checked_fixture(testCase, setup, teardown);
    tcase_add_test(testCase, IgnitionOff_NeedleInRestPosition);
    tcase_add_test(testCase, IgnitionOnNoVehSpd_NeedleInZeroPos);
    tcase_add_test(testCase, IgnitionOnAndInMotion_NeedleInCurrentSpeedPos);
    tcase_add_test(testCase, IgnitionOnNoVehSpd_ChangePowerModeToLow_NeedleInRest);
    tcase_add_test(testCase, IgnitionOnVehSpdOverMax_NeedleAtMax);
    tcase_add_test(testCase, IgnitionOnVehSpdChangeTo100kph_FilteredIncreasingValue);
    tcase_add_test(testCase, IgnitionOnVehSpdChangeto100kphThenRestartAndRepeat_ShouldFollowSameCurve);
    tcase_add_test(testCase, LowPowerAndSpeedNotZero_NeedleInRest);

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

static void assertValue(uint16 currentValue, uint16 expectedValue)
{
    ck_assert_msg(currentValue == expectedValue, "Expected: %d  Actual: %d", expectedValue, currentValue);
}

static void stabilizeGaugeValue(void)
{
    VehSpdGauge_Run();
    VehSpdGauge_Run();
    VehSpdGauge_Run();
    VehSpdGauge_Run();
    VehSpdGauge_Run();
    VehSpdGauge_Run();
}

uint32 SpeedToFrequency(uint16 speed)
{
    return ((speed * 311)/140) << 8;
}