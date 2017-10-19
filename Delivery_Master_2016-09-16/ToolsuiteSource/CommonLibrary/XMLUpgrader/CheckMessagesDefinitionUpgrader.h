#ifndef CHECKMESSAGESDEFINITIONUPGRADER_H
#define CHECKMESSAGESDEFINITIONUPGRADER_H


#include "XMLUpgraderBase.h"
#include "VersionNumber.h"

#include <QPair>
#include <QMap>
#include <QString>

class CheckMessagesDefinitionUpgrader : public XMLUpgraderBase
{
public:
    explicit CheckMessagesDefinitionUpgrader(QString filePath, VersionNumber fromVersion, VersionNumber toVersion);

    typedef bool (CheckMessagesDefinitionUpgrader::*VersionStepFunction)();

private:
    QMap<VersionNumber,QPair<VersionStepFunction,QString> > m_upgradeSteps;
    //Upgrade steps


    // XMLUpgraderBase interface
protected:
    virtual void populateUpgradeSteps(void);
    bool runStep(VersionNumber version);
    const QString generateVersionInfo(VersionNumber version) const;
};

#endif // CHECKMESSAGESDEFINITIONUPGRADER_H
