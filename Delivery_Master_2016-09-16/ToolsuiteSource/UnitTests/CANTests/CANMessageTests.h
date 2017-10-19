#ifndef CANMESSAGETESTS_H
#define CANMESSAGETESTS_H

#include "autotest.h"

class CANMessageTests : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void SendTypeFromString_ValidStrings_ValidEnumValues(void);
    void SendTypeString_ValidEnumValues_ValidStrings(void);
};

DECLARE_TEST(CANMessageTests)

#endif // CANMESSAGETESTS_H
