#include <stdlib.h>
#include <check.h>
#include "../inc/IlluminationManager.h"
#include "GCL_Stub.h"
#include "GCL_IlluminationManager.h"
#include "CAL_Stub.h"

/**** TEST SPECIFIC DEFINES ****/
#define ILLUMINATION_DAY   0U
#define ILLUMINATION_NIGHT 1U

void AssertDayTimeIllumination(void);
void AssertInstrumentIlluminationOff(void);
void AssertInstrumentIlluminationLevels(uint8 expectedLevel);

START_TEST (IgnitionOff_NoLight)
{
    // Arrange
    GCLStub_SetPowerMode(POWER_IGNITION_OFF);
    GCLStub_SetIlluminationMode(NIGHT_TIME);
    GCLStub_SetIlluminationLevel(1);
    IlluminationManager_Init();

    // Act
    IlluminationManager_Run();

    // Assert
    AssertInstrumentIlluminationOff();
}
END_TEST

START_TEST (IgnitionOnAndDayTime_FullDisplayAndNoGaugeLight)
{
    // Arrange
    GCLStub_SetPowerMode(POWER_IGNITION_ON);
    GCLStub_SetIlluminationMode(DAY_TIME);
    GCLStub_SetIlluminationLevel(1);
    IlluminationManager_Init();

    // Act
    IlluminationManager_Run();
    IlluminationManager_Run();

    // Assert
    AssertDayTimeIllumination();
}
END_TEST

START_TEST (IgnitionOnAndNightTimeIllumLevel1_1PercDisplayAnd7PrecGaugeLight)
{
    // Arrange
    GCLStub_SetPowerMode(POWER_IGNITION_ON);
    GCLStub_SetIlluminationMode(NIGHT_TIME);
    CALStub_SetIlluminationLevel(1);
    IlluminationManager_Init();

    // Act
    IlluminationManager_Run();
    IlluminationManager_Run();

    // Assert
    AssertInstrumentIlluminationLevels(1);
}
END_TEST

START_TEST (IgnitionOnAndNightTime_ChangeToIllumLevel2_7PercDisplayAnd14PrecGaugeLight)
{
    // Arrange
    GCLStub_SetPowerMode(POWER_IGNITION_ON);
    GCLStub_SetIlluminationMode(NIGHT_TIME);
    
    CALStub_SetIlluminationLevel(1);
    IlluminationManager_Init();

    IlluminationManager_Run();
    IlluminationManager_Run();

    // Act
    GCLStub_SetIlluminationLevel(2);
    IlluminationManager_IlluminationLevel_Indication();

    IlluminationManager_Run();

    // Assert
    uint8 calIlluminationLevel = CALStub_GetIlluminationLevel();
    uint8 calWriteCount = CALStub_GetCalWriteCount();
    ck_assert_msg(1 == calIlluminationLevel, "No change expected in CAL when changing level from GCL!");
    ck_assert_msg(0 == calWriteCount, "No CAL write expected when changing level from GCL! (writecount:%d)", calWriteCount);
    AssertInstrumentIlluminationLevels(2);
}
END_TEST

START_TEST (IgnitionOnAndNightTime_ChangeToIllumLevel2ViaCAL_7PercDisplayAnd14PrecGaugeLight)
{
    // Arrange
    GCLStub_SetPowerMode(POWER_IGNITION_ON);
    GCLStub_SetIlluminationMode(NIGHT_TIME);
    CALStub_SetIlluminationLevel(1);
    IlluminationManager_Init();
    IlluminationManager_Run();
    IlluminationManager_Run();
    CALStub_SetIlluminationLevel(2);

    // Act
    IlluminationManager_Run();

    // Assert
    uint8 calWriteCount = CALStub_GetCalWriteCount();
    ck_assert_msg(0 == calWriteCount, "Expected no CAL write when change came from CAL! (write count:%d)", calWriteCount);
    AssertInstrumentIlluminationLevels(2);
}
END_TEST

