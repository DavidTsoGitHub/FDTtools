#include "JailedDirectorySelector.h"
#include "ui_JailedDirectorySelector.h"

#include <QDialog>
#include <QFileSystemModel>

JailedDirectorySelector::JailedDirectorySelector(const QString& rootPath, QWidget *parent) :
    QDialog(parent),
    m_ui(new Ui::JailedDirectorySelector),
    m_rootPath(rootPath)
{
    m_ui->setupUi(this);
    setModal(this);
    m_fileSystemModel = new QFileSystemModel();
    m_fileSystemModel->setRootPath(rootPath);
    m_fileSystemModel->setFilter(QDir::AllDirs | QDir::NoDotAndDotDot);

    m_ui->fileTree->setModel(m_fileSystemModel);
    m_ui->fileTree->setRootIndex(m_fileSystemModel->index(rootPath));
    m_ui->fileTree->setColumnHidden(1, true); // Hide size column
    m_ui->fileTree->setColumnHidden(2, true); // Hide type column
    m_ui->fileTree->header()->setStretchLastSection(false);
    m_ui->fileTree->header()->setSectionResizeMode(0, QHeaderView::Stretch);
    m_ui->fileTree->header()->setSectionResizeMode(3, QHeaderView::ResizeToContents);
}

JailedDirectorySelector::~JailedDirectorySelector()
{
    delete m_ui;
    delete m_fileSystemModel;
}

void JailedDirectorySelector::SetSelectedDirectory(const QString& selectedPath)
{
    QModelIndex pathIndex = m_fileSystemModel->index(selectedPath);

    if (!pathIndex.isValid()) return;

    QModelIndex parent = pathIndex.parent();
    while (parent.isValid())
    {
        m_ui->fileTree->setExpanded(parent, true);
        parent = parent.parent();
    }

    m_ui->fileTree->selectionModel()->setCurrentIndex(pathIndex, QItemSelectionModel::ClearAndSelect);
}

QString JailedDirectorySelector::GetSelectedDirectoryAsRelative()
{
    QModelIndex selectedIndex = m_ui->fileTree->selectionModel()->currentIndex();
    QString path = m_fileSystemModel->filePath(selectedIndex);
    path.replace(m_rootPath + "/", "");

    return path;
}

// The result code will not be set properly before calling setModelData when used as a delegate
// unless we overload the accept/reject methods like this
void JailedDirectorySelector::accept()
{
    setResult(Accepted);
    QDialog::accept();
}

void JailedDirectorySelector::reject()
{
    setResult(Rejected);
    QDialog::reject();
}
