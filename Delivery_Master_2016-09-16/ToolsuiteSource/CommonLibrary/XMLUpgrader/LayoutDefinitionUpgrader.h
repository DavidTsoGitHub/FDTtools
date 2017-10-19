#ifndef LAYOUTDEFINITIONUPGRADER_H
#define LAYOUTDEFINITIONUPGRADER_H

#include "XMLUpgraderBase.h"
#include "VersionNumber.h"

#include <QPair>
#include <QMap>
#include <QString>

class LayoutDefinitionUpgrader : public XMLUpgraderBase
{
public:
    explicit LayoutDefinitionUpgrader(QString filePath, VersionNumber fromVersion, VersionNumber toVersion);

    typedef bool (LayoutDefinitionUpgrader::*VersionStepFunction)();

private:
    QMap<VersionNumber,QPair<VersionStepFunction,QString> > m_upgradeSteps;
    //Upgrade steps


    // XMLUpgraderBase interface
protected:
    virtual void populateUpgradeSteps(void);
    bool runStep(VersionNumber version);
    const QString generateVersionInfo(VersionNumber version) const;
};

#endif // LAYOUTDEFINITIONUPGRADER_H
