#ifndef IGCLSIGNALSOURCE
#define IGCLSIGNALSOURCE

#include <QList>
#include <QString>

class GCLSignalSource;

class IGCLSignalSource
{
public:
    virtual const QString GetComponentName(void) const = 0;
    virtual const QList<GCLSignalSource*>& GetSignalSources() const = 0;
    virtual GCLSignalSource* GetSignalSource(QString signalName) const = 0;
};

#endif // IGCLSIGNALSOURCE

