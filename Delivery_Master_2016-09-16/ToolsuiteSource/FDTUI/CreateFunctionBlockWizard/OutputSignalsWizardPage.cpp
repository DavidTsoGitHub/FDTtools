#include "OutputSignalsWizardPage.h"
#include "ui_OutputSignalsWizardPage.h"

#include "CustomizableDataTypesComboBox.h"
#include "FunctionDefinition.h"
#include <QWizardPage>

#define COLUMN_NAME         0
#define COLUMN_DATATYPE     1

OutputSignalsWizardPage::OutputSignalsWizardPage(FunctionDefinition& functionDefinition, QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::OutputSignalsWizardPage),
    m_functionDefinition(functionDefinition)
{
    ui->setupUi(this);

    hideError();

    ui->outputSignalsTable->setColumnWidth(COLUMN_NAME, 350);
    ui->outputSignalsTable->setColumnWidth(COLUMN_DATATYPE, 150);

//  ui->outputSignalsTable->insertRow(0);
//  ui->outputSignalsTable->setItem(0, COLUMN_NAME, new QTableWidgetItem("OutputSignal1"));
//  ui->outputSignalsTable->setCellWidget(0, COLUMN_DATATYPE, new CustomizableDataTypesComboBox());

    connect(ui->addOutputSignalButton, &QAbstractButton::clicked, this, &OutputSignalsWizardPage::slot_addOutputSignalButton_clicked);
    connect(ui->removeOutputSignalButton, &QAbstractButton::clicked, this, &OutputSignalsWizardPage::slot_removeOutputSignalButton_clicked);
}

OutputSignalsWizardPage::~OutputSignalsWizardPage()
{
    delete ui;
}

void OutputSignalsWizardPage::slot_addOutputSignalButton_clicked()
{
    ui->outputSignalsTable->blockSignals(true);

    int newRowIndex = ui->outputSignalsTable->rowCount();
    ui->outputSignalsTable->insertRow(newRowIndex);
    ui->outputSignalsTable->setItem(newRowIndex, COLUMN_NAME, new QTableWidgetItem());
    ui->outputSignalsTable->setCellWidget(newRowIndex, COLUMN_DATATYPE, new CustomizableDataTypesComboBox());
    ui->outputSignalsTable->editItem(ui->outputSignalsTable->item(newRowIndex, 0));

    ui->outputSignalsTable->blockSignals(false);
    hideError();
}

void OutputSignalsWizardPage::slot_removeOutputSignalButton_clicked()
{
    ui->outputSignalsTable->removeRow(ui->outputSignalsTable->currentRow());
    hideError();
}

void OutputSignalsWizardPage::showError(QString message)
{
    ui->errorLabel->setText(message);
    ui->errorLabel->show();
}

void OutputSignalsWizardPage::hideError()
{
    ui->errorLabel->hide();
}

bool OutputSignalsWizardPage::validatePage()
{
    m_functionDefinition.ClearSources();
    hideError();

    for (int i = 0; i < ui->outputSignalsTable->rowCount(); ++i)
    {
        QTableWidgetItem* signalNameItem = ui->outputSignalsTable->item(i, COLUMN_NAME);
        if (signalNameItem->text().isEmpty())
        {
            showError("Signal name cannot be empty!");
            ui->outputSignalsTable->editItem(signalNameItem);
            return false;
        }

        CustomizableDataTypesComboBox* dataTypeComboBox = static_cast<CustomizableDataTypesComboBox*>(ui->outputSignalsTable->cellWidget(i, COLUMN_DATATYPE));
        if (dataTypeComboBox->currentText().isEmpty())
        {
            showError("Data type cannot be empty!");
            dataTypeComboBox->setFocus();
            return false;
        }

        QString signalName = signalNameItem->text();
        QString dataType = dataTypeComboBox->currentText();

        m_functionDefinition.AddGCLSignalSource(new GCLSignalSource(signalName, m_functionDefinition.GetName(), dataType));
    }

    return true;
}
