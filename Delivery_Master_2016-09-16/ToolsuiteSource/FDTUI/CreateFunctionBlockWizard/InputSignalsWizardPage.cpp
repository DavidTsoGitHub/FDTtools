#include "InputSignalsWizardPage.h"
#include "ui_InputSignalsWizardPage.h"

#include "CustomizableDataTypesComboBox.h"
#include "FunctionDefinition.h"
#include <QCheckBox>
#include <QWizardPage>

#define COLUMN_NAME         0
#define COLUMN_DATATYPE     1
#define COLUMN_INDICATION   2
#define COLUMN_MANDATORY    3

InputSignalsWizardPage::InputSignalsWizardPage(FunctionDefinition& functionDefinition, QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::InputSignalsWizardPage),
    m_functionDefinition(functionDefinition)
{
    ui->setupUi(this);

    hideError();

    ui->inputSignalsTable->setColumnWidth(COLUMN_NAME, 290);
    ui->inputSignalsTable->setColumnWidth(COLUMN_DATATYPE, 150);
    ui->inputSignalsTable->setColumnWidth(COLUMN_INDICATION, 50);
    ui->inputSignalsTable->setColumnWidth(COLUMN_MANDATORY, 70);

//    ui->inputSignalsTable->insertRow(0);
//    ui->inputSignalsTable->setItem(0, COLUMN_NAME, new QTableWidgetItem("InputSignal1"));
//    ui->inputSignalsTable->setCellWidget(0, COLUMN_DATATYPE, new CustomizableDataTypesComboBox());
//    ui->inputSignalsTable->setCellWidget(0, COLUMN_INDICATION, new QCheckBox());

//    QCheckBox* mandatoryCheckBox = new QCheckBox();
//    mandatoryCheckBox->setChecked(true);
//    ui->inputSignalsTable->setCellWidget(0, COLUMN_MANDATORY, mandatoryCheckBox);

    connect(ui->addInputSignalButton, &QAbstractButton::clicked, this, &InputSignalsWizardPage::slot_addInputSignalButton_clicked);
    connect(ui->removeInputSignalButton, &QAbstractButton::clicked, this, &InputSignalsWizardPage::slot_removeInputSignalButton_clicked);
}

InputSignalsWizardPage::~InputSignalsWizardPage()
{
    delete ui;
}

void InputSignalsWizardPage::slot_addInputSignalButton_clicked()
{
    ui->inputSignalsTable->blockSignals(true);

    int newRowIndex = ui->inputSignalsTable->rowCount();
    ui->inputSignalsTable->insertRow(newRowIndex);
    ui->inputSignalsTable->setItem(newRowIndex, COLUMN_NAME, new QTableWidgetItem());
    ui->inputSignalsTable->setCellWidget(newRowIndex, COLUMN_DATATYPE, new CustomizableDataTypesComboBox());
    ui->inputSignalsTable->setCellWidget(newRowIndex, COLUMN_INDICATION, new QCheckBox());
    ui->inputSignalsTable->setCellWidget(newRowIndex, COLUMN_MANDATORY, new QCheckBox());


    ui->inputSignalsTable->editItem(ui->inputSignalsTable->item(newRowIndex, 0));

    ui->inputSignalsTable->blockSignals(false);

    hideError();
}

void InputSignalsWizardPage::slot_removeInputSignalButton_clicked()
{
    hideError();
    ui->inputSignalsTable->removeRow(ui->inputSignalsTable->currentRow());
}

void InputSignalsWizardPage::showError(QString message)
{
    ui->errorLabel->setText(message);
    ui->errorLabel->show();
}

void InputSignalsWizardPage::hideError()
{
    ui->errorLabel->hide();
}

bool InputSignalsWizardPage::validatePage()
{
    m_functionDefinition.ClearSinks();
    hideError();

    for (int i = 0; i < ui->inputSignalsTable->rowCount(); ++i)
    {
        QTableWidgetItem* signalNameItem = ui->inputSignalsTable->item(i, COLUMN_NAME);
        if (signalNameItem->text().isEmpty())
        {
            showError("Signal name cannot be empty!");
            ui->inputSignalsTable->editItem(signalNameItem);
            return false;
        }

        CustomizableDataTypesComboBox* dataTypeComboBox = static_cast<CustomizableDataTypesComboBox*>(ui->inputSignalsTable->cellWidget(i, COLUMN_DATATYPE));
        if (dataTypeComboBox->currentText().trimmed().isEmpty())
        {
            showError("Data type cannot be empty!");
            dataTypeComboBox->setFocus();
            return false;
        }

        QCheckBox* updateNotificationCheckBox = static_cast<QCheckBox*>(ui->inputSignalsTable->cellWidget(i, COLUMN_INDICATION));
        QCheckBox* isMandatoryCheckBox = static_cast<QCheckBox*>(ui->inputSignalsTable->cellWidget(i, COLUMN_MANDATORY));

        QString signalName = signalNameItem->text();
        QString dataType = dataTypeComboBox->currentText().trimmed();
        bool updateNotification = updateNotificationCheckBox->isChecked();
        bool isMandatory = isMandatoryCheckBox->isChecked();

        m_functionDefinition.AddGCLSignalSink(new GCLSignalSink(signalName, m_functionDefinition.GetName(), dataType, updateNotification, isMandatory));
    }

    return true;
}


