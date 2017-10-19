#include <stdlib.h>
#include <check.h>
#include "../inc/ODOTrip.h"
#include "GCL_Stub.h"
#include "systime_stub.h"
#include "CAL.h"
#include "CAL_Stub.h"

#define SPEED_ZERO      (0U)
#define SPEED_MAX       (316U)
#define MAX_ODO_VALUE   (9999999U)
#define MAX_TRIP_VALUE  (99999U)

START_TEST (FirstStart_ODOInit_FingerPrintSetAndOdoValuesZero)
{
    // Arrange
    uint32 faultyOdoValue = 1234;
    CALStub_SetCurrentOdo(ODO_STATUS, faultyOdoValue);
    CALStub_SetCurrentOdo(ODO_KM_REST_PULSES, faultyOdoValue);
    CALStub_SetCurrentOdo(ODO_VALUE1, faultyOdoValue);
    CALStub_SetCurrentOdo(ODO_VALUE2, faultyOdoValue);
    CALStub_SetCurrentOdo(ODO_VALUE3, faultyOdoValue);
    CALStub_SetCurrentOdo(ODO_VALUE4, faultyOdoValue);
    CALStub_SetCurrentOdo(ODO_VALUE5, faultyOdoValue);
    uint32 expectedOdoStatus = 0xABBA;
    uint32 expectedOdoValue = 0;
    
    // Act
    ODOTrip_Init();
    
    // Assert
    uint32 actualOdoValue;
    CALStub_GetCurrentOdo(ODO_STATUS, &actualOdoValue);
    ck_assert_msg(expectedOdoStatus == actualOdoValue, "ODO_STATUS: expected:%d actual:%d", expectedOdoStatus, actualOdoValue);
    CALStub_GetCurrentOdo(ODO_KM_REST_PULSES, &actualOdoValue);
    ck_assert_msg(expectedOdoValue == actualOdoValue, "ODO_KM_REST_PULSES: expected:%d actual:%d", expectedOdoValue, actualOdoValue);    
    CALStub_GetCurrentOdo(ODO_VALUE1, &actualOdoValue);
    ck_assert_msg(expectedOdoValue == actualOdoValue, "ODO_VALUE1: expected:%d actual:%d", expectedOdoValue, actualOdoValue);
    CALStub_GetCurrentOdo(ODO_VALUE2, &actualOdoValue);
    ck_assert_msg(expectedOdoValue == actualOdoValue, "ODO_VALUE2: expected:%d actual:%d", expectedOdoValue, actualOdoValue);
    CALStub_GetCurrentOdo(ODO_VALUE3, &actualOdoValue);
    ck_assert_msg(expectedOdoValue == actualOdoValue, "ODO_VALUE3: expected:%d actual:%d", expectedOdoValue, actualOdoValue);
    CALStub_GetCurrentOdo(ODO_VALUE4, &actualOdoValue);
    ck_assert_msg(expectedOdoValue == actualOdoValue, "ODO_VALUE4: expected:%d actual:%d", expectedOdoValue, actualOdoValue);
    CALStub_GetCurrentOdo(ODO_VALUE5, &actualOdoValue);
    ck_assert_msg(expectedOdoValue == actualOdoValue, "ODO_VALUE5: expected:%d actual:%d", expectedOdoValue, actualOdoValue);
}
END_TEST

START_TEST (FirstStart_ODOInit_FingerPrintIsSetAndOdoValuesShallNotBeZero)
{
    // Arrange
    uint32 expectedOdoStatus = 0xABBA;
    uint32 expectedOdoValue = 4321;
    CALStub_SetCurrentOdo(ODO_STATUS, expectedOdoStatus);
    CALStub_SetCurrentOdo(ODO_VALUE3, expectedOdoValue);
    
    // Act
    ODOTrip_Init();
    
    // Assert
    uint32 actualOdoValue;
    CALStub_GetCurrentOdo(ODO_STATUS, &actualOdoValue);
    ck_assert_msg(expectedOdoStatus == actualOdoValue, "ODO_STATUS: expected:%d actual:%d", expectedOdoStatus, actualOdoValue);
    ck_assert_msg(expectedOdoValue == GCLStub_GetCurrentOdo(), "ODOVALUE: expected:%d actual:%d", expectedOdoValue, GCLStub_GetCurrentOdo());
}
END_TEST

