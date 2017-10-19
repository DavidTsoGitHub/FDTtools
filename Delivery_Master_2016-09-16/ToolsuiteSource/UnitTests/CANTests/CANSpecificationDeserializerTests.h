#ifndef CANSPECIFICATIONDESERIALIZERTESTS_H
#define CANSPECIFICATIONDESERIALIZERTESTS_H

#include "autotest.h"

class CANSpecificationDeserializerTests : public QObject
{
    Q_OBJECT

public:

private Q_SLOTS:
    // Happy path
    void Deserialize_NoData_ReturnsTrueAndEmptySpecification(void);
    void Deserialize_SingleMessageNoSignals_ReturnsTrueAndMessageData(void);
    void Deserialize_SingleMessageOneSignal_ReturnsTrueAndSignalData(void);
    void Deserialize_MultipleMessages_ReturnsTrueAndAllMessages(void);


    // Errors
    void Deserialize_EmptyXML_ReturnsFalseAndSetsError(void);

};

DECLARE_TEST(CANSpecificationDeserializerTests)

#endif // CANSPECIFICATIONDESERIALIZERTESTS_H
