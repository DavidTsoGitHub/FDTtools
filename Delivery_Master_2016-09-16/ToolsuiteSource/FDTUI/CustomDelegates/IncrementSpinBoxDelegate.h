#ifndef INCREMENTSPINBOXDELEGATE_H
#define INCREMENTSPINBOXDELEGATE_H

class QItemDelegate;

class IncrementSpinBoxDelegate : public QItemDelegate
{
public:
    IncrementSpinBoxDelegate(int min, int max, int increment, QObject* parent = 0);

private:
    int m_min;
    int m_max;
    int m_increment;

    // QAbstractItemDelegate interface
public:
    QWidget*createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const;
    void setEditorData(QWidget* editor, const QModelIndex& index) const;
    void setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const;
};

#endif // INCREMENTSPINBOXDELEGATE_H
