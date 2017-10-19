#ifndef XMLUPGRADERBASE_H
#define XMLUPGRADERBASE_H

#include <QPair>
#include <QMap>
#include <QList>
#include <QStringList>
#include <QDomDocument>
#include <QString>
#include "VersionNumber.h"

class XMLUpgraderBase
{
public:
    explicit XMLUpgraderBase(QString xmlStartElement, QString filePath, VersionNumber fromVersion, VersionNumber toVersion);
    virtual const QString GetUpgradeInfo(const VersionNumber& fromVersion, const VersionNumber& toVersion) const;
    virtual const QStringList GetUpgradeLog(void) const;
    virtual bool PerformUpgrade(void);
    virtual const VersionNumber GetCurrentVersion(void) const;
    virtual const QString GetFilePath(void) const;
    virtual const QString UpdaterName(void) const;

private:
    QList<VersionNumber> m_versionList;
    QString m_xmlStartElement;
    QString m_filePath;
    VersionNumber m_fromVersion;
    VersionNumber m_toVersion;

protected:
    virtual void populateUpgradeSteps(void) = 0;
    virtual void updateFileVersion(void);
    virtual bool runStep(VersionNumber stepIndex);
    virtual const QString generateVersionInfo(VersionNumber version) const;

    QStringList m_upgradeLog;
    QDomDocument xmlDoc;

};

#endif // XMLUPGRADERBASE_H
