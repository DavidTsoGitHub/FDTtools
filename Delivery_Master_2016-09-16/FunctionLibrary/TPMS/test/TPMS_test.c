#include <stdlib.h>
#include <check.h>
#include "../inc/TPMS.h"
#include "GCL_Stub.h"
#include "CAL.h"

START_TEST (SetOneTirePressure_ReadTheSamePressureValue)
{
    // Arrange
    GCLStub_SetTirePressure(0, 100);
    uint16 expectedValue = 100 * TPMS_TIRE_PRESSURE_GAIN;

    // Act
    TPMS_Run();
    DynamicWidgetValue readValue = GCLStub_GetTPMSValue(0);

    // Assert
    ck_assert_msg(readValue.Value.U16 == expectedValue, "read value == %d, expected value == %d", readValue.Value.U16, expectedValue);
}
END_TEST

START_TEST (SetOneTirePressureSysPowerModeOff_ReadZeroPressureValue)
{
    // Arrange
    GCLStub_SetTirePressure(0, 100);
    uint16 expectedValue = 0;
    GCLStub_SetPowerMode(POWER_IGNITION_OFF);

    // Act
    TPMS_Run();
    DynamicWidgetValue readValue = GCLStub_GetTPMSValue(0);

    // Assert
    ck_assert_msg(readValue.Value.U16 == expectedValue, "read value == %d, expected value == %d", readValue.Value.U16, expectedValue);
}
END_TEST

START_TEST (SetOneTirePressureTPMSCalNotInstalled_ReadZeroPressureValue)
{
    // Arrange
    CALStub_SetTPMSInstalledStatus(0);
    GCLStub_SetTirePressure(0, 100);
    uint16 expectedValue = 0;

    // Act
    TPMS_Run();
    DynamicWidgetValue readValue = GCLStub_GetTPMSValue(0);

    // Assert
    ck_assert_msg(readValue.Value.U16 == expectedValue, "read value == %d, expected value == %d", readValue.Value.U16, expectedValue);
}
END_TEST

START_TEST (SetOneTirePressureInvalidValue_ReadValueInvalid)
{
    // Arrange
    GCLStub_SetTirePressure(0, 251);
    uint16 expectedValue =  251 * TPMS_TIRE_PRESSURE_GAIN;

    // Act
    TPMS_Run();
    DynamicWidgetValue readValue = GCLStub_GetTPMSValue(0);

    // Assert
    ck_assert_msg(readValue.Value.U16 == expectedValue, "read value == %d, expected value == %d", readValue.Value.U16, expectedValue);
    ck_assert_msg(readValue.Valid == FALSE, "read value == %d, expected value == %d", readValue.Valid, FALSE);
}
END_TEST

START_TEST (SetOneTirePressureValidValue_ReadValueValid)
{
    // Arrange
    GCLStub_SetTirePressure(0, 250);
    uint16 expectedValue =  250 * TPMS_TIRE_PRESSURE_GAIN;

    // Act
    TPMS_Run();
    DynamicWidgetValue readValue = GCLStub_GetTPMSValue(0);

    // Assert
    ck_assert_msg(readValue.Value.U16 == expectedValue, "read U16 value == %d, expected U16 value == %d", readValue.Value.U16, expectedValue);
    ck_assert_msg(readValue.Valid == TRUE, "read valid value == %d, expected valid value == %d", readValue.Valid, TRUE);
}
END_TEST

START_TEST (SetOneTirePressureValidValueNoWarning_ReadValueValidNoWarning)
{
    // Arrange
    GCLStub_SetTirePressure(0, 250);
    GCLStub_SetTirePressureWarning(0, NO_WARNING_PRESSURE);
    uint16 expectedValue =  250 * TPMS_TIRE_PRESSURE_GAIN;

    // Act
    TPMS_Run();
    DynamicWidgetValue readValue = GCLStub_GetTPMSValue(0);

    // Assert
    ck_assert_msg(readValue.Value.U16 == expectedValue, "read value == %d, expected value == %d", readValue.Value.U16, expectedValue);
    ck_assert_msg(readValue.Valid == TRUE, "read value == %d, expected value == %d", readValue.Valid, TRUE);
    ck_assert_msg(readValue.Flashing == FALSE, "read warning value == %d, expected warning value == %d", readValue.Flashing, FALSE);
}
END_TEST

