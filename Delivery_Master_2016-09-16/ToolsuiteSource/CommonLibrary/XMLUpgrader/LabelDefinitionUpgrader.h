#ifndef LABELDEFINITIONUPGRADER_H
#define LABELDEFINITIONUPGRADER_H

#include "XMLUpgraderBase.h"
#include "VersionNumber.h"

#include <QFileInfo>
#include <QObject>
#include <QDomDocument>
#include <QPair>
#include <QMap>
#include <QString>
#include <QStringList>

class LabelDefinitionUpgrader : public XMLUpgraderBase
{
public:
    explicit LabelDefinitionUpgrader(QString filePath, VersionNumber fromVersion, VersionNumber toVersion);

    typedef bool (LabelDefinitionUpgrader::*VersionStepFunction)();

private:
    QMap<VersionNumber,QPair<VersionStepFunction,QString> > m_upgradeSteps;
    //Upgrade steps
    bool upgradeStep_1_1(void);
    bool upgradeStep_1_2(void);
    QString upgradeStep_1_1_Info(void);
    QString upgradeStep_1_2_Info(void);

    // XMLUpgraderBase interface
protected:
    virtual void populateUpgradeSteps(void);
    bool runStep(VersionNumber version);
    const QString generateVersionInfo(VersionNumber version) const;
};

#endif // LABELDEFINITIONUPGRADER_H
