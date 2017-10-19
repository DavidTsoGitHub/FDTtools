#include <stdlib.h>
#include <check.h>
#include "../inc/FuelConsumption.h"
#include "GCL_Stub.h"
#include "GCL_FuelConsumption.h"
#include "CAL_Stub.h"
#include "CAL.h"

/**** TEST SPECIFIC DEFINES ****/
#define INSTANTANIOUS_FUEL_CONSUMPTION_GAIN             1953
#define INSTANTANIOUS_FUEL_CONSUMPTION_GAIN_DIVIDER     100000
#define COEFFICIENT_SCALING                             10000
const uint16 u16FuelConsumptionCoefficient = 5000;

START_TEST (SetInstantaniousFuelConsumption_ReadCorrectInstantaniousFuelConsumption)
{
    // Arrange
    GCLStub_Reset();
    FuelConsumption_Init();
    uint16 instantaniousFuelConsumption = 0x7800;
    uint16 expectedValue = ((instantaniousFuelConsumption * INSTANTANIOUS_FUEL_CONSUMPTION_GAIN) + (INSTANTANIOUS_FUEL_CONSUMPTION_GAIN_DIVIDER / 2)) / INSTANTANIOUS_FUEL_CONSUMPTION_GAIN_DIVIDER;
    GCLStub_SetPowerMode(POWER_IGNITION_ON);
    GCLStub_SetInstantaneousFuelConsumption(instantaniousFuelConsumption);

    // Act
    int i;
    for(i=0;i<6;i++)
    {
        FuelConsumption_Run();
    }
    uint16 readValue = GCLStub_GetFilteredInstantaniousFuelConsumption();

    // Assert
    ck_assert_msg(readValue == expectedValue, "Filtered value: %d, expected: %d", readValue, expectedValue);
}
END_TEST

START_TEST (SetDifferentFuelConsumptions_GetCorrectFilteredFuelConsumption)
{
    // Arrange
    GCLStub_Reset();
    FuelConsumption_Init();
    GCLStub_SetPowerMode(POWER_IGNITION_ON);
    uint16 instantaniousFuelConsumption = 0;
    uint16 fuelConsumptionContainer[6];

    // Act
    int i;
    for(i=0;i<6;i++)
    {
        instantaniousFuelConsumption += 0x1400;
        fuelConsumptionContainer[i] = instantaniousFuelConsumption;
        GCLStub_SetInstantaneousFuelConsumption(instantaniousFuelConsumption);
        FuelConsumption_Run();
    }
    uint16 readValue = GCLStub_GetFilteredInstantaniousFuelConsumption();

    uint32 sum = 0;
    for (i = 0; i < 5; ++i)
    {
        sum += fuelConsumptionContainer[i];
    }

    uint16 filteredInstantaniousFuelConsumption = (((COEFFICIENT_SCALING-u16FuelConsumptionCoefficient) * fuelConsumptionContainer[5]) + (u16FuelConsumptionCoefficient * (sum/5)))/ COEFFICIENT_SCALING;
    uint32 expectedValue = ((filteredInstantaniousFuelConsumption * INSTANTANIOUS_FUEL_CONSUMPTION_GAIN) + (INSTANTANIOUS_FUEL_CONSUMPTION_GAIN_DIVIDER / 2)) / INSTANTANIOUS_FUEL_CONSUMPTION_GAIN_DIVIDER;
    // Assert
    ck_assert_msg(readValue == expectedValue, "Filtered value: %d, expected: %d", readValue, expectedValue);
}
END_TEST

START_TEST (ClearTotalFuelAndRunOneHour_ReadALotOfFuel)
{
    // Arrange
    GCLStub_Reset();
    FuelConsumption_Init();
    uint32 fuelRate = 0x4B0; // 1200 * 0,05L/h == 60L/h
    GCLStub_SetPowerMode(POWER_IGNITION_ON);
    GCLStub_SetFuelRate(fuelRate);

    // Act
    int i;
    for(i=0;i<36000;i++)
    {
        FuelConsumption_Run();
    }
    uint32 readValue = GCLStub_GetTotalFuelConsumption();

    // Assert
    //0x0D is the tolerance of +-0.05L
    uint32 expectedValue = (fuelRate * 5 + 50) / 100;
    ck_assert_msg(readValue == expectedValue, "Read value: 0x%X, expected: <0x%X", readValue, expectedValue);
}
END_TEST

