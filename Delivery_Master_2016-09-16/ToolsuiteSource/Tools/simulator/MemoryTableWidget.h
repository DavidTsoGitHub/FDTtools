#ifndef MEMORYTABLEWIDGET_H
#define MEMORYTABLEWIDGET_H

#include <QTableWidget>

class MemoryTableWidget : public QTableWidget
{
    Q_OBJECT

public:
    MemoryTableWidget();

    void SetByte(quint16 address, quint8 value);

signals:
    void signal_ByteChanged(quint16 address, quint8 value);

private:
    bool m_emitSignals;
    QFont m_fixedSizeFont;
    QFont m_headerFont;

    quint16 cellToMemoryAddress(int row, int column);
    QTableWidgetItem* addressToTableWidgetItem(quint16 address);
    QTableWidgetItem* createColumnHeader(int column);
    QTableWidgetItem* createRowHeader(int row);

private slots:
    void slot_cellChanged(int row, int column);
    void slot_beginEdit(int column);
    void slot_endEdit(int column);
};

#endif // MEMORYTABLEWIDGET_H
