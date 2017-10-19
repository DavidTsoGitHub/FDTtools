#include "ResourceComboBoxDelegate.h"

#include "CheckMessage.h"
#include "CheckMessagesDefinition.h"
#include "CheckMessageModel.h"
#include "Icon.h"
#include "Image.h"
#include "IconDefinition.h"
#include "ImageDefinition.h"
#include "Label.h"
#include "LabelDefinition.h"
#include <QComboBox>
#include <QStyledItemDelegate>

ResourceComboBoxDelegate::ResourceComboBoxDelegate(CheckMessageDefinition& checkMessageDefinition, LabelDefinition& labelDefinition, ImageDefinition& imageDefinition, IconDefinition& iconDefinition, QObject* parent) : QStyledItemDelegate(parent),
    m_checkMessageDefinition(checkMessageDefinition),
    m_labelDefinition(labelDefinition),
    m_imageDefinition(imageDefinition),
    m_iconDefinition(iconDefinition)
{
}

QWidget* ResourceComboBoxDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    Q_UNUSED(option);

    if ((index.row() == 0) && (m_checkMessageDefinition.CheckMessages().count() == 0))
    {
        return NULL;
    }

    QString type = m_checkMessageDefinition.CheckMessages().first()->Items().at(index.column() - CheckMessageModel::CheckMessageColumnCount)->Type;
    QComboBox* comboBox = new QComboBox(parent);
    if(type.compare("DynamicLabel") == 0)
    {
        foreach (const Label* label, m_labelDefinition.Labels())
        {
            comboBox->addItem(label->GetMasterText(), QVariant::fromValue(label->GetID()));
        }
    }
    else if(type.compare("DynamicImage") == 0)
    {
        foreach (const Image* image, m_imageDefinition.Images())
        {
            comboBox->addItem(image->GetName(), QVariant::fromValue(image->GetID()));
        }
    }
    else if(type.compare("DynamicIcon") == 0)
    {
        foreach (const Icon* icon, m_iconDefinition.Icons())
        {
            comboBox->addItem(icon->GetName(), QVariant::fromValue(icon->GetID()));
        }
    }
    return comboBox;
}

void ResourceComboBoxDelegate::setEditorData(QWidget* editor, const QModelIndex& index) const
{
    QString type = m_checkMessageDefinition.CheckMessages().first()->Items().at(index.column() - CheckMessageModel::CheckMessageColumnCount)->Type;
    QComboBox* comboBox = qobject_cast<QComboBox*>(editor);

    comboBox->setCurrentText(index.data().toString());
    comboBox->showPopup();
}

void ResourceComboBoxDelegate::setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const
{
    QComboBox* comboBox = qobject_cast<QComboBox*>(editor);

    if (index.data().toString() != comboBox->currentText())
    {
        model->setData(index, comboBox->currentData(), Qt::EditRole);
    }
}

