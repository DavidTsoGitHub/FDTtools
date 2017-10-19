#include "ProjectUpdaterDialog.h"
#include "ui_ProjectUpdaterDialog.h"

#include "Project.h"

#include <QDialog>
#include <QDir>
#include <QFileInfo>
#include <QListWidget>
#include <QCheckBox>
#include <QMessageBox>
#include <QLabel>
#include <QDomDocument>

#include "XMLUpgrader/ProjectDefinitionUpgrader.h"
#include "XMLUpgrader/LabelDefinitionUpgrader.h"
#include "XMLUpgrader/ImageDefinitionUpgrader.h"
#include "XMLUpgrader/IconDefinitionUpgrader.h"
#include "XMLUpgrader/MenuDefinitionUpgrader.h"
#include "XMLUpgrader/LayoutDefinitionUpgrader.h"
#include "XMLUpgrader/SegmentDefinitionUpgrader.h"
#include "XMLUpgrader/SignalMappingUpgrader.h"
#include "XMLUpgrader/CheckMessagesDefinitionUpgrader.h"
#include "XMLUpgrader/AnimationDefinitionUpgrader.h"
#include "XMLUpgrader/DiagDefinitionUpgrader.h"



ProjectUpdaterDialog::ProjectUpdaterDialog(const QFileInfo& projectFile, Project* project, QWidget *parent) :
    QDialog(parent),
    m_ui(new Ui::ProjectUpdaterDialog),
    m_project(project),
    m_projectFile(projectFile),
    m_baseDirPath(projectFile.absoluteDir()),
    m_toVersionNumber(VersionNumber(0,0,0)),
    m_updateRequired(false),
    m_upgradeStatus(UpgradeUnknown)
{

    m_ui->setupUi(this);

    connect(m_ui->buttonApply, &QPushButton::clicked, this, &ProjectUpdaterDialog::slot_applyUpdates);
    connect(m_ui->listWidget, &QListWidget::itemSelectionChanged, this, &ProjectUpdaterDialog::slot_selectionChanged);
    connect(m_ui->buttonCancel, &QPushButton::clicked, this, &ProjectUpdaterDialog::slot_cancel);

    QStringList toVersionString = QString(AI_VERSION).split(".");

    VersionNumber fromVersion = getCurrentFileVersion(m_baseDirPath.absoluteFilePath(m_project->LabelDefinitionPath));
    m_toVersionNumber = VersionNumber(toVersionString.first().toInt(),toVersionString.last().toInt(), 0);


    //Initialize list of all project xml files
    if(QFile(projectFile.absoluteFilePath()).exists())
    {
        XMLUpgraderBase* projectUpgrader = new ProjectDefinitionUpgrader(projectFile.absoluteFilePath(), fromVersion, m_toVersionNumber);
        m_upgraderList.append(projectUpgrader);
    }

    if(QFile(m_baseDirPath.absoluteFilePath(m_project->LabelDefinitionPath)).exists())
    {
        XMLUpgraderBase* labelUpgrader = new LabelDefinitionUpgrader(m_baseDirPath.absoluteFilePath(m_project->LabelDefinitionPath), fromVersion, m_toVersionNumber);
        m_upgraderList.append(labelUpgrader);
    }

    if(QFile(m_baseDirPath.absoluteFilePath(m_project->ImageDefinitionPath)).exists())
    {
        XMLUpgraderBase* imageUpgrader = new ImageDefinitionUpgrader(m_baseDirPath.absoluteFilePath(m_project->ImageDefinitionPath), fromVersion, m_toVersionNumber);
        m_upgraderList.append(imageUpgrader);
    }

    if(QFile(m_baseDirPath.absoluteFilePath(m_project->IconDefinitionPath)).exists())
    {
        XMLUpgraderBase* iconUpgrader = new IconDefinitionUpgrader(m_baseDirPath.absoluteFilePath(m_project->IconDefinitionPath), fromVersion, m_toVersionNumber);
        m_upgraderList.append(iconUpgrader);
    }

    if(QFile(m_baseDirPath.absoluteFilePath(m_project->MenuDefinitionPath)).exists())
    {
        XMLUpgraderBase* menuUpgrader = new MenuDefinitionUpgrader(m_baseDirPath.absoluteFilePath(m_project->MenuDefinitionPath), fromVersion, m_toVersionNumber);
        m_upgraderList.append(menuUpgrader);
    }

    if(QFile(m_baseDirPath.absoluteFilePath(m_project->LayoutDefinitionPath)).exists())
    {
        XMLUpgraderBase* layoutUpgrader = new LayoutDefinitionUpgrader(m_baseDirPath.absoluteFilePath(m_project->LayoutDefinitionPath), fromVersion, m_toVersionNumber);
        m_upgraderList.append(layoutUpgrader);
    }

    if(QFile(m_baseDirPath.absoluteFilePath(m_project->SegmentDefinitionPath)).exists())
    {
        XMLUpgraderBase* segmentUpgrader = new SegmentDefinitionUpgrader(m_baseDirPath.absoluteFilePath(m_project->SegmentDefinitionPath), fromVersion, m_toVersionNumber);
        m_upgraderList.append(segmentUpgrader);
    }

    if(QFile(m_baseDirPath.absoluteFilePath(m_project->SignalMappingPath)).exists())
    {
        XMLUpgraderBase* signalUpgrader = new SignalMappingUpgrader(m_baseDirPath.absoluteFilePath(m_project->SignalMappingPath), fromVersion, m_toVersionNumber);
        m_upgraderList.append(signalUpgrader);
    }

    if(QFile(m_baseDirPath.absoluteFilePath(m_project->CheckMessagesDefinitionPath)).exists())
    {
        XMLUpgraderBase* checkMessUpgrader = new CheckMessagesDefinitionUpgrader(m_baseDirPath.absoluteFilePath(m_project->CheckMessagesDefinitionPath), fromVersion, m_toVersionNumber);
        m_upgraderList.append(checkMessUpgrader);
    }

    if(QFile(m_baseDirPath.absoluteFilePath(m_project->AnimationDefinitionPath)).exists())
    {
        XMLUpgraderBase* animationUpgrader = new AnimationDefinitionUpgrader(m_baseDirPath.absoluteFilePath(m_project->AnimationDefinitionPath), fromVersion, m_toVersionNumber);
        m_upgraderList.append(animationUpgrader);
    }

    if(QFile(m_baseDirPath.absoluteFilePath(m_project->DiagDefinitionPath)).exists())
    {
        XMLUpgraderBase* diagUpgrader = new DiagDefinitionUpgrader(m_baseDirPath.absoluteFilePath(m_project->DiagDefinitionPath), fromVersion, m_toVersionNumber);
        m_upgraderList.append(diagUpgrader);
    }

    populateList();
}

