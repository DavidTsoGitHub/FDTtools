#ifndef CANSIGNAL_H
#define CANSIGNAL_H

#include <QString>
#include <QStringList>
#include <QtGlobal>

class CANSignal
{
public:
    CANSignal();
    CANSignal(QString name, bool timeoutNotification, bool isEventTrigger, qint8 startBit, qint8 bitLength, double scaling, double offset, double min, double max, QString uom, QStringList consumers);
    ~CANSignal();

    QString Name;
    bool TimeoutNotification;
    bool IsEventTrigger;
    qint8 StartBit;
    qint8 BitLength;
    double Scaling;
    double Offset;
    double Min;
    double Max;
    QString UOM;
    QStringList Consumers;

};

#endif // CANSIGNAL_H
