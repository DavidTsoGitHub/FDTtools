#ifndef HEXBYTEDELEGATE_H
#define HEXBYTEDELEGATE_H

#include <QItemDelegate>

class HexByteDelegate : public QItemDelegate
{
    Q_OBJECT

public:
    explicit HexByteDelegate(QObject *parent = 0);

signals:
    void beginEdit(int column) const;
    void endEdit(int column) const;

private slots:
    void commitAndCloseEditor();

private:
    int getValueFromIndex(const QModelIndex& index) const;

    // QAbstractItemDelegate interface
public:
    QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const;
    void destroyEditor(QWidget* editor, const QModelIndex& index) const;
    void setEditorData(QWidget* editor, const QModelIndex& index) const;
    void setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const;

};

#endif // HEXBYTEDELEGATE_H
