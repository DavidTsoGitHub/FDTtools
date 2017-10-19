#ifndef DIAGDEFINITIONUPGRADER_H
#define DIAGDEFINITIONUPGRADER_H

#include "XMLUpgraderBase.h"
#include "VersionNumber.h"

#include <QPair>
#include <QMap>
#include <QString>

class DiagDefinitionUpgrader : public XMLUpgraderBase
{
public:
    explicit DiagDefinitionUpgrader(QString filePath, VersionNumber fromVersion, VersionNumber toVersion);

    typedef bool (DiagDefinitionUpgrader::*VersionStepFunction)();

private:
    QMap<VersionNumber,QPair<VersionStepFunction,QString> > m_upgradeSteps;
    //Upgrade steps


    // XMLUpgraderBase interface
protected:
    virtual void populateUpgradeSteps(void);
    bool runStep(VersionNumber version);
    const QString generateVersionInfo(VersionNumber version) const;
};

#endif // DIAGDEFINITIONUPGRADER_H
