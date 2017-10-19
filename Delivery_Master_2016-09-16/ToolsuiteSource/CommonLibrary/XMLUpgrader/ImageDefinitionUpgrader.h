#ifndef IMAGEDEFINITIONUPGRADER_H
#define IMAGEDEFINITIONUPGRADER_H

#include "XMLUpgraderBase.h"
#include "VersionNumber.h"

#include <QFileInfo>
#include <QObject>
#include <QDomDocument>
#include <QPair>
#include <QMap>
#include <QString>
#include <QStringList>

class ImageDefinitionUpgrader : public XMLUpgraderBase
{
public:
    explicit ImageDefinitionUpgrader(QString filePath, VersionNumber fromVersion, VersionNumber toVersion);

    typedef bool (ImageDefinitionUpgrader::*VersionStepFunction)();

private:
    QMap<VersionNumber,QPair<VersionStepFunction,QString> > m_upgradeSteps;
    //Upgrade steps


    // XMLUpgraderBase interface
protected:
    virtual void populateUpgradeSteps(void);
    bool runStep(VersionNumber version);
    const QString generateVersionInfo(VersionNumber version) const;
};

#endif // IMAGEDEFINITIONUPGRADER_H
