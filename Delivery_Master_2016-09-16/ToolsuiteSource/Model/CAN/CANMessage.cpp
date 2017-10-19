#include "CANMessage.h"

CANMessage::CANMessage() :
    CANMessageSendType(Undefined)
{}

CANMessage::CANMessage(qint32 canId, QString name, qint8 dlc, QString producer) :
    CANID(canId),
    Name(name),
    DLC(dlc),
    Producer(producer),
    Timeout(0),
    CANMessageSendType(Undefined)
{}

CANMessage::~CANMessage()
{}

const QList<const CANSignal*>& CANMessage::Signals() const
{
    return m_signals;
}

void CANMessage::AddSignal(CANSignal* signal)
{
    m_signals.append(signal);
    //AppendChild(signal);
}
