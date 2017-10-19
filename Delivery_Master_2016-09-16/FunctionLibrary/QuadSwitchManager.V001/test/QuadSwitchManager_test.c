#include <stdlib.h>
#include <check.h>
#include "../inc/QuadSwitchManager.h"
#include "GCL_Stub.h"
#include "SysTime_Stub.h"

const char* getEventName(uint8 eventId)
{
    switch (eventId)
    {
        case BUTTON_RELEASE: return "BUTTON_RELEASE";
        case BUTTON_PRESSED: return "BUTTON_PRESSED";
        case BUTTON_LONG_PRESSED: return "BUTTON_LONG_PRESSED";
        case BUTTON_EXTRALONG_PRESSED: return "BUTTON_EXTRALONG_PRESSED";
        default: return "UKNOWN";
    }
}

void TestButtonPressed(void(*setFunction)(uint8), uint8(*getFunction)(void))
{
    // Arrange
    const uint8 expectedEvent = BUTTON_PRESSED;
    setFunction(BUTTON_INPUT_ON);

    // Act
    QuadSwitchManager_Run();

    // Assert
    uint8 buttonEvent = getFunction();
    ck_assert_msg(buttonEvent == expectedEvent, "Expected: %s   Actual: %s", getEventName(expectedEvent), getEventName(buttonEvent));
}

void TestButtonLongPressed(void(*setFunction)(uint8), uint8(*getFunction)(void))
{
    // Arrange
    const uint8 expectedEvent = BUTTON_LONG_PRESSED;
    setFunction(BUTTON_INPUT_ON);

    // Act
    QuadSwitchManager_Run();
    SysTimeStub_FastForward(LONG_PRESS_TIME);
    QuadSwitchManager_Run();

    // Assert
    uint8 buttonEvent = getFunction();
    ck_assert_msg(buttonEvent == expectedEvent, "Expected: %s   Actual: %s", getEventName(expectedEvent), getEventName(buttonEvent));
}

void TestButtonExtraLongPressed(void(*setFunction)(uint8), uint8(*getFunction)(void))
{
    // Arrange
    const uint8 expectedEvent = BUTTON_EXTRALONG_PRESSED;
    setFunction(BUTTON_INPUT_ON);

    // Act
    QuadSwitchManager_Run();
    SysTimeStub_FastForward(LONG_PRESS_TIME);
    QuadSwitchManager_Run();
    SysTimeStub_FastForward(EXTRA_LONG_PRESS_TIME);
    QuadSwitchManager_Run();

    // Assert
    uint8 buttonEvent = getFunction();
    ck_assert_msg(buttonEvent == expectedEvent, "Expected: %s   Actual: %s", getEventName(expectedEvent), getEventName(buttonEvent));
}

void TestButtonPressedAndReleased(void(*setFunction)(uint8), uint8(*getFunction)(void))
{
    // Arrange
    const uint8 expectedEvent = BUTTON_RELEASE;
    setFunction(BUTTON_INPUT_ON);

    // Act
    QuadSwitchManager_Run();
    setFunction(BUTTON_INPUT_OFF);
    QuadSwitchManager_Run();

    // Assert
    uint8 buttonEvent = getFunction();
    ck_assert_msg(buttonEvent == expectedEvent, "Expected: %s   Actual: %s", getEventName(expectedEvent), getEventName(buttonEvent));
}

void TestButtonLongPressedAndReleased(void(*setFunction)(uint8), uint8(*getFunction)(void))
{
    // Arrange
    const uint8 expectedEvent = BUTTON_RELEASE;
    setFunction(BUTTON_INPUT_ON);

    // Act
    QuadSwitchManager_Run();
    SysTimeStub_SetTime(LONG_PRESS_TIME);
    QuadSwitchManager_Run();
    setFunction(BUTTON_INPUT_OFF);
    QuadSwitchManager_Run();

    // Assert
    uint8 buttonEvent = getFunction();
    ck_assert_msg(buttonEvent == expectedEvent, "Expected: %s   Actual: %s", getEventName(expectedEvent), getEventName(buttonEvent));
}

