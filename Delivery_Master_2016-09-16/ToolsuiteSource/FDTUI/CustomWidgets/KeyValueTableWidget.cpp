#include "KeyValueTableWidget.h"

#include <QMap>
#include <QKeyEvent>
#include <QTableWidget>
#include <QString>

#define KEYCOLUMN 0
#define VALUECOLUMN 1

KeyValueTableWidget::KeyValueTableWidget(QWidget* parent) : QTableWidget(parent)
{
    this->setColumnCount(2);
    this->setColumnWidth(KEYCOLUMN, 100);
    this->horizontalHeader()->setStretchLastSection(true);
    this->verticalHeader()->setVisible(false);

    addHeader(KEYCOLUMN, "Key");
    addHeader(VALUECOLUMN, "Value");

    insertEmptyRow();

    connect(this, &QTableWidget::cellChanged, this, &KeyValueTableWidget::slot_afterCellChanged);
}

void KeyValueTableWidget::SetData(const QMap<QString, QString>& data)
{
    this->blockSignals(true);
    this->setRowCount(0);

    QMapIterator<QString, QString> dataIterator(data);
    while (dataIterator.hasNext())
    {
        dataIterator.next();
        insertDataRow(dataIterator.key(), dataIterator.value());
    }

    insertEmptyRow();
    this->blockSignals(false);
}

const QMap<QString, QString> KeyValueTableWidget::GetData()
{
    QMap<QString, QString> data;

    for (int i=0; i<rowCount(); ++i)
    {
        QTableWidgetItem* keyItem = item(i, 0);
        if (keyItem == NULL) continue;
        QString key = keyItem->text();

        QTableWidgetItem* valueItem = item(i, 1);
        QString value("");
        if (valueItem != NULL)
        {
            value = valueItem->text();
        }

        data.insert(key, value);
    }

    return data;
}

void KeyValueTableWidget::addHeader(int column, QString header)
{
    QTableWidgetItem* columnHeader = new QTableWidgetItem(header);
    columnHeader->setFlags(0);
    this->setHorizontalHeaderItem(column, columnHeader);
}

void KeyValueTableWidget::insertDataRow(QString key, QString value)
{
    int row = insertEmptyRow();
    this->setItem(row, KEYCOLUMN, new QTableWidgetItem(key));
    this->setItem(row, VALUECOLUMN, new QTableWidgetItem(value));
}

int KeyValueTableWidget::insertEmptyRow()
{
    int row = this->rowCount();
    this->insertRow(row);
    this->setCurrentCell(row, 0);
    return row;
}

void KeyValueTableWidget::slot_afterCellChanged(int row, int column)
{
    if (row + 1 == this->rowCount())
    {
        insertEmptyRow();
    }
    if (rowIsEmpty(row))
    {
        this->removeRow(row);
    }
    else
    {
        if (column == KEYCOLUMN)
        {
            this->setCurrentCell(row, VALUECOLUMN);
        }
    }
    emit signal_DataChanged();
}

bool KeyValueTableWidget::rowIsEmpty(int row)
{
    QTableWidgetItem* keyItem = this->item(row, KEYCOLUMN);
    QTableWidgetItem* valueItem = this->item(row, VALUECOLUMN);

    return (keyItem == NULL || keyItem->text().isEmpty()) && (valueItem == NULL || valueItem->text().isEmpty());
}


void KeyValueTableWidget::keyReleaseEvent(QKeyEvent* event)
{
    if (this->state() != QTableView::NoState) return;

    if (event->key() == Qt::Key_Delete && (this->currentRow() != this->rowCount() - 1))
    {
        this->removeRow(this->currentRow());
    }
}
