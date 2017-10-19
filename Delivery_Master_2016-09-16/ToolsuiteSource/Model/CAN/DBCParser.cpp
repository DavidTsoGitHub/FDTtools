#include <iostream>
#include <QRegularExpression>
#include "DBCParser.h"

#include <limits.h>

using namespace std;

DBCParser::DBCParser(const QString& dbcData, MessageAttributeMapping* attributeMapping) :
    m_dbcContents(dbcData),
    m_moduleNames(NULL),
    m_attributeNames(NULL),
    m_messages(NULL),
    m_attributesByCANID(NULL),
    m_attributeMapping(attributeMapping),
    m_defaultAttributeValues(NULL),
    m_signalTimeoutValuesByCANID(NULL)
{
}

DBCParser::~DBCParser()
{}

void DBCParser::PrintDBC() const
{
    cout << "DBC: " << endl << m_dbcContents.toStdString() << endl;
}

const QStringList* DBCParser::GetModuleNames()
{
    if (m_moduleNames != NULL) return m_moduleNames;

    parseModuleNames();

    return m_moduleNames;
}

const QStringList* DBCParser::GetAttributeNames()
{
    if (m_attributeNames != NULL) return m_attributeNames;

    parseAttributeNames();

    return m_attributeNames;
}

const QList<CANMessage*>* DBCParser::GetMessages()
{
    if (m_messages != NULL) return m_messages;

    parseMessages();

    return m_messages;
}

qint32 DBCParser::getDefaultQInt32AttributeValue(QString attributeName)
{
    if (attributeName.isNull() || attributeName.isEmpty())
    {
        return -1;
    }

    if (m_defaultAttributeValues == NULL)
    {
        parseDefaultAttributeValues();
    }

    QString stringValue = m_defaultAttributeValues->value(attributeName, "");
    bool ok;
    qint32 value = stringValue.toInt(&ok);

    return ok ? value : -1;
}

qint32 DBCParser::getQInt32Attribute(qint32 canID, QString attributeName)
{
    if (m_attributesByCANID == NULL)
    {
        parseAttributeValues();
    }

    if (!m_attributesByCANID->contains(canID))
    {
        return getDefaultQInt32AttributeValue(attributeName);
    }

    if (!m_attributesByCANID->value(canID)->contains(attributeName))
    {
        return getDefaultQInt32AttributeValue(attributeName);
    }

    QString stringValue = m_attributesByCANID->value(canID)->value(attributeName, "");
    bool ok;
    qint32 value = stringValue.toInt(&ok);

    return ok ? value : -1;
}

QString DBCParser::getDefaultStringAttributeValue(QString attributeName)
{
    if (attributeName.isNull() || attributeName.isEmpty())
    {
        return QString("");
    }

    if (m_defaultAttributeValues == NULL)
    {
        parseDefaultAttributeValues();
    }

    return m_defaultAttributeValues->value(attributeName, "");
}

bool DBCParser::signalHasTimeout(qint32 canId, const QString& signalName)
{
    if (m_signalTimeoutValuesByCANID == NULL)
    {
        parseSignalTimeoutValues();
    }

    return m_signalTimeoutValuesByCANID->keys().contains(canId) &&
           m_signalTimeoutValuesByCANID->value(canId)->contains(signalName);
}

qint32 DBCParser::getSignalTimeoutValue(qint32 canId, const QString& signalName)
{
    if (m_signalTimeoutValuesByCANID == NULL)
    {
        parseSignalTimeoutValues();
    }

    // We default to UINT_MAX if the signal is not found, so that the signal will not timeout (within a reasonable period)
    if (m_signalTimeoutValuesByCANID->contains(canId))
    {
        return m_signalTimeoutValuesByCANID->value(canId)->value(signalName, UINT_MAX);
    }

    return UINT_MAX;
}

QString DBCParser::getStringAttribute(qint32 canID, QString attributeName)
{
    if (m_attributesByCANID == NULL)
    {
        parseAttributeValues();
    }

    if (!m_attributesByCANID->contains(canID))
    {
        return getDefaultStringAttributeValue(attributeName);
    }

    if (!m_attributesByCANID->value(canID)->contains(attributeName))
    {
        return getDefaultStringAttributeValue(attributeName);
    }

    return m_attributesByCANID->value(canID)->value(attributeName, "");
}