void TestButtonExtraLongPressedAndReleased(void(*setFunction)(uint8), uint8(*getFunction)(void))
{
    // Arrange
    const uint8 expectedEvent = BUTTON_RELEASE;
    setFunction(BUTTON_INPUT_ON);

    // Act
    QuadSwitchManager_Run();
    SysTimeStub_SetTime(LONG_PRESS_TIME);
    QuadSwitchManager_Run();
    SysTimeStub_SetTime(EXTRA_LONG_PRESS_TIME);
    setFunction(BUTTON_INPUT_OFF);
    QuadSwitchManager_Run();

    // Assert
    uint8 buttonEvent = getFunction();
    ck_assert_msg(buttonEvent == expectedEvent, "Expected: %s   Actual: %s", getEventName(expectedEvent), getEventName(buttonEvent));
}

START_TEST (Button1Pressed_Button1EventWithPressedStatus)
{
    TestButtonPressed(GCLStub_SetButton1State, GCLStub_GetButton1Event);
}
END_TEST

START_TEST (Button2Pressed_Button2EventWithPressedStatus)
{
    TestButtonPressed(GCLStub_SetButton2State, GCLStub_GetButton2Event);
}
END_TEST

START_TEST (Button3Pressed_Button3EventWithPressedStatus)
{
    TestButtonPressed(GCLStub_SetButton3State, GCLStub_GetButton3Event);
}
END_TEST

START_TEST (Button4Pressed_Button4EventWithPressedStatus)
{
    TestButtonPressed(GCLStub_SetButton4State, GCLStub_GetButton4Event);
}
END_TEST

START_TEST(Button1LongPressed_Button1EventWithLongPressStatus)
{
    TestButtonLongPressed(GCLStub_SetButton1State, GCLStub_GetButton1Event);
}
END_TEST

START_TEST(Button2LongPressed_Button2EventWithLongPressStatus)
{
    TestButtonLongPressed(GCLStub_SetButton2State, GCLStub_GetButton2Event);
}
END_TEST

START_TEST(Button3LongPressed_Button3EventWithLongPressStatus)
{
    TestButtonLongPressed(GCLStub_SetButton3State, GCLStub_GetButton3Event);
}
END_TEST

START_TEST(Button4LongPressed_Button4EventWithLongPressStatus)
{
    TestButtonLongPressed(GCLStub_SetButton4State, GCLStub_GetButton4Event);
}
END_TEST

START_TEST(Button1ExtraLongPressed_Button1EventWithExtraLongPressStatus)
{
    TestButtonExtraLongPressed(GCLStub_SetButton1State, GCLStub_GetButton1Event);
}
END_TEST

START_TEST(Button2ExtraLongPressed_Button2EventWithExtraLongPressStatus)
{
    TestButtonExtraLongPressed(GCLStub_SetButton2State, GCLStub_GetButton2Event);
}
END_TEST

START_TEST(Button3ExtraLongPressed_Button3EventWithExtraLongPressStatus)
{
    TestButtonExtraLongPressed(GCLStub_SetButton3State, GCLStub_GetButton3Event);
}
END_TEST

START_TEST(Button4ExtraLongPressed_Button4EventWithExtraLongPressStatus)
{
    TestButtonExtraLongPressed(GCLStub_SetButton4State, GCLStub_GetButton4Event);
}
END_TEST

START_TEST (PressedButton1Released_Button1Released)
{
    TestButtonPressedAndReleased(GCLStub_SetButton1State, GCLStub_GetButton1Event);
}
END_TEST

START_TEST (PressedButton2Released_Button2Released)
{
    TestButtonPressedAndReleased(GCLStub_SetButton2State, GCLStub_GetButton2Event);
}
END_TEST

START_TEST (PressedButton3Released_Button3Released)
{
    TestButtonPressedAndReleased(GCLStub_SetButton3State, GCLStub_GetButton3Event);
}
END_TEST

