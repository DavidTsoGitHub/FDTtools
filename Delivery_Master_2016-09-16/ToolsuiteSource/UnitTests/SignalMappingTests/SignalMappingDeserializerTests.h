#ifndef PROJECTTESTS
#define PROJECTTESTS

#include "autotest.h"

class SignalMappingDeserializerTests : public QObject
{
    Q_OBJECT

public:

private:

private Q_SLOTS:
    void Deserialize_EmptyXML_ReturnsFalseAndNoMappings(void);
    void Deserialize_ValidXMLWithoutMappings_ReturnsTrueAndNoMappings(void);
    void Deserialize_ValidXMLPointToPointMapping_ReturnsTrueAndOneMapping(void);
    void Deserialize_ValidXMLPointToMultiPointMapping_ReturnsTrueAndOneMapping(void);
};

DECLARE_TEST(SignalMappingDeserializerTests)

#endif // PROJECTTESTS

