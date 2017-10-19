#include <stdlib.h>
#include <check.h>
#include "../inc/FuelLevel.h"
#include "GCL_Stub.h"
#include "CAL.h"

#define WARNING_OFF 0
#define WARNING_ON  1

#define MAX_GAUGE_STABILIZER_ITERATIONS 15

const uint16 u16Coefficient = 5000;

void Assert(uint16 gaugeValue, uint8 warningIndicator);
void StabilizeGaugeValue(void);

START_TEST (TankFull_GaugeFullWarningOff)
{
    // Arrange
    GCLStub_SetPowerMode(POWER_IGNITION_ON, GCL_SIGNAL_OK);
    GCLStub_SetFuelLevel(100, GCL_SIGNAL_OK);

    // Act
    StabilizeGaugeValue();

    // Assert
    Assert(100, WARNING_OFF);
    
}
END_TEST

START_TEST (TankEmpty_GaugeEmptyWarningOn)
{
    // Arrange
    GCLStub_SetPowerMode(POWER_IGNITION_ON, GCL_SIGNAL_OK);
    GCLStub_SetFuelLevel(0, GCL_SIGNAL_OK);

    // Act
    StabilizeGaugeValue();

    // Assert
    Assert(0, WARNING_ON);
}
END_TEST

START_TEST (WarningIsOnValueAboveThresholdButBelowTolerance_WarningStillOn)
{
    // Arrange
    GCLStub_SetPowerMode(POWER_IGNITION_ON, GCL_SIGNAL_OK);
    CALStub_SetThreshold(10);
    CALStub_SetTolerance(5);
    GCLStub_SetFuelLevel(9, GCL_SIGNAL_OK);
    StabilizeGaugeValue();
    Assert(9, WARNING_ON);

    // Act
    GCLStub_SetFuelLevel(13, GCL_SIGNAL_OK);
    StabilizeGaugeValue();

    // Assert
    Assert(13, WARNING_ON);
}
END_TEST

START_TEST (WarningIsOnValueAboveThresholdAndTolerance_WarningTurnsOff)
{
    // Arrange
    GCLStub_SetPowerMode(POWER_IGNITION_ON, GCL_SIGNAL_OK);
    CALStub_SetThreshold(10);
    CALStub_SetTolerance(5);
    GCLStub_SetFuelLevel(9, GCL_SIGNAL_OK);
    StabilizeGaugeValue();
    Assert(9, WARNING_ON);

    // Act
    GCLStub_SetFuelLevel(16, GCL_SIGNAL_OK);
    StabilizeGaugeValue();

    // Assert
    Assert(16, WARNING_OFF);
}
END_TEST

START_TEST (TankFullThenIgnitionOff_GaugeStraightToZero)
{
    // Arrange
    GCLStub_SetPowerMode(POWER_IGNITION_ON, GCL_SIGNAL_OK);
    GCLStub_SetFuelLevel(100, GCL_SIGNAL_OK);
    StabilizeGaugeValue();
    Assert(100, WARNING_OFF);

    // Act
    GCLStub_SetPowerMode(POWER_IGNITION_OFF, GCL_SIGNAL_OK);
    FuelLevel_Run();
    
    // Assert
    Assert(0, WARNING_OFF);
}
END_TEST

START_TEST (TankFullThenLowPower_GaugeStraightToZero)
{
    // Arrange
    GCLStub_SetPowerMode(POWER_IGNITION_ON, GCL_SIGNAL_OK);
    GCLStub_SetFuelLevel(100, GCL_SIGNAL_OK);
    StabilizeGaugeValue();
    Assert(100, WARNING_OFF);

    // Act
    GCLStub_SetPowerMode(POWER_LOW, GCL_SIGNAL_OK);
    FuelLevel_Run();
    
    // Assert
    Assert(0, WARNING_OFF);
}
END_TEST

START_TEST (WarningIsOnThenIgnitionOff_WarningTurnsOff)
{
    // Arrange
    GCLStub_SetPowerMode(POWER_IGNITION_ON, GCL_SIGNAL_OK);
    CALStub_SetThreshold(10);
    CALStub_SetTolerance(5);
    GCLStub_SetFuelLevel(9, GCL_SIGNAL_OK);
    StabilizeGaugeValue();
    Assert(9, WARNING_ON);

    // Act
    GCLStub_SetPowerMode(POWER_IGNITION_OFF, GCL_SIGNAL_OK);
    FuelLevel_Run();

    // Assert
    Assert(0, WARNING_OFF);
}
END_TEST

void setup(void)
{
    GCLStub_Reset();
    FuelLevel_Init();
    CAL_Init();
}

void teardown(void)
{}

Suite* testSuite_create()
{
    Suite* suite = suite_create("FuelLevelSuite");

    TCase* testCase = tcase_create("FuelLevelTests");
    tcase_add_checked_fixture(testCase, setup, teardown);
    tcase_add_test(testCase, TankFull_GaugeFullWarningOff);
    tcase_add_test(testCase, TankEmpty_GaugeEmptyWarningOn);
    tcase_add_test(testCase, WarningIsOnValueAboveThresholdButBelowTolerance_WarningStillOn);
    tcase_add_test(testCase, WarningIsOnValueAboveThresholdAndTolerance_WarningTurnsOff);
    tcase_add_test(testCase, TankFullThenIgnitionOff_GaugeStraightToZero);
    tcase_add_test(testCase, TankFullThenLowPower_GaugeStraightToZero);
    tcase_add_test(testCase, WarningIsOnThenIgnitionOff_WarningTurnsOff);

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

    return failedTestCount;
}

void StabilizeGaugeValue()
{  
    for (int i=0; i<MAX_GAUGE_STABILIZER_ITERATIONS; ++i)
    {
        FuelLevel_Run();
    }
}

void Assert(uint16 expectedGaugeValue, uint8 expectedWarningIndicator)
{
    uint16 actualGaugeValue = GCLStub_GetFuelLevelGaugeValue();
    uint8 actualWarningIndicator = GCLStub_GetFuelLevelWarningIndication();
    ck_assert_msg(expectedGaugeValue == actualGaugeValue, "FUELGAUGE: expected:%d actual:%d", expectedGaugeValue, actualGaugeValue);
    ck_assert_msg(expectedWarningIndicator == actualWarningIndicator, "FUELWARNING: expected:%d actual:%d", expectedWarningIndicator, actualWarningIndicator);
}