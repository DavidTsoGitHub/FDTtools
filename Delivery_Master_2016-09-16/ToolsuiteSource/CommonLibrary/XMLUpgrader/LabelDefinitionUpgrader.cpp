#include "LabelDefinitionUpgrader.h"

#include <QStringList>
#include <QDomDocument>
#include <QTextStream>
#include <QDebug>

LabelDefinitionUpgrader::LabelDefinitionUpgrader(QString filePath, VersionNumber fromVersion, VersionNumber toVersion) :
    XMLUpgraderBase("LabelDefinition", filePath, fromVersion, toVersion)
{
    populateUpgradeSteps();
}

void LabelDefinitionUpgrader::populateUpgradeSteps()
{
    /* Example to add function steps */
    //m_upgradeSteps.insert(VersionNumber(1,1),qMakePair(&LabelDefinitionUpgrader::upgradeStep_1_1, upgradeStep_1_1_Info()));
    //m_upgradeSteps.insert(VersionNumber(1,2),qMakePair(&LabelDefinitionUpgrader::upgradeStep_1_2, upgradeStep_1_2_Info()));
}

bool LabelDefinitionUpgrader::runStep(VersionNumber version)
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

const QString LabelDefinitionUpgrader::generateVersionInfo(VersionNumber version) const
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
 * UPGRADE STEPS BELOW
 */

/**
 *  These are some example Steps
 */

/*** Upgrade function ***/
/*bool LabelDefinitionUpgrader::upgradeStep_1_1()
{
    m_upgradeLog.append(QString("Running upgrade step 1.1"));

    QDomNodeList elements = xmlDoc.elementsByTagName("Translation");

    for(int i=0; i < elements.count(); ++i)
    {
        if(elements.at(i).isElement())
        {
            elements.at(i).toElement().setAttribute("Size", 10);
        }
    }
    return true;
}

bool LabelDefinitionUpgrader::upgradeStep_1_2()
{
    m_upgradeLog.append(QString("Running upgrade step 1.2"));

    QDomNodeList elements = xmlDoc.elementsByTagName("Translation");

    for(int i=0; i < elements.count(); ++i)
    {
        if(elements.at(i).isElement())
        {
            elements.at(i).toElement().setAttribute("Width", 10);
        }
    }
    return true;
}*/

/*** Supporting information function ***/
/*QString LabelDefinitionUpgrader::upgradeStep_1_1_Info()
{
    QStringList* info = new QStringList();

    info->append("Added attribute Size=\"10\" to translation tag");

    return info->join("\n");
}*/

/*** Supporting information function, html coded ***/
/*QString LabelDefinitionUpgrader::upgradeStep_1_1_Info()
{
    QStringList* info = new QStringList();

    info->append("<h1>Added Size Attribute to translation tag.</h1>");
    info->append("<p>Each Translation tag gets a new attribute named Size.");
    info->append("This attribute will get set to 10 as default.<p>");
    info->append("<p><b>If anu other value is needed, please change after upgrade!</b><p>");

    return info->join("\n");
}

QString LabelDefinitionUpgrader::upgradeStep_1_2_Info()
{
    QStringList* info = new QStringList();

    info->append("<h1>Added Width Attribute to translation tag.</h1>");
    info->append("<p>Each Translation tag gets a new attribute named Width.");
    info->append("This attribute will get set to 10 as default.<p>");
    info->append("<p><b>If anu other value is needed, please change after upgrade!</b><p>");

    return info->join("\n");
}*/
