#ifndef PROJECTDESERIALIZERTESTS
#define PROJECTDESERIALIZERTESTS

#include "autotest.h"

class ProjectDeserializerTests : public QObject
{
    Q_OBJECT

public:

private Q_SLOTS:

    void Deserialize_ValidXML_ReturnsTrueAndSetsProjectParameters();
    void Deserialize_EmptyXML_ReturnsFalseAndSetsErrorText();
    void Deserialize_NoProjectElement_ReturnsFalseAndSetsErrorText();

};

DECLARE_TEST(ProjectDeserializerTests)



#endif // PROJECTDESERIALIZERTESTS

