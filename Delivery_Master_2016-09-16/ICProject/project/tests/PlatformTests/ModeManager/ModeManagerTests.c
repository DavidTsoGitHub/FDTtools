#include "check.h"
#include <stdio.h>

#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1

#include "ModeManager.h"
#include "GCLMock.h"
#include "BSPMock.h"
#include "CANDriverMock.h"

void ArrangeModeManager(BSP_ECUModes mode);
void AssertBSPStates(BSP_ECUModes expectedBSPMode, BSP_CanTrcvMdType expectedCANMode);
void AssertPowerMode(uint8 expectedPowerMode);

START_TEST(InitialState_BSPLowPowerCANNormalPowerModeLow)
{
    // Arrange
    ModeManager_Init();

    // Act
    ModeManager_Run();

    // Assert
    AssertBSPStates(BSP_MODE_LOWPOWER, BSP_CANTRCV_NORMAL);
    AssertPowerMode(POWER_LOW);
}
END_TEST

START_TEST(LowPowerMode_IgnitionOn_BSPNormalModeCANNormalPowerIgnitionOn)
{
    // Arrange
    ArrangeModeManager(BSP_MODE_LOWPOWER);
            
    // Act
    BSPMock_TurnOnIgnition();
    ModeManager_Run();
    
    // Assert
    AssertBSPStates(BSP_MODE_NORMAL, BSP_CANTRCV_NORMAL);
}
END_TEST

START_TEST(NormalMode_IgnitionOff_BSPLowPowerCANNormal)
{
    // Arrange
    ArrangeModeManager(BSP_MODE_NORMAL);
        
    // Act
    BSPMock_TurnOffIgnition();
    ModeManager_Run();
    
    // Assert
    AssertBSPStates(BSP_MODE_LOWPOWER, BSP_CANTRCV_NORMAL);
}
END_TEST

START_TEST(LowPower_CANTimeout_BSPSleepCANStandby)
{
    // Arrange
    ArrangeModeManager(BSP_MODE_LOWPOWER);
    
    // Act
    ModeManager_CAN_Timeout();
    ModeManager_Run();
    
    // Assert
    AssertBSPStates(BSP_MODE_SLEEP, BSP_CANTRCV_STANDBY);
}
END_TEST

START_TEST(Sleep_CANWakeUp_BSPLowPowerCANNormal)
{
    // Arrange
    ArrangeModeManager(BSP_MODE_SLEEP);

    // Act
    APPL_WakeupNotify(BSP_WAKEUP_CAN_COMM);
    ModeManager_Run();

    // Assert
    AssertBSPStates(BSP_MODE_LOWPOWER, BSP_CANTRCV_NORMAL);
}
END_TEST

START_TEST(Sleep_IgnitionWakeUp_BSPNormalCANNormal)
{
    // Arrange
    ArrangeModeManager(BSP_MODE_SLEEP);

    // Act
    APPL_WakeupNotify(BSP_WAKEUP_IGN);
    ModeManager_Run();

    // Assert
    AssertBSPStates(BSP_MODE_NORMAL, BSP_CANTRCV_NORMAL);
}
END_TEST

START_TEST(NormalMode_CANTimeout_NoChange)
{
    // Arrange
    ArrangeModeManager(BSP_MODE_NORMAL);
    uint32 modeChangeCallCountBefore = BSPMock_GetRequestModeChange_CallCount();
    uint32 canTrcvModeCallCountBefore = BSPMock_GetSetCanTrcvMode_CallCount();
    
    // Act
    ModeManager_CAN_Timeout();
    ModeManager_Run();

    // Assert
    AssertBSPStates(BSP_MODE_NORMAL, BSP_CANTRCV_NORMAL);
    uint32 modeChangeCallCountAfter = BSPMock_GetRequestModeChange_CallCount();
    uint32 canTrcvModeCallCountAfter = BSPMock_GetSetCanTrcvMode_CallCount();
    ck_assert_msg(modeChangeCallCountAfter == modeChangeCallCountBefore, "CAN Timeout during normal mode should not trigger RequestModeChange on BSP");
    ck_assert_msg(canTrcvModeCallCountAfter == canTrcvModeCallCountBefore, "CAN Timeout during normal mode should not trigger CAN Transceiver mode change on BSP");
}
END_TEST

