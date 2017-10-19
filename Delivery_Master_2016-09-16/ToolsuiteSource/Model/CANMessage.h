#ifndef CANMESSAGE_H
#define CANMESSAGE_H

#include "CANSignal.h"
#include <QList>
#include <QString>

class CANMessage
{
public:
    enum SendType
    {
        Undefined = 0,
        Cyclic,
        Event,
        CyclicAndEvent,
        Unsupported = 255
    };

    static QString SendTypeString(SendType type)
    {
        switch (type)
        {
        case Undefined: return QString("Undefined");
        case Cyclic: return QString("Cyclic");
        case Event: return QString("Event");
        case CyclicAndEvent: return QString("CyclicAndEvent");
        case Unsupported: return QString("Unsupported");
        default: return QString("Unknown");
        }
    }

    static SendType SendTypeFromString(QString type)
    {
        if (type.compare("cyclic", Qt::CaseInsensitive) == 0) return Cyclic;
        if (type.compare("event", Qt::CaseInsensitive) == 0) return Event;
        if (type.compare("cyclicandevent", Qt::CaseInsensitive) == 0) return CyclicAndEvent;
        if (type.compare("unsupported", Qt::CaseInsensitive) == 0) return Unsupported;
        return Undefined;
    }

    CANMessage();
    CANMessage(qint32 canID, QString name, qint8 dlc, QString producer);
    ~CANMessage();

     qint32 CANID;
     QString Name;
     qint8 DLC;
     QString Producer;
     qint32 CycleTime;
     qint32 Delay;
     qint32 InitialDelay;
     qint32 Timeout;
     CANMessage::SendType CANMessageSendType;

     const QList<const CANSignal*>& Signals() const;

     void AddSignal(CANSignal* signal);

private:
     QList<const CANSignal*> m_signals;
};

#endif // CANMESSAGE_H
