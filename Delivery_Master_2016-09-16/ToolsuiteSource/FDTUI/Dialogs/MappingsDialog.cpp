#include "MappingsDialog.h"
#include "ui_MappingsDialog.h"

#include "Label.h"
#include "LabelMappings.h"
#include <QDialog>

MappingsDialog::MappingsDialog(LabelMappings& labelMappings, const QList<Label*>& availableLabels, QWidget *parent) :
    QDialog(parent),
    m_ui(new Ui::MappingsDialog)
{
    m_ui->setupUi(this);

    setupSignals();
    setupLabelLists(labelMappings, availableLabels);

    m_ui->upButton->setEnabled(false);
    m_ui->downButton->setEnabled(false);
}

void MappingsDialog::setupSignals()
{
    connect(m_ui->selectedLabelsListWidget, &QListWidget::currentRowChanged, this, &MappingsDialog::slot_onCurrentRowChangedSelectedLabels);

    connect(m_ui->upButton, &QAbstractButton::clicked, this, &MappingsDialog::slot_moveLabelUp);
    connect(m_ui->downButton, &QAbstractButton::clicked, this, &MappingsDialog::slot_moveLabelDown);

    connect(m_ui->addButton, &QAbstractButton::clicked, this, &MappingsDialog::slot_addLabel);
    connect(m_ui->removeButton, &QAbstractButton::clicked, this, &MappingsDialog::slot_removeLabel);

    connect(m_ui->buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(m_ui->buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
}

void MappingsDialog::setupLabelLists(LabelMappings& labelMappings, const QList<Label*> &availableLabels)
{
    QList<Label*> mappedLabels = labelMappings.GetMappedLabels();

    m_selectedLabels.append(mappedLabels);
    m_availableLabels.append(availableLabels);

    foreach (Label* label, mappedLabels)
    {
        m_availableLabels.removeOne(label);
    }

    updateListWidgets();
}

void MappingsDialog::updateListWidgets()
{
    m_ui->availableLabelsListWidget->clear();

    for (int i = 0; i < m_availableLabels.size(); ++i)
    {
        QListWidgetItem *item = new QListWidgetItem(m_availableLabels[i]->GetMasterText());
        m_ui->availableLabelsListWidget->addItem(item);
    }

    m_ui->selectedLabelsListWidget->clear();

    for (int i = 0; i < m_selectedLabels.size(); ++i)
    {
        QListWidgetItem *item = new QListWidgetItem(m_selectedLabels[i]->GetMasterText());
        m_ui->selectedLabelsListWidget->addItem(item);
    }
}

QList<Label*> MappingsDialog::GetSelectedLabels()
{
    return m_selectedLabels;
}

void MappingsDialog::slot_onCurrentRowChangedSelectedLabels(int row)
{
    if(row < 0)
    {
        m_ui->upButton->setEnabled(false);
        m_ui->downButton->setEnabled(false);
        return;
    }

    if(row == 0)
    {
        m_ui->upButton->setEnabled(false);
    }
    else
    {
        m_ui->upButton->setEnabled(true);
    }

    if(row == m_selectedLabels.size()-1)
    {
        m_ui->downButton->setEnabled(false);
    }
    else
    {
        m_ui->downButton->setEnabled(true);
    }
}

void MappingsDialog::slot_moveLabelUp(void)
{
    int row = m_ui->selectedLabelsListWidget->currentRow();
    if (row == -1) return;

    if (row < m_selectedLabels.size())
    {
        m_selectedLabels.swap(row, row - 1);
        updateListWidgets();
        m_ui->selectedLabelsListWidget->setCurrentRow(row - 1);
    }
}

void MappingsDialog::slot_moveLabelDown(void)
{
    int row = m_ui->selectedLabelsListWidget->currentRow();
    if (row == -1) return;

    if (row < m_selectedLabels.size() - 1)
    {
        m_selectedLabels.swap(row, row + 1);
        updateListWidgets();
        m_ui->selectedLabelsListWidget->setCurrentRow(row + 1);
    }
}

void MappingsDialog::slot_addLabel(void)
{
    int row = m_ui->availableLabelsListWidget->currentRow();
    if (row == -1) return;

    if (row < m_availableLabels.size())
    {
        m_selectedLabels.append(m_availableLabels.at(row));
        m_availableLabels.removeAt(row);
        updateListWidgets();
        m_ui->availableLabelsListWidget->setCurrentRow(row);
    }
}

void MappingsDialog::slot_removeLabel(void)
{
    int row = m_ui->selectedLabelsListWidget->currentRow();
    if (row == -1) return;

    if (row < m_selectedLabels.size())
    {
        m_availableLabels.append(m_selectedLabels.at(row));
        m_selectedLabels.removeAt(row);
        updateListWidgets();
        m_ui->selectedLabelsListWidget->setCurrentRow(row);
    }
}