START_TEST (PressedButton4Released_Button4Released)
{
    TestButtonPressedAndReleased(GCLStub_SetButton4State, GCLStub_GetButton4Event);
}
END_TEST

START_TEST (LongPressedButton1Released_Button1Released)
{
    TestButtonLongPressedAndReleased(GCLStub_SetButton1State, GCLStub_GetButton1Event);
}
END_TEST

START_TEST (LongPressedButton2Released_Button2Released)
{
    TestButtonLongPressedAndReleased(GCLStub_SetButton2State, GCLStub_GetButton2Event);
}
END_TEST

START_TEST (LongPressedButton3Released_Button3Released)
{
    TestButtonLongPressedAndReleased(GCLStub_SetButton3State, GCLStub_GetButton3Event);
}
END_TEST

START_TEST (LongPressedButton4Released_Button4Released)
{
    TestButtonLongPressedAndReleased(GCLStub_SetButton4State, GCLStub_GetButton4Event);
}
END_TEST

START_TEST (ExtraLongPressedButton1Released_Button1Released)
{
    TestButtonExtraLongPressedAndReleased(GCLStub_SetButton1State, GCLStub_GetButton1Event);
}
END_TEST

START_TEST (ExtraLongPressedButton2Released_Button2Released)
{
    TestButtonExtraLongPressedAndReleased(GCLStub_SetButton2State, GCLStub_GetButton2Event);
}
END_TEST

START_TEST (ExtraLongPressedButton3Released_Button3Released)
{
    TestButtonExtraLongPressedAndReleased(GCLStub_SetButton3State, GCLStub_GetButton3Event);
}
END_TEST

START_TEST (ExtraLongPressedButton4Released_Button4Released)
{
    TestButtonExtraLongPressedAndReleased(GCLStub_SetButton4State, GCLStub_GetButton4Event);
}
END_TEST

void AssertButton1Event(uint8 expectedEvent)
{
    uint8 buttonEvent = GCLStub_GetButton1Event();
    ck_assert_msg(buttonEvent == expectedEvent, "Expected: %s   Actual: %s", getEventName(expectedEvent), getEventName(buttonEvent));
}

START_TEST (ButtonHeld_StateTransitionOnDefinedLimits)
{
    // Arrange
    GCLStub_SetButton1State(BUTTON_INPUT_ON);

    // Act
    QuadSwitchManager_Run();
    AssertButton1Event(BUTTON_PRESSED);
    SysTimeStub_FastForward(LONG_PRESS_TIME-1);
    QuadSwitchManager_Run();
    AssertButton1Event(BUTTON_PRESSED);
    SysTimeStub_FastForward(1);
    QuadSwitchManager_Run();
    AssertButton1Event(BUTTON_LONG_PRESSED);
    SysTimeStub_SetTime(EXTRA_LONG_PRESS_TIME-1);
    QuadSwitchManager_Run();
    AssertButton1Event(BUTTON_LONG_PRESSED);
    SysTimeStub_FastForward(1);
    QuadSwitchManager_Run();
    AssertButton1Event(BUTTON_EXTRALONG_PRESSED);
}
END_TEST

void AssertButtonStates(uint8 expectedEventButton1, uint8 expectedEventButton2, uint8 expectedEventButton3, uint8 expectedEventButton4)
{
    uint8 buttonEvent = GCLStub_GetButton1Event();
    ck_assert_msg(buttonEvent == expectedEventButton1, "Button1 Expected: %s   Actual: %s", getEventName(expectedEventButton1), getEventName(buttonEvent));
    buttonEvent = GCLStub_GetButton2Event();
    ck_assert_msg(buttonEvent == expectedEventButton2, "Button2 Expected: %s   Actual: %s", getEventName(expectedEventButton2), getEventName(buttonEvent));
    buttonEvent = GCLStub_GetButton3Event();
    ck_assert_msg(buttonEvent == expectedEventButton3, "Button3 Expected: %s   Actual: %s", getEventName(expectedEventButton3), getEventName(buttonEvent));
    buttonEvent = GCLStub_GetButton4Event();
    ck_assert_msg(buttonEvent == expectedEventButton4, "Button4 Expected: %s   Actual: %s", getEventName(expectedEventButton4), getEventName(buttonEvent));
}

