#ifndef SIGNALMAPPINGMETATYPE_H
#define SIGNALMAPPINGMETATYPE_H

#include <QMetaType>

class SignalMapping;

class SignalMappingMetaType
{
public:
    SignalMappingMetaType();
    explicit SignalMappingMetaType(SignalMapping* signalMapping);
    ~SignalMappingMetaType(void){}

    SignalMapping* GetSignalMapping(void) const;
    void SetSignalMapping(SignalMapping* signalMapping);

private:
    SignalMapping* m_signalMapping;
};

Q_DECLARE_METATYPE(SignalMappingMetaType)

#endif // SIGNALMAPPINGMETATYPE_H
