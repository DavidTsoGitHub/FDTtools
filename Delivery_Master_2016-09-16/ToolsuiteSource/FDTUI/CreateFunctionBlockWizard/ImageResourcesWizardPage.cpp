#include "ImageResourcesWizardPage.h"
#include "ui_ImageResourcesWizardPage.h"

#include "FunctionDefinition.h"
#include <QKeyEvent>
#include <QMessageBox>
#include <QWizardPage>

ImageResourcesWizardPage::ImageResourcesWizardPage(FunctionDefinition& functionDefinition, QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::ImageResourcesWizardPage),
    m_functionDefinition(functionDefinition)
{
    ui->setupUi(this);

    connect(ui->imageResourcesTable, &QTableWidget::cellChanged, this, &ImageResourcesWizardPage::slot_cellChanged);

    addEmptyRow();
}

ImageResourcesWizardPage::~ImageResourcesWizardPage()
{
    delete ui;
}

void ImageResourcesWizardPage::addEmptyRow()
{
    ui->imageResourcesTable->insertRow(ui->imageResourcesTable->rowCount());
}

void ImageResourcesWizardPage::slot_cellChanged(int row, int column)
{
    QString cellContents = ui->imageResourcesTable->item(row, column)->text();
    cellContents.remove(QRegularExpression("[^\\w\\d]"));
    ui->imageResourcesTable->item(row, column)->setText(cellContents);

    if (cellContents.isEmpty())
    {
        if (row < getLastRowIndex())
        {
            ui->imageResourcesTable->removeRow(row);
            ui->imageResourcesTable->selectRow(row);
        }
    }
    else
    {
        ui->imageResourcesTable->item(row, column)->setText(cellContents);

        if (row == getLastRowIndex())
        {
            addEmptyRow();
            ui->imageResourcesTable->selectRow(getLastRowIndex());
        }
    }

}

int ImageResourcesWizardPage::getLastRowIndex()
{
    return ui->imageResourcesTable->rowCount() - 1;
}

void ImageResourcesWizardPage::keyReleaseEvent(QKeyEvent* event)
{
    if (event->key() == Qt::Key_Delete && (ui->imageResourcesTable->currentRow() != getLastRowIndex()))
    {
        ui->imageResourcesTable->removeRow(ui->imageResourcesTable->currentRow());
    }
}

bool ImageResourcesWizardPage::validatePage()
{
    m_functionDefinition.ClearImageResources();

    QStringList resourceNames;

    for (int i = 0; i < getLastRowIndex(); ++i) // Ignore last row as it is always empty
    {
        QString resourceName = ui->imageResourcesTable->item(i, 0)->text().trimmed();

        if (resourceName.isEmpty()) continue;

        if (resourceNames.contains(resourceName))
        {
            QMessageBox::information(this, "Duplicate resource names found!", "The resource name '" + resourceName + "' was found multiple times in the list, each Image resource must have a unique name");
            return false;
        }

        resourceNames.append(resourceName);

        m_functionDefinition.AddImageResource(resourceName);
    }

    return true;
}
