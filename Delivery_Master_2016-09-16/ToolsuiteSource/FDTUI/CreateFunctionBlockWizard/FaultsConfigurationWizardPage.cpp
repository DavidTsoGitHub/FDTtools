#include "FaultsConfigurationWizardPage.h"
#include "ui_FaultsConfigurationWizardPage.h"

#include "FunctionDefinition.h"

#include <QMessageBox>
#include <QWizardPage>

FaultsConfigurationWizardPage::FaultsConfigurationWizardPage(FunctionDefinition& functionDefinition, QWidget* parent) :
    QWizardPage(parent),
    ui(new Ui::FaultsConfigurationWizardPage),
    m_functionDefinition(functionDefinition)
{
    ui->setupUi(this);

    connect(ui->errorConditionsTable, &QTableWidget::cellChanged, this, &FaultsConfigurationWizardPage::slot_cellChanged);

    addEmptyRow();
}

FaultsConfigurationWizardPage::~FaultsConfigurationWizardPage()
{
    delete ui;
}

void FaultsConfigurationWizardPage::addEmptyRow()
{
    ui->errorConditionsTable->insertRow(ui->errorConditionsTable->rowCount());
}

void FaultsConfigurationWizardPage::slot_cellChanged(int row, int column)
{
    QString cellContents = ui->errorConditionsTable->item(row, column)->text();

    if (column == 0)
    {
        cellContents.remove(QRegularExpression("[^\\w\\d]"));
        ui->errorConditionsTable->item(row, column)->setText(cellContents);

        QTableWidgetItem* nextItem = ui->errorConditionsTable->item(row, 1);
        if (!cellContents.isEmpty() && nextItem == NULL)
        {
            nextItem = new QTableWidgetItem("Description");
            ui->errorConditionsTable->setItem(row, 1, nextItem);
        }

        ui->errorConditionsTable->editItem(ui->errorConditionsTable->item(row, 1));
    }

    if (rowIsEmpty(row))
    {
        if (row != getLastRowIndex())
        {
            ui->errorConditionsTable->removeRow(row);
        }
    }
    else
    {
        if (row == getLastRowIndex())
        {
            addEmptyRow();
        }
    }
}

bool FaultsConfigurationWizardPage::rowIsEmpty(int row)
{
    return (ui->errorConditionsTable->item(row, 0) == NULL || ui->errorConditionsTable->item(row, 0)->text().isEmpty()) &&
           (ui->errorConditionsTable->item(row, 1) == NULL || ui->errorConditionsTable->item(row, 1)->text().isEmpty());
}

int FaultsConfigurationWizardPage::getLastRowIndex()
{
    return ui->errorConditionsTable->rowCount() - 1;
}

bool FaultsConfigurationWizardPage::validatePage()
{
    m_functionDefinition.ClearFaultDefinitions();

    QStringList faultNames;

    for (int i = 0; i < getLastRowIndex(); ++i) // Ignore last row as it is always empty
    {
        if(ui->errorConditionsTable->item(i, 0) == NULL)
        {
            continue;
        }
        QString faultName = ui->errorConditionsTable->item(i, 0)->text().trimmed();

        if (faultName.isEmpty()) continue;

        if (faultNames.contains(faultName))
        {
            QMessageBox::information(this, "Duplicate fault names found!", "The fault name '" + faultName + "' was found multiple times in the list, each fault must have a unique name");
            return false;
        }

        faultNames.append(faultName);

        QString faultDescription = ui->errorConditionsTable->item(i, 1)->text().trimmed();
        m_functionDefinition.AddFaultDefinition(faultName, faultDescription);
    }

    return true;
}


