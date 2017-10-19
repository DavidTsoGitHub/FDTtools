#include <stdlib.h>
#include <check.h>
#include "../inc/TurnIndication.h"
#include "SoundManager.h"
#include "CAL.h"
#include "GCL_Stub.h"

#define TELLTALE_OFF    0
#define TELLTALE_ON     1

typedef enum
{
    LEFT,
    RIGHT
} indicator_t;

typedef enum
{
    INDICATOR_OFF,
    INDICATOR_ON
} indicator_status_t;

void SetIndicator(indicator_t indicator, indicator_status_t status);

void TestPositiveFlank(indicator_t indicator);
void TestNegativeFlank(indicator_t indicator);
void TestPositiveFlankTelltaleDelay(indicator_t indicator);
void TestNegativeFlankTelltaleDelay(indicator_t indicator);

void AssertSignals(indicator_t indicator, BSP_Sound currentSound, uint8 telltaleStatus);

START_TEST (AllSignalsOff_NoSoundOrTellTale)
{
    // Arrange
    SetIndicator(LEFT, INDICATOR_OFF);
    SetIndicator(RIGHT, INDICATOR_OFF);
    
    // Act
    TurnIndication_Run();

    // Assert
    AssertSignals(LEFT, NO_SOUND, TELLTALE_OFF);
    AssertSignals(RIGHT, NO_SOUND, TELLTALE_OFF);
}
END_TEST

START_TEST (LeftSignalPositiveFlank_TickSoundAndTelltaleOn)
{
    TestPositiveFlank(LEFT);
}
END_TEST

START_TEST (LeftSignalNegativeFlank_TackSoundAndTelltaleOff)
{
    TestNegativeFlank(LEFT);
}
END_TEST

START_TEST (RightSignalPositiveFlank_TickSoundAndTelltaleOn)
{
    TestPositiveFlank(RIGHT);
}
END_TEST

START_TEST (RightSignalNegativeFlank_TackSoundAndTelltaleOff)
{
    TestNegativeFlank(RIGHT);
}
END_TEST

START_TEST (LeftSignalPositiveFlankWithTelltaleDelay_SoundOnAndTelltaleDelayed)
{
    TestPositiveFlankTelltaleDelay(LEFT);
}
END_TEST

START_TEST (RightSignalPositiveFlankWithTelltaleDelay_SoundOnAndTelltaleDelayed)
{
    TestPositiveFlankTelltaleDelay(RIGHT);
}
END_TEST

START_TEST (LeftSignalNegativeFlankWithTelltaleDelay_SoundOffAndTelltaleDelayed)
{
    TestNegativeFlankTelltaleDelay(LEFT);
}
END_TEST

START_TEST (RightSignalNegativeFlankWithTelltaleDelay_SoundOffAndTelltaleDelayed)
{
    TestNegativeFlankTelltaleDelay(RIGHT);
}
END_TEST

void setup(void)
{
    GCLStub_Reset();
    SoundManager_Init();
    CAL_Init();
}

void teardown(void)
{}