START_TEST(NormalMode_IgnitionWakeUp_NoChange)
{
    // Arrange
    ArrangeModeManager(BSP_MODE_NORMAL);
    uint32 modeChangeCallCountBefore = BSPMock_GetRequestModeChange_CallCount();
    uint32 canTrcvModeCallCountBefore = BSPMock_GetSetCanTrcvMode_CallCount();

    // Act
    APPL_WakeupNotify(BSP_WAKEUP_IGN);
    ModeManager_Run();

    // Assert
    AssertBSPStates(BSP_MODE_NORMAL, BSP_CANTRCV_NORMAL);
    uint32 modeChangeCallCountAfter = BSPMock_GetRequestModeChange_CallCount();
    uint32 canTrcvModeCallCountAfter = BSPMock_GetSetCanTrcvMode_CallCount();
    ck_assert_msg(modeChangeCallCountAfter == modeChangeCallCountBefore, "Ignition wakeup during normal mode should not trigger RequestModeChange on BSP");
    ck_assert_msg(canTrcvModeCallCountAfter == canTrcvModeCallCountBefore, "Ignition wakeup during normal mode should not trigger CAN Transceiver mode change on BSP");
}
END_TEST

START_TEST(NormalModeCANTimedOut_CANWakeUp_NoChange)
{
    // Arrange
    ArrangeModeManager(BSP_MODE_NORMAL);
    ModeManager_CAN_Timeout();
    ModeManager_Run();
    uint32 modeChangeCallCountBefore = BSPMock_GetRequestModeChange_CallCount();
    uint32 canTrcvModeCallCountBefore = BSPMock_GetSetCanTrcvMode_CallCount();

    // Act
    APPL_WakeupNotify(BSP_WAKEUP_CAN_COMM);
    ModeManager_Run();

    // Assert
    AssertBSPStates(BSP_MODE_NORMAL, BSP_CANTRCV_NORMAL);
    uint32 modeChangeCallCountAfter = BSPMock_GetRequestModeChange_CallCount();
    uint32 canTrcvModeCallCountAfter = BSPMock_GetSetCanTrcvMode_CallCount();
    ck_assert_msg(modeChangeCallCountAfter == modeChangeCallCountBefore, "CAN wake up during normal mode should not trigger RequestModeChange on BSP");
    ck_assert_msg(canTrcvModeCallCountAfter == canTrcvModeCallCountBefore, "CAN wake up during normal mode should not trigger CAN Transceiver mode change on BSP");
}
END_TEST

START_TEST(LowPower_IgnitionWakeUp_NoChange)
{
    // Arrange
    ArrangeModeManager(BSP_MODE_LOWPOWER);
    uint32 modeChangeCallCountBefore = BSPMock_GetRequestModeChange_CallCount();
    uint32 canTrcvModeCallCountBefore = BSPMock_GetSetCanTrcvMode_CallCount();

    // Act
    APPL_WakeupNotify(BSP_WAKEUP_IGN);
    ModeManager_Run();

    // Assert
    AssertBSPStates(BSP_MODE_LOWPOWER, BSP_CANTRCV_NORMAL);
    uint32 modeChangeCallCountAfter = BSPMock_GetRequestModeChange_CallCount();
    uint32 canTrcvModeCallCountAfter = BSPMock_GetSetCanTrcvMode_CallCount();
    ck_assert_msg(modeChangeCallCountAfter == modeChangeCallCountBefore, "Ignition wake up during low power mode should not trigger RequestModeChange on BSP");
    ck_assert_msg(canTrcvModeCallCountAfter == canTrcvModeCallCountBefore, "Ignition wake up during low power mode should not trigger CAN Transceiver mode change on BSP");
}
END_TEST

