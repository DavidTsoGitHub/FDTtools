#include "MenuItemPropertiesWidget.h"

#include "BSPDisplay.h"
#include "Image.h"
#include "ImageDefinition.h"
#include "Label.h"
#include "LabelDefinition.h"
#include "MenuItem.h"
#include "Project.h"
#include "RegEx/RegExPatterns.h"

#include <QCheckBox>
#include <QComboBox>
#include <QLineEdit>
#include <QModelIndex>
#include <QWidget>

MenuItemPropertiesWidget::MenuItemPropertiesWidget(Project* project, QWidget *parent) :
    ProjectObjectPropertiesWidget(project, parent),
    m_project(project),
    m_labelDefinition(m_project->GetLabelDefinition()),
    m_imageDefinition(m_project->GetImageDefinition()),
    m_menuItem(NULL),
    m_menuItemNameEditor(NULL),
    m_labelIDCombobox(NULL),
    m_imageIDCombobox(NULL),
    m_wrapCheckBox(NULL)
{
    connect(m_project, &Project::PropertyUpdated, this, &MenuItemPropertiesWidget::projectUpdated);
}

MenuItemPropertiesWidget::~MenuItemPropertiesWidget()
{
}

void MenuItemPropertiesWidget::SetMenuItem(MenuItem* menuItem)
{
    m_menuItem = menuItem;
    SetProjectObject(m_menuItem);
}

void MenuItemPropertiesWidget::projectUpdated()
{
    if (m_labelDefinition.IsDirty())
    {
        refreshLabelIDCombobox();
    }

    if (m_imageDefinition.IsDirty())
    {
        refreshImageIDCombobox();
    }
}

void MenuItemPropertiesWidget::refreshLabelIDCombobox()
{
    m_labelIDCombobox->blockSignals(true);
    m_labelIDCombobox->clear();
    m_labelIDCombobox->setMinimumContentsLength(10);
    m_labelIDCombobox->setSizeAdjustPolicy(QComboBox::AdjustToMinimumContentsLength);
    m_labelIDCombobox->addItem(" ", QVariant(0));
    foreach (Label* label, m_labelDefinition.Labels())
    {
        m_labelIDCombobox->addItem(label->GetMasterText(), QVariant(label->GetID()));
    }
    m_labelIDCombobox->setCurrentText(m_labelDefinition.GetLabel(m_menuItem->GetLabelID())->GetMasterText());
    m_labelIDCombobox->blockSignals(false);

}

void MenuItemPropertiesWidget::labelComboboxChanged(QVariant key)
{
    if (key.toInt() != m_menuItem->GetLabelID())
    {
        m_menuItem->SetLabelID(key.toInt());
    }
}

void MenuItemPropertiesWidget::beforePopulateSignals()
{
    m_menuItemNameEditor = new QLineEdit(m_menuItem->GetName());
    connect(m_menuItemNameEditor, &QLineEdit::textChanged, [=](const QString& newName)
    {
        QString trimmedName(newName);
        trimmedName.replace(RegExPatterns::NotAcceptedCodeCharacters(), "");
        if (trimmedName.compare(m_menuItem->GetName()) == 0) return;
        m_menuItemNameEditor->setText(trimmedName);
        m_menuItem->SetName(trimmedName);
    });
    AddEditableData("Name", m_menuItemNameEditor);

    m_labelIDCombobox = new QComboBox();
    refreshLabelIDCombobox();
    connect(m_labelIDCombobox, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged), [=] ()
    {
        labelComboboxChanged(m_labelIDCombobox->currentData());
    });

    AddEditableData("Label", m_labelIDCombobox);

    if (m_project->GetBSPSpecification().GetDisplay()->GetRenderingType() == BSPDisplay::HW)
    {
        m_imageIDCombobox = new QComboBox();
        refreshImageIDCombobox();
        AddEditableData("Menu Image", m_imageIDCombobox);
    }

    m_wrapCheckBox = new QCheckBox();
    m_wrapCheckBox->setChecked(m_menuItem->IsSubMenuWrapped());
    connect(m_wrapCheckBox, &QCheckBox::toggled, [=](bool toggled)
    {
       if (toggled != m_menuItem->IsSubMenuWrapped())
       {
           m_menuItem->SetSubMenuWrapped(toggled);
       }
    });
    AddEditableData("Wrap submenu", m_wrapCheckBox);

}

void MenuItemPropertiesWidget::refreshImageIDCombobox()
{
    m_imageIDCombobox->clear();
    m_imageIDCombobox->setMinimumContentsLength(10);
    m_imageIDCombobox->setSizeAdjustPolicy(QComboBox::AdjustToMinimumContentsLength);
    m_imageIDCombobox->addItem(" ");
    foreach (Image* image, m_imageDefinition.Images())
    {
        if(!image->GetUseForNonAnimations())
            continue;

        m_imageIDCombobox->addItem(image->GetName(), QVariant(image->GetID()));
    }
    m_imageIDCombobox->setCurrentIndex(m_imageIDCombobox->findData(m_menuItem->GetImageID()));
    connect(m_imageIDCombobox, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &MenuItemPropertiesWidget::imageComboboxIndexChanged);

}

void MenuItemPropertiesWidget::imageComboboxIndexChanged(int index)
{
    quint32 newId = m_imageIDCombobox->itemData(index).toUInt();
    if (newId != m_menuItem->GetImageID())
    {
        m_menuItem->setImageID(newId);
    }
}