START_TEST (Write5OdoValues_GetHighestValueFromGCL)
{
    // Arrange
    uint32 expectedOdoValue = 1090;
    uint32 expectedOdoStatus = 0xABBA;
    CALStub_SetCurrentOdo(ODO_STATUS, expectedOdoStatus);
    CALStub_SetCurrentOdo(ODO_VALUE1, 1000);
    CALStub_SetCurrentOdo(ODO_VALUE2, expectedOdoValue);
    CALStub_SetCurrentOdo(ODO_VALUE3, 1020);
    CALStub_SetCurrentOdo(ODO_VALUE4, 1030);
    CALStub_SetCurrentOdo(ODO_VALUE5, 1040);
    
    // Act
    ODOTrip_Init();
    
    // Assert
    ck_assert_msg(expectedOdoValue == GCLStub_GetCurrentOdo(), "expected:%d actual:%d", expectedOdoValue, GCLStub_GetCurrentOdo());
}
END_TEST

START_TEST (FirstStart_SpeedZero_ODOShallBeZero)
{
    // Arrange
    ODOTrip_Init();    
    GCLStub_SetPowerMode(POWER_IGNITION_ON, GCL_SIGNAL_OK);
    GCLStub_SetFrequency(SPEED_ZERO);
    uint32 expectedOdoValue = 0;

    // Act
    for(uint32 counter = 0; counter < 30; ++counter)
    {
        ODOTrip_Run();
        SysTimeStub_FastForward(1000);
    }

    // Assert
    uint32 actualOdoValue = GCLStub_GetCurrentOdo();
    ck_assert_msg(expectedOdoValue == actualOdoValue, "ODO: expected:%d actual:%d", expectedOdoValue, actualOdoValue);
}
END_TEST

START_TEST (Speed140_ODOShallBe1KmAfter30Seconds)
{
    // Arrange
    ODOTrip_Init();    
    GCLStub_SetPowerMode(POWER_IGNITION_ON, GCL_SIGNAL_OK);
    GCLStub_SetFrequency(SPEED_MAX);
    uint32 expectedOdoValue = 1;

    // Act
    for(uint32 counter = 0; counter < 30; ++counter)
    {
        ODOTrip_Run();
        SysTimeStub_FastForward(1000);
    }

    // Assert
    uint32 actualOdoValue = GCLStub_GetCurrentOdo();
    ck_assert_msg(expectedOdoValue == actualOdoValue, "ODO: expected:%d actual:%d", expectedOdoValue, actualOdoValue);
}
END_TEST

START_TEST (OdoAtMax_DontChangeOdoOnRun)
{
    // Arrange
    uint32 expectedOdoValue = MAX_ODO_VALUE;
    uint32 initialOdoValue = MAX_ODO_VALUE - 1;    
    uint32 expectedOdoStatus = 0xABBA;
    CALStub_SetCurrentOdo(ODO_STATUS, expectedOdoStatus);    
    CALStub_SetCurrentOdo(ODO_VALUE3, initialOdoValue);
    ODOTrip_Init();
    GCLStub_SetPowerMode(POWER_IGNITION_ON, GCL_SIGNAL_OK);
    GCLStub_SetFrequency(SPEED_MAX);


    // Act
    for(int counter = 0; counter < 150; ++counter)
    {
        ODOTrip_Run();
        SysTimeStub_FastForward(1000);
    }

    // Assert
    uint32 actualOdoValue = GCLStub_GetCurrentOdo();
    ck_assert_msg(expectedOdoValue == actualOdoValue, "ODO: expected:%d actual:%d", expectedOdoValue, actualOdoValue);
}
END_TEST

