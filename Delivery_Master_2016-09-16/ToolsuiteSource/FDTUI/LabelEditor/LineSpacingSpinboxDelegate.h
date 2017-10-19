#ifndef LINESPACINGSPINBOXDELEGATE_H
#define LINESPACINGSPINBOXDELEGATE_H

#include <QObject>
#include <QItemDelegate>

class LineSpacingSpinboxDelegate : public QItemDelegate
{
    Q_OBJECT

public:
    explicit LineSpacingSpinboxDelegate(QObject *parent = 0);

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &, const QModelIndex &) const;

    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;

    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

#endif // LINESPACINGSPINBOXDELEGATE_H
