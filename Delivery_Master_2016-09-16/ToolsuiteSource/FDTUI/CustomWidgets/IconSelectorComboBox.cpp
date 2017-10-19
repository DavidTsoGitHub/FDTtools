#include "IconSelectorComboBox.h"

#include "FunctionBlock.h"
#include "Icon.h"
#include "IconDefinition.h"
#include "Project.h"
#include <QComboBox>
#include <QString>
#include <QWidget>

IconSelectorComboBox::IconSelectorComboBox(FunctionBlock* functionBlock, QString iconResourceName, Project* project, QWidget* parent) :
    QComboBox(parent),
    m_iconDefinition(project->GetIconDefinition()),
    m_iconResourceName(iconResourceName),
    m_functionBlock(functionBlock)
{
    connect(project, &Project::PropertyUpdated, this, &IconSelectorComboBox::projectUpdated);

    populate();
    connect(this, static_cast<void(IconSelectorComboBox::*)(const QString&)>(&IconSelectorComboBox::currentIndexChanged), this, &IconSelectorComboBox::slot_currentIndexChanged);
}

void IconSelectorComboBox::populate()
{
    quint32 iconResourceID = m_functionBlock->GetIconResourceID(m_iconResourceName);
    QString selectedItem("Not selected");

    m_signalsEnabled = false;

    clear();

    addItem("Not selected", QVariant(0));
    foreach (Icon* icon, m_iconDefinition.Icons())
    {
        if (icon->GetID() == iconResourceID)
        {
            selectedItem = icon->GetName();
        }
        addItem(icon->GetName(), QVariant(icon->GetID()));
    }

    setCurrentText(selectedItem);

    m_signalsEnabled = true;
}

void IconSelectorComboBox::projectUpdated()
{
    if (m_iconDefinition.IsDirty())
    {
        populate();
    }
}

void IconSelectorComboBox::slot_currentIndexChanged(QString)
{
    if (!m_signalsEnabled) return;

    QVariant resourceID = currentData();
    m_functionBlock->SetIconResourceID(m_iconResourceName, resourceID.toInt());
}
