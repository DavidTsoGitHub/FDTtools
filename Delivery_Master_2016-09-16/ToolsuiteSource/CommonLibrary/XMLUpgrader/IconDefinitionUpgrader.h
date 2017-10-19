#ifndef ICONDEFINITIONUPGRADER_H
#define ICONDEFINITIONUPGRADER_H

#include "XMLUpgraderBase.h"

#include <QPair>
#include <QMap>
#include <QString>

class IconDefinitionUpgrader : public XMLUpgraderBase
{
public:
    explicit IconDefinitionUpgrader(QString filePath, VersionNumber fromVersion, VersionNumber toVersion);

    typedef bool (IconDefinitionUpgrader::*VersionStepFunction)();

private:
    QMap<VersionNumber,QPair<VersionStepFunction,QString> > m_upgradeSteps;
    //Upgrade steps


    // XMLUpgraderBase interface
protected:
    virtual void populateUpgradeSteps(void);
    bool runStep(VersionNumber version);
    const QString generateVersionInfo(VersionNumber version) const;
};


#endif // ICONDEFINITIONUPGRADER_H
