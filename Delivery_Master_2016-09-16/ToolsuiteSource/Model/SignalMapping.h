#ifndef SIGNALMAPPING
#define SIGNALMAPPING

#include <QString>
#include <QList>

typedef struct
{
    QString Name;
    QString Signal;
} SimpleGCLEndpointDescription;

class SignalMapping
{
public:
    SignalMapping();
    ~SignalMapping();

    void AddSource(QString componentName, QString signalName);
    SimpleGCLEndpointDescription& GetSource();

    void AddSink(QString componentName, QString signalName);
    const QList<SimpleGCLEndpointDescription>& GetSinks();

private:
    SimpleGCLEndpointDescription m_source;
    QList<SimpleGCLEndpointDescription> m_sinks;
};

#endif // SIGNALMAPPING

