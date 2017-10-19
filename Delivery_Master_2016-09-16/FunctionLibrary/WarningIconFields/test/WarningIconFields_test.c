#include <stdlib.h>
#include <check.h>
#include "../inc/WarningIconFields.h"
#include "WarningIconFields_cfg.h"
#include "GCL_Stub.h"

/**** TEST SPECIFIC DEFINES ****/
#define NO_ICON     0U

const uint32 IconID_Charging = 1;
const uint32 IconID_ParkBrake = 2;
const uint32 IconID_BrakePressureLow = 3;
const uint32 IconID_EngineOilPressure = 4;

void AssertIcons(uint32 icon1, uint32 icon2, uint32 icon3, uint32 icon4);

START_TEST (NoIndicationsActive_AllIconsOff)
{
    // Arrange
    
    // Act
    WarningIconFields_Run();

    // Assert
    AssertIcons(NO_ICON, NO_ICON, NO_ICON, NO_ICON);
}
END_TEST

START_TEST (ChargingIndicationActive_FirstIconDisplaysChargingIcon)
{
    // Arrange
    GCLStub_SetChargingIndication(1);

    // Act
    WarningIconFields_Run();

    // Assert
    AssertIcons(IconID_Charging, NO_ICON, NO_ICON, NO_ICON);
}
END_TEST

START_TEST (ChargingIndicationNegativeFlank_ChargingIconTurnsOff)
{
    // Arrange
    GCLStub_SetChargingIndication(1);
    WarningIconFields_Run();    
    GCLStub_SetChargingIndication(0);

    // Act
    WarningIconFields_Run();

    // Assert
    AssertIcons(NO_ICON, NO_ICON, NO_ICON, NO_ICON);
}
END_TEST

START_TEST (TwoIndicationsActive_TwoFirstIconsShown)
{
    // Arrange
    GCLStub_SetChargingIndication(1);
    GCLStub_SetParkBrakeIndication(1);

    // Act
    WarningIconFields_Run();

    // Assert
    AssertIcons(IconID_Charging, IconID_ParkBrake, NO_ICON, NO_ICON);
}
END_TEST

START_TEST (TwoIndicationsActiveFirstIndicationTurnsOff_SecondIconIsDisplayedInFirstPlace)
{
    // Arrange
    GCLStub_SetChargingIndication(1);
    GCLStub_SetParkBrakeIndication(1);
    WarningIconFields_Run();

    // Act
    GCLStub_SetChargingIndication(0);
    WarningIconFields_Run();

    // Assert
    AssertIcons(IconID_ParkBrake, NO_ICON, NO_ICON, NO_ICON);
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
    Suite* suite = suite_create("WarningIconFieldsSuite");

    TCase* testCase = tcase_create("WarningIconFieldsTests");
    tcase_add_checked_fixture(testCase, setup, teardown);
    
    tcase_add_test(testCase, NoIndicationsActive_AllIconsOff);
    tcase_add_test(testCase, ChargingIndicationActive_FirstIconDisplaysChargingIcon);
    tcase_add_test(testCase, ChargingIndicationNegativeFlank_ChargingIconTurnsOff);
    tcase_add_test(testCase, TwoIndicationsActive_TwoFirstIconsShown);
    tcase_add_test(testCase, TwoIndicationsActiveFirstIndicationTurnsOff_SecondIconIsDisplayedInFirstPlace);
    
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


void AssertIcons(uint32 expectedIcon1, uint32 expectedIcon2, uint32 expectedIcon3, uint32 expectedIcon4)
{
    uint32 icon1ID = GCLStub_GetIcon1ID();
    uint32 icon2ID = GCLStub_GetIcon2ID();
    uint32 icon3ID = GCLStub_GetIcon3ID();
    uint32 icon4ID = GCLStub_GetIcon4ID();
   
   ck_assert_msg(icon1ID == expectedIcon1, "Icon1 mismatch! actual: %d, expected:%d", icon1ID, expectedIcon1);
   ck_assert_msg(icon2ID == expectedIcon2, "Icon2 mismatch! actual: %d, expected:%d", icon2ID, expectedIcon2);
   ck_assert_msg(icon3ID == expectedIcon3, "Icon3 mismatch! actual: %d, expected:%d", icon3ID, expectedIcon3);
   ck_assert_msg(icon4ID == expectedIcon4, "Icon4 mismatch! actual: %d, expected:%d", icon4ID, expectedIcon4);
    
}