START_TEST (IgnitionOff_ReadZeroFromInstantaniousFuelConsumption)
{
    // Arrange
    GCLStub_Reset();
    FuelConsumption_Init();
    uint32 expectedValue = 0x0000FF00;
    GCLStub_SetPowerMode(POWER_IGNITION_ON);
    //uint32 avg=0;
    GCLStub_SetInstantaneousFuelConsumption(expectedValue);
    FuelConsumption_Run();

    // Act
    GCLStub_SetPowerMode(POWER_IGNITION_OFF);
    FuelConsumption_Run();
    uint32 readValue = GCLStub_GetFilteredInstantaniousFuelConsumption();

    // Assert
    ck_assert_msg(readValue == 0x00, "Filtered value: %d, expected: 0x00", readValue);
}
END_TEST

START_TEST (IgnitionOff_ReadTotalFuelConsumption)
{
    // Arrange
    GCLStub_Reset();
    FuelConsumption_Init();
    uint32 fuelRate = 0x0001001;
    GCLStub_SetPowerMode(POWER_IGNITION_ON);
    GCLStub_SetFuelRate(fuelRate);

    // Act
    int i;
    for(i=0;i<36000;i++)
    {
        FuelConsumption_Run();
    }

    GCLStub_SetPowerMode(POWER_IGNITION_OFF);
    FuelConsumption_Run();
    uint32 readValue = GCLStub_GetTotalFuelConsumption();

    // Assert
    uint32 expectedValue = ((fuelRate * 5) + 50) / 100;
    ck_assert_msg(readValue <= (expectedValue + 0x0D), "Read value: 0x%X, expected: <0x%X", readValue, (expectedValue + 0x0D));
    ck_assert_msg(readValue >= (expectedValue - 0x0D), "Read value: 0x%X, expected: >0x%X", readValue, (expectedValue - 0x0D));
}
END_TEST

START_TEST (SetVehicleFuelRate_ReadCorrectTripFuelConsumptionAfter10min)
{
    // Arrange
    GCLStub_Reset();
    FuelConsumption_Init();
    uint32 fuelRate = 0x4B0;       // 60 litres per hour
    
    GCLStub_SetPowerMode(POWER_IGNITION_ON);
    GCLStub_SetFuelRate(fuelRate);
    
    // Trip Fuel Consumption is stored in decilitres
    uint32 expectedValue = (((fuelRate * 5) + 3) / 6) / 10;

    // Act
    // Itterate for 600 seconds
    for (int i = 0; i < 6000; ++i)
    {
        FuelConsumption_Run();
    }

    uint32 readValue = GCLStub_GetTripFuelConsumption();

    // Assert
    ck_assert_msg(readValue == expectedValue, "Read value: 0x%X, expected: < %d", readValue, expectedValue);
}
END_TEST

START_TEST (IgnitionOff_ReadZeroTripFuelConsumptionAfter10min)
{
    // Arrange
    GCLStub_Reset();
    FuelConsumption_Init();
    uint16 vehicleSpeed = 0x8C;     // 140km/h
    uint32 fuelRate = 0x3C00;       // 60 litres per hour
    
    GCLStub_SetPowerMode(POWER_IGNITION_OFF);
    GCLStub_SetFuelRate(fuelRate);
    
    // Calculate the distance to add for each itteration. + 180 is used to convert the value to closest integer
    uint32 distanceToAdd = (((uint32)vehicleSpeed * 1000) + 180) / 360;

    // Act
    // Itterate for 600 seconds
    for (int i = 0; i < 6000; ++i)
    {
        GCLStub_IncreaseTripDistance(distanceToAdd);
        FuelConsumption_Run();
    }
    
    uint32 readValue = GCLStub_GetTripFuelConsumption();
    
    // Assert
    ck_assert_msg(readValue == 0, "Read value: 0x%X, expected: <0x%X", readValue, 0);
}
END_TEST

