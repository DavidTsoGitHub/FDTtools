#ifndef PROJECTDEFINITIONUPGRADER_H
#define PROJECTDEFINITIONUPGRADER_H

#include "XMLUpgraderBase.h"
#include "VersionNumber.h"

#include <QPair>
#include <QMap>
#include <QString>

class ProjectDefinitionUpgrader : public XMLUpgraderBase
{
public:
    explicit ProjectDefinitionUpgrader(QString filePath, VersionNumber fromVersion, VersionNumber toVersion);

    typedef bool (ProjectDefinitionUpgrader::*VersionStepFunction)();

private:
    QMap<VersionNumber,QPair<VersionStepFunction,QString> > m_upgradeSteps;

    //Upgrade steps


    // XMLUpgraderBase interface
protected:
    virtual void populateUpgradeSteps(void);
    bool runStep(VersionNumber version);
    const QString generateVersionInfo(VersionNumber version) const;
};

#endif // PROJECTDEFINITIONUPGRADER_H
