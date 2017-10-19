#include <stdlib.h>
#include <check.h>
#include "../inc/LocalizationManager.h"
#include "LocalizationManager_cfg.h"
#include "CAL_Stub.h"
#include "GCL_Stub.h"
#include "GCL_LocalizationManager.h"

// The CUT has defined "extern const LabelMappings languages"
LabelMappings languages =
{
    .Count = 3,
    .Mappings = 
    {
        { .LabelID = 11, .Value = 0 },
        { .LabelID = 22, .Value = 1 },
        { .LabelID = 33, .Value = 2 }
    }
};

START_TEST (FunctionBlockInitialized_SubmitsLabelMappingsToGCL)
{
    // Act
    LocalizationManager_Init();
    const LabelMappings* currentLanguageMappings = GCLStub_GetCurrentLanguageMappings();

    // Assert
    ck_assert_msg(currentLanguageMappings != NULL, "LocalizationManager should write LanguageMappings to GCL on initialization, mapping was NULL!");
    ck_assert_msg(currentLanguageMappings == &languages, "LocalizationManager did not write the expected language mappings to GCL");
}
END_TEST

START_TEST (FunctionBlockInitialized_CurrentLanguageReadFromCALAndWrittenToGCL)
{
    // Arrange
    const uint8 languageValueInCAL = 3;
    CALStub_SetLanguageValue(languageValueInCAL);    
    
    // Act
    LocalizationManager_Init();

    // Assert
    uint8 languageValueWrittenToGCL = GCLStub_GetCurrentLanguageValue();
    ck_assert_msg(languageValueWrittenToGCL == languageValueInCAL, "LocalizationManager should write the language value in CAL to GCL upon initialization! GCL: %d, CAL: %d", languageValueWrittenToGCL, languageValueInCAL);
}
END_TEST

START_TEST (NonZeroInitialLanguage_SelectedLanguageNotIndicated_GCLAndCALAreNotOverwritten)
{
    // Arrange
    GCLStub_SetSelectedLanguage(0, GCL_SIGNAL_OK);
    const uint8 languageValueInCAL = 3;
    CALStub_SetLanguageValue(languageValueInCAL);
    LocalizationManager_Init();
    
    // Act
    LocalizationManager_Run();
    
    // Assert
    uint8 languageWrittenToGCL = GCLStub_GetCurrentLanguageValue();
    uint8 languageWrittenToCAL = CALStub_GetLanguageValue(); 
    ck_assert_msg(languageValueInCAL == languageWrittenToGCL, "LocalizationManager should not write selected language to GCL unless indicated!");
    ck_assert_msg(languageValueInCAL == languageWrittenToCAL, "LocalizationManager should not write selected language to CAL unless indicated!");
}
END_TEST

START_TEST (NewLanguageSelectionIndicatedByGCL_SelectedLanguageWrittenToGCLAndCAL)
{
    // Arrange
    const uint8 selectedLanguageValue = 3;
    const uint8 initialLanguageValue = 0;
    CALStub_SetLanguageValue(initialLanguageValue);
    LocalizationManager_Init();

    // Act
    GCLStub_SetSelectedLanguage(selectedLanguageValue, GCL_SIGNAL_OK);
    LocalizationManager_SelectedLanguageValue_Indication(); 
    LocalizationManager_Run();

    // Assert
    uint8 languageWrittenToGCL = GCLStub_GetCurrentLanguageValue();
    uint8 languageWrittenToCAL = CALStub_GetLanguageValue();
    
    ck_assert_msg(selectedLanguageValue == languageWrittenToGCL, "LocalizationManager should write the selected language value to GCL! current: %d, expected: %d", languageWrittenToGCL, selectedLanguageValue);
    ck_assert_msg(selectedLanguageValue == languageWrittenToCAL, "LocalizationManager should write the selected language value to CAL! current: %d, expected: %d", languageWrittenToCAL, selectedLanguageValue);
}
END_TEST

START_TEST (LanguageUpdatedInCAL_SelectedLanguageWrittenToGCLOnly)
{
    // Arrange
    const uint8 newLanguageInCAL = 3;
    CALStub_SetLanguageValue(0);
    LocalizationManager_Init();

    // Act
    CALStub_SetLanguageValue(newLanguageInCAL);
    LocalizationManager_Run();

    // Assert
    uint8 languageWrittenToGCL = GCLStub_GetCurrentLanguageValue();
    uint8 calWriteCount = CALStub_GetCalWriteCounter();
    
    ck_assert_msg(newLanguageInCAL == languageWrittenToGCL, "LocalizationManager should write the new CAL language value to GCL! current: %d, expected: %d", languageWrittenToGCL, newLanguageInCAL);
    ck_assert_msg(calWriteCount == 0, "LocalizationManager should not write the new CAL language back to CAL!");
}
END_TEST

START_TEST (NewLanguageSelectionIndicatedByGCLButSameAsPrevious_NoWritesToGCLNorCAL)
{
   // Arrange
    const uint8 selectedLanguageValue = 3;
    const uint8 initialLanguageValue = 3;
    CALStub_SetLanguageValue(initialLanguageValue);
    LocalizationManager_Init();
    
    // Act
    GCLStub_SetSelectedLanguage(selectedLanguageValue, GCL_SIGNAL_OK);
    LocalizationManager_SelectedLanguageValue_Indication(); 
    LocalizationManager_Run();

    // Assert
    uint8 writesToCAL = CALStub_GetCalWriteCounter();
    uint8 writesToGCL = GCLStub_GetCurrentLanguageWriteCount();
    
    ck_assert_msg(writesToCAL == 0, "LocalizationManager should not write the selected language value to CAL when same as previous!");
    // we assert for 1 here because there will always be one write to GCL in the Init routine
    ck_assert_msg(writesToGCL == 1, "LocalizationManager should not write the selected language value to GCL when same as previous!");
}
END_TEST

void setup(void)
{
    GCLStub_Reset();
    CALStub_Reset();
}

void teardown(void)
{}

Suite* testSuite_create()
{
    Suite* suite = suite_create("LocalizationManagerSuite");

    TCase* testCase = tcase_create("LocalizationManagerTests");
    tcase_add_checked_fixture(testCase, setup, teardown);

    /* --------------------------------------------------------------------- */

    tcase_add_test(testCase, FunctionBlockInitialized_SubmitsLabelMappingsToGCL);
    tcase_add_test(testCase, FunctionBlockInitialized_CurrentLanguageReadFromCALAndWrittenToGCL);
    tcase_add_test(testCase, NonZeroInitialLanguage_SelectedLanguageNotIndicated_GCLAndCALAreNotOverwritten);
    tcase_add_test(testCase, NewLanguageSelectionIndicatedByGCL_SelectedLanguageWrittenToGCLAndCAL);
    tcase_add_test(testCase, LanguageUpdatedInCAL_SelectedLanguageWrittenToGCLOnly);
    tcase_add_test(testCase, NewLanguageSelectionIndicatedByGCLButSameAsPrevious_NoWritesToGCLNorCAL);

    /* --------------------------------------------------------------------- */

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


