#include "CompilerComboBoxDelegate.h"

#include "CompilerBase.h"
#include "ToolSettings.h"
#include <QComboBox>
#include <QStyledItemDelegate>

CompilerComboBoxDelegate::CompilerComboBoxDelegate(const ToolSettings& toolSettings, QObject* parent) : QStyledItemDelegate(parent),
    m_toolSettings(toolSettings)
{
}

QWidget* CompilerComboBoxDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem& /*option*/, const QModelIndex& /*index*/) const
{
    QComboBox* editor = new QComboBox(parent);
    editor->addItem("<unset>", QVariant());

    foreach (CompilerBase* compiler, m_toolSettings.Compilers)
    {
        editor->addItem(compiler->GetName(), QVariant(compiler->GetName()));
    }

    return editor;
}

void CompilerComboBoxDelegate::setEditorData(QWidget* editor, const QModelIndex& index) const
{
    QComboBox* combobox = static_cast<QComboBox*>(editor);

    if (index.data().toString().isEmpty())
    {
        combobox->setCurrentIndex(0);
    }
    else
    {
        combobox->setCurrentText(index.data().toString());
    }

    combobox->showPopup();
}

void CompilerComboBoxDelegate::setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const
{
    QComboBox* combobox = static_cast<QComboBox*>(editor);

    model->setData(index, combobox->currentData());
}
