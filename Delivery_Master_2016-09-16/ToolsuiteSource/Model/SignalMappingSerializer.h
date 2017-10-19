#ifndef SIGNALMAPPINGSERIALIZER_H
#define SIGNALMAPPINGSERIALIZER_H

#include "SerializerBase.h"

class Project;

class QString;

class SignalMappingSerializer : public SerializerBase
{
public:
    explicit SignalMappingSerializer(Project& project);
    ~SignalMappingSerializer();

    QString Serialize();

private:
    Project& m_project;
};

#endif // SIGNALMAPPINGSERIALIZER_H
