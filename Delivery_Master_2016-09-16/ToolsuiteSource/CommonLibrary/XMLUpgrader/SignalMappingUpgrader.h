#ifndef SIGNALMAPPINGUPGRADER_H
#define SIGNALMAPPINGUPGRADER_H

#include "XMLUpgraderBase.h"
#include "VersionNumber.h"

#include <QPair>
#include <QMap>
#include <QString>

class SignalMappingUpgrader : public XMLUpgraderBase
{
public:
    explicit SignalMappingUpgrader(QString filePath, VersionNumber fromVersion, VersionNumber toVersion);

    typedef bool (SignalMappingUpgrader::*VersionStepFunction)();

private:
    QMap<VersionNumber,QPair<VersionStepFunction,QString> > m_upgradeSteps;
    //Upgrade steps


    // XMLUpgraderBase interface
protected:
    virtual void populateUpgradeSteps(void);
    bool runStep(VersionNumber version);
    const QString generateVersionInfo(VersionNumber version) const;
};

#endif // SIGNALMAPPINGUPGRADER_H
