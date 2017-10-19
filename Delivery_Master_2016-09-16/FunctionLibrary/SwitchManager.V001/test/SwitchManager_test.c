#include <stdlib.h>
#include <check.h>
#include "../inc/SwitchManager.h"
#include "GCL_Stub.h"
#include "SysTime_Stub.h"

#define BUTTON_INACTIVE 0U
#define BUTTON_UP       1U
#define BUTTON_DOWN     2U
#define BUTTON_LEFT     3U
#define BUTTON_RIGHT    4U
#define BUTTON_SET      5U
#define BUTTON_CLEAR    6U

#define BUTTON_RELEASED             0U
#define BUTTON_PRESSED              1U
#define BUTTON_LONG_PRESSED         2U
#define BUTTON_EXTRALONG_PRESSED    3U

void AssertButtonState(uint8 button, uint8 expectedStatus)
{
    uint8 buttonState;
    switch (button)
    {
        case BUTTON_INACTIVE:
        break;
        case BUTTON_UP:
            buttonState = GCLStub_GetButtonEventUp();
        break;
        case BUTTON_DOWN:
            buttonState = GCLStub_GetButtonEventDown();
        break;
        case BUTTON_LEFT:
            buttonState = GCLStub_GetButtonEventLeft();
        break;
        case BUTTON_RIGHT:
            buttonState = GCLStub_GetButtonEventRight();
        break;
        case BUTTON_SET:
            buttonState = GCLStub_GetButtonEventSet();
        break;
        case BUTTON_CLEAR:
            buttonState = GCLStub_GetButtonEventClear();
        break;
        default:
        ck_assert_msg(FALSE, "Unknown button (%d) supplied to AssertButtonState method", button);
    }

    ck_assert_msg(buttonState == expectedStatus, "Expected: %d   Actual: %d", expectedStatus, buttonState);
}

void TestButtonPressed(uint8 button)
{
    // Arrange
    GCLStub_SetButtonRaw(button);

    // Act
    SwitchManager_Run();

    // Assert
    AssertButtonState(button, BUTTON_PRESSED);

}

void TestButtonLongPress(uint8 button)
{
    // Arrange
    GCLStub_SetButtonRaw(button);

    // Act
    SwitchManager_Run();
    SysTimeStub_FastForward(LONG_PRESS_TIME);
    SwitchManager_Run();

    // Assert
    AssertButtonState(button, BUTTON_LONG_PRESSED);
}

void TestButtonExtraLongPress(uint8 button)
{
    // Arrange
    GCLStub_SetButtonRaw(button);

    // Act
    SwitchManager_Run();
    SysTimeStub_FastForward(LONG_PRESS_TIME);
    SwitchManager_Run();
    SysTimeStub_FastForward(EXTRA_LONG_PRESS_TIME);
    SwitchManager_Run();

    // Assert
    AssertButtonState(button, BUTTON_EXTRALONG_PRESSED);
}


START_TEST (ButtonUpPressed_ButtonUpEventWithPressedStatus)
{
    TestButtonPressed(BUTTON_UP);
}
END_TEST

START_TEST (ButtonDownPressed_ButtonDownEventWithPressedStatus)
{
    TestButtonPressed(BUTTON_DOWN);
}
END_TEST

START_TEST (ButtonLeftPressed_ButtonLeftEventWithPressedStatus)
{
    TestButtonPressed(BUTTON_LEFT);
}
END_TEST

START_TEST (ButtonRightPressed_ButtonRightEventWithPressedStatus)
{
    TestButtonPressed(BUTTON_RIGHT);
}
END_TEST

START_TEST (ButtonSetPressed_ButtonSetEventWithPressedStatus)
{
    TestButtonPressed(BUTTON_SET);
}
END_TEST

START_TEST (ButtonClearPressed_ButtonClearEventWithPressedStatus)
{
    TestButtonPressed(BUTTON_CLEAR);
}
END_TEST