START_TEST (SetOneTirePressureValidValueWithWarning_ReadValueValidWarning)
{
    // Arrange
    GCLStub_SetTirePressure(0, 250);
    GCLStub_SetTirePressureWarning(0, PRESSURE_WARNING_MASK);
    uint16 expectedValue =  250 * TPMS_TIRE_PRESSURE_GAIN;

    // Act
    TPMS_Run();
    DynamicWidgetValue readValue = GCLStub_GetTPMSValue(0);

    // Assert
    ck_assert_msg(readValue.Value.U16 == expectedValue, "read value == %d, expected value == %d", readValue.Value.U16, expectedValue);
    ck_assert_msg(readValue.Valid == TRUE, "read value == %d, expected value == %d", readValue.Valid, TRUE);
    ck_assert_msg(readValue.Flashing == TRUE, "read value == %d, expected value == %d", readValue.Flashing, TRUE);
}
END_TEST

START_TEST (SetOneTirePressureValidValueNoWarning_ReadValueValidNoWarningIndicatorOff)
{
    // Arrange
    GCLStub_SetTirePressure(0, 250);
    GCLStub_SetTirePressureWarning(0, NO_WARNING_PRESSURE);
    uint16 expectedValue =  250 * TPMS_TIRE_PRESSURE_GAIN;

    // Act
    TPMS_Run();
    DynamicWidgetValue readValue = GCLStub_GetTPMSValue(0);
    uint8 indicatorValue = GCLStub_GetTPMSIndicatorValue();

    // Assert
    ck_assert_msg(readValue.Value.U16 == expectedValue, "read value == %d, expected value == %d", readValue.Value.U16, expectedValue);
    ck_assert_msg(readValue.Valid == TRUE, "read value == %d, expected value == %d", readValue.Valid, TRUE);
    ck_assert_msg(readValue.Flashing == FALSE, "read value == %d, expected value == %d", readValue.Flashing, FALSE);
    ck_assert_msg(indicatorValue == 0, "read value == %d, expected value == %d", indicatorValue, 0);
}
END_TEST

START_TEST (SetOneTirePressureValidValueWithWarning_ReadValueValidWarningIndicatorOn)
{
    // Arrange
    GCLStub_SetTirePressure(0, 250);
    GCLStub_SetTirePressureWarning(0, PRESSURE_WARNING_MASK);
    uint16 expectedValue =  250 * TPMS_TIRE_PRESSURE_GAIN;

    // Act
    TPMS_Run();
    DynamicWidgetValue readValue = GCLStub_GetTPMSValue(0);
    uint8 indicatorValue = GCLStub_GetTPMSIndicatorValue();

    // Assert
    ck_assert_msg(readValue.Value.U16 == expectedValue, "read value == %d, expected value == %d", readValue.Value.U16, expectedValue);
    ck_assert_msg(readValue.Valid == TRUE, "read value == %d, expected value == %d", readValue.Valid, TRUE);
    ck_assert_msg(readValue.Flashing == TRUE, "read value == %d, expected value == %d", readValue.Flashing, TRUE);
    ck_assert_msg(indicatorValue == 1, "read value == %d, expected value == %d", indicatorValue, 1);
}
END_TEST

START_TEST (SetTenTirePressuresValidValueNoWarning_ReadValueValidNoWarningIndicatorOff)
{
    // Arrange
    uint16 expectedValues[NUMBER_OF_TIRES];
    DynamicWidgetValue readValues[NUMBER_OF_TIRES];

    for (uint8 i = 0; i < NUMBER_OF_TIRES; ++i)
    {
        GCLStub_SetTirePressure(i, 100 + i);
        GCLStub_SetTirePressureWarning(i, NO_WARNING_PRESSURE);
        expectedValues[i] = (100 + i) * TPMS_TIRE_PRESSURE_GAIN;
    }

    // Act
    for (int i = 0; i < NUMBER_OF_TIRES; ++i)
    {
        TPMS_Run();
        readValues[i] = GCLStub_GetTPMSValue(i);
    }
    
    uint8 indicatorValue = GCLStub_GetTPMSIndicatorValue();

    // Assert
    for (int i = 0; i < NUMBER_OF_TIRES; ++i)
    {
        ck_assert_msg(readValues[i].Value.U16 == expectedValues[i], "index: %d, read u16 value == %d, expected u16 value == %d", i, readValues[i].Value.U16, expectedValues[i]);
        ck_assert_msg(readValues[i].Valid == TRUE, "index: %d, read value == %d, expected value == %d", i, readValues[i].Valid, TRUE);
        ck_assert_msg(readValues[i].Flashing == FALSE, "index: %d, read value == %d, expected value == %d", i, readValues[i].Flashing, FALSE);
    }

    ck_assert_msg(indicatorValue == 0, "read value == %d, expected value == %d", indicatorValue, 0);
}
END_TEST

