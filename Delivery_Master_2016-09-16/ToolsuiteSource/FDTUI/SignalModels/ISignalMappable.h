#ifndef ISIGNALMAPPABLE_H
#define ISIGNALMAPPABLE_H

class GCLSignalSink;
class GCLSignalSource;

#include <QList>
#include <QString>

class ISignalMappable
{
public:
    virtual GCLSignalSink* FindSink(const QString& longName) const = 0;
    virtual QList<GCLSignalSink*> GetSinkList() const = 0;
    virtual QList<GCLSignalSource*> GetSourceList() const = 0;
    virtual GCLSignalSource* FindSource(const QString &longName) const = 0;
};

#endif // ISIGNALMAPPABLE_H
