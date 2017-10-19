#include <stdlib.h>
#include <check.h>
#include "../inc/OdoRuntimeHandler.h"
#include "GCL_Stub.h"
#include "GCL_OdoRuntimeHandler.h"


/**** TEST SPECIFIC DEFINES ****/
// Sample
#define SIGNAL_NAME_ON  0U
#define SIGNAL_NAME_OFF 1U

START_TEST (SetOdoValue_ReadCorrectValue)
{
    // Arrange
    uint32 expectedValue = 234;
    uint32 readValue =0;

    OdoRuntimeHandler_Init();

    uint8 signalStat = GCL_SIGNAL_OK;
    GCLStub_Set_OdoValue(expectedValue, signalStat);

    // Act
    OdoRuntimeHandler_Run();
    readValue = GCLStub_Get_Value();

    // Assert
    ck_assert_msg(readValue == expectedValue, "Read value = %d, expected Value = %d" ,readValue, expectedValue);
}
END_TEST

START_TEST (SetRunTimeValue_ReadCorrectValue)
{
    // Arrange
    uint16 expectedValue = 23444;
    uint16 readValue =0;

    OdoRuntimeHandler_Init();
    
    uint8 signalStat = GCL_SIGNAL_OK;
    GCLStub_Set_RuntimeValue(expectedValue, signalStat);
    GCLStub_Indicate_ShowNext();
    
    // Act
    OdoRuntimeHandler_Run();
    readValue = GCLStub_Get_Value();

    // Assert
    ck_assert_msg(readValue == expectedValue, "Read value = %d, expected Value = %d" ,readValue, expectedValue);
}
END_TEST


START_TEST (SwitchBetweenODORuntime_DotAndHourGlassToggled)
{
    // Arrange
    uint32 expectedValue = 234;
    boolean ShowDotAfterOdo = FALSE;
    boolean ShowDotAfterRuntime = FALSE;
    boolean ShowHourGlassAfterRuntime = FALSE;
    boolean ShowHourGlassAfterOdo = FALSE;    

    
    OdoRuntimeHandler_Init();
    
    uint8 signalStat = GCL_SIGNAL_OK;
    GCLStub_Set_RuntimeValue(expectedValue, signalStat);
    GCLStub_Indicate_ShowNext();

    // Act
    OdoRuntimeHandler_Run();
    ShowDotAfterRuntime = GCLStub_Get_ShowDot();
    ShowHourGlassAfterRuntime = GCLStub_Get_ShowHourGlass();        

    GCLStub_Indicate_ShowNext();
    OdoRuntimeHandler_Run();

    ShowDotAfterOdo = GCLStub_Get_ShowDot();
    ShowHourGlassAfterOdo = GCLStub_Get_ShowHourGlass();    
    
    // Assert
    ck_assert_msg(ShowDotAfterRuntime == TRUE, "1Read value = %d, expected Value = %d" ,ShowDotAfterRuntime, TRUE);
    ck_assert_msg(ShowHourGlassAfterRuntime == TRUE, "2Read value = %d, expected Value = %d" ,ShowHourGlassAfterRuntime, TRUE);
    ck_assert_msg(ShowDotAfterOdo == FALSE, "3Read value = %d, expected Value = %d" ,ShowDotAfterOdo, FALSE);
    ck_assert_msg(ShowHourGlassAfterOdo == FALSE, "4Read value = %d, expected Value = %d" ,ShowHourGlassAfterOdo, FALSE);    
}
END_TEST

START_TEST (SetDotValue_ReadCorrectValue)
{
    // Arrange
    uint32 expectedValue = 234;
    boolean ShowDot = FALSE;
    boolean ShowHourGlass = FALSE;    

    OdoRuntimeHandler_Init();
    
    uint8 signalStat = GCL_SIGNAL_OK;
    GCLStub_Set_RuntimeValue(expectedValue, signalStat);
    GCLStub_Indicate_ShowNext();

    // Act
    OdoRuntimeHandler_Run();
    ShowDot = GCLStub_Get_ShowDot();
    ShowHourGlass = GCLStub_Get_ShowHourGlass();        

    // Assert
    ck_assert_msg(ShowDot == TRUE, "Read value = %d, expected Value = %d" ,ShowDot, TRUE);
    ck_assert_msg(ShowHourGlass == TRUE, "Read value = %d, expected Value = %d" ,ShowHourGlass, TRUE);
    
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
    Suite* suite = suite_create("OdoRuntimeHandlerSuite");

    TCase* testCase = tcase_create("OdoRuntimeHandlerTests");
    tcase_add_checked_fixture(testCase, setup, teardown);
    tcase_add_test(testCase, SetOdoValue_ReadCorrectValue);
    tcase_add_test(testCase, SetRunTimeValue_ReadCorrectValue);
    tcase_add_test(testCase, SwitchBetweenODORuntime_DotAndHourGlassToggled);
    tcase_add_test(testCase, SetDotValue_ReadCorrectValue);
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


