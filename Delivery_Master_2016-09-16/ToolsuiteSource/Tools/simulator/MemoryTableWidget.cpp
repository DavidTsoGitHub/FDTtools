#include "MemoryTableWidget.h"

#include <HexByteDelegate.h>
#include <QHeaderView>
#include <QFontDatabase>

#define COLUMN_READ_WIDTH 25
#define COLUMN_EDIT_WIDTH 45

#define COLUMNS 32
#define ROWS    20

MemoryTableWidget::MemoryTableWidget()
{
    m_fixedSizeFont = QFont(QFontDatabase::systemFont(QFontDatabase::FixedFont));
    m_fixedSizeFont.setPixelSize(14);

    m_headerFont = QFont(m_fixedSizeFont);
    m_headerFont.setBold(true);

    setFont(m_fixedSizeFont);
    horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);

    for (int columnIndex=0; columnIndex<COLUMNS; ++columnIndex)
    {
        insertColumn(columnIndex);
        setColumnWidth(columnIndex, COLUMN_READ_WIDTH);
        setHorizontalHeaderItem(columnIndex, createColumnHeader(columnIndex));
    }

    for (int rowIndex=0; rowIndex<ROWS; ++rowIndex)
    {
        insertRow(rowIndex);
        setVerticalHeaderItem(rowIndex, createRowHeader(rowIndex));
    }

    for (int rowIndex=0; rowIndex<ROWS; ++rowIndex)
    {
        for (int columnIndex=0; columnIndex<COLUMNS; ++columnIndex)
        {
            quint16 address = cellToMemoryAddress(rowIndex, columnIndex);
            SetByte(address, 0);
        }
    }

    HexByteDelegate* hexEditor = new HexByteDelegate(this);
    setItemDelegate(hexEditor);

    connect(hexEditor, &HexByteDelegate::beginEdit, this, &MemoryTableWidget::slot_beginEdit);
    connect(hexEditor, &HexByteDelegate::endEdit, this, &MemoryTableWidget::slot_endEdit);
    connect(this, &QTableWidget::cellChanged, this, &MemoryTableWidget::slot_cellChanged);
}

void MemoryTableWidget::SetByte(quint16 address, quint8 value)
{
    QTableWidgetItem* tableItem = addressToTableWidgetItem(address);
    QString valueString("%1");
    m_emitSignals = false;
    tableItem->setText(valueString.arg(value, 2, 16, QChar('0')));
    m_emitSignals = true;
}

quint16 MemoryTableWidget::cellToMemoryAddress(int row, int column)
{
    return row * COLUMNS | column;
}

QTableWidgetItem* MemoryTableWidget::addressToTableWidgetItem(quint16 address)
{
    int column = address % COLUMNS;
    int row = address / COLUMNS;

    if (item(row, column) == NULL)
    {
        QTableWidgetItem* newItem = new QTableWidgetItem();

        newItem->setToolTip(QString("0x%1").arg(address, 4, 16, QChar('0')));
        newItem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsEditable);
        newItem->setTextAlignment(Qt::AlignCenter);
        setItem(row, column, newItem);
    }

    return item(row, column);
}

QTableWidgetItem*MemoryTableWidget::createColumnHeader(int column)
{
    QTableWidgetItem* header = new QTableWidgetItem(QString("%1").arg(column, 2, 16, QChar('0')));
    header->setFont(m_headerFont);

    return header;
}

QTableWidgetItem*MemoryTableWidget::createRowHeader(int row)
{

    QTableWidgetItem* header = new QTableWidgetItem(QString("%1").arg((row*COLUMNS), 4, 16, QChar('0')));
    header->setFont(m_headerFont);

    return header;
}

void MemoryTableWidget::slot_cellChanged(int row, int column)
{
    quint16 address = cellToMemoryAddress(row, column);
    quint8 value = addressToTableWidgetItem(address)->text().toInt(NULL, 16);
    if (m_emitSignals) emit signal_ByteChanged(address, value);
}

void MemoryTableWidget::slot_beginEdit(int column)
{
    setColumnWidth(column, COLUMN_EDIT_WIDTH); // To fit the editor spinbox
}

void MemoryTableWidget::slot_endEdit(int column)
{
    setColumnWidth(column, COLUMN_READ_WIDTH); // Restore column width
}
