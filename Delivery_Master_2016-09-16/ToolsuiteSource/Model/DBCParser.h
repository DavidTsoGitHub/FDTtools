#ifndef DBCUTILS_H
#define DBCUTILS_H

#include "CANMessage.h"
#include "MessageAttributes.h"

#include <QString>
#include <QStringList>
#include <QList>
#include <QMap>

class DBCParser
{
public:
    explicit DBCParser(const QString& contents, MessageAttributeMapping* attributeMapping = NULL);
    ~DBCParser();
    void PrintDBC(void) const;

    const QStringList* GetModuleNames(void);
    const QStringList* GetAttributeNames(void);
    const QList<CANMessage*>* GetMessages(void);

private:
    const QString& m_dbcContents;
    QStringList* m_moduleNames;
    QStringList* m_attributeNames;
    QList<CANMessage*>* m_messages;
    QMap<qint32, QMap<QString, QString>*>* m_attributesByCANID;
    MessageAttributeMapping* m_attributeMapping;
    QMap<QString, QString>* m_defaultAttributeValues;
    QMap<qint32, QMap<QString, qint32>*>* m_signalTimeoutValuesByCANID;
    QMap<QString, quint32>* m_sendTypeBySignalName;

    qint32 getQInt32Attribute(qint32 canID, QString attributeName);
    qint32 getDefaultQInt32AttributeValue(QString attributeName);

    QString getStringAttribute(qint32 canID, QString attributeName);
    QString getDefaultStringAttributeValue(QString attributeName);

    bool signalHasTimeout(qint32 canId, const QString& signalName);
    qint32 getSignalTimeoutValue(qint32 canId, const QString& signalName);

    void parseModuleNames(void);
    void parseAttributeNames(void);
    void parseMessages(void);
    void parseAttributeValues(void);
    void parseDefaultAttributeValues(void);
    void parseSignalTimeoutValues(void);
    void parseSignalSendTypes(void);
};

#endif // DBCUTILS_H

