#ifndef MESSAGEATTRIBUTES_H
#define MESSAGEATTRIBUTES_H

#include <QString>
#include <QMap>
#include "CANMessage.h"

typedef struct
{
    QString CycleTimeAttributeName;
    QString DelayTimeAttributeName;
    QString InitialDelayTimeAttributeName;
    QString SendTypeAttributeName;
    QMap<QString, CANMessage::SendType> SendTypeMapping;
} MessageAttributeMapping;

#endif // MESSAGEATTRIBUTES_H
