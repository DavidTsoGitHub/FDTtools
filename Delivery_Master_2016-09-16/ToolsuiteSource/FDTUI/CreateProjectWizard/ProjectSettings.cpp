#include "ProjectSettings.h"

#include <QString>
#include <QDir>

QString ProjectSettings::GetProjectFilePath()
{
    return ProjectRoot.absoluteFilePath(ProjectName.append(".fdt"));
}
