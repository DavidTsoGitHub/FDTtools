#ifndef GCLSIGNALSOURCEMETATYPE_H
#define GCLSIGNALSOURCEMETATYPE_H

#include <QMetaType>

class GCLSignalSource;

class GCLSignalSourceMetaType
{
public:
    GCLSignalSourceMetaType();
    explicit GCLSignalSourceMetaType(GCLSignalSource* signalSource);
    ~GCLSignalSourceMetaType(void){}

    GCLSignalSource* GetSignalSource(void) const;
    void SetSignalSource(GCLSignalSource* signalSource);

private:
    GCLSignalSource* m_signalSource;
};

Q_DECLARE_METATYPE(GCLSignalSourceMetaType)

#endif // GCLSIGNALSOURCEMETATYPE_H