START_TEST(LowPower_CANWakeUp_NoChange)
{
    // Arrange
    ArrangeModeManager(BSP_MODE_LOWPOWER);
    uint32 modeChangeCallCountBefore = BSPMock_GetRequestModeChange_CallCount();
    uint32 canTrcvModeCallCountBefore = BSPMock_GetSetCanTrcvMode_CallCount();

    // Act
    APPL_WakeupNotify(BSP_WAKEUP_CAN_COMM);
    ModeManager_Run();

    // Assert
    AssertBSPStates(BSP_MODE_LOWPOWER, BSP_CANTRCV_NORMAL);
    uint32 modeChangeCallCountAfter = BSPMock_GetRequestModeChange_CallCount();
    uint32 canTrcvModeCallCountAfter = BSPMock_GetSetCanTrcvMode_CallCount();
    ck_assert_msg(modeChangeCallCountAfter == modeChangeCallCountBefore, "CAN wake up during low power mode should not trigger RequestModeChange on BSP");
    ck_assert_msg(canTrcvModeCallCountAfter == canTrcvModeCallCountBefore, "CAN wake up during low power mode should not trigger CAN Transceiver mode change on BSP");
}
END_TEST

START_TEST(NormalMode_IgnitionWakeUpThenSleep_NoWakeUp)
{
    // Arrange
    ArrangeModeManager(BSP_MODE_NORMAL);
    APPL_WakeupNotify(BSP_WAKEUP_IGN);
    ModeManager_Run();  // Normal mode
    BSPMock_TurnOffIgnition();
    ModeManager_Run();  // Low power mode
    ModeManager_CAN_Timeout();
    ModeManager_Run();  // Sleep mode
        
    // Act
    ModeManager_Run();
    
    // Assert
    AssertBSPStates(BSP_MODE_SLEEP, BSP_CANTRCV_STANDBY);
}
END_TEST

START_TEST(LowPower_IgnitionWakeUpThenSleep_NoWakeUp)
{
    // Arrange
    ArrangeModeManager(BSP_MODE_LOWPOWER);
    APPL_WakeupNotify(BSP_WAKEUP_IGN);
    ModeManager_Run();
    ModeManager_CAN_Timeout();
    ModeManager_Run();  // Sleep mode
        
    // Act
    ModeManager_Run();
    
    // Assert
    AssertBSPStates(BSP_MODE_SLEEP, BSP_CANTRCV_STANDBY);
}
END_TEST

START_TEST(NormalMode_CANWakeUpThenSleep_NoWakeUp)
{
    // Arrange
    ArrangeModeManager(BSP_MODE_NORMAL);
    APPL_WakeupNotify(BSP_WAKEUP_CAN_COMM);
    ModeManager_Run();  // Normal mode
    BSPMock_TurnOffIgnition();
    ModeManager_Run();  // Low power mode
    ModeManager_CAN_Timeout();
    ModeManager_Run();  // Sleep mode
        
    // Act
    ModeManager_Run();
    
    // Assert
    AssertBSPStates(BSP_MODE_SLEEP, BSP_CANTRCV_STANDBY);
}
END_TEST

START_TEST(LowPower_CANWakeUpThenSleep_NoWakeUp)
{
    // Arrange
    ArrangeModeManager(BSP_MODE_LOWPOWER);
    APPL_WakeupNotify(BSP_WAKEUP_CAN_COMM);
    ModeManager_Run();
    ModeManager_CAN_Timeout();
    ModeManager_Run();  // Sleep mode
        
    // Act
    ModeManager_Run();
    
    // Assert
    AssertBSPStates(BSP_MODE_SLEEP, BSP_CANTRCV_STANDBY);
}
END_TEST

