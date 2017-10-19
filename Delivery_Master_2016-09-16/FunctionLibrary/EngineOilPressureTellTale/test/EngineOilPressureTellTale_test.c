#include <stdlib.h>
#include <check.h>
#include "../inc/EngineOilPressureTellTale.h"
#include "GCL_Stub.h"

/**** TEST SPECIFIC DEFINES ****/

static const uint8 TELLTALE_OFF = 0x00;
static const uint8 TELLTALE_ON = 0x01;

#define OIL_PRESSURE_OK     0x00
#define OIL_PRESSURE_NOK    0x01

void AssertTellTaleOff(void)
{
    uint8 telltaleStatus = GCLStub_GetEngineOilPressureWarningIndication();
    ck_assert_msg(telltaleStatus == TELLTALE_OFF, "Engine Oil Pressure telltale was not OFF as expected (actual value:%d)", telltaleStatus);
}

void AssertTellTaleOn(void)
{
    uint8 telltaleStatus = GCLStub_GetEngineOilPressureWarningIndication();
    ck_assert_msg(telltaleStatus == TELLTALE_ON, "Engine Oil Pressure telltale was not ON as expected (actual value:%d)", telltaleStatus);
}

START_TEST (PowerIgnitionOffOilPressureOk_TellTaleOff)
{
    // Arrange
    GCLStub_SetPowerMode(POWER_IGNITION_OFF, GCL_SIGNAL_OK);
    GCLStub_SetEngineOilPressureWarningRequest(OIL_PRESSURE_OK, GCL_SIGNAL_OK);
    EngineOilPressureTellTale_Init();
    
    // Act
    EngineOilPressureTellTale_Run();
    
    // Assert
    AssertTellTaleOff();
}
END_TEST

START_TEST (PowerIgnitionOffOilPressureNOk_TellTaleOff)
{
    // Arrange
    GCLStub_SetPowerMode(POWER_IGNITION_OFF, GCL_SIGNAL_OK);
    GCLStub_SetEngineOilPressureWarningRequest(OIL_PRESSURE_NOK, GCL_SIGNAL_OK);
    EngineOilPressureTellTale_Init();
    
    // Act
    EngineOilPressureTellTale_Run();
    
    // Assert
    AssertTellTaleOff();
}
END_TEST

START_TEST (PowerModeLowOilPressureOk_TellTaleOff)
{
    // Arrange
    GCLStub_SetPowerMode(POWER_LOW, GCL_SIGNAL_OK);
    GCLStub_SetEngineOilPressureWarningRequest(OIL_PRESSURE_OK, GCL_SIGNAL_OK);
    EngineOilPressureTellTale_Init();
    
    // Act
    EngineOilPressureTellTale_Run();
    
    // Assert
    AssertTellTaleOff();
}
END_TEST

START_TEST (PowerModeLowOilPressureNOk_TellTaleOff)
{
    // Arrange
    GCLStub_SetPowerMode(POWER_LOW, GCL_SIGNAL_OK);
    GCLStub_SetEngineOilPressureWarningRequest(OIL_PRESSURE_NOK, GCL_SIGNAL_OK);
    EngineOilPressureTellTale_Init();
    
    // Act
    EngineOilPressureTellTale_Run();
    
    // Assert
    AssertTellTaleOff();
}
END_TEST

START_TEST (PowerIgnitionOnOilPressureOk_TellTaleOff)
{
    // Arrange
    GCLStub_SetPowerMode(POWER_IGNITION_ON, GCL_SIGNAL_OK);
    GCLStub_SetEngineOilPressureWarningRequest(OIL_PRESSURE_OK, GCL_SIGNAL_OK);
    EngineOilPressureTellTale_Init();
    
    // Act
    EngineOilPressureTellTale_Run();
    
    // Assert
    AssertTellTaleOff();
}
END_TEST

START_TEST (PowerIgnitionOnOilPressureNOk_TellTaleOff)
{
    // Arrange
    GCLStub_SetPowerMode(POWER_IGNITION_ON, GCL_SIGNAL_OK);
    GCLStub_SetEngineOilPressureWarningRequest(OIL_PRESSURE_NOK, GCL_SIGNAL_OK);
    EngineOilPressureTellTale_Init();
    
    // Act
    EngineOilPressureTellTale_Run();
    
    // Assert
    AssertTellTaleOn();
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
    Suite* suite = suite_create("EngineOilPressureTellTaleSuite");

    TCase* testCase = tcase_create("EngineOilPressureTellTaleTests");
    tcase_add_checked_fixture(testCase, setup, teardown);
    tcase_add_test(testCase, PowerIgnitionOffOilPressureOk_TellTaleOff);
    tcase_add_test(testCase, PowerIgnitionOffOilPressureNOk_TellTaleOff);
    tcase_add_test(testCase, PowerModeLowOilPressureOk_TellTaleOff);
    tcase_add_test(testCase, PowerModeLowOilPressureNOk_TellTaleOff);
    tcase_add_test(testCase, PowerIgnitionOnOilPressureOk_TellTaleOff);
    tcase_add_test(testCase, PowerIgnitionOnOilPressureNOk_TellTaleOff);

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


