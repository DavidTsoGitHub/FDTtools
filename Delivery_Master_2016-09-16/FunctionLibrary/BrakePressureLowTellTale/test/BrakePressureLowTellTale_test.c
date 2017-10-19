#include <stdlib.h>
#include <check.h>
#include "../inc/BrakePressureLowTellTale.h"
#include "GCL_Stub.h"

typedef enum reservoir_e
{
    FrontReservoir,
    RearReservoir,
    ParkReservoir
} reservoir_e;

typedef enum circuit_e
{
    FrontCircuit,
    RearCircuit
} circuit_e;

static const uint16 CIRCUIT_PRESSURE_LOW_LIMIT = 590;
static const uint16 CIRCUIT_PRESSURE_HIGH_LIMIT = 700;
static const uint16 CIRCUIT_PRESSURE_NORMAL = 650;

static const uint16 RESERVOIR_PRESSURE_LOW_LIMIT = 590;
static const uint16 RESERVOIR_PRESSURE_HIGH_LIMIT = 1000;
static const uint16 RESERVOIR_PRESSURE_NORMAL = 790;

static const uint8 INDICATOR_OFF = 0;
static const uint8 INDICATOR_ON = 1;

static void AssertIndicatorOff(void);
static void AssertIndicatorOn(void);
static void TestLowReservoirPressure(reservoir_e reservoir);
static void TestLowCircuitPressure(circuit_e circuit);

START_TEST (AllReservoirPressureOkCircuitPressureOK_IndicatorOff)
{
    // Arrange
    GCLStub_SetFrontCircuitPressure(CIRCUIT_PRESSURE_NORMAL);
    GCLStub_SetRearCircuitPressure(CIRCUIT_PRESSURE_NORMAL);
	GCLStub_SetFrontReservoirPressure(RESERVOIR_PRESSURE_NORMAL);
	GCLStub_SetRearReservoirPressure(RESERVOIR_PRESSURE_NORMAL);
	GCLStub_SetParkBrakeReservoirPressure(RESERVOIR_PRESSURE_NORMAL);

    // Act
	BrakePressureLowTellTale_Run();

    // Assert
    AssertIndicatorOff();
}
END_TEST

START_TEST (FrontReservoirPressureLowCircuitPressureNormal_IndicatorOn)
{
    TestLowReservoirPressure(FrontReservoir);
}
END_TEST

START_TEST (RearReservoirPressureLowCircuitPressureNormal_IndicatorOn)
{
    TestLowReservoirPressure(RearReservoir);
}
END_TEST

START_TEST (ParkReservoirPressureLowCircuitPressureNormal_IndicatorOn)
{
    TestLowReservoirPressure(ParkReservoir);
}
END_TEST

START_TEST (AllReservoirPressureNormalFrontCircuitPressureLow_IndicatorOn)
{
    TestLowCircuitPressure(FrontCircuit);
}
END_TEST

START_TEST (AllReservoirPressureNormalRearCircuitPressureLow_IndicatorOn)
{
    TestLowCircuitPressure(RearCircuit);
}
END_TEST

START_TEST (OneReservoirPressureHighCircuitPressureOK_IndicatorOff)
{
    // Arrange
    GCLStub_SetFrontCircuitPressure(CIRCUIT_PRESSURE_HIGH_LIMIT + 1);
    GCLStub_SetRearCircuitPressure(CIRCUIT_PRESSURE_HIGH_LIMIT + 1);
	GCLStub_SetFrontReservoirPressure(RESERVOIR_PRESSURE_NORMAL);
	GCLStub_SetRearReservoirPressure(RESERVOIR_PRESSURE_HIGH_LIMIT + 1);
	GCLStub_SetParkBrakeReservoirPressure(RESERVOIR_PRESSURE_NORMAL);

    // Act
	BrakePressureLowTellTale_Run();
	
    // Assert
    AssertIndicatorOff();
}
END_TEST

START_TEST (OneReservoirPressureHighOneCircuitPressureLow_IndicatorOn)
{
    // Arrange
    GCLStub_SetFrontCircuitPressure(CIRCUIT_PRESSURE_HIGH_LIMIT);
    GCLStub_SetRearCircuitPressure(CIRCUIT_PRESSURE_HIGH_LIMIT + 1);
	GCLStub_SetFrontReservoirPressure(RESERVOIR_PRESSURE_NORMAL);
	GCLStub_SetRearReservoirPressure(RESERVOIR_PRESSURE_HIGH_LIMIT + 1);
	GCLStub_SetParkBrakeReservoirPressure(RESERVOIR_PRESSURE_NORMAL);

    // Act
	BrakePressureLowTellTale_Run();

    // Assert
    AssertIndicatorOn();
}
END_TEST

START_TEST (AllReservoirPressureHighOneCircuitPressureBelow700_IndicatorOff)
{
    // Arrange
	uint8 expectedValue = 0;
	uint16 frontReservoirPressure = 600;
	uint16 rearReservoirPressure = 900;
	uint16 parkBrakeReservoirPressure = 800;
    uint16 frontCircuitPressure = 700;
    uint16 rearCircuitPressure = 800;
    GCLStub_SetFrontCircuitPressure(frontCircuitPressure);
    GCLStub_SetRearCircuitPressure(rearCircuitPressure);
	GCLStub_SetFrontReservoirPressure(frontReservoirPressure);
	GCLStub_SetRearReservoirPressure(rearReservoirPressure);
	GCLStub_SetParkBrakeReservoirPressure(parkBrakeReservoirPressure);

    // Act
	BrakePressureLowTellTale_Run();
	uint8 readIndicatorValue = GCLStub_GetBrakePressureLowTellTaleValue();

    // Assert
	ck_assert_msg(readIndicatorValue == expectedValue, "AllReservoirPressureHighOneCircuitPressureBelow700_IndicatorOff: readIndicatorValue == %d, expectedValue == %d", readIndicatorValue, expectedValue);
}
END_TEST

