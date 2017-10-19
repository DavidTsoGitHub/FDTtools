#ifndef GCL_STUB_H
#define GCL_STUB_H

#include "StandardTypes.h"

#define GCL_SIGNAL_OK         (0U)
#define GCL_SIGNAL_TIMEOUT    (1 << 0)
#define GCL_SIGNAL_ERROR      (1 << 1)

void GCLStub_Reset(void);

void GCLStub_SetSelectedLanguage(uint8 value, uint8 status);
uint8 GCLStub_GetSelectedLanguageReadCount(void);

uint8 GCLStub_GetCurrentLanguageValue(void);
uint8 GCLStub_GetCurrentLanguageWriteCount(void);

const LabelMappings* GCLStub_GetCurrentLanguageMappings(void);
uint8 GCLStub_GetLanguageMappingsWriteCount(void);

#endif //GCL_STUB_H
