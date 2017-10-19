#include "IconManager.h"
#include "ui_IconManager.h"

#include "Icon.h"
#include "IconModel.h"
#include "Project.h"
#include "ProjectObject.h"
#include <QDir>
#include <QFileDialog>
#include <QMessageBox>
#include <QWidget>

IconManager::IconManager(QWidget *parent) :
    QWidget(parent),
    m_ui(new Ui::IconManager),
    m_project(NULL),
    m_iconModel(NULL),
    m_currentSelectedIcon(NULL)
{
    m_ui->setupUi(this);

    connect(m_ui->addIconButton, &QPushButton::clicked, this, &IconManager::slot_addIconButtonClicked);
    connect(m_ui->removeIconsButton, &QPushButton::clicked, this, &IconManager::slot_removeIconButtonClicked);
    connect(m_ui->replaceIconButton, &QPushButton::clicked, this, &IconManager::slot_replaceIconButtonClicked);
}

IconManager::~IconManager()
{
    delete m_ui;
    delete m_iconModel;
}

void IconManager::SetProject(Project* project)
{
    m_project = project;
    m_projectRoot = m_project->GetProjectRoot();

    m_iconModel = new IconModel(m_project->GetIconDefinition());
    m_ui->iconsTable->setModel(m_iconModel);
    m_ui->iconsTable->resizeColumnsToContents();
    m_ui->iconsTable->resizeRowsToContents();

    connect(m_ui->iconsTable->selectionModel(), &QItemSelectionModel::selectionChanged, this, &IconManager::slot_iconSelectionChanged);
    connect(m_iconModel, &IconModel::signal_modified, this, &IconManager::signal_modified);
}

void IconManager::ClearProject()
{
    delete m_iconModel;
    m_iconModel = NULL;
}

void IconManager::JustSelected()
{
    if (m_currentSelectedIcon != NULL)
    {
        emit signal_iconSelectionChanged(m_currentSelectedIcon);
    }
}

void IconManager::slot_iconSelectionChanged(const QItemSelection& selected, const QItemSelection& /*deselected*/)
{
    if (selected.isEmpty())
    {
        m_currentSelectedIcon = NULL;
        emit signal_iconSelectionChanged(NULL);
        return;
    }

    m_ui->removeIconsButton->setEnabled(m_ui->iconsTable->selectionModel()->hasSelection());
    m_ui->replaceIconButton->setEnabled(m_ui->iconsTable->selectionModel()->hasSelection());

    int selectedIndex = m_ui->iconsTable->selectionModel()->currentIndex().row();
    m_currentSelectedIcon = static_cast<ProjectObject*>(m_iconModel->GetIconDefinition()->Icons().at(selectedIndex));
    emit signal_iconSelectionChanged(m_currentSelectedIcon);
}

void IconManager::slot_addIconButtonClicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Select icon file"), m_projectRoot.absolutePath(), QString("Images (*.png *.jpg *.bmp)"));
    if(!fileName.isEmpty())
    {
        QFileInfo fileInfo(fileName);
        QFileInfo expectedProjectFile(m_projectRoot.absoluteFilePath(Project::IconsDirectoryName +  "/" + fileInfo.fileName()));

        if (!expectedProjectFile.absoluteDir().exists())
        {
            expectedProjectFile.absoluteDir().mkpath(".");
        }

        if (!expectedProjectFile.exists())
        {
            QFile file(fileName);

            if (!file.open(QFile::ReadOnly))
            {
                QMessageBox::warning(this, "Error opening icon file!", "Couldn't open icon file for copying:\n" + file.errorString());
                return;
            }

            if (!file.copy(expectedProjectFile.absoluteFilePath()))
            {
                QMessageBox::warning(this, "Error copying icon file!", "Couldn't copy icon file!\n" + file.fileName());
            }
        }

        QString path = expectedProjectFile.absoluteFilePath().replace(m_projectRoot.absolutePath() + "/","");

        m_iconModel->AddIcon(fileInfo.fileName(), path);
    }
}

void IconManager::slot_removeIconButtonClicked()
{
    QMessageBox::StandardButton reply = QMessageBox::warning(this, "Delete icon(s)", "You are about to delete icon(s). The icon(s) will be removed from the project and the hard drive! Do you want to proceed?", QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::No)
    {
        return;
    }

    const QItemSelection selection = m_ui->iconsTable->selectionModel()->selection();

    foreach(QModelIndex index, selection.indexes())
    {
        int selectedIndex = index.row();
        QString fileName = m_project->GetProjectRoot().absoluteFilePath(m_iconModel->GetIconDefinition()->Icons().at(selectedIndex)->GetPath());
        QFile file(fileName);
        if (file.exists())
        {
            bool fileRemoved = file.remove();
            if (!fileRemoved)
            {
                QMessageBox::warning(this, "File removal failed", "The file: " + fileName + " could not be removed. Please try closing any application that could be using the file and remove the file manually.", QMessageBox::Ok);
            }
        }
    }

    foreach (QItemSelectionRange range, selection)
    {
        m_iconModel->removeRows(range.top(), range.height(), range.parent());
    }

    m_currentSelectedIcon = NULL;
}

void IconManager::slot_replaceIconButtonClicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Select new icon file"), m_projectRoot.absolutePath(), QString("Images (*.png *.jpg *.bmp)"));

    if(!fileName.isEmpty())
    {
        QFileInfo iconFileInfo(fileName);
        QFileInfo expectedProjectIconFile(m_projectRoot.absoluteFilePath(Project::IconsDirectoryName +  "/" + iconFileInfo.fileName()));

        if (!expectedProjectIconFile.absoluteDir().exists())
        {
           expectedProjectIconFile.absoluteDir().mkpath(".");
        }

        if (!expectedProjectIconFile.exists())
        {
           QFile fontFile(fileName);

           if (!fontFile.open(QFile::ReadOnly))
           {
               QMessageBox::warning(this, "Error opening icon file!", "Couldn't open icon file for copying:\n" + fontFile.errorString());
               return;
           }

           if (!fontFile.copy(expectedProjectIconFile.absoluteFilePath()))
           {
               QMessageBox::warning(this, "Error copying icon file!", "Couldn't copy icon file!\n" + fontFile.fileName());
           }
        }

        QString iconPath = expectedProjectIconFile.absoluteFilePath().replace(m_projectRoot.absolutePath() + "/","");

        QModelIndex selectedIconIndex = m_ui->iconsTable->selectionModel()->currentIndex();

        if (selectedIconIndex.isValid())
        {
            QModelIndex iconPathIndex = m_iconModel->index(selectedIconIndex.row(), IconModel::PathColumn);
            m_iconModel->setData(iconPathIndex, iconPath, Qt::EditRole);
        }
    }
}