START_TEST(ButtonUpLongPressed_ButtonUpEventWithLongPressStatus)
{
    TestButtonLongPress(BUTTON_UP);
}
END_TEST

START_TEST (ButtonDownLongPressed_ButtonDownEventWithLongPressStatus)
{
    TestButtonLongPress(BUTTON_DOWN);
}
END_TEST

START_TEST (ButtonLeftLongPressed_ButtonLeftEventWithLongPressStatus)
{
    TestButtonLongPress(BUTTON_LEFT);
}
END_TEST

START_TEST (ButtonRightLongPressed_ButtonRightEventWithLongPressStatus)
{
    TestButtonLongPress(BUTTON_RIGHT);
}
END_TEST

START_TEST (ButtonSetLongPressed_ButtonSetEventWithLongPressStatus)
{
    TestButtonLongPress(BUTTON_SET);
}
END_TEST

START_TEST (ButtonClearLongPressed_ButtonClearEventWithLongPressStatus)
{
    TestButtonLongPress(BUTTON_CLEAR);
}
END_TEST

START_TEST(ButtonUpExtraLongPressed_ButtonUpEventWithExtraLongPressStatus)
{
    TestButtonExtraLongPress(BUTTON_UP);
}
END_TEST

START_TEST (ButtonDownExtraLongPressed_ButtonDownEventWithExtraLongPressStatus)
{
    TestButtonExtraLongPress(BUTTON_DOWN);
}
END_TEST

START_TEST (ButtonLeftExtraLongPressed_ButtonLeftEventWithExtraLongPressStatus)
{
    TestButtonExtraLongPress(BUTTON_LEFT);
}
END_TEST

START_TEST (ButtonRightExtraLongPressed_ButtonRightEventWithExtraLongPressStatus)
{
    TestButtonExtraLongPress(BUTTON_RIGHT);
}
END_TEST

START_TEST (ButtonSetExtraLongPressed_ButtonSetEventWithExtraLongPressStatus)
{
    TestButtonExtraLongPress(BUTTON_SET);
}
END_TEST

START_TEST (ButtonClearExtraLongPressed_ButtonClearEventWithExtraLongPressStatus)
{
    TestButtonExtraLongPress(BUTTON_CLEAR);
}
END_TEST

START_TEST (ButtonReleased_SendsButtonReleasedEvent)
{
    // Arrange
    GCLStub_SetButtonRaw(BUTTON_UP);

    // Act
    SwitchManager_Run();
    GCLStub_SetButtonRaw(BUTTON_INACTIVE);
    SwitchManager_Run();

    // Assert
    AssertButtonState(BUTTON_UP, BUTTON_RELEASED);
}
END_TEST

START_TEST (SecondButtonPressed_FirstButtonRemainsActive)
{
    // Arrange
    GCLStub_SetButtonRaw(BUTTON_UP);

    // Act
    SwitchManager_Run();
    GCLStub_SetButtonRaw(BUTTON_DOWN);
    SwitchManager_Run();

    // Assert
    AssertButtonState(BUTTON_UP, BUTTON_PRESSED);
    AssertButtonState(BUTTON_DOWN, BUTTON_RELEASED);
}
END_TEST

START_TEST (ButtonHeld_StateTransitionOnDefinedLimits)
{
    // Arrange
    GCLStub_SetButtonRaw(BUTTON_UP);

    // Act
    SwitchManager_Run();
    AssertButtonState(BUTTON_UP, BUTTON_PRESSED);
    SysTimeStub_FastForward(LONG_PRESS_TIME-1);
    SwitchManager_Run();
    AssertButtonState(BUTTON_UP, BUTTON_PRESSED);
    SysTimeStub_FastForward(1);
    SwitchManager_Run();
    AssertButtonState(BUTTON_UP, BUTTON_LONG_PRESSED);
    SysTimeStub_SetTime(EXTRA_LONG_PRESS_TIME-1);
    SwitchManager_Run();
    AssertButtonState(BUTTON_UP, BUTTON_LONG_PRESSED);
    SysTimeStub_FastForward(1);
    SwitchManager_Run();
    AssertButtonState(BUTTON_UP, BUTTON_EXTRALONG_PRESSED);
}
END_TEST

