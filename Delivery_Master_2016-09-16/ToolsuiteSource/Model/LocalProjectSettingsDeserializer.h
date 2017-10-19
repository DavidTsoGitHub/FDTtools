#ifndef LOCALPROJECTSETTINGSDESERIALIZER_H
#define LOCALPROJECTSETTINGSDESERIALIZER_H

#include "DeserializerBase.h"

class Project;
class QString;

class LocalProjectSettingsDeserializer : public DeserializerBase
{
public:

    explicit LocalProjectSettingsDeserializer(Project& project);
    ~LocalProjectSettingsDeserializer(void);

    // DeserializerBase interface
public:
    DeserializerRespons Deserialize(const QString& xml);

private:
    Project& m_project;
};

#endif // LOCALPROJECTSETTINGSDESERIALIZER_H
