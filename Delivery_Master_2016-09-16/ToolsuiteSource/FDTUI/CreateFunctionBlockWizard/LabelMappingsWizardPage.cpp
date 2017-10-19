#include "LabelMappingsWizardPage.h"
#include "ui_LabelMappingsWizardPage.h"

#include "FunctionDefinition.h"
#include <QWizardPage>

#define COLUMN_NAME             0
#define COLUMN_DESCRIPTIVENAME  1
#define COLUMN_DESCRIPTION      2

LabelMappingsWizardPage::LabelMappingsWizardPage(FunctionDefinition& functionDefinition, QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::LabelMappingsWizardPage),
    m_functionDefinition(functionDefinition)
{
    ui->setupUi(this);

    ui->labelMappingsTable->setColumnWidth(COLUMN_NAME, 80);
    ui->labelMappingsTable->setColumnWidth(COLUMN_DESCRIPTIVENAME, 150);
    ui->labelMappingsTable->horizontalHeader()->setStretchLastSection(true);

    connect(ui->addLabelMappingsButton, &QAbstractButton::clicked, this, &LabelMappingsWizardPage::slot_addLabelMappingsButton_clicked);
    connect(ui->removeLabelMappingsButton, &QAbstractButton::clicked, this, &LabelMappingsWizardPage::slot_removeLabelMappingsButton_clicked);
}

LabelMappingsWizardPage::~LabelMappingsWizardPage()
{
    delete ui;
}

void LabelMappingsWizardPage::slot_addLabelMappingsButton_clicked()
{
    ui->labelMappingsTable->blockSignals(true);

    int newRowIndex = ui->labelMappingsTable->rowCount();
    ui->labelMappingsTable->insertRow(newRowIndex);
    ui->labelMappingsTable->setItem(newRowIndex, COLUMN_NAME, new QTableWidgetItem());
    ui->labelMappingsTable->setItem(newRowIndex, COLUMN_DESCRIPTIVENAME, new QTableWidgetItem());
    ui->labelMappingsTable->setItem(newRowIndex, COLUMN_DESCRIPTION, new QTableWidgetItem());
    ui->labelMappingsTable->editItem(ui->labelMappingsTable->item(newRowIndex, 0));

    ui->labelMappingsTable->blockSignals(false);
}

void LabelMappingsWizardPage::slot_removeLabelMappingsButton_clicked()
{
    ui->labelMappingsTable->removeRow(ui->labelMappingsTable->currentRow());
}

bool LabelMappingsWizardPage::validatePage()
{
    m_functionDefinition.ClearLabelMappings();

    for (int i = 0; i < ui->labelMappingsTable->rowCount(); ++i)
    {
        QTableWidgetItem* labelMappingsNameItem = ui->labelMappingsTable->item(i, COLUMN_NAME);
        QTableWidgetItem* descriptiveNameItem = ui->labelMappingsTable->item(i, COLUMN_DESCRIPTIVENAME);
        QTableWidgetItem* descriptionItem = ui->labelMappingsTable->item(i, COLUMN_DESCRIPTION);

        if (labelMappingsNameItem->text().isEmpty())
        {
            ui->labelMappingsTable->editItem(labelMappingsNameItem);
            return false;
        }

        if (descriptiveNameItem->text().isEmpty())
        {
            ui->labelMappingsTable->editItem(descriptiveNameItem);
            return false;
        }

        QString labelMappingsName = labelMappingsNameItem->text();
        QString descriptiveName = descriptiveNameItem->text();
        QString description = descriptionItem->text();

        LabelMappings* labelMappings = new LabelMappings();
        labelMappings->SetLabelMappingsName(labelMappingsName);
        labelMappings->SetDescription(description);
        labelMappings->SetDescriptiveName(descriptiveName);

        m_functionDefinition.AddLabelMappings(labelMappings);
    }

    return true;
}
