#ifndef LABELCOMBOBOXDELEGATE_H
#define LABELCOMBOBOXDELEGATE_H

class LabelDefinition;
class QStyledItemDelegate;

class LabelComboBoxDelegate : public QStyledItemDelegate
{
public:
    explicit LabelComboBoxDelegate(LabelDefinition& labelDefinition, QObject* parent = 0);

private:
    LabelDefinition& m_labelDefinition;

    // QAbstractItemDelegate interface
public:
    QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const;
    void setEditorData(QWidget* editor, const QModelIndex& index) const;
    void setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const;

    // QStyledItemDelegate interface
public:
    QString displayText(const QVariant& value, const QLocale& locale) const;
};

#endif // LABELCOMBOBOXDELEGATE_H