Suite* testSuite_create()
{
    Suite* suite = suite_create("TurnIndicationSuite");

    TCase* testCase = tcase_create("TurnIndicationTests");
    tcase_add_checked_fixture(testCase, setup, teardown);
    tcase_add_test(testCase, AllSignalsOff_NoSoundOrTellTale);
    tcase_add_test(testCase, LeftSignalPositiveFlank_TickSoundAndTelltaleOn);
    tcase_add_test(testCase, LeftSignalNegativeFlank_TackSoundAndTelltaleOff);
    tcase_add_test(testCase, RightSignalPositiveFlank_TickSoundAndTelltaleOn);
    tcase_add_test(testCase, RightSignalNegativeFlank_TackSoundAndTelltaleOff);
    tcase_add_test(testCase, LeftSignalPositiveFlankWithTelltaleDelay_SoundOnAndTelltaleDelayed);
    tcase_add_test(testCase, RightSignalPositiveFlankWithTelltaleDelay_SoundOnAndTelltaleDelayed);
    tcase_add_test(testCase, LeftSignalNegativeFlankWithTelltaleDelay_SoundOffAndTelltaleDelayed);
    tcase_add_test(testCase, RightSignalNegativeFlankWithTelltaleDelay_SoundOffAndTelltaleDelayed);
    
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

void SetIndicator(indicator_t indicator, indicator_status_t status)
{
    switch (indicator)
    {
        case LEFT:
            GCLStub_SetLeftTurnIndication(status, GCL_SIGNAL_OK);    
        break;
        case RIGHT:
            GCLStub_SetRightTurnIndication(status, GCL_SIGNAL_OK);    
        break;
        default:
            ck_abort_msg("unknown indicator (%d) in SetIndicator call!", indicator);
        break;
    }
}

void AssertSignals(indicator_t indicator, BSP_Sound expectedSound, uint8 expectedTelltaleStatus)
{
    uint8 currentTelltaleStatus = 0;
    BSP_Sound currentSound = SoundManagerStub_GetCurrentSound();
    
    if (indicator == LEFT)
    {
        currentTelltaleStatus = GCLStub_GetLeftTurnTelltaleValue();
    }
    else if (indicator == RIGHT)
    {
        currentTelltaleStatus = GCLStub_GetRightTurnTelltaleValue();
    }
    else
    {
        ck_abort_msg("unknown indicator (%d) in assert statement!", indicator);
    }
    
    ck_assert_msg(expectedTelltaleStatus == currentTelltaleStatus, "TELLTALE expected:%d actual:%d", expectedTelltaleStatus, currentTelltaleStatus);
    ck_assert_msg(expectedSound == currentSound, "SOUND expected:%d actual:%d", expectedSound, currentSound);
}

void TestPositiveFlank(indicator_t indicator)
{
    // Arrange
    const uint8 expectedSoundRequests = 1;
    TurnIndication_Init();
    AssertSignals(indicator, NO_SOUND, TELLTALE_OFF);
    
    // Act 
    SetIndicator(indicator, INDICATOR_ON);
    TurnIndication_Run();
    TurnIndication_Run();
    TurnIndication_Run();
    
    // Assert
    AssertSignals(indicator, BSP_SOUND_TICK, TELLTALE_ON);
    uint8 actualSoundRequests = SoundManagerStub_GetSoundRequestCallCount();
    ck_assert_msg(expectedSoundRequests == actualSoundRequests, "SOUNDREQUESTS expected:%d actual:%d", expectedSoundRequests, actualSoundRequests);
}

void TestNegativeFlank(indicator_t indicator)
{
    // Arrange
    const uint8 expectedSoundRequests = 2;
    TurnIndication_Init();
    SetIndicator(indicator, INDICATOR_ON);
    TurnIndication_Run();
    AssertSignals(indicator, BSP_SOUND_TICK, TELLTALE_ON);
    
    // Act 
    SetIndicator(indicator, INDICATOR_OFF);
    TurnIndication_Run();
    TurnIndication_Run();
    TurnIndication_Run();
    
    // Assert
    AssertSignals(indicator, BSP_SOUND_TACK, TELLTALE_OFF);
    uint8 actualSoundRequests = SoundManagerStub_GetSoundRequestCallCount();
    ck_assert_msg(expectedSoundRequests == actualSoundRequests, "SOUNDREQUESTS expected:%d actual:%d", expectedSoundRequests, actualSoundRequests);
}

void TestPositiveFlankTelltaleDelay(indicator_t indicator)
{
    // Arrange
    CALStub_SetTelltaleDelay(3); // Delays the telltale three iterations
    TurnIndication_Init();
    
    // Act & Assert
    SetIndicator(indicator, INDICATOR_ON);
    TurnIndication_Run();
    AssertSignals(indicator, BSP_SOUND_TICK, TELLTALE_OFF);
    TurnIndication_Run();
    AssertSignals(indicator, BSP_SOUND_TICK, TELLTALE_OFF);
    TurnIndication_Run();
    AssertSignals(indicator, BSP_SOUND_TICK, TELLTALE_OFF);
    TurnIndication_Run();
    AssertSignals(indicator, BSP_SOUND_TICK, TELLTALE_ON);
}

void TestNegativeFlankTelltaleDelay(indicator_t indicator)
{
    // Arrange
    CALStub_SetTelltaleDelay(2); 
    TurnIndication_Init();
    SetIndicator(indicator, INDICATOR_ON);
    TurnIndication_Run();
    AssertSignals(indicator, BSP_SOUND_TICK, TELLTALE_OFF); 
    TurnIndication_Run();
    AssertSignals(indicator, BSP_SOUND_TICK, TELLTALE_OFF); 
    TurnIndication_Run();
    AssertSignals(indicator, BSP_SOUND_TICK, TELLTALE_ON); 
        
    // Act & Assert
    SetIndicator(indicator, INDICATOR_OFF);
    TurnIndication_Run();
    AssertSignals(indicator, BSP_SOUND_TACK, TELLTALE_ON);
    TurnIndication_Run();
    AssertSignals(indicator, BSP_SOUND_TACK, TELLTALE_ON);
    TurnIndication_Run();
    AssertSignals(indicator, BSP_SOUND_TACK, TELLTALE_OFF);
}
