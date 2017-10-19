#ifndef BSPMAPPINGTESTS_H
#define BSPMAPPINGTESTS_H

#include "autotest.h"

class BSPMappingTests : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void GetIndexFromSignalName_KnownSignal_ReturnsIndex(void);
    void GetIndexFromSignalName_UnknownSignal_ReturnsUnknownIndex(void);
};

DECLARE_TEST(BSPMappingTests)

#endif // BSPMAPPINGTESTS_H
