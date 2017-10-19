#include "FontComboBoxDelegate.h"
#include "LabelModel.h"

#include "LabelDefinition.h"
#include <QComboBox>
#include <QStyledItemDelegate>

FontComboBoxDelegate::FontComboBoxDelegate(LabelDefinition& labelDefinition) :
    m_labelDefinition(labelDefinition)
{
}

QWidget* FontComboBoxDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    if (index.column() >= LabelModel::LabelColumnCount && index.column() % 2 == (1 - (LabelModel::LabelColumnCount % 2)))
    {
        QComboBox* fontComboBox = new QComboBox(parent);
        Font* defaultFont = m_labelDefinition.GetFontByID(UndefinedFontID);
        fontComboBox->addItem(defaultFont->Description, QVariant::fromValue(defaultFont->ID));
        foreach (Font* font, m_labelDefinition.Fonts())
        {
            QString descr;
            if(font->Description == "")
            {
                descr = QString(font->BDFPath).split("/").last();
            }
            else
            {
                descr = font->Description;
            }
            fontComboBox->addItem(descr, QVariant::fromValue(font->ID));
        }
        return fontComboBox;
    }
    else
    {
        return QStyledItemDelegate::createEditor(parent, option, index);
    }
}

void FontComboBoxDelegate::setEditorData(QWidget* editor, const QModelIndex& index) const
{
    if (index.column() >= LabelModel::LabelColumnCount && index.column() % 2 == (1 - (LabelModel::LabelColumnCount % 2)))
    {
        QComboBox* fontComboBox = qobject_cast<QComboBox*>(editor);
        fontComboBox->setCurrentText(index.data().toString());
        fontComboBox->showPopup();
    }
    else
    {
        QStyledItemDelegate::setEditorData(editor, index);
    }
}

void FontComboBoxDelegate::setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const
{
    if (index.column() >= LabelModel::LabelColumnCount && index.column() % 2 == (1 - (LabelModel::LabelColumnCount % 2)))
    {
        QComboBox* fontComboBox = qobject_cast<QComboBox*>(editor);

        if (index.data().toString() != fontComboBox->currentText())
        {
            model->setData(index, fontComboBox->currentData());
        }
    }
    else
    {
        QStyledItemDelegate::setModelData(editor, model, index);
    }
}