void setup()
{
    CANDriverMock_Reset();
    GCLMock_Reset();
    BSPMock_Reset();
}

void teardown()
{

}

int main(void)
 {
	Suite *s = suite_create("MoM_TestSuite");
    TCase*  tc_NoResource = tcase_create("AllTests");

    tcase_add_checked_fixture(tc_NoResource, setup, teardown);
    
    // Until the tests have been adapted for the new ResourceManager and MenuManager setup
    tcase_add_test(tc_NoResource, InitialState_BSPLowPowerCANNormalPowerModeLow);
    tcase_add_test(tc_NoResource, LowPowerMode_IgnitionOn_BSPNormalModeCANNormalPowerIgnitionOn);
    tcase_add_test(tc_NoResource, NormalMode_IgnitionOff_BSPLowPowerCANNormal);
    tcase_add_test(tc_NoResource, LowPower_CANTimeout_BSPSleepCANStandby);
    tcase_add_test(tc_NoResource, Sleep_CANWakeUp_BSPLowPowerCANNormal);
    tcase_add_test(tc_NoResource, Sleep_IgnitionWakeUp_BSPNormalCANNormal);
    tcase_add_test(tc_NoResource, NormalMode_CANTimeout_NoChange);
    tcase_add_test(tc_NoResource, NormalMode_IgnitionWakeUp_NoChange);
    tcase_add_test(tc_NoResource, NormalModeCANTimedOut_CANWakeUp_NoChange);
    tcase_add_test(tc_NoResource, LowPower_IgnitionWakeUp_NoChange);
    tcase_add_test(tc_NoResource, LowPower_CANWakeUp_NoChange);
    tcase_add_test(tc_NoResource, NormalMode_IgnitionWakeUpThenSleep_NoWakeUp);
    tcase_add_test(tc_NoResource, LowPower_IgnitionWakeUpThenSleep_NoWakeUp);
    tcase_add_test(tc_NoResource, NormalMode_CANWakeUpThenSleep_NoWakeUp);
    tcase_add_test(tc_NoResource, LowPower_CANWakeUpThenSleep_NoWakeUp);
       

    suite_add_tcase(s, tc_NoResource);
    SRunner *sr = srunner_create(s);

    srunner_run_all(sr, CK_NORMAL);
    int number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);
    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;

 }

 
 void ArrangeModeManager(BSP_ECUModes mode)
 {
     switch (mode)
     {
         case BSP_MODE_LOWPOWER:
         ModeManager_Init();
         ModeManager_Run();
         break;
         case BSP_MODE_NORMAL:
         ModeManager_Init();
         ModeManager_Run();
         BSPMock_TurnOnIgnition();
         ModeManager_Run();
         break;
         case BSP_MODE_SLEEP:
         ModeManager_Init();
         ModeManager_Run();
         ModeManager_CAN_Timeout();
         ModeManager_Run();
         break;
     }
 }
 
 void AssertBSPStates(BSP_ECUModes expectedBSPMode, BSP_CanTrcvMdType expectedCANMode)
 {
    BSP_ECUModes actualBSPMode = BSPMock_GetCurrentMode();
    BSP_CanTrcvMdType actualCANMode = BSPMock_GetCANTransceiverMode();
        
    ck_assert_msg(actualBSPMode == expectedBSPMode, "BSP Mode not as expected! (expected:%d, actual:%d)", expectedBSPMode, actualBSPMode);
    ck_assert_msg(actualCANMode == expectedCANMode, "CAN Mode not as expected! (expected:%d, actual:%d)", expectedCANMode, actualCANMode);
 }
 
 void AssertPowerMode(uint8 expectedPowerMode)
 {
     uint8 actualPowerMode = GCLMock_Get_PowerMode();
     ck_assert_msg(actualPowerMode == expectedPowerMode, "Power Mode not as expected! (expected:%d, actual:%d)", expectedPowerMode, actualPowerMode);
 }
 