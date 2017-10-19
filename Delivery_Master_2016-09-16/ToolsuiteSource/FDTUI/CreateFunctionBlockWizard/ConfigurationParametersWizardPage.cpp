#include "ConfigurationParametersWizardPage.h"
#include "ui_ConfigurationParametersWizardPage.h"

#include "DataTypeComboBox.h"
#include "FunctionDefinition.h"

#include <QSpinBox>
#include <QWizardPage>

typedef enum
{
    NameColumn,
    DataTypeColumn,
    ArrayLengthColumn,
    DescriptiveNameColumn,
    DescriptionColumn,
    ColumnCount
} CalibrationParameterColumns;

ConfigurationParametersWizardPage::ConfigurationParametersWizardPage(FunctionDefinition& functionDefinition, QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::ConfigurationParametersWizardPage),
    m_functionDefinition(functionDefinition)
{
    ui->setupUi(this);
    hideError();

    ui->configurationParametersTable->setColumnCount(ColumnCount);

    ui->configurationParametersTable->setHorizontalHeaderItem(NameColumn, new QTableWidgetItem("Name"));
    ui->configurationParametersTable->setHorizontalHeaderItem(DataTypeColumn, new QTableWidgetItem("Data Type"));
    ui->configurationParametersTable->setHorizontalHeaderItem(ArrayLengthColumn, new QTableWidgetItem("Array Length"));
    ui->configurationParametersTable->setHorizontalHeaderItem(DescriptiveNameColumn, new QTableWidgetItem("Descriptive Name"));
    ui->configurationParametersTable->setHorizontalHeaderItem(DescriptionColumn, new QTableWidgetItem("Description"));


    ui->configurationParametersTable->setColumnWidth(NameColumn, 80);
    ui->configurationParametersTable->setColumnWidth(DataTypeColumn, 80);
    ui->configurationParametersTable->setColumnWidth(DescriptiveNameColumn, 150);

    connect(ui->addConfigurationParameterButton, &QAbstractButton::clicked, this, &ConfigurationParametersWizardPage::slot_addConfigurationParameterButton_clicked);
    connect(ui->removeConfigurationParameterButton, &QAbstractButton::clicked, this, &ConfigurationParametersWizardPage::slot_removeConfigurationParameterButton_clicked);
}

ConfigurationParametersWizardPage::~ConfigurationParametersWizardPage()
{
    delete ui;
}

void ConfigurationParametersWizardPage::slot_addConfigurationParameterButton_clicked()
{
    ui->configurationParametersTable->blockSignals(true);

    int newRowIndex = ui->configurationParametersTable->rowCount();
    ui->configurationParametersTable->insertRow(newRowIndex);
    ui->configurationParametersTable->setItem(newRowIndex, NameColumn, new QTableWidgetItem());
    ui->configurationParametersTable->setCellWidget(newRowIndex, DataTypeColumn, new BasicDataTypesComboBox());

    QSpinBox* arrayLengthSpinBox = new QSpinBox();
    arrayLengthSpinBox->setMinimum(1);
    arrayLengthSpinBox->setMaximum(255);
    arrayLengthSpinBox->setValue(1);

    ui->configurationParametersTable->setCellWidget(newRowIndex, ArrayLengthColumn, arrayLengthSpinBox);
    ui->configurationParametersTable->setItem(newRowIndex, DescriptiveNameColumn, new QTableWidgetItem());
    ui->configurationParametersTable->setItem(newRowIndex, DescriptionColumn, new QTableWidgetItem());
    ui->configurationParametersTable->editItem(ui->configurationParametersTable->item(newRowIndex, 0));

    ui->configurationParametersTable->blockSignals(false);

    hideError();
}

void ConfigurationParametersWizardPage::slot_removeConfigurationParameterButton_clicked()
{
    ui->configurationParametersTable->removeRow(ui->configurationParametersTable->currentRow());
    hideError();
}


void ConfigurationParametersWizardPage::showError(QString message)
{
    ui->errorLabel->setText(message);
    ui->errorLabel->show();
}

void ConfigurationParametersWizardPage::hideError()
{
    ui->errorLabel->hide();
}

bool ConfigurationParametersWizardPage::validatePage()
{
    m_functionDefinition.ClearConfigurationParameters();
    hideError();

    for (int i = 0; i < ui->configurationParametersTable->rowCount(); ++i)
    {
        QTableWidgetItem* parameterNameItem = ui->configurationParametersTable->item(i, NameColumn);
        BasicDataTypesComboBox* dataTypeCombobox = static_cast<BasicDataTypesComboBox*>(ui->configurationParametersTable->cellWidget(i, DataTypeColumn));
        QSpinBox* arrayLengthSpinBox = static_cast<QSpinBox*>(ui->configurationParametersTable->cellWidget(i, ArrayLengthColumn));
        QTableWidgetItem* descriptiveNameItem = ui->configurationParametersTable->item(i, DescriptiveNameColumn);
        QTableWidgetItem* descriptionItem = ui->configurationParametersTable->item(i, DescriptionColumn);

        if (parameterNameItem->text().isEmpty())
        {
            showError("Configuration parameter name cannot be empty!");
            ui->configurationParametersTable->editItem(parameterNameItem);
            return false;
        }

        if (descriptiveNameItem->text().isEmpty())
        {
            showError("Configuration parameter descriptive name cannot be empty!");
            ui->configurationParametersTable->editItem(descriptiveNameItem);
            return false;
        }

        QString parameterName = parameterNameItem->text();
        QString dataType = dataTypeCombobox->currentText();
        QString descriptiveName = descriptiveNameItem->text();
        QString description = descriptionItem->text();
        quint8 arrayLength = static_cast<quint8>(arrayLengthSpinBox->value());

        ConfigurationParameter* configurationParameter = new ConfigurationParameter();
        configurationParameter->SetDataType(dataType);
        configurationParameter->SetParameterName(parameterName);
        configurationParameter->SetDescription(description);
        configurationParameter->SetDescriptiveName(descriptiveName);
        configurationParameter->SetArrayLength(arrayLength);

        m_functionDefinition.AddConfigurationParameter(configurationParameter);
    }

    return true;
}