START_TEST (IgnitionOff_DontUpdateOdoOrTrip)
{
    // Arrange
    const uint32 expectedTripValue = 100;
    const uint32 expectedOdoValue = 100;
    const uint32 initialOdoValue = 100;    
    const uint32 expectedOdoStatus = 0xABBA;
    CALStub_SetTripValue(TRIP_A_VALUE, expectedTripValue);
    CALStub_SetTripValue(TRIP_B_VALUE, expectedTripValue);
    CALStub_SetCurrentOdo(ODO_STATUS, expectedOdoStatus);    
    CALStub_SetCurrentOdo(ODO_VALUE3, initialOdoValue);
    ODOTrip_Init();
    GCLStub_SetPowerMode(POWER_IGNITION_OFF, GCL_SIGNAL_OK);
    GCLStub_SetFrequency(SPEED_MAX);


    // Act
    for(int counter = 0; counter < 180; ++counter)
    {
        ODOTrip_Run();
        SysTimeStub_FastForward(1000);
    }

    GCLStub_SetPowerMode(POWER_IGNITION_ON, GCL_SIGNAL_OK);
    ODOTrip_Run();
    
    // Assert
    uint32 actualOdoValue = GCLStub_GetCurrentOdo();
    uint32 actualTripAValue = GCLStub_GetCurrentTripA();
    uint32 actualTripBValue = GCLStub_GetCurrentTripB();
    
    ck_assert_msg(expectedOdoValue == actualOdoValue, "ODO: expected:%d actual:%d", expectedOdoValue, actualOdoValue);
    ck_assert_msg(expectedTripValue == actualTripAValue, "TripA: expected:%d actual:%d", expectedTripValue, actualTripAValue);
    ck_assert_msg(expectedTripValue == actualTripAValue, "TripB: expected:%d actual:%d", expectedTripValue, actualTripBValue);
}
END_TEST

START_TEST (PowerLow_DontUpdateOdoOrTrip)
{
    // Arrange
    const uint32 expectedTripValue = 100;
    const uint32 expectedOdoValue = 100;
    const uint32 initialOdoValue = 100;    
    const uint32 expectedOdoStatus = 0xABBA;
    CALStub_SetTripValue(TRIP_A_VALUE, expectedTripValue);
    CALStub_SetTripValue(TRIP_B_VALUE, expectedTripValue);
    CALStub_SetCurrentOdo(ODO_STATUS, expectedOdoStatus);    
    CALStub_SetCurrentOdo(ODO_VALUE3, initialOdoValue);
    ODOTrip_Init();
    GCLStub_SetPowerMode(POWER_LOW, GCL_SIGNAL_OK);
    GCLStub_SetFrequency(SPEED_MAX);

    // Act
    for(int counter = 0; counter < 180; ++counter)
    {
        ODOTrip_Run();
        SysTimeStub_FastForward(1000);
    }

    GCLStub_SetPowerMode(POWER_IGNITION_ON, GCL_SIGNAL_OK);
    ODOTrip_Run();
    
    // Assert
    uint32 actualOdoValue = GCLStub_GetCurrentOdo();
    uint32 actualTripAValue = GCLStub_GetCurrentTripA();
    uint32 actualTripBValue = GCLStub_GetCurrentTripB();
    
    ck_assert_msg(expectedOdoValue == actualOdoValue, "ODO: expected:%d actual:%d", expectedOdoValue, actualOdoValue);
    ck_assert_msg(expectedTripValue == actualTripAValue, "TripA: expected:%d actual:%d", expectedTripValue, actualTripAValue);
    ck_assert_msg(expectedTripValue == actualTripAValue, "TripB: expected:%d actual:%d", expectedTripValue, actualTripBValue);
}
END_TEST

START_TEST (Speed140_TripAShallBe100mAfter4Seconds)
{
    // Arrange
    ODOTrip_Init();    
    GCLStub_SetPowerMode(POWER_IGNITION_ON, GCL_SIGNAL_OK);
    GCLStub_SetFrequency(SPEED_MAX);
    uint32 expectedTripValue = 1;  // One tick per 100 m

    // Act
    for(uint32 counter = 0; counter < 4; ++counter)
    {
        ODOTrip_Run();
        SysTimeStub_FastForward(1000);
    }

    // Assert
    uint32 actualTripValue = GCLStub_GetCurrentTripA();
    ck_assert_msg(expectedTripValue == actualTripValue, "Trip: expected:%d actual:%d", expectedTripValue, actualTripValue);
}
END_TEST