ProjectUpdaterDialog::~ProjectUpdaterDialog()
{
    delete m_ui;
}

bool ProjectUpdaterDialog::IsUpdateRequired()
{
    return m_updateRequired;
}

ProjectUpdaterDialog::UpgradeStatus ProjectUpdaterDialog::GetUpdateSuccessful()
{
    return m_upgradeStatus;
}

void ProjectUpdaterDialog::slot_applyUpdates()
{
    bool upgradeSuccessful = true;
    for(int i = 0; i < m_ui->listWidget->count(); ++i)
    {
        QListWidgetItem* item = m_ui->listWidget->item(i);

        if(item->checkState() == Qt::Checked)
        {
            int upgraderIndex = item->data(ProjectUpdaterDialog::UpgraderIndex).toInt();
            upgradeSuccessful = m_upgraderList.at(upgraderIndex)->PerformUpgrade();
            qDebug() << m_upgraderList.at(upgraderIndex)->GetUpgradeLog().join("\n");
        }
    }
    if(upgradeSuccessful == true)
    {
        m_upgradeStatus = UpgradeSuccessful;
        QDialog::accept();
    }
    else
    {
        m_upgradeStatus = UpgradeFailed;
        QDialog::reject();
    }
}

void ProjectUpdaterDialog::slot_cancel()
{
    int result = QMessageBox::warning(this, "Exit upgrade?", "If you do not apply these changes the project will not be loaded?", "Exit Upgrade", "Cancel",0,1);

    switch (result)
    {
        case 0:
            m_upgradeStatus = UpgradeSkipped;
            QDialog::accept();
            break;
        case 1:
        default:
            m_upgradeStatus = UpgradeUnknown;
            break;
    }
}

void ProjectUpdaterDialog::slot_selectionChanged()
{
    if (m_ui->listWidget->currentItem() == NULL)
    {
        return;
    }

    int selectedIndex = m_ui->listWidget->currentItem()->data(ProjectUpdaterDialog::UpgraderIndex).toInt();
    m_ui->textBrowser->clear();


    XMLUpgraderBase* currentUpgrader = m_upgraderList.at(selectedIndex);
    VersionNumber currentVersion = currentUpgrader->GetCurrentVersion();
    m_ui->textBrowser->setHtml(currentUpgrader->GetUpgradeInfo(currentVersion, m_toVersionNumber));
}



void ProjectUpdaterDialog::populateList()
{
    m_updateRequired = false;

    foreach (XMLUpgraderBase* upgrader, m_upgraderList)
    {
        QString itemStringText;
        Qt::ItemFlags flags;
        Qt::CheckState checkState;

        if(upgrader->GetCurrentVersion() == VersionNumber(0,0,0))
        {
            itemStringText = QString("%1 - NO VERSION").arg(upgrader->UpdaterName());
            flags = Qt::ItemIsUserCheckable;
            checkState = Qt::Unchecked;
        }
        else if(upgrader->GetCurrentVersion() == m_toVersionNumber)
        {
            itemStringText = QString("%1 (%2)").arg(upgrader->UpdaterName(), upgrader->GetCurrentVersion().GetVersion());
            flags = Qt::ItemIsUserCheckable;
            checkState = Qt::Unchecked;
        }
        else
        {
            itemStringText = QString("%1 (%2 -> %3)").arg(upgrader->UpdaterName(), upgrader->GetCurrentVersion().GetVersion(), m_toVersionNumber.GetVersion());
            flags = Qt::ItemIsUserCheckable | Qt::ItemIsEnabled;
            checkState = Qt::Checked;
            m_updateRequired = true;
        }

        QListWidgetItem* item = new QListWidgetItem(itemStringText);
        item->setData(ProjectUpdaterDialog::UpgraderIndex, QVariant(m_upgraderList.indexOf(upgrader)));
        item->setFlags(flags);
        item->setCheckState(checkState);
        m_ui->listWidget->addItem(item);
    }
}

VersionNumber ProjectUpdaterDialog::getCurrentFileVersion(QString fileName)
{
    QFile file(m_baseDirPath.absoluteFilePath(fileName));
    if (!file.open(QFile::ReadOnly))
        return VersionNumber(0,0,0);

    QDomDocument xmlDoc;
    xmlDoc.setContent(&file);
    file.close();


    QDomElement rootNode = xmlDoc.documentElement();
    if(!rootNode.hasAttribute("AIVersion"))
    {
        return VersionNumber(0,0,0);
    }
    else
    {
        QStringList versionString = rootNode.attributeNode("AIVersion").value().split(".");
        return VersionNumber(versionString.first().toInt(), versionString.last().toInt(),0);
    }
}
