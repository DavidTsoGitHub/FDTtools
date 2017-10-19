#include "CalibrationParametersWizardPage.h"
#include "ui_CalibrationParametersWizardPage.h"

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

CalibrationParametersWizardPage::CalibrationParametersWizardPage(FunctionDefinition& functionDefinition, QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::CalibrationParametersWizardPage),
    m_functionDefinition(functionDefinition)
{
    ui->setupUi(this);
    hideError();

    ui->calibrationParametersTable->setColumnCount(ColumnCount);
    ui->calibrationParametersTable->setHorizontalHeaderItem(NameColumn, new QTableWidgetItem("Name"));
    ui->calibrationParametersTable->setHorizontalHeaderItem(DataTypeColumn, new QTableWidgetItem("Data Type"));
    ui->calibrationParametersTable->setHorizontalHeaderItem(ArrayLengthColumn, new QTableWidgetItem("Array Length"));
    ui->calibrationParametersTable->setHorizontalHeaderItem(DescriptiveNameColumn, new QTableWidgetItem("Descriptive Name"));
    ui->calibrationParametersTable->setHorizontalHeaderItem(DescriptionColumn, new QTableWidgetItem("Description"));


    ui->calibrationParametersTable->setColumnWidth(NameColumn, 80);
    ui->calibrationParametersTable->setColumnWidth(DataTypeColumn, 80);
    ui->calibrationParametersTable->setColumnWidth(DescriptiveNameColumn, 150);


//  QSpinBox* arrayLengthSpinBox = new QSpinBox();
//  arrayLengthSpinBox->setMinimum(1);
//  arrayLengthSpinBox->setMaximum(255);
//  arrayLengthSpinBox->setValue(1);

//  ui->calibrationParametersTable->insertRow(0);
//  ui->calibrationParametersTable->setItem(0, NameColumn, new QTableWidgetItem("SAMPLE_PARAMETER"));
//  ui->calibrationParametersTable->setCellWidget(0, DataTypeColumn, new BasicDataTypesComboBox());
//  ui->calibrationParametersTable->setCellWidget(0, ArrayLengthColumn, arrayLengthSpinBox);
//  ui->calibrationParametersTable->setItem(0, DescriptiveNameColumn, new QTableWidgetItem("The sample parameter"));
//  ui->calibrationParametersTable->setItem(0, DescriptionColumn, new QTableWidgetItem("A parameter just to see how the table should be populated"));

    connect(ui->addCalibrationParameterButton, &QAbstractButton::clicked, this, &CalibrationParametersWizardPage::slot_addCalibrationParameterButton_clicked);
    connect(ui->removeCalibrationParameterButton, &QAbstractButton::clicked, this, &CalibrationParametersWizardPage::slot_removeCalibrationParameterButton_clicked);
    connect(ui->calibrationParametersTable, &QTableWidget::cellChanged, this, &CalibrationParametersWizardPage::slot_calibrationParametersTable_cellChanged);
}

CalibrationParametersWizardPage::~CalibrationParametersWizardPage()
{
    delete ui;
}

void CalibrationParametersWizardPage::slot_addCalibrationParameterButton_clicked()
{
    ui->calibrationParametersTable->blockSignals(true);

    int newRowIndex = ui->calibrationParametersTable->rowCount();
    ui->calibrationParametersTable->insertRow(newRowIndex);

    QSpinBox* arrayLengthSpinBox = new QSpinBox();
    arrayLengthSpinBox->setMinimum(1);
    arrayLengthSpinBox->setMaximum(255);
    arrayLengthSpinBox->setValue(1);

    ui->calibrationParametersTable->setItem(newRowIndex, NameColumn, new QTableWidgetItem());
    ui->calibrationParametersTable->setCellWidget(newRowIndex, DataTypeColumn, new BasicDataTypesComboBox());
    ui->calibrationParametersTable->setCellWidget(newRowIndex, ArrayLengthColumn, arrayLengthSpinBox);
    ui->calibrationParametersTable->setItem(newRowIndex, DescriptiveNameColumn, new QTableWidgetItem());
    ui->calibrationParametersTable->setItem(newRowIndex, DescriptionColumn, new QTableWidgetItem());
    ui->calibrationParametersTable->editItem(ui->calibrationParametersTable->item(newRowIndex, 0));

    ui->calibrationParametersTable->blockSignals(false);

    hideError();
}

void CalibrationParametersWizardPage::slot_removeCalibrationParameterButton_clicked()
{
    ui->calibrationParametersTable->removeRow(ui->calibrationParametersTable->currentRow());
}

void CalibrationParametersWizardPage::slot_calibrationParametersTable_cellChanged(int row, int column)
{
    if (column != NameColumn) return;

    QString text = ui->calibrationParametersTable->item(row, column)->text();
    text.remove(QRegularExpression("[^\\w_]"));
    ui->calibrationParametersTable->item(row, column)->setText(text.toUpper());
}

void CalibrationParametersWizardPage::showError(QString message)
{
    ui->errorLabel->setText(message);
    ui->errorLabel->show();
}

void CalibrationParametersWizardPage::hideError()
{
    ui->errorLabel->hide();
}

bool CalibrationParametersWizardPage::validatePage()
{
    m_functionDefinition.ClearCalibrationParameters();
    hideError();

    for (int i = 0; i < ui->calibrationParametersTable->rowCount(); ++i)
    {
        QTableWidgetItem* parameterNameItem = ui->calibrationParametersTable->item(i, NameColumn);
        BasicDataTypesComboBox* dataTypeCombobox = static_cast<BasicDataTypesComboBox*>(ui->calibrationParametersTable->cellWidget(i, DataTypeColumn));
        QSpinBox* arrayLengthSpinBox = static_cast<QSpinBox*>(ui->calibrationParametersTable->cellWidget(i, ArrayLengthColumn));
        QTableWidgetItem* descriptiveNameItem = ui->calibrationParametersTable->item(i, DescriptiveNameColumn);
        QTableWidgetItem* descriptionItem = ui->calibrationParametersTable->item(i, DescriptionColumn);

        if (parameterNameItem->text().isEmpty())
        {
            showError("Calibration parameter name cannot be empty!");
            ui->calibrationParametersTable->editItem(parameterNameItem);
            return false;
        }

        if (descriptiveNameItem->text().isEmpty())
        {
            showError("Calibration parameter descriptive name cannot be empty!");
            ui->calibrationParametersTable->editItem(descriptiveNameItem);
            return false;
        }

        QString parameterName = parameterNameItem->text();
        QString dataType = dataTypeCombobox->currentText();
        QString descriptiveName = descriptiveNameItem->text();
        QString description = descriptionItem->text();
        quint8 arrayLength = static_cast<quint8>(arrayLengthSpinBox->value());

        CalibrationParameter* calibrationParameter = new CalibrationParameter();
        calibrationParameter->SetDataType(dataType);
        calibrationParameter->SetParameterName(parameterName);
        calibrationParameter->SetDescription(description);
        calibrationParameter->SetDescriptiveName(descriptiveName);
        calibrationParameter->SetArrayLength(arrayLength);

        m_functionDefinition.AddCalibrationParameter(calibrationParameter);
    }

    return true;
}