START_TEST (Speed140_TripBShallBe100mAfter4Seconds)
{
    // Arrange
    ODOTrip_Init();    
    GCLStub_SetPowerMode(POWER_IGNITION_ON, GCL_SIGNAL_OK);
    GCLStub_SetFrequency(SPEED_MAX);
    uint32 expectedTripValue = 1; // One tick per 100 m

    // Act
    for(uint32 counter = 0; counter < 4; ++counter)
    {
        ODOTrip_Run();
        SysTimeStub_FastForward(1000);
    }

    // Assert
    uint32 actualTripValue = GCLStub_GetCurrentTripB();
    ck_assert_msg(expectedTripValue == actualTripValue, "Trip: expected:%d actual:%d", expectedTripValue, actualTripValue);
}
END_TEST

START_TEST (TripAAtMax_DontUpdateTripAAtRun)
{
    // Arrange
    CALStub_SetTripValue(TRIP_A_VALUE, MAX_TRIP_VALUE-1);
    ODOTrip_Init();
    GCLStub_SetPowerMode(POWER_IGNITION_ON, GCL_SIGNAL_OK);
    GCLStub_SetFrequency(SPEED_MAX);
    
    // Act
    for(uint32 counter = 0; counter < 12; ++counter)
    {
        ODOTrip_Run();
        SysTimeStub_FastForward(1000);
    }

    // Assert
    uint32 actualTripValue = GCLStub_GetCurrentTripA();
    ck_assert_msg(MAX_TRIP_VALUE == actualTripValue, "Trip: expected:%d actual:%d", MAX_TRIP_VALUE, actualTripValue);
}
END_TEST

START_TEST (TripBAtMax_DontUpdateTripBAtRun)
{
    // Arrange
    CALStub_SetTripValue(TRIP_B_VALUE, MAX_TRIP_VALUE-1);
    ODOTrip_Init();
    GCLStub_SetPowerMode(POWER_IGNITION_ON, GCL_SIGNAL_OK);
    GCLStub_SetFrequency(SPEED_MAX);
    
    // Act
    for(uint32 counter = 0; counter < 12; ++counter)
    {
        ODOTrip_Run();
        SysTimeStub_FastForward(1000);
    }

    // Assert
    uint32 actualTripValue = GCLStub_GetCurrentTripB();
    ck_assert_msg(MAX_TRIP_VALUE == actualTripValue, "Trip: expected:%d actual:%d", MAX_TRIP_VALUE, actualTripValue);
}
END_TEST

START_TEST (TripAResetIndication_TripA300MetersAfter12Seconds)
{
    // Arrange
    uint32 initialTripValue = 2356;
    uint32 expectedTripValue = 3;  // One tick per 100 m
    CALStub_SetTripValue(TRIP_A_VALUE, initialTripValue);
    ODOTrip_Init();
    GCLStub_SetPowerMode(POWER_IGNITION_ON, GCL_SIGNAL_OK);
    GCLStub_SetFrequency(SPEED_MAX);
    
    // Act
    for(uint32 counter = 0; counter < 4; ++counter)
    {
        ODOTrip_Run();
        SysTimeStub_FastForward(1000);
    }
    uint32 actualTripValue = GCLStub_GetCurrentTripA();
    ck_assert_msg(initialTripValue + 1 == actualTripValue, "Trip: expected:%d actual:%d", initialTripValue + 1, actualTripValue);
    
    ODOTrip_TripResetA_Indication();
    
    for(uint32 counter = 0; counter < 12; ++counter)
    {
        ODOTrip_Run();
        SysTimeStub_FastForward(1000);
    }

    // Assert
    actualTripValue = GCLStub_GetCurrentTripA();
    ck_assert_msg(expectedTripValue + 1 >= actualTripValue, "Trip: expected:%d actual:%d", expectedTripValue, actualTripValue);
    ck_assert_msg(expectedTripValue - 1 <= actualTripValue, "Trip: expected:%d actual:%d", expectedTripValue, actualTripValue);
}
END_TEST