START_TEST (SetTenTirePressuresValidValueOneTireWithWarning_ReadValueValidOneWarningOnIndicatorOn)
{
    // Arrange
    uint16 expectedValues[NUMBER_OF_TIRES];
    DynamicWidgetValue readValues[NUMBER_OF_TIRES];

    for (uint8 i = 0; i < NUMBER_OF_TIRES; ++i)
    {
        GCLStub_SetTirePressure(i, 100 + i);
        if (i == 5)
        {
            GCLStub_SetTirePressureWarning(i, PRESSURE_WARNING_MASK);
        }
        else
        {
            GCLStub_SetTirePressureWarning(i, NO_WARNING_PRESSURE);
        }
        expectedValues[i] = (100 + i) * TPMS_TIRE_PRESSURE_GAIN;
    }

    // Act
    for (int i = 0; i < NUMBER_OF_TIRES; ++i)
    {
        TPMS_Run();
        readValues[i] = GCLStub_GetTPMSValue(i);
    }
    
    uint8 indicatorValue = GCLStub_GetTPMSIndicatorValue();

    // Assert
    for (int i = 0; i < NUMBER_OF_TIRES; ++i)
    {
        ck_assert_msg(readValues[i].Value.U16 == expectedValues[i], "index: %d, read u16 value == %d, expected u16 value == %d", i, readValues[i].Value.U16, expectedValues[i]);
        ck_assert_msg(readValues[i].Valid == TRUE, "index: %d, read value == %d, expected value == %d", i, readValues[i].Valid, TRUE);
        if (i == 5)
        {
            ck_assert_msg(readValues[i].Flashing == TRUE, "index: %d, read value == %d, expected value == %d", i, readValues[i].Flashing, TRUE);
        }
        else
        {
            ck_assert_msg(readValues[i].Flashing == FALSE, "index: %d, read value == %d, expected value == %d", i, readValues[i].Flashing, FALSE);
        }
        
    }

    ck_assert_msg(indicatorValue == 1, "read value == %d, expected value == %d", indicatorValue, 1);
}
END_TEST

void setup(void)
{
    GCLStub_Reset();
    TPMS_Init();
    CAL_Init();
    CALStub_SetTPMSInstalledStatus(1);
    GCLStub_SetPowerMode(POWER_IGNITION_ON);
    GCLStub_SetPowerModeSignalStatus(GCL_SIGNAL_OK);
}

void teardown(void)
{}

Suite* testSuite_create()
{
    Suite* suite = suite_create("TPMSSuite");

    TCase* testCase = tcase_create("TPMSTests");
    tcase_add_checked_fixture(testCase, setup, teardown);
    tcase_add_test(testCase, SetOneTirePressure_ReadTheSamePressureValue);
    tcase_add_test(testCase, SetOneTirePressureSysPowerModeOff_ReadZeroPressureValue);
    tcase_add_test(testCase, SetOneTirePressureTPMSCalNotInstalled_ReadZeroPressureValue);
    tcase_add_test(testCase, SetOneTirePressureInvalidValue_ReadValueInvalid);
    tcase_add_test(testCase, SetOneTirePressureValidValue_ReadValueValid);
    tcase_add_test(testCase, SetOneTirePressureValidValueNoWarning_ReadValueValidNoWarning);
    tcase_add_test(testCase, SetOneTirePressureValidValueWithWarning_ReadValueValidWarning);
    tcase_add_test(testCase, SetOneTirePressureValidValueNoWarning_ReadValueValidNoWarningIndicatorOff);
    tcase_add_test(testCase, SetOneTirePressureValidValueWithWarning_ReadValueValidWarningIndicatorOn);
    tcase_add_test(testCase, SetTenTirePressuresValidValueNoWarning_ReadValueValidNoWarningIndicatorOff);
    tcase_add_test(testCase, SetTenTirePressuresValidValueOneTireWithWarning_ReadValueValidOneWarningOnIndicatorOn);
    
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


