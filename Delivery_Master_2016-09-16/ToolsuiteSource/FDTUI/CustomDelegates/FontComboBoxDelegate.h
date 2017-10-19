#ifndef FONTCOMBOBOXDELEGATE_H
#define FONTCOMBOBOXDELEGATE_H

class LabelDefinition;
class QStyledItemDelegate;

class FontComboBoxDelegate : public QStyledItemDelegate
{
public:
    explicit FontComboBoxDelegate(LabelDefinition &labelDefinition);

private:
    LabelDefinition& m_labelDefinition;

    // QAbstractItemDelegate interface
public:
    QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const;
    void setEditorData(QWidget* editor, const QModelIndex& index) const;
    void setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const;
};

#endif // FONTCOMBOBOXDELEGATE_H