void setup(void)
{
    GCLStub_Reset();
    SwitchManager_Init();
    SysTimeStub_SetTime(0);
}

void teardown(void)
{}

Suite* testSuite_create()
{
    Suite* suite = suite_create("SwitchManagerSuite");

    TCase* basicButtonPressesTestCase = tcase_create("BasicButtonPresses");
    tcase_add_checked_fixture(basicButtonPressesTestCase, setup, teardown);
    tcase_add_test(basicButtonPressesTestCase, ButtonUpPressed_ButtonUpEventWithPressedStatus);
    tcase_add_test(basicButtonPressesTestCase, ButtonDownPressed_ButtonDownEventWithPressedStatus);
    tcase_add_test(basicButtonPressesTestCase, ButtonLeftPressed_ButtonLeftEventWithPressedStatus);
    tcase_add_test(basicButtonPressesTestCase, ButtonRightPressed_ButtonRightEventWithPressedStatus);
    tcase_add_test(basicButtonPressesTestCase, ButtonSetPressed_ButtonSetEventWithPressedStatus);
    tcase_add_test(basicButtonPressesTestCase, ButtonClearPressed_ButtonClearEventWithPressedStatus);
    tcase_add_test(basicButtonPressesTestCase, ButtonUpLongPressed_ButtonUpEventWithLongPressStatus);
    tcase_add_test(basicButtonPressesTestCase, ButtonDownLongPressed_ButtonDownEventWithLongPressStatus);
    tcase_add_test(basicButtonPressesTestCase, ButtonLeftLongPressed_ButtonLeftEventWithLongPressStatus);
    tcase_add_test(basicButtonPressesTestCase, ButtonRightLongPressed_ButtonRightEventWithLongPressStatus);
    tcase_add_test(basicButtonPressesTestCase, ButtonSetLongPressed_ButtonSetEventWithLongPressStatus);
    tcase_add_test(basicButtonPressesTestCase, ButtonClearLongPressed_ButtonClearEventWithLongPressStatus);
    tcase_add_test(basicButtonPressesTestCase, ButtonUpExtraLongPressed_ButtonUpEventWithExtraLongPressStatus);
    tcase_add_test(basicButtonPressesTestCase, ButtonDownExtraLongPressed_ButtonDownEventWithExtraLongPressStatus);
    tcase_add_test(basicButtonPressesTestCase, ButtonLeftExtraLongPressed_ButtonLeftEventWithExtraLongPressStatus);
    tcase_add_test(basicButtonPressesTestCase, ButtonRightExtraLongPressed_ButtonRightEventWithExtraLongPressStatus);
    tcase_add_test(basicButtonPressesTestCase, ButtonSetExtraLongPressed_ButtonSetEventWithExtraLongPressStatus);
    tcase_add_test(basicButtonPressesTestCase, ButtonClearExtraLongPressed_ButtonClearEventWithExtraLongPressStatus);

    TCase* transitionsTestCase = tcase_create("Transitions");
    tcase_add_checked_fixture(transitionsTestCase, setup, teardown);
    tcase_add_test(transitionsTestCase, ButtonReleased_SendsButtonReleasedEvent);
    tcase_add_test(transitionsTestCase, SecondButtonPressed_FirstButtonRemainsActive);
    tcase_add_test(transitionsTestCase, ButtonHeld_StateTransitionOnDefinedLimits);

    suite_add_tcase(suite, basicButtonPressesTestCase);
    suite_add_tcase(suite, transitionsTestCase);

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


