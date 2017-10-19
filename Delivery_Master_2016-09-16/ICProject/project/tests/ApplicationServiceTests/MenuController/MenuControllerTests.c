#include "check.h"
#include <stdio.h>

#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1

#include "MenuController.h"
#include "DisplayManagerMock.h"
#include "MenuRendererMock.h"
#include "GCLMock.h"
#include "ResourceManagerMock.h"

void AssertMenuDrawCallCount(int expected);
void AssertMenuRefreshCallCount(int expected);

START_TEST(Startup_MenuControllerInactive)
{
    // Arrange
    MenuController_Init();
    
    // Act
    MenuController_Run();
    MenuController_Run();
    MenuController_Run();
    MenuController_Run();
    MenuController_Run();
    
    // Assert
    AssertMenuDrawCallCount(0);
    AssertMenuRefreshCallCount(0);
}
END_TEST

START_TEST(Run_ResourceDenied_NoCallToRenderer)
{
    // Arrange
	MenuController_Init();
    DisplayManagerMock_SetState(DM_STATE_RUN);
    MenuController_Run();
    
    // Act
    ResourceManagerMock_RevokeResource();
    MenuController_Run();
    
    // Assert
    AssertMenuDrawCallCount(0);
    AssertMenuRefreshCallCount(0);
}
END_TEST

START_TEST(Run_ResourceGranted_CallToRenderer)
{
    // Arrange
    MenuController_Init();
    DisplayManagerMock_SetState(DM_STATE_RUN);
    MenuController_Run();

    // Act
    ResourceManagerMock_GrantResource();
    MenuController_Run();

    // Assert
    AssertMenuDrawCallCount(1);
    AssertMenuRefreshCallCount(0);
}
END_TEST

START_TEST(Run_AfterFirstDraw_CallsRefreshOnly)
{
    // Arrange
    MenuController_Init();
    DisplayManagerMock_SetState(DM_STATE_RUN);
    ResourceManagerMock_GrantResource();
    MenuController_Run();
    
    // Act
    MenuController_Run();
    int drawCountAfterFirstDraw = MenuRendererMock_DrawCallCount();
    int refreshCountAfterFirstDraw = MenuRendererMock_RefreshCallCount();
    MenuController_Run();
    
    // Assert
    AssertMenuDrawCallCount(drawCountAfterFirstDraw);
    AssertMenuRefreshCallCount(refreshCountAfterFirstDraw+1);
}
END_TEST

START_TEST(Run_EnterSubMenuItem_CallToRenderer)
{
    // Arrange
    MenuController_Init();
    DisplayManagerMock_SetState(DM_STATE_RUN);
    ResourceManagerMock_GrantResource();
    MenuController_Run();
    MenuController_Run();
    int initialCallCount = MenuRendererMock_DrawCallCount();

    // Act
    MenuController_Run();
    int callCountWhenUnchanged = MenuRendererMock_DrawCallCount();
    GCLMock_MenuButtonSet(1);
    MenuController_Run();
    int callCountAfterNextMenuItem = MenuRendererMock_DrawCallCount();

    // Assert
    ck_assert_msg(initialCallCount == callCountWhenUnchanged, "initialCallCount %d == callCountWhenUnchanged %d", initialCallCount, callCountWhenUnchanged);
    ck_assert_msg(callCountWhenUnchanged < callCountAfterNextMenuItem, "callCountWhenUnchanged %d < callCountAfterNextMenuItem %d", callCountWhenUnchanged, callCountAfterNextMenuItem);
}
END_TEST

START_TEST(Run_LanguageChanged_CallToRenderer)
{
    // Arrange
    MenuController_Init();
    DisplayManagerMock_SetState(DM_STATE_RUN);
    ResourceManagerMock_GrantResource();
    GCLMock_CurrentLanguage(0);
    MenuController_Run();
    MenuController_Run();
    int initialCallCount = MenuRendererMock_DrawCallCount();

    // Act
    GCLMock_CurrentLanguage(1);
    MenuController_Run();
    int callCountAfterLanguageChange = MenuRendererMock_DrawCallCount();

    // Assert
    ck_assert_msg(initialCallCount < callCountAfterLanguageChange, "callCountWhenUnchanged %d < callCountAfterLanguageChange %d", initialCallCount, callCountAfterLanguageChange);

}
END_TEST