void DBCParser::parseMessages()
{

    parseSignalSendTypes();

    m_messages = new QList<CANMessage*>();

    QRegularExpression messageRegex("BO_\\s+(\\d+)\\s+(\\S+):\\s+(\\d)\\s+(\\w+)\\n((?: SG_ .+(?:\\n))+)", QRegularExpression::MultilineOption);
    QRegularExpression signalRegex("SG_\\s+(\\w+)\\s+:\\s+(\\d+)\\|(\\d+)@\\d(?:\\+|-)\\s+\\((-?(?:\\d+)(?:\\.\\d+)?(?:E\\+\\d+)?),(-?(?:\\d+)(?:\\.\\d+)?(?:E\\+\\d+)?)\\)\\s+\\[(-?(?:\\d+)(?:\\.\\d+)?(?:E\\+\\d+)?)\\|(-?(?:\\d+)(?:\\.\\d+)?(?:E\\+\\d+)?)\\]\\s+\\\"(\\S*)\\\"\\s+(\\S+)", QRegularExpression::MultilineOption);

    QRegularExpressionMatchIterator messageMatches = messageRegex.globalMatch(m_dbcContents);

    while (messageMatches.hasNext())
    {
        QRegularExpressionMatch messageMatch = messageMatches.next();

        // Mask out 29-bits to fit the CAN ID extended mode
        qint32 canId = messageMatch.captured(1).toUInt() & 0x1FFFFFFF;
        QString name = messageMatch.captured(2);
        qint8 dlc = messageMatch.captured(3).toUInt();
        QString producer = messageMatch.captured(4);
        QString signalstring = messageMatch.captured(5);

        CANMessage* message = new CANMessage(canId, name, dlc, producer);

        if (m_attributeMapping != NULL)
        {
            message->CycleTime = getQInt32Attribute(canId, m_attributeMapping->CycleTimeAttributeName);
            message->Delay = getQInt32Attribute(canId, m_attributeMapping->DelayTimeAttributeName);
            message->InitialDelay = getQInt32Attribute(canId, m_attributeMapping->InitialDelayTimeAttributeName);

            if (m_attributeMapping->SendTypeMapping.count() > 0)
            {
                QString sendType = getStringAttribute(canId, m_attributeMapping->SendTypeAttributeName);
                message->CANMessageSendType = m_attributeMapping->SendTypeMapping.value(sendType, CANMessage::Unsupported);
            }
            else
            {
                message->CANMessageSendType = CANMessage::Undefined;
            }
        }

        QRegularExpressionMatchIterator signalMatches = signalRegex.globalMatch(signalstring);

        while (signalMatches.hasNext())
        {
            QRegularExpressionMatch signalMatch = signalMatches.next();
            QString name = signalMatch.captured(1);
            bool ok;
            bool eventTrigger = false;
            qint8 startBit = signalMatch.captured(2).toInt(&ok);
            if (!ok) continue;
            qint8 bitLength = signalMatch.captured(3).toInt(&ok);
            if (!ok) continue;
            double scaling = signalMatch.captured(4).toDouble(&ok);
            if (!ok) continue;
            double offset = signalMatch.captured(5).toDouble(&ok);
            if (!ok) continue;
            double min = signalMatch.captured(6).toDouble(&ok);
            if (!ok) continue;
            double max = signalMatch.captured(7).toDouble(&ok);
            if (!ok) continue;
            QString uom = signalMatch.captured(8);
            QString consumers = signalMatch.captured(9);

            QStringList consumerList = consumers.trimmed().split(QChar(','));
            bool timeoutNotification = signalHasTimeout(canId, name);

            if(m_sendTypeBySignalName->value(name) > 0)
            {
                eventTrigger = true;
            }

            CANSignal* canSignal = new CANSignal(name, timeoutNotification, eventTrigger, startBit, bitLength, scaling, offset, min, max, uom, consumerList);

            if (canSignal->TimeoutNotification)
            {
                message->Timeout = getSignalTimeoutValue(canId, name);
            }

            message->AddSignal(canSignal);
        }

        m_messages->append(message);
    }
}

void DBCParser::parseAttributeNames()
{
    m_attributeNames = new QStringList();

    QRegularExpression rx("^BA_DEF_\\s+(?:BO_|SG_)\\s+\\\"(\\S+)\\\"", QRegularExpression::MultilineOption);

    QRegularExpressionMatchIterator matches = rx.globalMatch(m_dbcContents);

    while (matches.hasNext())
    {
        QRegularExpressionMatch match = matches.next();
       m_attributeNames->append(match.captured(1));
    }
}

