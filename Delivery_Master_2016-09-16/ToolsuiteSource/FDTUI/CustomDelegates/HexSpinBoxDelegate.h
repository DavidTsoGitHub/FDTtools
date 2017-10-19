#ifndef HEXSPINBOXDELEGATE_H
#define HEXSPINBOXDELEGATE_H

class QStyledItemDelegate;

class HexSpinBoxDelegate : public QStyledItemDelegate
{
public:
    explicit HexSpinBoxDelegate(int fieldWidth = 8, int maxValue = INT_MAX, int minValue = 0, QObject* parent = 0);

private:
    int m_fieldWidth;
    int m_maxValue;
    int m_minValue;


    // QStyledItemDelegate interface
public:
    QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const;
    void setEditorData(QWidget* editor, const QModelIndex& index) const;
    void setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const;
    QString displayText(const QVariant& value, const QLocale& locale) const;
};

#endif // HEXSPINBOXDELEGATE_H