START_TEST (PowerLow_ReadZeroTripFuelConsumptionAfter10min)
{
    // Arrange
    GCLStub_Reset();
    FuelConsumption_Init();
    uint16 vehicleSpeed = 0x8C;     // 140km/h
    uint32 fuelRate = 0x3C00;       // 60 litres per hour
    
    GCLStub_SetPowerMode(POWER_LOW);
    GCLStub_SetFuelRate(fuelRate);
    
    // Calculate the distance to add for each itteration. + 180 is used to convert the value to closest integer
    uint32 distanceToAdd = (((uint32)vehicleSpeed * 1000) + 180) / 360;

    // Act
    // Itterate for 600 seconds
    for (int i = 0; i < 6000; ++i)
    {
        GCLStub_IncreaseTripDistance(distanceToAdd);
        FuelConsumption_Run();
    }

    // Assert
    uint32 readValue = GCLStub_GetTripFuelConsumption();
    ck_assert_msg(readValue == 0, "Read value: 0x%X, expected: <0x%X", readValue, 0);
}
END_TEST

START_TEST (Run10minutesThenToggleIgnitionOffOn_TripFuelConsumptionSetToZero)
{
    // Arrange
    GCLStub_Reset();
    FuelConsumption_Init();
    uint32 fuelRate = 0x4B0;       // 60 litres per hour
    
    GCLStub_SetPowerMode(POWER_IGNITION_ON);
    GCLStub_SetFuelRate(fuelRate);

    uint32 expectedValue = ((fuelRate * 5 + 3) / 6) / 10 ;

    // Act
    // Itterate for 600 seconds
    for (int i = 0; i < 6000; ++i)
    {
        FuelConsumption_Run();
    }
    
    uint32 readValueAfter10min = GCLStub_GetTripFuelConsumption();
    
    // Toggle Power Off then On
    GCLStub_SetPowerMode(POWER_IGNITION_OFF);
    FuelConsumption_Run();
    GCLStub_SetPowerMode(POWER_IGNITION_ON);
    FuelConsumption_Run();
    
    uint32 readValueAfterPowerToggle = GCLStub_GetTripFuelConsumption();
    
    //Assert
    ck_assert_msg(readValueAfter10min = expectedValue, "Read value: 0x%X, expected: <0x%X", readValueAfter10min, expectedValue);
    ck_assert_msg(readValueAfterPowerToggle == 0, "Read value: 0x%X, expected: 0x%X", readValueAfterPowerToggle, 0);
}
END_TEST

START_TEST (Run10minutesThenSendReset_TripFuelConsumptionSetToZero)
{
    // Arrange
    GCLStub_Reset();
    FuelConsumption_Init();
    uint32 fuelRate = 0x4B0;       // 60 litres per hour
    
    GCLStub_SetPowerMode(POWER_IGNITION_ON);
    GCLStub_SetFuelRate(fuelRate);
    
    uint32 expectedValue = ((fuelRate * 5 + 3) / 6) / 10 ;

    // Act
    // Itterate for 600 seconds
    for (int i = 0; i < 6000; ++i)
    {
        FuelConsumption_Run();
    }
    
    uint32 readValueAfter10min = GCLStub_GetTripFuelConsumption();
    
    // Toggle Power Off then On
    FuelConsumption_ResetSignal_Indication();
    FuelConsumption_Run();
    
    uint32 readValueAfterReset = GCLStub_GetTripFuelConsumption();
    
    //Assert
    ck_assert_msg(readValueAfter10min == expectedValue, "Read value: 0x%X, expected: 0x%X", readValueAfter10min, expectedValue);
    ck_assert_msg(readValueAfterReset == 0, "Read value: 0x%X, expected: 0x%X", readValueAfterReset, 0);
}
END_TEST

START_TEST (SetTotalFuelInCAL_ReadCorrectTotalFuelValue)
{
    // Arrange
    GCLStub_Reset();
    GCLStub_SetPowerMode(POWER_IGNITION_ON);
    uint32 expectedValue = 2546;
    CALStub_SetTotalFuel(expectedValue);
    
    // Act
    FuelConsumption_Init();
    FuelConsumption_Run();
    
    uint32 readValue = GCLStub_GetTotalFuelConsumption();
    
    // Assert
    ck_assert_msg(readValue == expectedValue, "Read value: %d, expected: %d", readValue, expectedValue);
}
END_TEST

