#include <stdlib.h>
#include <check.h>
#include "../inc/ParkBrakeIndicator.h"
#include "SoundManager.h"
#include "GCL_Stub.h"

#define CHECK_MESSAGE_DISABLED  0U
#define CHECK_MESSAGE_ENABLED   1U

#define PARK_BRAKE_DISABLED 0U
#define PARK_BRAKE_ENABLED  1U

START_TEST (ParkBrakeEnabled_ParkBrakeIndicatorOn)
{
	// Arrange
	GCLStub_SetParkBrakeSwitch(PARK_BRAKE_ENABLED);
	
	// Act
	ParkBrakeIndicator_Run();
    ParkBrakeIndicator_Run();
	uint8 value = GCLStub_GetParkBrakeIndicatorValue();
	
	// Assert
	ck_assert_msg(value == PARK_BRAKE_ENABLED, "Expected: %d, Actual: %d", PARK_BRAKE_ENABLED, value);
}
END_TEST

START_TEST (ParkBrakeDisabled_ParkBrakeIndicatorOff)
{
	// Arrange
	GCLStub_SetParkBrakeSwitch(PARK_BRAKE_DISABLED);
	
	// Act
	ParkBrakeIndicator_Run();
    ParkBrakeIndicator_Run();
	uint8 value = GCLStub_GetParkBrakeIndicatorValue();
	
	// Assert
	ck_assert_msg(value == PARK_BRAKE_DISABLED, "Expected: %d, Actual: %d", PARK_BRAKE_DISABLED, value);
}
END_TEST

START_TEST (ParkBrakeEnabledSpeedOverZero_ParkBrakeIndicatorOn)
{
	// Arrange
	GCLStub_SetParkBrakeSwitch(PARK_BRAKE_ENABLED);
	GCLStub_SetVehSpd(1);
	
	// Act
	ParkBrakeIndicator_Run();
    ParkBrakeIndicator_Run();
	uint8 value = GCLStub_GetParkBrakeIndicatorValue();
	
	// Assert
	ck_assert_msg(value == PARK_BRAKE_ENABLED, "Expected: %d, Actual: %d", PARK_BRAKE_ENABLED, value);
}
END_TEST

START_TEST (ParkBrakeDisabledSpeedOverZero_ParkBrakeIndicatorOff)
{
	// Arrange
	GCLStub_SetParkBrakeSwitch(PARK_BRAKE_DISABLED);
	GCLStub_SetVehSpd(1);
	
	// Act
	ParkBrakeIndicator_Run();
    ParkBrakeIndicator_Run();
	uint8 value = GCLStub_GetParkBrakeIndicatorValue();
	
	// Assert
	ck_assert_msg(value == PARK_BRAKE_DISABLED, "Expected: %d, Actual: %d", PARK_BRAKE_DISABLED, value);
}
END_TEST

START_TEST (ParkBrakeEnabledSpeedOverZero_ParkBrakeSoundOn)
{
	// Arrange
	GCLStub_SetParkBrakeSwitch(PARK_BRAKE_ENABLED);
	GCLStub_SetVehSpd(1);
	
	// Act
	ParkBrakeIndicator_Run();
    ParkBrakeIndicator_Run();
	BSP_BuzzerSound currentBuzzerSound = SoundManagerStub_GetCurrentActiveBuzzerSound();
	
	// Assert
	ck_assert_msg(currentBuzzerSound == BSP_SOUND_ON, "Expected sound: %d, Actual sound: %d", BSP_SOUND_ON, currentBuzzerSound);
}
END_TEST

START_TEST (ParkBrakeDisabledSpeedOverZero_ParkBrakeSoundOff)
{
	// Arrange
	GCLStub_SetParkBrakeSwitch(PARK_BRAKE_DISABLED);
	GCLStub_SetVehSpd(1);
	
	// Act
	ParkBrakeIndicator_Run();
    ParkBrakeIndicator_Run();
	BSP_BuzzerSound currentBuzzerSound = SoundManagerStub_GetCurrentActiveBuzzerSound();
	
	// Assert
	ck_assert_msg(currentBuzzerSound == BSP_SOUND_1HZ, "Expected sound: %d, Actual sound: %d", BSP_SOUND_1HZ, currentBuzzerSound);
}
END_TEST

START_TEST (ParkBrakeEnabledAndSpeedOverZero_ParkBrakeIndicatorCheckMessageActive)
{
    // Arrange
    GCLStub_SetParkBrakeSwitch(PARK_BRAKE_ENABLED);
    GCLStub_SetVehSpd(1);

    // Act
    ParkBrakeIndicator_Run();
    ParkBrakeIndicator_Run();
    uint8 checkMessageStatus = GCLStub_GetParkBrakeIndicatorCheckMessageActive();

    // Assert
    ck_assert_msg(checkMessageStatus == CHECK_MESSAGE_ENABLED, "Expected: %d  Actual: %d", CHECK_MESSAGE_ENABLED, checkMessageStatus);
}
END_TEST