START_TEST (CALValueIsUpdatedOnIgnitionOff)
{
    // Arrange
    GCLStub_SetPowerMode(POWER_IGNITION_ON);
    GCLStub_SetIlluminationMode(NIGHT_TIME);
    CALStub_SetIlluminationLevel(1);
    IlluminationManager_Init();
    IlluminationManager_Run();
    IlluminationManager_Run();

    // Act
    GCLStub_SetIlluminationLevel(2);
    IlluminationManager_IlluminationLevel_Indication();
    IlluminationManager_Run();
    GCLStub_SetPowerMode(POWER_IGNITION_OFF);
    IlluminationManager_Run();
    
    // Assert
    uint8 calWriteCount = CALStub_GetCalWriteCount();
    uint8 calIlluminationLevel = CALStub_GetIlluminationLevel();
    ck_assert_msg(2 == calIlluminationLevel, "Expected CAL to be updated when a new illumination level is set and ignition is turned off!");
    ck_assert_msg(1 == calWriteCount, "Expected one CAL write when ignition turns off! (write count:%d)", calWriteCount);
    AssertInstrumentIlluminationOff();
}
END_TEST

START_TEST (IgnitionOnAndNightTimeIllumLevel1AndBackToIgnitionOff_NoLights)
{
    // Arrange
    GCLStub_SetPowerMode(POWER_IGNITION_ON);
    GCLStub_SetIlluminationMode(NIGHT_TIME);
    CALStub_SetIlluminationLevel(1);
    IlluminationManager_Init();
    IlluminationManager_Run();
    IlluminationManager_Run();

    // Act
    GCLStub_SetPowerMode(POWER_IGNITION_OFF);
    IlluminationManager_Run();
    
    // Assert
    AssertInstrumentIlluminationOff();
}
END_TEST

START_TEST (IgnitionOnAndNightTimeIllumLevel1ThenPowerLow_NoLights)
{
    // Arrange
    GCLStub_SetPowerMode(POWER_IGNITION_ON);
    GCLStub_SetIlluminationMode(NIGHT_TIME);
    CALStub_SetIlluminationLevel(1);
    IlluminationManager_Init();
    IlluminationManager_Run();
    IlluminationManager_Run();

    // Act
    GCLStub_SetPowerMode(POWER_LOW);
    IlluminationManager_Run();
    
    // Assert
    AssertInstrumentIlluminationOff();
}
END_TEST

START_TEST (ChangedValuesInDayModeAreRememberedUntilNightMode)
{
    // Arrange
    GCLStub_SetPowerMode(POWER_IGNITION_ON);
    GCLStub_SetIlluminationMode(DAY_TIME);
    CALStub_SetIlluminationLevel(1);
    IlluminationManager_Init();
    IlluminationManager_Run();
    IlluminationManager_Run();

    // Act
    GCLStub_SetIlluminationLevel(3);
    IlluminationManager_IlluminationLevel_Indication();
    IlluminationManager_Run();
    GCLStub_SetIlluminationMode(NIGHT_TIME);
    IlluminationManager_Run();
    
    // Assert
    AssertInstrumentIlluminationLevels(3);
}
END_TEST

START_TEST (TransitionFromNightModeToDayMode_SetsDayMode)
{
    // Arrange
    GCLStub_SetPowerMode(POWER_IGNITION_ON);
    GCLStub_SetIlluminationMode(NIGHT_TIME);
    CALStub_SetIlluminationLevel(3);
    IlluminationManager_Init();
    IlluminationManager_Run();
    IlluminationManager_Run();

    // Act
    GCLStub_SetIlluminationMode(DAY_TIME);
    IlluminationManager_Run();
    
    // Assert
    uint8 currentIlluminationLevel = GCLStub_GetCurrentIlluminationLevel();
    ck_assert_msg(currentIlluminationLevel == 3, "Setting day mode should not change the illumination level in GCL (actual:%d  expected:3)", currentIlluminationLevel);
    AssertDayTimeIllumination();
}
END_TEST


void setup(void)
{
    GCLStub_Reset();
    CALStub_Init();
}

void teardown(void)
{

}