START_TEST (FirstButton1ThenButton2Pressed_OnlyButton1Active)
{
    // Arrange
    GCLStub_SetButton1State(BUTTON_INPUT_ON);

    // Act
    QuadSwitchManager_Run();
    AssertButtonStates(BUTTON_PRESSED, BUTTON_RELEASE, BUTTON_RELEASE, BUTTON_RELEASE);
    GCLStub_SetButton2State(BUTTON_INPUT_ON);
    QuadSwitchManager_Run();
    AssertButtonStates(BUTTON_PRESSED, BUTTON_RELEASE, BUTTON_RELEASE, BUTTON_RELEASE);
}
END_TEST

START_TEST (FirstButton4ThenButton3Pressed_OnlyButton4Active)
{
    // Arrange
    GCLStub_SetButton4State(BUTTON_INPUT_ON);

    // Act
    QuadSwitchManager_Run();
    AssertButtonStates(BUTTON_RELEASE, BUTTON_RELEASE, BUTTON_RELEASE, BUTTON_PRESSED);
    GCLStub_SetButton3State(BUTTON_INPUT_ON);
    QuadSwitchManager_Run();
    AssertButtonStates(BUTTON_RELEASE, BUTTON_RELEASE, BUTTON_RELEASE, BUTTON_PRESSED);
}
END_TEST

START_TEST (FirstButton1ThenButton2PressedThenButton1Released_Button2Active)
{
    // Arrange
    GCLStub_SetButton1State(BUTTON_INPUT_ON);

    // Act
    QuadSwitchManager_Run();
    AssertButtonStates(BUTTON_PRESSED, BUTTON_RELEASE, BUTTON_RELEASE, BUTTON_RELEASE);
    GCLStub_SetButton2State(BUTTON_INPUT_ON);
    QuadSwitchManager_Run();
    AssertButtonStates(BUTTON_PRESSED, BUTTON_RELEASE, BUTTON_RELEASE, BUTTON_RELEASE);
    GCLStub_SetButton1State(BUTTON_INPUT_OFF);
    QuadSwitchManager_Run();
    AssertButtonStates(BUTTON_RELEASE, BUTTON_PRESSED, BUTTON_RELEASE, BUTTON_RELEASE);
}
END_TEST

START_TEST (FirstButton4ThenButton3PressedThenButton4Released_Button3Active)
{
    // Arrange
    GCLStub_SetButton4State(BUTTON_INPUT_ON);

    // Act
    QuadSwitchManager_Run();
    AssertButtonStates(BUTTON_RELEASE, BUTTON_RELEASE, BUTTON_RELEASE, BUTTON_PRESSED);
    GCLStub_SetButton3State(BUTTON_INPUT_ON);
    QuadSwitchManager_Run();
    AssertButtonStates(BUTTON_RELEASE, BUTTON_RELEASE, BUTTON_RELEASE, BUTTON_PRESSED);
    GCLStub_SetButton4State(BUTTON_INPUT_OFF);
    // When releasing a button later in sequence two iterations are required,
    // The first iteration to clear the state of the released button and
    // the second iteration to activate the pressed state of the other button
    QuadSwitchManager_Run();
    QuadSwitchManager_Run();
    AssertButtonStates(BUTTON_RELEASE, BUTTON_RELEASE, BUTTON_PRESSED, BUTTON_RELEASE);
}
END_TEST

void setup(void)
{
    GCLStub_Reset();
    QuadSwitchManager_Init();
    SysTimeStub_SetTime(0);
}

void teardown(void)
{}

