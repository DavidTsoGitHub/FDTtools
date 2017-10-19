#include <stdlib.h>
#include <check.h>
#include "../inc/OutputShaftRotationSpeed.h"
#include "GCL_Stub.h"
#include "CAL.h"


/**** TEST SPECIFIC DEFINES ****/
// Sample
#define SIGNAL_NAME_ON  0U
#define SIGNAL_NAME_OFF 1U

START_TEST (ZeroInput_ZeroOutput)
{
    // Arrange
    OutputShaftRotationSpeed_Init();

    // Act
    uint32 vehSpd = 0x0000;
    GCLStub_SetVehicleSpeed(vehSpd);
    OutputShaftRotationSpeed_Run();

    uint16 rotationSpeed = GCLStub_GetRotationSpeed();

    // Assert
    ck_assert_msg(0x0000 == rotationSpeed, "RotationSpeed expected:%X actual:%X", 0x0000, rotationSpeed);
}
END_TEST

START_TEST (In100Input_5Output)
{
    // Arrange
    uint16 pulsPerRev = 10;
    uint8 numerator = 1;
    uint8 deNominator = 2;
    OutputShaftRotationSpeed_Init();
    uint16 expectedShaftSpeed = 0x05;
    CALStub_Write8(RATIO_NUMERATOR, numerator);
    CALStub_Write8(RATIO_DENOMINATOR, deNominator);
    CALStub_Write16(PULSES_PER_ROTATION, pulsPerRev);

    
    // Act
    uint32 vehSpd = 0x6400;
    GCLStub_SetVehicleSpeed(vehSpd);
    OutputShaftRotationSpeed_Run();

    uint16 rotationSpeed = GCLStub_GetRotationSpeed();
    
    // Assert
    ck_assert_msg(expectedShaftSpeed == rotationSpeed, "RotationSpeed expected:%X actual:%X", expectedShaftSpeed, rotationSpeed);
}
END_TEST


START_TEST (In10Input_2_5Output)
{
    // Arrange
    uint16 pulsPerRev = 4;
    uint8 numerator = 1;
    uint8 deNominator = 1;
    OutputShaftRotationSpeed_Init();
    uint16 expectedShaftSpeed = 0x02;
    CALStub_Write8(RATIO_NUMERATOR, numerator);
    CALStub_Write8(RATIO_DENOMINATOR, deNominator);
    CALStub_Write16(PULSES_PER_ROTATION, pulsPerRev);

    
    // Act
    uint32 vehSpd = 0x0A00;
    GCLStub_SetVehicleSpeed(vehSpd);
    OutputShaftRotationSpeed_Run();

    uint16 rotationSpeed = GCLStub_GetRotationSpeed();
    
    // Assert
    ck_assert_msg(expectedShaftSpeed == rotationSpeed, "RotationSpeed expected:%X actual:%X", expectedShaftSpeed, rotationSpeed);
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
    Suite* suite = suite_create("OutputShaftRotationSpeedSuite");

    TCase* testCase = tcase_create("OutputShaftRotationSpeedTests");
    tcase_add_checked_fixture(testCase, setup, teardown);
    tcase_add_test(testCase, ZeroInput_ZeroOutput);
    tcase_add_test(testCase, In100Input_5Output);
    tcase_add_test(testCase, In10Input_2_5Output);
        
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


