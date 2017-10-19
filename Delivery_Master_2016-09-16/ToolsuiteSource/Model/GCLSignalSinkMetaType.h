#ifndef GCLSIGNALSINKMETATYPE_H
#define GCLSIGNALSINKMETATYPE_H

#include <QMetaType>

class GCLSignalSink;

class GCLSignalSinkMetaType
{
public:
    GCLSignalSinkMetaType();
    explicit GCLSignalSinkMetaType(GCLSignalSink* signalSink);
    ~GCLSignalSinkMetaType(void){}

    GCLSignalSink* GetSignalSink(void) const;
    void SetSignalSink(GCLSignalSink* signalSink);

private:
    GCLSignalSink* m_signalSink;
};

Q_DECLARE_METATYPE(GCLSignalSinkMetaType)

#endif // GCLSIGNALSINKMETATYPE_H
