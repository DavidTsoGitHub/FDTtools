#ifndef IGCLSIGNALSINK
#define IGCLSIGNALSINK

#include <QList>
#include <QString>

class GCLSignalSink;

class IGCLSignalSink
{
public:
    virtual const QList<GCLSignalSink*>& GetSignalSinks() const = 0;
    virtual GCLSignalSink* GetSignalSink(QString signalName) const = 0;
};


#endif // IGCLSIGNALSINK