START_TEST (TripBResetIndication_TripB300MetersAfter12Seconds)
{
    // Arrange
    uint32 initialTripValue = 2356;
    uint32 expectedTripValue = 3;  // One tick per 100 m
    CALStub_SetTripValue(TRIP_B_VALUE, initialTripValue);
    ODOTrip_Init();
    GCLStub_SetPowerMode(POWER_IGNITION_ON, GCL_SIGNAL_OK);
    GCLStub_SetFrequency(SPEED_MAX);
    
    // Act
    for(uint32 counter = 0; counter < 4; ++counter)
    {
        ODOTrip_Run();
        SysTimeStub_FastForward(1000);
    }
    uint32 actualTripValue = GCLStub_GetCurrentTripB();
    ck_assert_msg(initialTripValue + 1 == actualTripValue, "Trip: expected:%d actual:%d", initialTripValue + 1, actualTripValue);
    
    ODOTrip_TripResetB_Indication();
    
    for(uint32 counter = 0; counter < 12; ++counter)
    {
        ODOTrip_Run();
        SysTimeStub_FastForward(1000);
    }

    // Assert
    actualTripValue = GCLStub_GetCurrentTripB();
    ck_assert_msg(expectedTripValue + 1 >= actualTripValue, "Trip: expected:%d actual:%d", expectedTripValue, actualTripValue);
    ck_assert_msg(expectedTripValue - 1 <= actualTripValue, "Trip: expected:%d actual:%d", expectedTripValue, actualTripValue);
}
END_TEST

START_TEST (OdoRestCount_OdoRestcounterUpdatedOnIgnitionOff)
{
    // Arrange
    uint32 initialRestPulseValue = 7996;
    uint32 expectedRestPulseValue = (316 + initialRestPulseValue) - 7997;  // Approximate number of restpulses after 1 seconds at SPEED_MAX
    CALStub_SetCurrentOdo(ODO_KM_REST_PULSES, (initialRestPulseValue<<8)*1000);
    CALStub_SetCurrentOdo(ODO_STATUS, 0xABBA);
    ODOTrip_Init();
    GCLStub_SetPowerMode(POWER_IGNITION_ON, GCL_SIGNAL_OK);
    GCLStub_SetFrequency(SPEED_MAX);
    ODOTrip_Run();
    SysTimeStub_FastForward(1000);
    
    // Act
    ODOTrip_Run();
    SysTimeStub_FastForward(1000);
    
    GCLStub_SetPowerMode(POWER_IGNITION_OFF, GCL_SIGNAL_OK);
    ODOTrip_Run();

    // Assert
    uint32 actualRestPulseValue = (CALStub_GetODORestPulseValue()>>8)/1000;
    ck_assert_msg(expectedRestPulseValue + 1 >= actualRestPulseValue, "Rest pulse value: expected:%d actual:%d", expectedRestPulseValue, actualRestPulseValue);
    ck_assert_msg(expectedRestPulseValue - 1 <= actualRestPulseValue, "Rest pulse value: expected:%d actual:%d", expectedRestPulseValue, actualRestPulseValue);
}
END_TEST

START_TEST (TripARestCount_TripARestcounterUpdatedOnIgnitionOff)
{
    // Arrange
    uint32 initialRestPulseValue = 799;
    uint32 expectedRestPulseValue = (initialRestPulseValue + 316) - 799;  // Approximate number of restpulses after 1 seconds at SPEED_MAX
    CALStub_SetTripValue(ODO_HM_REST_PULSES_TRIP_A, (initialRestPulseValue<<8)*1000);
    CALStub_SetCurrentOdo(ODO_STATUS, 0xABBA);
    ODOTrip_Init();
    GCLStub_SetPowerMode(POWER_IGNITION_ON, GCL_SIGNAL_OK);
    GCLStub_SetFrequency(SPEED_MAX);
    ODOTrip_Run();
    SysTimeStub_FastForward(1000);
    
    // Act
    ODOTrip_Run();
    SysTimeStub_FastForward(1000);
    
    GCLStub_SetPowerMode(POWER_IGNITION_OFF, GCL_SIGNAL_OK);
    ODOTrip_Run();

    // Assert
    uint32 actualRestPulseValue = (CALStub_GetTripARestPulseValue()>>8)/1000;
    ck_assert_msg(expectedRestPulseValue + 1 >= actualRestPulseValue, "Rest pulse value: expected:%d actual:%d", expectedRestPulseValue, actualRestPulseValue);
    ck_assert_msg(expectedRestPulseValue - 1 <= actualRestPulseValue, "Rest pulse value: expected:%d actual:%d", expectedRestPulseValue, actualRestPulseValue);
}
END_TEST


