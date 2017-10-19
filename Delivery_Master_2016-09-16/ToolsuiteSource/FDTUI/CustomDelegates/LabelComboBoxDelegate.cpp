#include "LabelComboBoxDelegate.h"

#include "Label.h"
#include "LabelDefinition.h"
#include <QComboBox>
#include <QStyledItemDelegate>

LabelComboBoxDelegate::LabelComboBoxDelegate(LabelDefinition& labelDefinition, QObject* parent) : QStyledItemDelegate(parent),
    m_labelDefinition(labelDefinition)
{
}

QWidget* LabelComboBoxDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem& /*option*/, const QModelIndex& /*index*/) const
{
    QComboBox* editor = new QComboBox(parent);

    editor->addItem(Label::UndefinedLabel.GetMasterText(), Label::UndefinedLabel.GetID());
    foreach (Label* label, m_labelDefinition.Labels())
    {
        editor->addItem(label->GetMasterText(), QVariant::fromValue(label->GetID()));
    }

    return editor;
}

void LabelComboBoxDelegate::setEditorData(QWidget* editor, const QModelIndex& index) const
{
    QComboBox* combobox = static_cast<QComboBox*>(editor);
    quint32 labelId = index.data().toInt();
    QString labelText = m_labelDefinition.GetLabel(labelId)->GetMasterText();
    combobox->setCurrentText(labelText);
    combobox->showPopup();
}

void LabelComboBoxDelegate::setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const
{
    QComboBox* combobox = static_cast<QComboBox*>(editor);
    model->setData(index, combobox->currentData());
}

QString LabelComboBoxDelegate::displayText(const QVariant& value, const QLocale& /*locale*/) const
{
    Label* label = m_labelDefinition.GetLabel(value.toInt());
    return label->GetMasterText();
}
