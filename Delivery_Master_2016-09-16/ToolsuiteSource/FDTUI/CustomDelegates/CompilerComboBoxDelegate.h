#ifndef COMPILERCOMBOBOXDELEGATE_H
#define COMPILERCOMBOBOXDELEGATE_H

class ToolSettings;
class QStyledItemDelegate;

class CompilerComboBoxDelegate : public QStyledItemDelegate
{
public:
    explicit CompilerComboBoxDelegate(const ToolSettings& toolSettings, QObject* parent = 0);

private:
    const ToolSettings& m_toolSettings;

    // QAbstractItemDelegate interface
public:
    QWidget*createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const;
    void setEditorData(QWidget* editor, const QModelIndex& index) const;
    void setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const;
};

#endif // COMPILERCOMBOBOXDELEGATE_H