START_TEST (TripBRestCount_TripBRestcounterUpdatedOnIgnitionOff)
{
    // Arrange
    uint32 initialRestPulseValue = 799;
    uint32 expectedRestPulseValue = (initialRestPulseValue + 316) - 799;  // Approximate number of restpulses after 1 seconds at SPEED_MAX
    CALStub_SetTripValue(ODO_HM_REST_PULSES_TRIP_B, (initialRestPulseValue<<8)*1000);
    CALStub_SetCurrentOdo(ODO_STATUS, 0xABBA);
    ODOTrip_Init();
    GCLStub_SetPowerMode(POWER_IGNITION_ON, GCL_SIGNAL_OK);
    GCLStub_SetFrequency(SPEED_MAX);
    ODOTrip_Run();
    SysTimeStub_FastForward(1000);
    
    // Act
    ODOTrip_Run();
    SysTimeStub_FastForward(1000);
    
    GCLStub_SetPowerMode(POWER_IGNITION_OFF, GCL_SIGNAL_OK);
    ODOTrip_Run();

    // Assert
    uint32 actualRestPulseValue = (CALStub_GetTripBRestPulseValue()>>8)/1000;
    ck_assert_msg(expectedRestPulseValue + 1 >= actualRestPulseValue, "Rest pulse value: expected:%d actual:%d", expectedRestPulseValue, actualRestPulseValue);
    ck_assert_msg(expectedRestPulseValue - 1 <= actualRestPulseValue, "Rest pulse value: expected:%d actual:%d", expectedRestPulseValue, actualRestPulseValue);
}
END_TEST


void setup(void)
{
    GCLStub_Reset();
    CAL_Init();
    SysTimeStub_SetTime(0);
}

void teardown(void)
{}

Suite* testSuite_create()
{
    Suite* suite = suite_create("ODOTripSuite");

    TCase* testCase = tcase_create("ODOTripTests");
    tcase_add_checked_fixture(testCase, setup, teardown);
    tcase_add_test(testCase, FirstStart_ODOInit_FingerPrintSetAndOdoValuesZero);
    tcase_add_test(testCase, Write5OdoValues_GetHighestValueFromGCL);
    tcase_add_test(testCase, FirstStart_ODOInit_FingerPrintIsSetAndOdoValuesShallNotBeZero);    
    tcase_add_test(testCase, FirstStart_SpeedZero_ODOShallBeZero);
    tcase_add_test(testCase, Speed140_ODOShallBe1KmAfter30Seconds);
    tcase_add_test(testCase, OdoAtMax_DontChangeOdoOnRun);
    tcase_add_test(testCase, IgnitionOff_DontUpdateOdoOrTrip);
    tcase_add_test(testCase, PowerLow_DontUpdateOdoOrTrip);
    tcase_add_test(testCase, Speed140_TripAShallBe100mAfter4Seconds);    
    tcase_add_test(testCase, Speed140_TripBShallBe100mAfter4Seconds);
    tcase_add_test(testCase, TripAAtMax_DontUpdateTripAAtRun);
    tcase_add_test(testCase, TripBAtMax_DontUpdateTripBAtRun);
    tcase_add_test(testCase, TripAResetIndication_TripA300MetersAfter12Seconds);
    tcase_add_test(testCase, TripBResetIndication_TripB300MetersAfter12Seconds);
    tcase_add_test(testCase, OdoRestCount_OdoRestcounterUpdatedOnIgnitionOff);
    tcase_add_test(testCase, TripARestCount_TripARestcounterUpdatedOnIgnitionOff);
    tcase_add_test(testCase, TripBRestCount_TripBRestcounterUpdatedOnIgnitionOff);

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


