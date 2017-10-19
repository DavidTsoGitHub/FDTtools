#include "IncrementSpinBoxDelegate.h"
#include "SchedulerAnalysisConstants.h"

#include <QItemDelegate>
#include <QSpinBox>

IncrementSpinBoxDelegate::IncrementSpinBoxDelegate(int min, int max, int increment, QObject* parent) :
    QItemDelegate(parent),
    m_min(min),
    m_max(max),
    m_increment(increment)
{
}

QWidget* IncrementSpinBoxDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem& /*option*/, const QModelIndex& /*index*/) const
{
    QSpinBox* editor= new QSpinBox(parent);
    editor->setMinimum(m_min);
    editor->setMaximum(m_max);
    editor->setSingleStep(m_increment);

    return editor;

}

void IncrementSpinBoxDelegate::setEditorData(QWidget* editor, const QModelIndex& index) const
{
    int value = index.model()->data(index, Qt::EditRole).toInt();
    QSpinBox* spinbox = static_cast<QSpinBox*>(editor);
    spinbox->setValue(value);
}

void IncrementSpinBoxDelegate::setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const
{
    QSpinBox* spinbox = static_cast<QSpinBox*>(editor);
    spinbox->interpretText();
    int value = spinbox->value();

    if (value % 5 != 0)
    {
        value += MillisecondsPerSlot - (value % 5);
    }

    model->setData(index, value, Qt::EditRole);
}
