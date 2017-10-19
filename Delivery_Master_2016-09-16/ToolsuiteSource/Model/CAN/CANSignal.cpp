#include "CANSignal.h"

CANSignal::CANSignal() :
    Scaling(0),
    Offset(0),
    Min(0),
    Max(0),
    Consumers()
{

}

CANSignal::CANSignal(QString name,
                     bool timeoutNotification,
                     bool isEventTrigger,
                     qint8 startBit,
                     qint8 bitLength,
                     double scaling,
                     double offset,
                     double min,
                     double max,
                     QString uom,
                     QStringList consumers) :
    Name(name),
    TimeoutNotification(timeoutNotification),
    IsEventTrigger(isEventTrigger),
    StartBit(startBit),
    BitLength(bitLength),
    Scaling(scaling),
    Offset(offset),
    Min(min),
    Max(max),
    UOM(uom),
    Consumers(consumers)

{
}

CANSignal::~CANSignal()
{
}