START_TEST (ParkBrakeEnabledAndSpeedZero_ParkBrakeCheckMessageInactive)
{
    // Arrange
    GCLStub_SetParkBrakeSwitch(PARK_BRAKE_ENABLED);
    GCLStub_SetVehSpd(0);

    // Act
    ParkBrakeIndicator_Run();
    ParkBrakeIndicator_Run();
    uint8 checkMessageStatus = GCLStub_GetParkBrakeIndicatorCheckMessageActive();

    // Assert
    ck_assert_msg(checkMessageStatus == CHECK_MESSAGE_DISABLED, "Expected: %d  Actual: %d", CHECK_MESSAGE_DISABLED, checkMessageStatus);
}
END_TEST

START_TEST (ParkBrakeDisabledAndSpeedOverZero_ParkBrakeCheckMessageInactive)
{
    // Arrange
    GCLStub_SetParkBrakeSwitch(PARK_BRAKE_DISABLED);
    GCLStub_SetVehSpd(1);

    // Act
    ParkBrakeIndicator_Run();
    ParkBrakeIndicator_Run();
    uint8 checkMessageStatus = GCLStub_GetParkBrakeIndicatorCheckMessageActive();

    // Assert
    ck_assert_msg(checkMessageStatus == CHECK_MESSAGE_DISABLED, "Expected: %d  Actual: %d", CHECK_MESSAGE_DISABLED, checkMessageStatus);
}
END_TEST

START_TEST (CheckMessageEnabledParkBrakeReleased_CheckMessageGoesInactive)
{
    // Arrange
    GCLStub_SetParkBrakeSwitch(PARK_BRAKE_ENABLED);
    GCLStub_SetVehSpd(1);
    ParkBrakeIndicator_Run();
    ParkBrakeIndicator_Run();

    // Act
    GCLStub_SetParkBrakeSwitch(PARK_BRAKE_DISABLED);
    ParkBrakeIndicator_Run();
    ParkBrakeIndicator_Run();
    uint8 checkMessageStatus = GCLStub_GetParkBrakeIndicatorCheckMessageActive();

    // Assert
    ck_assert_msg(checkMessageStatus == CHECK_MESSAGE_DISABLED, "Expected: %d  Actual: %d", CHECK_MESSAGE_DISABLED, checkMessageStatus);
}
END_TEST

START_TEST (CheckMessageEnabledVehicleStops_CheckMessageGoesInactive)
{
    // Arrange
    GCLStub_SetParkBrakeSwitch(PARK_BRAKE_ENABLED);
    GCLStub_SetVehSpd(1);
    ParkBrakeIndicator_Run();
    ParkBrakeIndicator_Run();
    
    // Act
    GCLStub_SetVehSpd(0);
    ParkBrakeIndicator_Run();
    ParkBrakeIndicator_Run();
    uint8 checkMessageStatus = GCLStub_GetParkBrakeIndicatorCheckMessageActive();

    // Assert
    ck_assert_msg(checkMessageStatus == CHECK_MESSAGE_DISABLED, "Expected: %d  Actual: %d", CHECK_MESSAGE_DISABLED, checkMessageStatus);
}
END_TEST

START_TEST (ParkBrakeEnabledTransitionToPowerLow_ParkBrakeIndicatorOff)
{
	// Arrange
	GCLStub_SetParkBrakeSwitch(PARK_BRAKE_ENABLED);
    
	// Act
    ParkBrakeIndicator_Run();
    GCLStub_SetSystemPowerMode(POWER_LOW);
    ParkBrakeIndicator_Run();

	// Assert
	uint8 value = GCLStub_GetParkBrakeIndicatorValue();
	ck_assert_msg(value == PARK_BRAKE_DISABLED, "Expected: %d, Actual: %d", PARK_BRAKE_DISABLED, value);
}
END_TEST

void setup(void)
{
    GCLStub_Reset();
    ParkBrakeIndicator_Init();
    SoundManager_Init();
}

void teardown(void)
{}

Suite* testSuite_create()
{
    Suite* suite = suite_create("ParkBrakeIndicatorSuite");

    TCase* testCase = tcase_create("ParkBrakeIndicatorTests");
    tcase_add_checked_fixture(testCase, setup, teardown);
	tcase_add_test(testCase, ParkBrakeEnabled_ParkBrakeIndicatorOn);
	tcase_add_test(testCase, ParkBrakeDisabled_ParkBrakeIndicatorOff);
	tcase_add_test(testCase, ParkBrakeEnabledSpeedOverZero_ParkBrakeIndicatorOn);
	tcase_add_test(testCase, ParkBrakeDisabledSpeedOverZero_ParkBrakeIndicatorOff);
	tcase_add_test(testCase, ParkBrakeEnabledSpeedOverZero_ParkBrakeSoundOn);
	tcase_add_test(testCase, ParkBrakeDisabledSpeedOverZero_ParkBrakeSoundOff);
    tcase_add_test(testCase, ParkBrakeEnabledAndSpeedOverZero_ParkBrakeIndicatorCheckMessageActive);
    tcase_add_test(testCase, ParkBrakeEnabledAndSpeedZero_ParkBrakeCheckMessageInactive);
    tcase_add_test(testCase, ParkBrakeDisabledAndSpeedOverZero_ParkBrakeCheckMessageInactive);
    tcase_add_test(testCase, CheckMessageEnabledParkBrakeReleased_CheckMessageGoesInactive);
    tcase_add_test(testCase, CheckMessageEnabledVehicleStops_CheckMessageGoesInactive);
    
    tcase_add_test(testCase, ParkBrakeEnabledTransitionToPowerLow_ParkBrakeIndicatorOff);
    
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