START_TEST (Run10minThenIgnitionOff_ReadCorrectTotalFuelValueFromCal)
{
    // Arrange
    GCLStub_Reset();
    GCLStub_SetPowerMode(POWER_IGNITION_ON);
    uint32 fuelRate = 0x4B0;
    GCLStub_SetFuelRate(fuelRate);
    uint32 calTotalFuelValue = 2546;
    CALStub_SetTotalFuel(calTotalFuelValue);
    FuelConsumption_Init();
    
    // Act
    for (int i = 0; i < 6000; ++i)
    {
        FuelConsumption_Run();
    }
    
    GCLStub_SetPowerMode(POWER_IGNITION_OFF);
    FuelConsumption_Run();
    
    uint32 readValue = CALStub_GetTotalFuel();
    uint32 expectedValue = calTotalFuelValue + (((fuelRate * 5 + 3) / 6) / 100);
    
    // Assert
    ck_assert_msg(readValue == expectedValue, "Read value: %d, expected: %d", readValue, expectedValue);
}
END_TEST

START_TEST (SetTicksInCALRun10timesThenIgnitionOff_ReadCorrectTicksFromCAL)
{
    // Arrange
    GCLStub_Reset();
    GCLStub_SetPowerMode(POWER_IGNITION_ON);
    uint32 fuelRate = 0x4B0;
    GCLStub_SetFuelRate(fuelRate);
    uint32 calTickValue = 0x0BCC07;
    CALStub_SetTickValue(calTickValue);
    FuelConsumption_Init();
    uint32 expectedValue = calTickValue;
    
    // Act
    for (int i = 0; i < 10; ++i)
    {
        expectedValue += (((fuelRate * 5 * 10) + 18) / 36);
        FuelConsumption_Run();
    }
    
    GCLStub_SetPowerMode(POWER_IGNITION_OFF);
    FuelConsumption_Run();
    uint32 readValue = CALStub_GetTickValue();
    
    // Assert
    ck_assert_msg(readValue == expectedValue, "Read value: 0x%08X, expected: 0x%08X", readValue, expectedValue);
}
END_TEST

START_TEST(SetTotalFuelAbove9999999_Read9999999OutputSignal)
{
    // Arrange
    GCLStub_Reset();
    GCLStub_SetPowerMode(POWER_IGNITION_ON);
    // Act
    CALStub_SetTotalFuel(100000000);
    FuelConsumption_Init();
    FuelConsumption_Run();
    
    uint32 readValue = GCLStub_GetTotalFuelConsumption();
    
    // Assert
    ck_assert_msg(readValue == 9999999, "Read value: %d, expected: 9999999", readValue);
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
    Suite* suite = suite_create("FuelConsumptionSuite");

    TCase* testCase = tcase_create("FuelConsumptionTests");
    tcase_add_checked_fixture(testCase, setup, teardown);
    tcase_add_test(testCase, SetInstantaniousFuelConsumption_ReadCorrectInstantaniousFuelConsumption);
    tcase_add_test(testCase, SetDifferentFuelConsumptions_GetCorrectFilteredFuelConsumption);
    tcase_add_test(testCase, ClearTotalFuelAndRunOneHour_ReadALotOfFuel);
    tcase_add_test(testCase, IgnitionOff_ReadZeroFromInstantaniousFuelConsumption);
    tcase_add_test(testCase, IgnitionOff_ReadTotalFuelConsumption);
    tcase_add_test(testCase, SetVehicleFuelRate_ReadCorrectTripFuelConsumptionAfter10min);
    tcase_add_test(testCase, IgnitionOff_ReadZeroTripFuelConsumptionAfter10min);
    tcase_add_test(testCase, PowerLow_ReadZeroTripFuelConsumptionAfter10min);
    tcase_add_test(testCase, Run10minutesThenToggleIgnitionOffOn_TripFuelConsumptionSetToZero);
    tcase_add_test(testCase, Run10minutesThenSendReset_TripFuelConsumptionSetToZero);
    tcase_add_test(testCase, SetTotalFuelInCAL_ReadCorrectTotalFuelValue);
    tcase_add_test(testCase, Run10minThenIgnitionOff_ReadCorrectTotalFuelValueFromCal);
    tcase_add_test(testCase, SetTicksInCALRun10timesThenIgnitionOff_ReadCorrectTicksFromCAL);
    tcase_add_test(testCase, SetTotalFuelAbove9999999_Read9999999OutputSignal);
    

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


