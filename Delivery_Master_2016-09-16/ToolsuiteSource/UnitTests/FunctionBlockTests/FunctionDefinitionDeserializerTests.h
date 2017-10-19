#ifndef FUNCTIONDEFINITIONDESERIALIZERTESTS
#define FUNCTIONDEFINITIONDESERIALIZERTESTS

#include "autotest.h"

class FunctionDefinitionDeserializerTests : public QObject
{
    Q_OBJECT

public:

private Q_SLOTS:
    void Deserialize_ValidXML_ReturnsTrueAndSetsFunctionParameters(void);
    void Deserialize_EmptyXML_ReturnsFalse(void);
    void Deserialize_NoFunctionDefinitionElement_ReturnsFalseAndSetsErrorText(void);

};

DECLARE_TEST(FunctionDefinitionDeserializerTests)

#endif // FUNCTIONDEFINITIONDESERIALIZERTESTS

