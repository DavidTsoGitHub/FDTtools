#include "CheckMessagesDefinitionUpgrader.h"

#include <QStringList>
#include <QDomDocument>
#include <QTextStream>
#include <QDebug>

CheckMessagesDefinitionUpgrader::CheckMessagesDefinitionUpgrader(QString filePath, VersionNumber fromVersion, VersionNumber toVersion) :
    XMLUpgraderBase("CheckMessageDefinition", filePath, fromVersion, toVersion)
{
    populateUpgradeSteps();
}

void CheckMessagesDefinitionUpgrader::populateUpgradeSteps()
{
    /* Add function steps */

}

bool CheckMessagesDefinitionUpgrader::runStep(VersionNumber version)
{
    if(m_upgradeSteps.value(version, QPair<VersionStepFunction,QString>()) != QPair<VersionStepFunction,QString>())
    {
        return (*this.*m_upgradeSteps.value(version).first)();
    }
    else
    {
        return true;
    }
}

const QString CheckMessagesDefinitionUpgrader::generateVersionInfo(VersionNumber version) const
{
    if (m_upgradeSteps.contains(version))
    {
        return m_upgradeSteps.value(version).second;
    }
    else
    {
        return QString("No upgrades for version %1.").arg(version.GetVersion());
    }
}

/**
 * UPGRADE & UPGRADE INFORMATION STEPS BELOW
 */
