#ifndef ANIMATIONDEFINITIONUPGRADER_H
#define ANIMATIONDEFINITIONUPGRADER_H

#include "XMLUpgraderBase.h"
#include "VersionNumber.h"

#include <QPair>
#include <QMap>
#include <QString>

class AnimationDefinitionUpgrader : public XMLUpgraderBase
{
public:
    explicit AnimationDefinitionUpgrader(QString filePath, VersionNumber fromVersion, VersionNumber toVersion);

    typedef bool (AnimationDefinitionUpgrader::*VersionStepFunction)();

private:
    QMap<VersionNumber,QPair<VersionStepFunction,QString> > m_upgradeSteps;
    //Upgrade steps


    // XMLUpgraderBase interface
protected:
    virtual void populateUpgradeSteps(void);
    bool runStep(VersionNumber version);
    const QString generateVersionInfo(VersionNumber version) const;
};

#endif // ANIMATIONDEFINITIONUPGRADER_H
