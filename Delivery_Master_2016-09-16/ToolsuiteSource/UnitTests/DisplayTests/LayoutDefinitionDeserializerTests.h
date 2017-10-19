#ifndef LAYOUTDEFINITIONDESERIALIZERTESTS_H
#define LAYOUTDEFINITIONDESERIALIZERTESTS_H

#include "autotest.h"

class LayoutDefinitionDeserializerTests : public QObject
{
    Q_OBJECT
public:

private Q_SLOTS:

    void Deserialize_EmptyXML_ReturnsFalseAndSetsErrorMessage(void);
    void Deserialize_SingleDisplayAreaXml_ReturnsTrueAndSetsDisplayParameters(void);
    void Deserialize_DisplayAreaWithChild_ReturnsTrueAndSetsChildParameters(void);

};


DECLARE_TEST(LayoutDefinitionDeserializerTests)

#endif // LAYOUTDEFINITIONDESERIALIZERTESTS_H
