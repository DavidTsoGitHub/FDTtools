#ifndef PROJECTSETTINGS
#define PROJECTSETTINGS

class QString;
class QDir;

class ProjectSettings
{

public:

    QString ProjectName;
    QDir ProjectRoot;
    QFileInfo BSPSpecificationPath;
    QFileInfo CANSpecificationPath;
    QString CANECUName;
    bool HasCANSpecification;

    QString GetProjectFilePath(void);
};

#endif // PROJECTSETTINGS

