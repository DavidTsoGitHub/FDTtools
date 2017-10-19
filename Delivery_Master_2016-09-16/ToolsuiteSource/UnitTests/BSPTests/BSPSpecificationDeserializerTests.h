#ifndef BSPSPECIFICATIONDESERIALIZERTESTS_H
#define BSPSPECIFICATIONDESERIALIZERTESTS_H

#include "autotest.h"

class BSPSpecificationDeserializerTests : public QObject
{
    Q_OBJECT

public:

private Q_SLOTS:
    // Happy path
    void Deserialize_NoData_ReturnsTrueAndEmptySpecification(void);
    void Deserialize_Display_ReturnsTrueAndDisplay(void);
    void Deserialize_SingleEnum_ReturnsTrueAndOneEnum(void);
    void Deserialize_SingleFunction_ReturnsTrueAndOneFunction(void);

    // Error cases
    void Deserialize_EmptyXML_ReturnsFalse(void);
    void Deserialize_UnknownRootElement_ReturnsFalseAndErrorMessage(void);
};

DECLARE_TEST(BSPSpecificationDeserializerTests)

#endif // BSPSPECIFICATIONDESERIALIZERTESTS_H
