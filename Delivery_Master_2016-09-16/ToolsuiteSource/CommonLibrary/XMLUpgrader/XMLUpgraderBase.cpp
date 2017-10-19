#include "XMLUpgraderBase.h"
#include "VersionNumber.h"
#include <QFile>
#include <QDomDocument>
#include <QString>
#include <QStringList>
#include <QTextStream>

XMLUpgraderBase::XMLUpgraderBase(QString xmlStartElement, QString filePath, VersionNumber fromVersion, VersionNumber toVersion) :
    m_xmlStartElement(xmlStartElement),
    m_filePath(filePath),
    m_fromVersion(fromVersion),
    m_toVersion(toVersion)
{
    m_upgradeLog.append("Initializing upgrader for " + m_xmlStartElement);

    //Add each version to the versionlist and a corresponding m_upgradeSteps to the subclasses.
    //m_versionList.append(VersionNumber(1,1,0));
    //m_versionList.append(VersionNumber(1,2,1));
    //m_versionList.append(VersionNumber(2,0,0));
}

const QString XMLUpgraderBase::GetUpgradeInfo(const VersionNumber& fromVersion, const VersionNumber& toVersion) const
{
    QStringList versionsInfo;
    foreach (VersionNumber version, m_versionList)
    {
        if((version > fromVersion) && (version <= toVersion))
        {
            versionsInfo.append(generateVersionInfo(version));
        }
    }
    return versionsInfo.join("\n\n<hr />\n\n");
}

const QStringList XMLUpgraderBase::GetUpgradeLog() const
{
    return m_upgradeLog;
}

bool XMLUpgraderBase::PerformUpgrade()
{
    bool upgradeStatus = true;

    QFile file(m_filePath);
    if (!file.open(QFile::ReadOnly))
    {
        m_upgradeLog.append(QString("Failed to open file for read: %1").arg(m_filePath));
        return false;
    }

    xmlDoc.setContent(&file);
    file.close();

    foreach (VersionNumber version, m_versionList)
    {
        if((version > m_fromVersion) && (version <= m_toVersion))
        {
            if (!runStep(version))
            {
                upgradeStatus = false;
                m_upgradeLog.append(QString("Upgrade failed on step %1").arg(version.GetVersion()));
                break;
            }
        }
    }

    if(upgradeStatus)
    {
        updateFileVersion();
        if (!file.open(QFile::WriteOnly))
        {
            m_upgradeLog.append(QString("Failed to open file for write: %1").arg(m_filePath));
            return false;
        }


        QTextStream stream(&file);
        xmlDoc.save(stream, 4, QDomNode::EncodingFromDocument);
        file.close();
        m_upgradeLog.append(QString("Saving upgrades"));
    }
    else
    {
        m_upgradeLog.append(QString("Not writing to file!"));
    }


    return upgradeStatus;
}

const VersionNumber XMLUpgraderBase::GetCurrentVersion() const
{
    return m_fromVersion;
}

const QString XMLUpgraderBase::GetFilePath() const
{
    return m_filePath;
}

const QString XMLUpgraderBase::UpdaterName() const
{
    return m_xmlStartElement;
}

void XMLUpgraderBase::updateFileVersion()
{
    QDomNodeList elements = xmlDoc.elementsByTagName(m_xmlStartElement);
    elements.at(0).toElement().setAttribute("AIVersion", m_toVersion.GetVersion());
}

bool XMLUpgraderBase::runStep(VersionNumber /*stepIndex*/)
{
    //override this function
    return false;
}

const QString XMLUpgraderBase::generateVersionInfo(VersionNumber /*startIndex*/) const
{
    //override this function
    return QString();
}
