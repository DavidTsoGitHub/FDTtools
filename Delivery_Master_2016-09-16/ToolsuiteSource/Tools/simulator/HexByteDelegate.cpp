#include "HexByteDelegate.h"

#include <QSpinBox>

HexByteDelegate::HexByteDelegate(QObject* parent) : QItemDelegate(parent)
{

}

QWidget*HexByteDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    Q_UNUSED(option);

    emit beginEdit(index.column());

    QSpinBox* hexEdit = new QSpinBox(parent);
    hexEdit->setMaximum(0xFF);
    hexEdit->setMinimum(0x00);
    hexEdit->setDisplayIntegerBase(16);

    int value = getValueFromIndex(index);
    hexEdit->setValue(value);

    connect(hexEdit, &QAbstractSpinBox::editingFinished, this, &HexByteDelegate::commitAndCloseEditor);

    return hexEdit;
}


void HexByteDelegate::destroyEditor(QWidget* editor, const QModelIndex& index) const
{
    emit endEdit(index.column());
    editor->deleteLater();
}

void HexByteDelegate::setEditorData(QWidget* editor, const QModelIndex& index) const
{
    QSpinBox* hexEdit = qobject_cast<QSpinBox*>(editor);
    int value = getValueFromIndex(index);
    hexEdit->setValue(value);
}

void HexByteDelegate::setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const
{
    QSpinBox* hexEdit = qobject_cast<QSpinBox*>(editor);
    model->setData(index, QString("%1").arg(hexEdit->value(), 2, 16, QChar('0')));
}

void HexByteDelegate::commitAndCloseEditor()
{
    QSpinBox* editor = qobject_cast<QSpinBox*>(sender());
    emit commitData(editor);
    emit closeEditor(editor);
}

int HexByteDelegate::getValueFromIndex(const QModelIndex& index) const
{
    QString data = index.model()->data(index, Qt::DisplayRole).toString();

    return data.toInt(NULL, 16);
}