START_TEST(Run_VehSpdNotZero_NavigationDisabled)
{
    // Arrange
    MenuController_Init();
    DisplayManagerMock_SetState(DM_STATE_RUN);
    ResourceManagerMock_GrantResource();
    MenuController_Run();
    GCLMock_VehicleSpeed(16);
    MenuController_Run(); // Twice to make sure we transition into main screen mode
    MenuController_Run();
    int initialCallCount = MenuRendererMock_DrawCallCount();

    // Act
    GCLMock_MenuButtonDown(1);
    MenuController_Run();
    int callCountAfter = MenuRendererMock_DrawCallCount();
    
    // Assert
    ck_assert_msg(initialCallCount == callCountAfter, "initialCallCount %d == callCountAfter %d", initialCallCount, callCountAfter);
}
END_TEST

START_TEST(Run_VehSpdZeroCancelPressed_TopMenuCancelSignalWritten)
{
    // Arrange
    MenuController_Init();
    DisplayManagerMock_SetState(DM_STATE_RUN);
    ResourceManagerMock_GrantResource();
    MenuController_Run();
    MenuController_Run();
    int initialCallCount = GCLMock_TopMenuCancelCallCount();
    
    // Act
    for (int i = 0; i < 30; ++i)
    {
        // This loop is run to make sure that we have tried Cancel button at Top menu position in case StartupMenu has been set to a MenuItem
        // in a submenu.
        GCLMock_MenuButtonClear(1);
        MenuController_Run();
    }
    int callCountAfter = GCLMock_TopMenuCancelCallCount();
    
    // Assert
    ck_assert_msg(initialCallCount <= callCountAfter, "initialCallCount %d == callCountAfter %d", initialCallCount, callCountAfter);
}
END_TEST

START_TEST(Run_VehSpdNotZeroCancelPressed_TopMenuCancelSignalWritten)
{
    // Arrange
    MenuController_Init();
    DisplayManagerMock_SetState(DM_STATE_RUN);
    ResourceManagerMock_GrantResource();
    GCLMock_VehicleSpeed(16);
    MenuController_Run();
    MenuController_Run();
    MenuController_Run();
    MenuController_Run();
    int initialCallCount = GCLMock_TopMenuCancelCallCount();

    // Act
    GCLMock_MenuButtonClear(1);
    MenuController_Run();

    int callCountAfter = GCLMock_TopMenuCancelCallCount();
    
    // Assert
    ck_assert_msg((initialCallCount + 1) == callCountAfter, "expectedCallCount %d == callCountAfter %d", (initialCallCount + 1), callCountAfter);
}
END_TEST

void setup()
{
    GCLMock_Reset();
    DisplayManagerMock_Reset();
    MenuRendererMock_Reset();
    ResourceManagerMock_Reset();
}

void teardown()
{

}

int main(void)
 {
	Suite *s = suite_create("MenuManager TestSuite");
    TCase*  tc_Init = tcase_create("Initial Tests");
    TCase*  tc_NoResource = tcase_create("Functional Tests");

    tcase_add_checked_fixture(tc_Init, setup, teardown);
    tcase_add_test(tc_Init, Startup_MenuControllerInactive);

    tcase_add_checked_fixture(tc_NoResource, setup, teardown);
    tcase_add_test(tc_NoResource, Run_ResourceDenied_NoCallToRenderer);
    tcase_add_test(tc_NoResource, Run_ResourceGranted_CallToRenderer);
    tcase_add_test(tc_NoResource, Run_AfterFirstDraw_CallsRefreshOnly);
    tcase_add_test(tc_NoResource, Run_EnterSubMenuItem_CallToRenderer);
    tcase_add_test(tc_NoResource, Run_LanguageChanged_CallToRenderer);
    tcase_add_test(tc_NoResource, Run_VehSpdNotZero_NavigationDisabled);
    tcase_add_test(tc_NoResource, Run_VehSpdNotZeroCancelPressed_TopMenuCancelSignalWritten);
    tcase_add_test(tc_NoResource, Run_VehSpdZeroCancelPressed_TopMenuCancelSignalWritten);
    
    suite_add_tcase(s, tc_Init);
    suite_add_tcase(s, tc_NoResource);
    SRunner *sr = srunner_create(s);

    srunner_run_all(sr, CK_NORMAL);
    int number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);
    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;

 }

void AssertMenuDrawCallCount(int expected)
{
     int drawCallCount = MenuRendererMock_DrawCallCount();
     ck_assert_msg(drawCallCount == expected, "Expected %d draw calls to menu renderer, actual: %d", expected, drawCallCount);
}

void AssertMenuRefreshCallCount(int expected)
{
     int refreshCallCount = MenuRendererMock_RefreshCallCount();
     ck_assert_msg(refreshCallCount == expected, "Expected %d refresh calls to menu renderer, actual: %d", expected, refreshCallCount);
 }