START_TEST (PowerModeLowWithPressureWarning_IndicatorOff)
{
    // Arrange
    GCLStub_SetPowerMode(POWER_LOW);
    
    // Act
    BrakePressureLowTellTale_Run();
    
    // Act
    AssertIndicatorOff();
}
END_TEST

void setup(void)
{
    GCLStub_Reset();
	GCLStub_SetPowerMode(POWER_IGNITION_ON);
	BrakePressureLowTellTale_Init();
}

void teardown(void)
{}

Suite* testSuite_create()
{
    Suite* suite = suite_create("BrakePressureLowTellTaleSuite");

    TCase* testCase = tcase_create("BrakePressureLowTellTaleTests");
    tcase_add_checked_fixture(testCase, setup, teardown);
    
	tcase_add_test(testCase, AllReservoirPressureOkCircuitPressureOK_IndicatorOff);
    
    tcase_add_test(testCase, FrontReservoirPressureLowCircuitPressureNormal_IndicatorOn);
    tcase_add_test(testCase, RearReservoirPressureLowCircuitPressureNormal_IndicatorOn);
    tcase_add_test(testCase, ParkReservoirPressureLowCircuitPressureNormal_IndicatorOn);
    
    tcase_add_test(testCase, AllReservoirPressureNormalFrontCircuitPressureLow_IndicatorOn);
    tcase_add_test(testCase, AllReservoirPressureNormalRearCircuitPressureLow_IndicatorOn);
    
    
	tcase_add_test(testCase, OneReservoirPressureHighCircuitPressureOK_IndicatorOff);
    tcase_add_test(testCase, OneReservoirPressureHighOneCircuitPressureLow_IndicatorOn);
    tcase_add_test(testCase, AllReservoirPressureHighOneCircuitPressureBelow700_IndicatorOff);
    tcase_add_test(testCase, PowerModeLowWithPressureWarning_IndicatorOff);

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

static void TestLowReservoirPressure(reservoir_e reservoir)
{
    // Arrange
    GCLStub_SetFrontCircuitPressure(CIRCUIT_PRESSURE_NORMAL);
    GCLStub_SetRearCircuitPressure(CIRCUIT_PRESSURE_NORMAL);
	GCLStub_SetFrontReservoirPressure(RESERVOIR_PRESSURE_NORMAL);
	GCLStub_SetRearReservoirPressure(RESERVOIR_PRESSURE_NORMAL);
	GCLStub_SetParkBrakeReservoirPressure(RESERVOIR_PRESSURE_NORMAL);
    
    switch (reservoir)
    {
        case FrontReservoir:
            GCLStub_SetFrontReservoirPressure(RESERVOIR_PRESSURE_LOW_LIMIT-1);
        break;
        case RearReservoir:
            GCLStub_SetRearReservoirPressure(RESERVOIR_PRESSURE_LOW_LIMIT-1);
        break;
        case ParkReservoir:
            GCLStub_SetParkBrakeReservoirPressure(RESERVOIR_PRESSURE_LOW_LIMIT-1);
        break;
        default:
        ck_abort_msg("Invalid reservoir ID (%d) provided to test method!", reservoir);
        break;
    }

    // Act
	BrakePressureLowTellTale_Run();
    
    // Assert
    AssertIndicatorOn();
}

static void TestLowCircuitPressure(circuit_e circuit)
{
    // Arrange
    GCLStub_SetFrontCircuitPressure(CIRCUIT_PRESSURE_NORMAL);
    GCLStub_SetRearCircuitPressure(CIRCUIT_PRESSURE_NORMAL);
	GCLStub_SetFrontReservoirPressure(RESERVOIR_PRESSURE_NORMAL);
	GCLStub_SetRearReservoirPressure(RESERVOIR_PRESSURE_NORMAL);
	GCLStub_SetParkBrakeReservoirPressure(RESERVOIR_PRESSURE_NORMAL);
    
    switch (circuit)
    {
        case FrontCircuit:
            GCLStub_SetFrontCircuitPressure(CIRCUIT_PRESSURE_LOW_LIMIT-1);
        break;
        case RearCircuit:
            GCLStub_SetRearCircuitPressure(CIRCUIT_PRESSURE_LOW_LIMIT-1);
        break;
        default:
        ck_abort_msg("Invalid circuit ID (%d) provided to test method!", circuit);
        break;
    }

    // Act
	BrakePressureLowTellTale_Run();
    
    // Assert
    AssertIndicatorOn();
}

static void AssertIndicatorOn()
{
    uint8 readIndicatorValue = GCLStub_GetBrakePressureLowTellTaleValue();
	ck_assert_msg(readIndicatorValue == INDICATOR_ON, "Indicator was not ON as expected! (readIndicatorValue == %d)", readIndicatorValue);
}

static void AssertIndicatorOff()
{
    uint8 readIndicatorValue = GCLStub_GetBrakePressureLowTellTaleValue();
	ck_assert_msg(readIndicatorValue == INDICATOR_OFF, "Indicator was not OFF as expected! (readIndicatorValue == %d)", readIndicatorValue);
}

