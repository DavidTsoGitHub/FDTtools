#ifndef DBCPARSERTESTS
#define DBCPARSERTESTS

#include "autotest.h"

class DBCParserTests : public QObject
{
    Q_OBJECT

public:


private Q_SLOTS:

    void Parse_NoModuleDirectiveInData_EmptyModuleList();
    void Parse_NoModulesInModuleDirective_EmptyModuleList();
    void Parse_ModulesInData_NonEmptyModuleList();
    void Parse_NoAttributesInData_EmptyAttributeList();
    void Parse_AttributesInData_NonEmptyAttributeList();
    void Parse_NoMessagesInData_EmptyMessageList();
    void Parse_SingleMessageInData_SingleMessageInList();
    void Parse_SingleMessageWithMultipleSignalsInData_SingleMessageWithMultipleSignalsInList();
    void Parse_MultipleMessagesWithMultipleSignalsInData_MultipleMessagesWithMultipleSignalsInList();
    void Parse_SingleMessageWithMultipleConsumersInData_SingleMessageWithMultipleConsumersInList();
    void Parse_SingleMessageWithAssociatedAttributesInData_SingleMessageWithAttributesInList();
    void Parse_SingleMessageWithUnmappedSendType_SingleMessageWithUnsupportedSendType();
    void Parse_SingleMessageWithoutAttributesInData_SingleMessageWithDefaultAttributes();
};

DECLARE_TEST(DBCParserTests)


#endif // DBCPARSERTESTS

