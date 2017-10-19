#include <stdlib.h>
#include <check.h>
#include "../inc/OdoAndTripDisplayHandler.h"
#include "GCL_Stub.h"

void AssertFieldContents(const wchar* actual, const char* expected)
{
    boolean areEqual = TRUE;
    int i=0;
    do
    {
        if ((actual[i] & 0xFF) != expected[i])
        {
            areEqual = FALSE;
        }
    }
    while (actual[i++] != '\0');
            
    ck_assert_msg(areEqual == TRUE, "Text mismatch!! actual: %ls expected: %s", actual, expected);
}

START_TEST (Initialization_ProvidesOutputWithINIT)
{
    // Act
    OdoAndTripDisplayHandler_Init();
    
    // Assert
    wchar* tripFieldTest = GCLStub_Get_TripFieldText();
    wchar* odoAndInstFuelField = GCLStub_Get_OdoAndInstFuelFieldText();
    AssertFieldContents(tripFieldTest, "INIT");
    AssertFieldContents(odoAndInstFuelField, "INIT");
}
END_TEST

START_TEST (FirstRunAfterInit_ShowsDistanceValues)
{
    // Arrange
    OdoAndTripDisplayHandler_Init();
    GCLStub_Set_OdoValue(987654, GCL_SIGNAL_OK);
    GCLStub_Set_TripValue(23456, GCL_SIGNAL_OK);

    // Act
    OdoAndTripDisplayHandler_Run();
    
    // Assert
    wchar* tripFieldTest = GCLStub_Get_TripFieldText();
    wchar* odoAndInstFuelField = GCLStub_Get_OdoAndInstFuelFieldText();
    AssertFieldContents(odoAndInstFuelField, "987654km");
    AssertFieldContents(tripFieldTest, "2345.6km");
}
END_TEST

START_TEST (ZeroDistanceValuesAreDisplayedCorrectly)
{
    // Arrange
    OdoAndTripDisplayHandler_Init();
    GCLStub_Set_OdoValue(0, GCL_SIGNAL_OK);
    GCLStub_Set_TripValue(0, GCL_SIGNAL_OK);

    // Act
    OdoAndTripDisplayHandler_Run();
    
    // Assert
    wchar* tripFieldTest = GCLStub_Get_TripFieldText();
    wchar* odoAndInstFuelField = GCLStub_Get_OdoAndInstFuelFieldText();
    AssertFieldContents(odoAndInstFuelField, "0km");
    AssertFieldContents(tripFieldTest, "0.0km");
}
END_TEST

START_TEST (DecimalTripValueBelowTenIsDisplayedCorrectly)
{
    // Arrange
    OdoAndTripDisplayHandler_Init();
    GCLStub_Set_TripValue(5, GCL_SIGNAL_OK);

    // Act
    OdoAndTripDisplayHandler_Run();
    
    // Assert
    wchar* tripFieldTest = GCLStub_Get_TripFieldText();
    AssertFieldContents(tripFieldTest, "0.5km");
}
END_TEST

START_TEST (ShowNextIndicationAfterFirstRun_ShowsFuelValues)
{
    // Arrange
    OdoAndTripDisplayHandler_Init();
    OdoAndTripDisplayHandler_Run();
    GCLStub_Set_InstantaneousFuelConsumption(234, GCL_SIGNAL_OK);
    GCLStub_Set_TripFuelConsumption(3456, GCL_SIGNAL_OK);

    // Act
    GCLStub_Indicate_ShowNext();
    OdoAndTripDisplayHandler_Run();
    
    // Assert
    wchar* tripFieldTest = GCLStub_Get_TripFieldText();
    wchar* odoAndInstFuelField = GCLStub_Get_OdoAndInstFuelFieldText();
    AssertFieldContents(odoAndInstFuelField, "23.4L/100km");
    AssertFieldContents(tripFieldTest, "345.6L");
}
END_TEST

START_TEST (ZeroFuelValuesAreDisplayedCorrectly)
{
    // Arrange
    OdoAndTripDisplayHandler_Init();
    OdoAndTripDisplayHandler_Run();
    GCLStub_Set_InstantaneousFuelConsumption(0, GCL_SIGNAL_OK);
    GCLStub_Set_TripFuelConsumption(0, GCL_SIGNAL_OK);

    // Act
    GCLStub_Indicate_ShowNext();
    OdoAndTripDisplayHandler_Run();
    
    // Assert
    wchar* tripFieldTest = GCLStub_Get_TripFieldText();
    wchar* odoAndInstFuelField = GCLStub_Get_OdoAndInstFuelFieldText();
    AssertFieldContents(odoAndInstFuelField, "0.0L/100km");
    AssertFieldContents(tripFieldTest, "0.0L");
}
END_TEST

START_TEST (DecimalFuelValueBelowTenIsDisplayedCorrectly)
{
    // Arrange
    OdoAndTripDisplayHandler_Init();
    OdoAndTripDisplayHandler_Run();
    GCLStub_Set_InstantaneousFuelConsumption(5, GCL_SIGNAL_OK);
    GCLStub_Set_TripFuelConsumption(8, GCL_SIGNAL_OK);

    // Act
    GCLStub_Indicate_ShowNext();
    OdoAndTripDisplayHandler_Run();
    
    // Assert
    wchar* tripFieldTest = GCLStub_Get_TripFieldText();
    wchar* odoAndInstFuelField = GCLStub_Get_OdoAndInstFuelFieldText();
    AssertFieldContents(odoAndInstFuelField, "0.5L/100km");
    AssertFieldContents(tripFieldTest, "0.8L");
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
    Suite* suite = suite_create("OdoAndTripDisplayHandlerSuite");

    TCase* testCase = tcase_create("OdoAndTripDisplayHandlerTests");
    tcase_add_checked_fixture(testCase, setup, teardown);
    tcase_add_test(testCase, Initialization_ProvidesOutputWithINIT);
    tcase_add_test(testCase, FirstRunAfterInit_ShowsDistanceValues);
    tcase_add_test(testCase, ZeroDistanceValuesAreDisplayedCorrectly);
    tcase_add_test(testCase, DecimalTripValueBelowTenIsDisplayedCorrectly);
    tcase_add_test(testCase, ShowNextIndicationAfterFirstRun_ShowsFuelValues);
    tcase_add_test(testCase, ZeroFuelValuesAreDisplayedCorrectly);
    tcase_add_test(testCase, DecimalFuelValueBelowTenIsDisplayedCorrectly);

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

wchar DigitToWchar(uint8 digit)
{
    return (wchar)0x0030 + (wchar)digit;
}

