#include "HexSpinBoxDelegate.h"

#include <QSpinBox>
#include <QStyledItemDelegate>

HexSpinBoxDelegate::HexSpinBoxDelegate(int fieldWidth, int maxValue, int minValue, QObject* parent) : QStyledItemDelegate(parent),
    m_fieldWidth(fieldWidth),
    m_maxValue(maxValue),
    m_minValue(minValue)
{
}

QWidget* HexSpinBoxDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem& /*option*/, const QModelIndex& /*index*/) const
{
    QSpinBox* editor = new QSpinBox(parent);
    editor->setPrefix("0x");
    editor->setDisplayIntegerBase(16);
    editor->setMaximum(m_maxValue);
    editor->setMinimum(m_minValue);

    return editor;
}

void HexSpinBoxDelegate::setEditorData(QWidget* editor, const QModelIndex& index) const
{
    QSpinBox* spinbox = static_cast<QSpinBox*>(editor);
    spinbox->setValue(index.data(Qt::EditRole).toInt());
}

void HexSpinBoxDelegate::setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const
{
    QSpinBox* spinbox = static_cast<QSpinBox*>(editor);
    model->setData(index, QVariant::fromValue(spinbox->value()));
}

QString HexSpinBoxDelegate::displayText(const QVariant& value, const QLocale& /*locale*/) const
{
    return QString("0x%1").arg(value.toInt(), m_fieldWidth, 16, QChar('0'));
}
