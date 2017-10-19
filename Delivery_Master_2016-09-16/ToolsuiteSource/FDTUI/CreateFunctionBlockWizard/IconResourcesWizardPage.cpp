#include "IconResourcesWizardPage.h"
#include "ui_IconResourcesWizardPage.h"

#include "FunctionDefinition.h"
#include <QKeyEvent>
#include <QMessageBox>
#include <QWizardPage>

IconResourcesWizardPage::IconResourcesWizardPage(FunctionDefinition& functionDefinition, QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::IconResourcesWizardPage),
    m_functionDefinition(functionDefinition)
{
    ui->setupUi(this);

    connect(ui->iconResourcesTable, &QTableWidget::cellChanged, this, &IconResourcesWizardPage::slot_cellChanged);

    addEmptyRow();
}

IconResourcesWizardPage::~IconResourcesWizardPage()
{
    delete ui;
}

void IconResourcesWizardPage::addEmptyRow()
{
    ui->iconResourcesTable->insertRow(ui->iconResourcesTable->rowCount());
}

void IconResourcesWizardPage::slot_cellChanged(int row, int column)
{
    QString cellContents = ui->iconResourcesTable->item(row, column)->text();
    cellContents.remove(QRegularExpression("[^\\w\\d]"));
    ui->iconResourcesTable->item(row, column)->setText(cellContents);

    if (cellContents.isEmpty())
    {
        if (row < getLastRowIndex())
        {
            ui->iconResourcesTable->removeRow(row);
            ui->iconResourcesTable->selectRow(row);
        }
    }
    else
    {
        ui->iconResourcesTable->item(row, column)->setText(cellContents);

        if (row == getLastRowIndex())
        {
            addEmptyRow();
            ui->iconResourcesTable->selectRow(getLastRowIndex());
        }
    }

}

int IconResourcesWizardPage::getLastRowIndex()
{
    return ui->iconResourcesTable->rowCount() - 1;
}

void IconResourcesWizardPage::keyReleaseEvent(QKeyEvent* event)
{
    if (event->key() == Qt::Key_Delete && (ui->iconResourcesTable->currentRow() != getLastRowIndex()))
    {
        ui->iconResourcesTable->removeRow(ui->iconResourcesTable->currentRow());
    }
}

bool IconResourcesWizardPage::validatePage()
{
    m_functionDefinition.ClearIconResources();

    QStringList resourceNames;

    for (int i = 0; i < getLastRowIndex(); ++i) // Ignore last row as it is always empty
    {
        QString resourceName = ui->iconResourcesTable->item(i, 0)->text().trimmed();

        if (resourceName.isEmpty()) continue;

        if (resourceNames.contains(resourceName))
        {
            QMessageBox::information(this, "Duplicate resource names found!", "The resource name '" + resourceName + "' was found multiple times in the list, each Icon resource must have a unique name");
            return false;
        }

        resourceNames.append(resourceName);

        m_functionDefinition.AddIconResource(resourceName);
    }

    return true;
}
