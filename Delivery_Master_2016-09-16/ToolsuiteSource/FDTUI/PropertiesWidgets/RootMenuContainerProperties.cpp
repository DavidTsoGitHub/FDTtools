#include "RootMenuContainerProperties.h"

#include "MenuItem.h"
#include "Project.h"
#include "RegEx/RegExPatterns.h"
#include "RootMenuItemContainer.h"

#include <QCheckBox>
#include <QComboBox>
#include <QModelIndex>
#include <QWidget>

RootMenuContainerPropertiesWidget::RootMenuContainerPropertiesWidget(Project* project, QWidget *parent) :
    ProjectObjectPropertiesWidget(project, parent),
    m_rootMenuItemContainer(NULL),
    m_wrapCheckBox(NULL),
    m_startupMenuItemComboBox(NULL)
{
}

RootMenuContainerPropertiesWidget::~RootMenuContainerPropertiesWidget()
{
}

void RootMenuContainerPropertiesWidget::SetRootMenuContainer(RootMenuItemContainer* rootMenuItemContainer)
{
    m_rootMenuItemContainer = rootMenuItemContainer;
    SetProjectObject(rootMenuItemContainer);
}

void RootMenuContainerPropertiesWidget::slot_wrapCheckBox_toggled(bool checked)
{
    if (checked != m_rootMenuItemContainer->IsWrapped())
    {
        m_rootMenuItemContainer->SetWrapped(checked);
    }
}

void RootMenuContainerPropertiesWidget::slot_startupMenuItemComboBox_valueChanged(int index)
{
    if(index == 0)
    {
        m_rootMenuItemContainer->SetStartupMenuItem(NULL);

        return;
    }

    quint32 value = m_startupMenuItemComboBox->itemData(index).toUInt();
    foreach (MenuItem* menuItem, m_rootMenuItemContainer->MenuItems())
    {
        if (menuItem->GetID() == value)
        {
            m_rootMenuItemContainer->SetStartupMenuItem(menuItem);
            return;
        }
    }
}

void RootMenuContainerPropertiesWidget::populateComboBoxes()
{
    const MenuItem* const currentStartupMenuItem = m_rootMenuItemContainer->GetStartupMenuItem();

    m_startupMenuItemComboBox->addItem("<None>", 0);

    foreach (MenuItem* menuItem, m_rootMenuItemContainer->MenuItems())
    {
        m_startupMenuItemComboBox->addItem(menuItem->GetName(), menuItem->GetID());
        if (currentStartupMenuItem != NULL && currentStartupMenuItem->GetName() == menuItem->GetName())
        {
            m_startupMenuItemComboBox->setCurrentText(menuItem->GetName());
        }
    }
}

void RootMenuContainerPropertiesWidget::beforePopulateSignals()
{
    m_wrapCheckBox = new QCheckBox();
    m_wrapCheckBox->setChecked(m_rootMenuItemContainer->IsWrapped());
    AddEditableData("Top Menu Wrapped", m_wrapCheckBox);
    connect(m_wrapCheckBox, &QAbstractButton::toggled, this, &RootMenuContainerPropertiesWidget::slot_wrapCheckBox_toggled);

    m_startupMenuItemComboBox = new QComboBox();
    m_startupMenuItemComboBox->setMaximumWidth(150);

    populateComboBoxes();

    AddEditableData("Startup MenuItem", m_startupMenuItemComboBox);
    connect(m_startupMenuItemComboBox, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &RootMenuContainerPropertiesWidget::slot_startupMenuItemComboBox_valueChanged);
}
