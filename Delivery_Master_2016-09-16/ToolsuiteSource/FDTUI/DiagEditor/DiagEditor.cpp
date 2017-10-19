#include "DiagEditor.h"
#include "ui_DiagEditor.h"

#include "DIDToCALMappingModel.h"
#include "DTCLabelMappingModel.h"
#include "HexSpinBoxDelegate.h"
#include "LabelComboBoxDelegate.h"
#include "Project.h"
#include "CalibrationParametersDelegate.h"
#include <QItemSelection>
#include <QModelIndex>
#include <QWidget>
#include <QMessageBox>

DiagEditor::DiagEditor(QWidget *parent) :
    QWidget(parent),
    m_ui(new Ui::DiagEditor),
    m_dtcLabelMappingModel(NULL),
    m_didMappingModel(NULL),
    m_project(NULL)
{
    m_ui->setupUi(this);

    connect(m_ui->removeDTCLabelMappingButton, &QPushButton::clicked, this, &DiagEditor::slot_removeDTCLabelMappingButtonClicked);
    connect(m_ui->removeDIDMappingButton, &QPushButton::clicked, this, &DiagEditor::slot_removeDIDMappingButtonClicked);
}

DiagEditor::~DiagEditor()
{
    delete m_ui;
    delete m_dtcLabelMappingModel;
    delete m_didMappingModel;
}

void DiagEditor::SetProject(Project* project)
{
    m_project = project;

    /* DTC Label Mapping Tab */
    m_dtcLabelMappingModel = new DTCLabelMappingModel(project->GetDiagDefinition());
    m_ui->dtcLabelMappingTable->setModel(m_dtcLabelMappingModel);
    m_ui->dtcLabelMappingTable->setItemDelegateForColumn(DTCColumn, new HexSpinBoxDelegate(6, 0xFFFFFF, 0));
    m_ui->dtcLabelMappingTable->setItemDelegateForColumn(LabelColumn, new LabelComboBoxDelegate(project->GetLabelDefinition()));

    /* DID To CAL Mapping Tab */
    m_didMappingModel = new DIDToCALMappingModel(project->GetDiagDefinition());
    m_ui->didMappingTable->setModel(m_didMappingModel);
    m_ui->didMappingTable->setItemDelegateForColumn(DIDColumn, new HexSpinBoxDelegate(4, 0xFFFF, 0));
    m_ui->didMappingTable->setItemDelegateForColumn(CALColumn, new CalibrationParametersDelegate(project));

    /* Signals */
    connect(m_ui->dtcLabelMappingTable->selectionModel(), &QItemSelectionModel::selectionChanged, this, &DiagEditor::slot_dtcLabelMappingSelectionChanged);
    connect(m_dtcLabelMappingModel, &DTCLabelMappingModel::signal_dtcAdded, this, &DiagEditor::slot_dtcAdded);
    connect(m_ui->didMappingTable->selectionModel(), &QItemSelectionModel::selectionChanged, this, &DiagEditor::slot_didMappingSelectionChanged);
    connect(m_didMappingModel, &DIDToCALMappingModel::signal_didMappingAdded, this, &DiagEditor::slot_didMappingAdded);

    connect(m_project, &Project::PropertyUpdated, this, &DiagEditor::slot_projectUpdated);
}

void DiagEditor::ClearProject()
{}

void DiagEditor::slot_projectUpdated()
{
    if (m_project->GetFunctionBlocks().IsDirty())
    {
        m_didMappingModel->ResetData();
    }
}

void DiagEditor::slot_removeDTCLabelMappingButtonClicked()
{
    QMessageBox::StandardButton reply = QMessageBox::warning(this, "Delete DTC", "You are about to delete DTC(s)\n\nDo you want to proceed?", QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::No)
    {
        return;
    }

    const QItemSelection selection = m_ui->dtcLabelMappingTable->selectionModel()->selection();

    foreach (QItemSelectionRange range, selection)
    {
        m_dtcLabelMappingModel->removeRows(range.top(), range.height(), QModelIndex());
    }
}

void DiagEditor::slot_dtcLabelMappingSelectionChanged(const QItemSelection& /*selected*/, const QItemSelection& /*deselected*/)
{
    m_ui->removeDTCLabelMappingButton->setEnabled(m_ui->dtcLabelMappingTable->selectionModel()->hasSelection());
}

void DiagEditor::slot_removeDIDMappingButtonClicked()
{
    QMessageBox::StandardButton reply = QMessageBox::warning(this, "Delete DID", "You are about to delete DID(s)\n\nDo you want to proceed?", QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::No)
    {
        return;
    }

    const QItemSelection selection = m_ui->didMappingTable->selectionModel()->selection();

    foreach (QItemSelectionRange range, selection)
    {
        m_didMappingModel->removeRows(range.top(), range.height(), QModelIndex());
    }
}

void DiagEditor::slot_didMappingSelectionChanged(const QItemSelection& /*selected*/, const QItemSelection& /*deselected*/)
{
    m_ui->removeDIDMappingButton->setEnabled(m_ui->didMappingTable->selectionModel()->hasSelection());
}

void DiagEditor::slot_dtcAdded(const QModelIndex& index)
{
    QModelIndex newIndex = m_ui->dtcLabelMappingTable->model()->index(m_project->GetDiagDefinition().GetDTCLabelMappings().count() - 1, index.column());
    m_ui->dtcLabelMappingTable->setCurrentIndex(newIndex);
}

void DiagEditor::slot_didMappingAdded(const QModelIndex& index)
{
    QModelIndex newIndex = m_ui->didMappingTable->model()->index(m_project->GetDiagDefinition().GetDIDToCALMappings().count() - 1, index.column());
    m_ui->didMappingTable->setCurrentIndex(newIndex);
}
