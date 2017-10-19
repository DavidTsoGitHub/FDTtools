#include "Icon.h"
#include "ProjectObjectVisitor.h"

#include <QFileInfo>

Icon::Icon() : ProjectObject(QString("Icon"))
{}

quint32 Icon::GetID() const
{
    return m_id;
}

void Icon::SetID(const quint32 id)
{
    m_id = id;
    NotifyPropertyUpdated();
}

const QString Icon::GetName() const
{
    return m_name;
}

void Icon::SetName(const QString& name)
{
    m_name = name;
    setObjectName(name);
    NotifyPropertyUpdated();
}

const QString Icon::GetFileName() const
{
    return QFileInfo(m_filePath).fileName();
}

const QString Icon::GetPath() const
{
    return m_filePath;
}

void Icon::SetPath(const QString& path)
{
    m_filePath = path;
    NotifyPropertyUpdated();
}

const QString Icon::GetDescription() const
{
    return m_description;
}

void Icon::SetDescription(const QString& description)
{
    m_description = description;
    NotifyPropertyUpdated();
}

void Icon::AcceptVisitor(ProjectObjectVisitor& visitor)
{
    visitor.VisitIcon(this);
}
