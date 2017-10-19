#ifndef LOCALPROJECTSETTINGSSERIALIZER_H
#define LOCALPROJECTSETTINGSSERIALIZER_H

#include "SerializerBase.h"

class Project;

class LocalProjectSettingsSerializer : public SerializerBase
{
public:
    explicit LocalProjectSettingsSerializer(Project& project);
    ~LocalProjectSettingsSerializer();

    // SerializerBase interface
public:
    QString Serialize();

private:
    Project& m_project;
};

#endif // LOCALPROJECTSETTINGSSERIALIZER_H