Suite* testSuite_create()
{
    Suite* suite = suite_create("IlluminationManagerSuite");

    TCase* testCase = tcase_create("IlluminationManagerTests");
    tcase_add_checked_fixture(testCase, setup, teardown);

    //Tests
    tcase_add_test(testCase, IgnitionOff_NoLight);
    tcase_add_test(testCase, IgnitionOnAndDayTime_FullDisplayAndNoGaugeLight);
    tcase_add_test(testCase, IgnitionOnAndNightTimeIllumLevel1_1PercDisplayAnd7PrecGaugeLight);
    tcase_add_test(testCase, IgnitionOnAndNightTime_ChangeToIllumLevel2_7PercDisplayAnd14PrecGaugeLight);
    tcase_add_test(testCase, IgnitionOnAndNightTime_ChangeToIllumLevel2ViaCAL_7PercDisplayAnd14PrecGaugeLight);
    tcase_add_test(testCase, IgnitionOnAndNightTimeIllumLevel1AndBackToIgnitionOff_NoLights);
    tcase_add_test(testCase, IgnitionOnAndNightTimeIllumLevel1ThenPowerLow_NoLights);
    tcase_add_test(testCase, CALValueIsUpdatedOnIgnitionOff);
    tcase_add_test(testCase, ChangedValuesInDayModeAreRememberedUntilNightMode);
    tcase_add_test(testCase, TransitionFromNightModeToDayMode_SetsDayMode);
    
    
    
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


void AssertDayTimeIllumination(void)
{
    uint8 currentDisplayIllumination = GCLStub_GetDisplayIlluminationLevel();
    uint8 currentGaugeIllumination = GCLStub_GetGaugeIlluminationLevel();
    ck_assert_msg(100 == currentDisplayIllumination, "Display Illumination was not daytime mode (100)! actual:%d", currentDisplayIllumination);
    ck_assert_msg(0 == currentGaugeIllumination, "Gauge Illumination was not daytime mode (0)! actual:%d", currentGaugeIllumination);
}

void AssertInstrumentIlluminationOff(void)
{
    uint8 currentDisplayIllumination = GCLStub_GetDisplayIlluminationLevel();
    uint8 currentGaugeIllumination = GCLStub_GetGaugeIlluminationLevel();
    ck_assert_msg(0 == currentDisplayIllumination, "Display Illumination was not 0 (OFF)! actual:%d", currentDisplayIllumination);
    ck_assert_msg(0 == currentGaugeIllumination, "Gauge Illumination was not 0 (OFF)! actual:%d", currentGaugeIllumination);
}

void AssertInstrumentIlluminationLevels(uint8 expectedLevel)
{
    uint8 expectedDisplayIllumination, expectedGaugeIllumination;
    uint8 gclIlluminationLevel = GCLStub_GetCurrentIlluminationLevel();
    ck_assert_msg(expectedLevel == gclIlluminationLevel, "Wrong illumination level in GCL! expected:%d actual:%d", expectedLevel, gclIlluminationLevel);
    
    switch (expectedLevel)
    {
        case 1:
            expectedDisplayIllumination = DisplayIlluminationLevel1;
            expectedGaugeIllumination = GaugeIlluminationLevel1;
        break;
        case 2:
            expectedDisplayIllumination = DisplayIlluminationLevel2;
            expectedGaugeIllumination = GaugeIlluminationLevel2;
        break;
        case 3:
            expectedDisplayIllumination = DisplayIlluminationLevel3;
            expectedGaugeIllumination = GaugeIlluminationLevel3;
        break;
        case 4:
            expectedDisplayIllumination = DisplayIlluminationLevel4;
            expectedGaugeIllumination = GaugeIlluminationLevel4;
        break;
        case 5:
            expectedDisplayIllumination = DisplayIlluminationLevel5;
            expectedGaugeIllumination = GaugeIlluminationLevel5;
        break;
        default:
        ck_abort_msg("Undefined illumination level (%d) provided to assert function!", expectedLevel);
        break;
    }
    
    uint8 currentDisplayIllumination = GCLStub_GetDisplayIlluminationLevel();
    uint8 currentGaugeIllumination = GCLStub_GetGaugeIlluminationLevel();
    ck_assert_msg(expectedDisplayIllumination == currentDisplayIllumination, "Display illumination expected:%d actual:%d", expectedDisplayIllumination, currentDisplayIllumination);
    ck_assert_msg(expectedGaugeIllumination == currentGaugeIllumination, "Gauge illumination expected:%d actual:%d", expectedGaugeIllumination, currentGaugeIllumination);
}
