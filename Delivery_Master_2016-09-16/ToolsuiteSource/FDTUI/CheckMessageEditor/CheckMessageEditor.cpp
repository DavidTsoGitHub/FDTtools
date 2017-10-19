#include "CheckMessageEditor.h"
#include "ui_CheckMessageEditor.h"

#include "CheckMessage.h"
#include "CheckMessageModel.h"
#include "Logger.h"
#include "AddLanguageDialog.h"
#include "ResourceComboBoxDelegate.h"

#include <QFile>
#include <QMessageBox>
#include <QComboBox>
#include <QSignalMapper>
#include <QFileDialog>
#include <QRadioButton>
#include <QException>
#include <QWidget>

typedef enum
{
    NameColumn,
    CheckMessageEditorStandardColumnCount
} CheckMessageEditorStandardColumns;

CheckMessageEditor::CheckMessageEditor(QWidget* parent) : QWidget(parent),
    m_ui(new Ui::CheckMessageEditor),
    m_project(NULL),
    m_checkMessageModel(NULL),
    m_currentCheckMessageObject(NULL)
{
    m_ui->setupUi(this);

    connect(m_ui->removeCheckMessageButton, &QPushButton::clicked, this, &CheckMessageEditor::slot_removeSelectedCheckMessages);
}

CheckMessageEditor::~CheckMessageEditor()
{
    delete m_ui;
}

void CheckMessageEditor::slot_SetProject(Project* project)
{
    m_project = project;
    m_projectRoot = project->GetProjectRoot();

    /* Messages tab */
    m_checkMessageModel = new CheckMessageModel(m_project->GetCheckMessagesDefinition(), m_project->GetLabelDefinition(), m_project->GetImageDefinition(), m_project->GetIconDefinition(), m_project->GetLayoutDefinition());
    m_ui->checkMessageTable->setModel(m_checkMessageModel);

    if(m_project->GetCheckMessagesDefinition().CheckMessages().count() > 0)
    {
        for(int i = CheckMessageModel::CheckMessageColumnCount; i <= m_project->GetCheckMessagesDefinition().GetSignalSources().count(); ++i)
        {
            m_ui->checkMessageTable->setItemDelegateForColumn(i, new ResourceComboBoxDelegate(m_project->GetCheckMessagesDefinition(), m_project->GetLabelDefinition(), m_project->GetImageDefinition(), m_project->GetIconDefinition()) );
        }
    }
    m_ui->checkMessageTable->setColumnWidth(NameColumn,200);

    /* Signals */
    connect(m_ui->checkMessageTable->selectionModel(), &QItemSelectionModel::selectionChanged, this, &CheckMessageEditor::slot_checkMessageSelectionChanged);
    connect(m_checkMessageModel, &CheckMessageModel::signal_modified, this, &CheckMessageEditor::signal_modified);
    connect(m_checkMessageModel, &CheckMessageModel::signal_checkMessageAdded, this, &CheckMessageEditor::slot_checkMessageAdded);
    connect(m_ui->moveSelectedItemUpButton, &QPushButton::clicked,[=] { m_checkMessageModel->slot_moveSelectedItemUp(m_ui->checkMessageTable->currentIndex());});
    connect(m_ui->moveSelectedItemDownButton, &QPushButton::clicked,[=] { m_checkMessageModel->slot_moveSelectedItemDown(m_ui->checkMessageTable->currentIndex());});

    connect(m_ui->checkMessageTable->selectionModel(), &QItemSelectionModel::selectionChanged, this, &CheckMessageEditor::slot_selectionChanged);
}

void CheckMessageEditor::slot_ClearProject()
{
    m_project = NULL;
    delete m_checkMessageModel;
    m_checkMessageModel = NULL;

    disconnect(m_ui->checkMessageTable->selectionModel(), &QItemSelectionModel::selectionChanged, this, &CheckMessageEditor::slot_checkMessageSelectionChanged);
}

void CheckMessageEditor::slot_RemoveWidgetSink(GCLSignalSink *sink, quint32 widgetID)
{
    m_checkMessageModel->GetCheckMessageDefinition().RemoveCheckMessageItemEverywhere(widgetID);
    m_checkMessageModel->RemoveWidgetSink(sink);
}

void CheckMessageEditor::slot_checkMessageSelectionChanged(const QItemSelection& /*selected*/, const QItemSelection& /*deselected*/)
{
    m_ui->removeCheckMessageButton->setEnabled(m_ui->checkMessageTable->selectionModel()->hasSelection());
}

void CheckMessageEditor::slot_checkMessageAdded(const QModelIndex& index)
{
    QModelIndex newIndex = m_ui->checkMessageTable->model()->index(m_project->GetCheckMessagesDefinition().CheckMessages().count() - 1, index.column() + 1);

    m_ui->checkMessageTable->setSelectionBehavior(QAbstractItemView::SelectItems);
    m_ui->checkMessageTable->setCurrentIndex(newIndex);
    m_ui->checkMessageTable->setSelectionBehavior(QAbstractItemView::SelectRows);
}

void CheckMessageEditor::slot_layoutAdded(quint32 layoutID)
{
    Q_UNUSED(layoutID);
    m_checkMessageModel->slot_layoutModelChanged();
}

void CheckMessageEditor::slot_layoutRemoved(quint32 layoutID)
{
    Q_UNUSED(layoutID);
    m_checkMessageModel->slot_layoutModelChanged();
}

void CheckMessageEditor::slot_layoutModelChanged()
{
    slot_SetProject(m_project);
}

void CheckMessageEditor::JustSelected()
{
    if (m_currentCheckMessageObject != NULL)
    {
        emit signal_CheckMessageEditorItemSelected(m_currentCheckMessageObject);
    }
}

void CheckMessageEditor::slot_removeSelectedCheckMessages()
{
    const QItemSelection selection = m_ui->checkMessageTable->selectionModel()->selection();

    foreach (QItemSelectionRange range, selection)
    {
        m_checkMessageModel->removeRows(range.top(), range.height(), range.parent());
    }

    //removes the outdated PropertyWidget
    m_currentCheckMessageObject = NULL;
    emit signal_CheckMessageEditorItemSelected(NULL);
}

void CheckMessageEditor::slot_selectionChanged(const QItemSelection &selected, const QItemSelection &deselected)
{
    Q_UNUSED(selected);
    Q_UNUSED(deselected);

    if(m_ui->checkMessageTable->selectionModel()->hasSelection() && m_ui->checkMessageTable->selectionModel()->selectedRows().count() == 1)
    {
        int selectedIndex = m_ui->checkMessageTable->selectionModel()->currentIndex().row();
        m_currentCheckMessageObject = static_cast<ProjectObject*>(m_checkMessageModel->GetCheckMessageDefinition().CheckMessages().at(selectedIndex));

        emit signal_CheckMessageEditorItemSelected(m_currentCheckMessageObject);
    }
}
