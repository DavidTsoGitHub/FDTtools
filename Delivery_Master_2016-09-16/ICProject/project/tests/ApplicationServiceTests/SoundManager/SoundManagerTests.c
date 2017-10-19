#include "check.h"
#include <stdio.h>

#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1

#include "SoundManager.h"
#include "BSPMock.h"

START_TEST(RequestActiveBuzzerSound_NoActiveBuzzerSound_ActiveBuzzer)
{
	//Arrange
	const BSP_BuzzerSound expectedSound = BSP_SOUND_ON;
	
	//Act
	SoundManager_RequestActiveBuzzerSound(expectedSound);
	
	//Assert
	BSP_BuzzerSound result = BSPMock_GetCurrentActiveBuzzerSound();
	ck_assert(expectedSound == result);
}
END_TEST

START_TEST(RequestActiveBuzzerSound_BuzzerAlreadyPlaying_ActiveOtherBuzzerSound)
{
	//Arrange
	const BSP_BuzzerSound expectedSound = BSP_SOUND_1HZ;
	const BSP_BuzzerSound initialSound = BSP_SOUND_ON;
	SoundManager_RequestActiveBuzzerSound(initialSound);
	
	//Act
	BSP_BuzzerSound initialSoundResult = BSPMock_GetCurrentActiveBuzzerSound();
	SoundManager_RequestActiveBuzzerSound(expectedSound);
	
	//Assert
	BSP_BuzzerSound result = BSPMock_GetCurrentActiveBuzzerSound();
	ck_assert(initialSound == initialSoundResult);
	ck_assert(expectedSound == result);
}
END_TEST

START_TEST(RequestSound_NoActiveSound_PlaysSound)
{
	//Arrange
	const BSP_Sound expectedSound = BSP_SOUND_TICK;

	//Act
	SoundManager_RequestSound(expectedSound);

	//Assert
    BSP_Sound result = BSPMock_GetCurrentSound();
	ck_assert(expectedSound == result);
}
END_TEST

START_TEST(RequestSound_SoundWithHigherPriorityPlaying_NoChange)
{
	//Arrange
    SoundManager_RequestSound(BSP_SOUND_WARN3);
    
	//Act
	SoundManager_RequestSound(BSP_SOUND_TICK);

	//Assert
    BSP_Sound result = BSPMock_GetCurrentSound();
	ck_assert(BSP_SOUND_WARN3 == result);
}
END_TEST

START_TEST(RequestSound_SoundWithHigherPriorityFinished_PlaysSound)
{
	//Arrange
    SoundManager_RequestSound(BSP_SOUND_WARN3);
    BSPMock_SetSoundGeneratorStatus(BSP_SOUND_GEN_INACTIVE);
    
	//Act
	SoundManager_RequestSound(BSP_SOUND_TICK);

	//Assert
    BSP_Sound result = BSPMock_GetCurrentSound();
	ck_assert(BSP_SOUND_TICK == result);
}
END_TEST

START_TEST(RequestSound_SoundWithLowerPriorityPlaying_StopSoundAndPlayNewSound)
{
	//Arrange
    SoundManager_RequestSound(BSP_SOUND_TICK);
    
	//Act
	SoundManager_RequestSound(BSP_SOUND_WARN3);

	//Assert
	ck_assert(BSP_SOUND_WARN3 == BSPMock_GetCurrentSound());
    ck_assert(1 == BSPMock_GetStopSoundCalls());
}
END_TEST

START_TEST(StopSoundWhenActiveBuzzerPlaying_CallsStopSoundOnBSP)
{
	//Arrange
    SoundManager_RequestActiveBuzzerSound(BSP_SOUND_1HZ);
    
	//Act
	SoundManager_StopActiveBuzzerSound();

	//Assert
    ck_assert(1 == BSPMock_GetStopActiveSoundCalls());
}
END_TEST


void setup(void)
{
    SoundManager_Init();
    BSPMock_Init();    
}

void teardown(void)
{
}

Suite* createSuite(void)
{
    Suite* s;
    TCase* tc;

    s = suite_create("SoundManager_Suite");

    /* Core test case */
    tc = tcase_create("SoundManager_Tests");
	tcase_add_checked_fixture(tc, setup, teardown);
    tcase_add_test(tc, RequestSound_NoActiveSound_PlaysSound);
    tcase_add_test(tc, RequestSound_SoundWithHigherPriorityPlaying_NoChange);
    tcase_add_test(tc, RequestSound_SoundWithHigherPriorityFinished_PlaysSound);
    tcase_add_test(tc, RequestSound_SoundWithLowerPriorityPlaying_StopSoundAndPlayNewSound);
	tcase_add_test(tc, RequestActiveBuzzerSound_NoActiveBuzzerSound_ActiveBuzzer);
	tcase_add_test(tc, RequestActiveBuzzerSound_BuzzerAlreadyPlaying_ActiveOtherBuzzerSound);
    tcase_add_test(tc, StopSoundWhenActiveBuzzerPlaying_CallsStopSoundOnBSP);
    
	
    
    suite_add_tcase(s, tc);
	
    return s;
}

int main(void)
{
	int number_failed;
    Suite *s;
    SRunner *sr;

    s = createSuite();
    sr = srunner_create(s);

    srunner_run_all(sr, CK_NORMAL);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);
    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;

}
