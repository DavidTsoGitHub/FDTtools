#ifndef RESOURCECOMBOBOXDELEGATE_H
#define RESOURCECOMBOBOXDELEGATE_H

class CheckMessageDefinition;
class IconDefinition;
class ImageDefinition;
class LabelDefinition;
class QStyledItemDelegate;

class ResourceComboBoxDelegate : public QStyledItemDelegate
{
public:
    explicit ResourceComboBoxDelegate(CheckMessageDefinition& checkMessageDefinition, LabelDefinition &labelDefinition, ImageDefinition &imageDefinition, IconDefinition &iconDefinition, QObject* parent = 0);
private:
    CheckMessageDefinition& m_checkMessageDefinition;
    LabelDefinition& m_labelDefinition;
    ImageDefinition& m_imageDefinition;
    IconDefinition& m_iconDefinition;

    // QAbstractItemDelegate interface
public:
    QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const;
    void setEditorData(QWidget* editor, const QModelIndex& index) const;
    void setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const;

};

#endif // RESOURCECOMBOBOXDELEGATE_H