void DBCParser::parseModuleNames()
{
    m_moduleNames = new QStringList();

    QRegExp rx("BU_:\\s+([^\n]+)");

    if (rx.indexIn(m_dbcContents) != -1)
    {
        QString moduleLine = rx.cap(1);
        QStringList modules = moduleLine.split(QChar(' '));
        QStringList::iterator it = modules.begin();
        while (it!=modules.end())
        {
            m_moduleNames->append(*it);
            //std::cout << (*it).toStdString() << std::endl;
            ++it;
        }
    }
}

void DBCParser::parseAttributeValues()
{
    m_attributesByCANID = new QMap<qint32, QMap<QString, QString>* >();

    QRegularExpression rx("^BA_\\s+\\\"(.+)\\\"\\s+BO_\\s+(\\d+)\\s+(\\w+);$", QRegularExpression::MultilineOption);
    QRegularExpressionMatchIterator matches = rx.globalMatch(m_dbcContents);
    while (matches.hasNext())
    {
        QRegularExpressionMatch match = matches.next();
        qint32 canID = match.captured(2).toUInt() & 0x1FFFFFFF;
        QString attributeName = match.captured(1);
        QString attributeValue = match.captured(3);

        if (!m_attributesByCANID->contains(canID))
        {
            m_attributesByCANID->insert(canID, new QMap<QString, QString>());
        }

        m_attributesByCANID->value(canID)->insert(attributeName, attributeValue);
    }
}

void DBCParser::parseDefaultAttributeValues()
{
    m_defaultAttributeValues = new QMap<QString, QString>();

    QRegularExpression rx("^BA_DEF_DEF_\\s+\\\"(.+)\\\"\\s+\\\"?(\\w*)\\\"?;$", QRegularExpression::MultilineOption);
    QRegularExpressionMatchIterator matches = rx.globalMatch(m_dbcContents);
    while (matches.hasNext())
    {
        QRegularExpressionMatch match = matches.next();
        QString attributeName = match.captured(1);
        QString attributeValue = match.captured(2);

        m_defaultAttributeValues->insert(attributeName, attributeValue);
    }
}

void DBCParser::parseSignalTimeoutValues()
{
    m_signalTimeoutValuesByCANID = new QMap<qint32, QMap<QString, qint32>*>();

    QRegularExpression rx("^BA_REL_\\s+\\\"GenSigTimeoutTime\\\"\\s+BU_SG_REL_\\s+\\w+\\s+SG_\\s+(\\d+)\\s+(\\w+)\\s+(\\d+);$", QRegularExpression::MultilineOption);
    QRegularExpressionMatchIterator matches = rx.globalMatch(m_dbcContents);
    while (matches.hasNext())
    {
        QRegularExpressionMatch match = matches.next();
        qint32 canID = match.captured(1).toUInt() & 0x1FFFFFFF;
        QString signalName = match.captured(2);
        qint32 timeoutValue = match.captured(3).toInt();

        QString debugValue;
        debugValue.append(QString::number(canID));
        debugValue.append(" " + signalName);
        debugValue.append("=" + QString::number(timeoutValue));

        qDebug(debugValue.toStdString().c_str());

        if (!m_signalTimeoutValuesByCANID->contains(canID))
        {
            m_signalTimeoutValuesByCANID->insert(canID, new QMap<QString, qint32>());
        }

        m_signalTimeoutValuesByCANID->value(canID)->insert(signalName, timeoutValue);
    }
}

void DBCParser::parseSignalSendTypes()
{
    m_sendTypeBySignalName = new QMap<QString, quint32>();

    QRegularExpression rx("^BA_\\s+\\\"(.+)\\\"\\s+SG_\\s+(\\d+)\\s+(\\w+)\\s+(\\d+);$", QRegularExpression::MultilineOption);
    QRegularExpressionMatchIterator matches = rx.globalMatch(m_dbcContents);
    while (matches.hasNext())
    {
        QRegularExpressionMatch match = matches.next();
        QString signalName = match.captured(3);
        quint32 signalType = match.captured(4).toUInt();

        if (!m_sendTypeBySignalName->contains(signalName))
        {
            m_sendTypeBySignalName->insert(signalName, signalType);
        }
    }
}