Suite* testSuite_create()
{
    Suite* suite = suite_create("QuadSwitchManagerSuite");

    TCase* buttonPressesTestCase = tcase_create("ButtonPresses");
    tcase_add_checked_fixture(buttonPressesTestCase, setup, teardown);
    tcase_add_test(buttonPressesTestCase, Button1Pressed_Button1EventWithPressedStatus);
    tcase_add_test(buttonPressesTestCase, Button1LongPressed_Button1EventWithLongPressStatus);
    tcase_add_test(buttonPressesTestCase, Button1ExtraLongPressed_Button1EventWithExtraLongPressStatus);
    tcase_add_test(buttonPressesTestCase, Button2Pressed_Button2EventWithPressedStatus);
    tcase_add_test(buttonPressesTestCase, Button2LongPressed_Button2EventWithLongPressStatus);
    tcase_add_test(buttonPressesTestCase, Button2ExtraLongPressed_Button2EventWithExtraLongPressStatus);
    tcase_add_test(buttonPressesTestCase, Button3Pressed_Button3EventWithPressedStatus);
    tcase_add_test(buttonPressesTestCase, Button3LongPressed_Button3EventWithLongPressStatus);
    tcase_add_test(buttonPressesTestCase, Button3ExtraLongPressed_Button3EventWithExtraLongPressStatus);
    tcase_add_test(buttonPressesTestCase, Button4Pressed_Button4EventWithPressedStatus);
    tcase_add_test(buttonPressesTestCase, Button4LongPressed_Button4EventWithLongPressStatus);
    tcase_add_test(buttonPressesTestCase, Button4ExtraLongPressed_Button4EventWithExtraLongPressStatus);
    tcase_add_test(buttonPressesTestCase, ButtonHeld_StateTransitionOnDefinedLimits);

    TCase* buttonReleaseTestCase = tcase_create("ButtonReleases");
    tcase_add_checked_fixture(buttonReleaseTestCase, setup, teardown);
    tcase_add_test(buttonReleaseTestCase, PressedButton1Released_Button1Released);
    tcase_add_test(buttonReleaseTestCase, LongPressedButton1Released_Button1Released);
    tcase_add_test(buttonReleaseTestCase, ExtraLongPressedButton1Released_Button1Released);
    tcase_add_test(buttonReleaseTestCase, PressedButton2Released_Button2Released);
    tcase_add_test(buttonReleaseTestCase, LongPressedButton2Released_Button2Released);
    tcase_add_test(buttonReleaseTestCase, ExtraLongPressedButton2Released_Button2Released);
    tcase_add_test(buttonReleaseTestCase, PressedButton3Released_Button3Released);
    tcase_add_test(buttonReleaseTestCase, LongPressedButton3Released_Button3Released);
    tcase_add_test(buttonReleaseTestCase, ExtraLongPressedButton3Released_Button3Released);
    tcase_add_test(buttonReleaseTestCase, PressedButton4Released_Button4Released);
    tcase_add_test(buttonReleaseTestCase, LongPressedButton4Released_Button4Released);
    tcase_add_test(buttonReleaseTestCase, ExtraLongPressedButton4Released_Button4Released);

    TCase* multipleButtonsTestCase = tcase_create("MultipleButtons");
    tcase_add_checked_fixture(multipleButtonsTestCase, setup, teardown);
    tcase_add_test(multipleButtonsTestCase, FirstButton1ThenButton2Pressed_OnlyButton1Active);
    tcase_add_test(multipleButtonsTestCase, FirstButton4ThenButton3Pressed_OnlyButton4Active);
    tcase_add_test(multipleButtonsTestCase, FirstButton1ThenButton2PressedThenButton1Released_Button2Active);
    tcase_add_test(multipleButtonsTestCase, FirstButton4ThenButton3PressedThenButton4Released_Button3Active);

    suite_add_tcase(suite, buttonPressesTestCase);
    suite_add_tcase(suite, buttonReleaseTestCase);
    suite_add_tcase(suite, multipleButtonsTestCase);

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


