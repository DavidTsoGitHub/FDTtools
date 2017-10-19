#include "GCL_LocalizationManager.h"

// Signals to be read from CUT
static uint8 m_SelectedLanguageValue = 0;
static uint8 m_SelectedLanguageValueStatus = 0;
static uint8 m_SelectedLanguageReadCount = 0;

// Signals to be written by CUT
static uint8 m_CurrentLanguageValue = 0;
static uint8 m_CurrentLanguageWriteCount = 0;
const LabelMappings* m_LanguageMappings = NULL;
static uint8 m_LanguageMappingsWriteCount = 0;

void GCLStub_Reset()
{
    m_CurrentLanguageValue = 0;
    m_CurrentLanguageWriteCount = 0;
    
    m_SelectedLanguageValue = 0;
    m_SelectedLanguageValueStatus = 0;
	m_SelectedLanguageReadCount = 0;
    
    m_LanguageMappings = NULL;
    m_LanguageMappingsWriteCount = 0;
}

// SelectedLanguage
void GCLStub_SetSelectedLanguage(uint8 value, uint8 status)
{
	m_SelectedLanguageValue = value;
    m_SelectedLanguageValueStatus = status;
}

void GCL_Read_LocalizationManager_SelectedLanguageValue(uint8* value, uint8* status)
{
	++m_SelectedLanguageReadCount;
	*value = m_SelectedLanguageValue;
	*status = m_SelectedLanguageValueStatus;
}

uint8 GCLStub_GetSelectedLanguageReadCount()
{
	return m_SelectedLanguageReadCount;
}

// CurrentLanguage
uint8 GCLStub_GetCurrentLanguageValue()
{
	return m_CurrentLanguageValue;
}

void GCL_Write_LocalizationManager_CurrentLanguageValue(uint8 value)
{
	++m_CurrentLanguageWriteCount;
	m_CurrentLanguageValue = value;
}

uint8 GCLStub_GetCurrentLanguageWriteCount()
{
	return m_CurrentLanguageWriteCount;
}

// Languages
const LabelMappings* GCLStub_GetCurrentLanguageMappings()
{
	return m_LanguageMappings;	
}

void GCL_Write_LocalizationManager_Languages(const LabelMappings* languages)
{
    ++m_LanguageMappingsWriteCount;
	m_LanguageMappings = languages;
}

uint8 GCLStub_GetLanguageMappingsWriteCount()
{
    return m_LanguageMappingsWriteCount;
}

