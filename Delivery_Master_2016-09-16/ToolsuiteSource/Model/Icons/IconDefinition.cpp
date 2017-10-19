#include "IconDefinition.h"

#include "Icon.h"
#include "ProjectObject.h"
#include "ProjectObjectVisitor.h"
#include "UIWidgetDefinitionBase.h"
#include "Logger.h"
#include "Exceptions/ObjectNotFoundException.h"

#include <limits>

IconDefinition::IconDefinition(ProjectObject* parent) : UIWidgetDefinitionBase(QString("Icons"), parent)
{
    m_undefinedIcon = new Icon();
    m_undefinedIcon->SetID(std::numeric_limits<quint32>::max());
    m_undefinedIcon->SetName("Undefined");
}

IconDefinition::~IconDefinition()
{
    delete m_undefinedIcon;
}

quint32 IconDefinition::GetNextAvailableIconId() const
{
    quint32 highestOccupiedIconId = 0;

    foreach (Icon* icon, m_icons)
    {
        if(icon->GetID() >= highestOccupiedIconId)
        {
            highestOccupiedIconId = icon->GetID();
        }
    }

    return highestOccupiedIconId + 1;
}

const QList<Icon*>&IconDefinition::Icons() const
{
    return m_icons;
}

Icon* IconDefinition::GetIconByID(quint32 id)
{
    foreach (Icon* icon, m_icons)
    {
        if(icon->GetID() == id)
        {
            return icon;
        }
    }

    QString info("Icon with ID " + QString::number(id) + " not found. Returning Undefined.");
    LOG_INFO(info);

    return m_undefinedIcon;
}

void IconDefinition::AddIcon(Icon* icon)
{
    m_icons.append(icon);
    icon->SetParent(this);
    NotifyPropertyUpdated();
}

void IconDefinition::RemoveIconAtIndex(int index)
{
    Icon* iconToRemove = m_icons.at(index);
    m_icons.removeAll(iconToRemove);
    delete iconToRemove;
    NotifyPropertyUpdated();
}

QString IconDefinition::GetResource(quint32 id)
{
    return GetIconFolderPath() + "/" + GetIconByID(id)->GetFileName();
}

QString IconDefinition::GetResourceName(quint32 id)
{
    return GetIconByID(id)->GetName();
}

const QString IconDefinition::GetIconFolderPath() const
{
    return m_iconFolderPath;
}

void IconDefinition::SetIconFolderPath(QString iconFolderPath)
{
    m_iconFolderPath = iconFolderPath; 
}

void IconDefinition::AcceptVisitor(ProjectObjectVisitor& visitor)
{
    visitor.VisitIconDefinition(this);
}
