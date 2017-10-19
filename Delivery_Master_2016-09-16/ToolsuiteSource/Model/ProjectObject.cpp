#include "ProjectObject.h"
#include "ProjectObjectVisitor.h"
#include <QTimer>

#include "Logger.h"

ProjectObject::ProjectObject(const QString& objectName, ProjectObject* parent) :
    m_parent(parent),
    m_enabled(true),
    m_dirty(false),
    m_propertyUpdateTimer(new QTimer(this)),
    m_propertyUpdatedRedrawNeededTimer(new QTimer(this))
{
    setObjectName(objectName);
    if (parent != NULL) parent->addChild(this);

    m_propertyUpdateTimer->setSingleShot(true);
    connect(m_propertyUpdateTimer, &QTimer::timeout, [=]()
    {
        emit PropertyUpdated();
    });

    m_propertyUpdatedRedrawNeededTimer->setSingleShot(true);
    connect(m_propertyUpdatedRedrawNeededTimer, &QTimer::timeout, [=] ()
    {
       emit PropertyUpdatedRedrawNeeded();
    });
}

ProjectObject::~ProjectObject()
{
    foreach (ProjectObject* child, m_children)
    {
        disconnect(child, &ProjectObject::destroyed, this, &ProjectObject::removeDestroyedChild);
        disconnect(child, &ProjectObject::PropertyUpdated, this, &ProjectObject::PropertyUpdated);
        delete child;
    }
}

ProjectObject* ProjectObject::GetParent() const
{
    return m_parent;
}

void ProjectObject::SetParent(ProjectObject* parent)
{
    if (m_parent != NULL) m_parent->removeChild(this);
    m_parent = parent;
    if (m_parent != NULL) m_parent->addChild(this);
}

const ProjectObjectList& ProjectObject::GetChildren() const
{
    return m_children;
}

const ProjectObjectList ProjectObject::GetEnabledChildren() const
{
    ProjectObjectList enabledChildren;

    foreach (ProjectObject* child, m_children)
    {
        if (child->GetEnabled())
        {
            enabledChildren.append(child);
        }
    }

    return enabledChildren;
}

bool ProjectObject::GetEnabled() const
{
    return m_enabled;
}

void ProjectObject::SetEnabled(bool enabled)
{
    m_enabled = enabled;
}

bool ProjectObject::IsDirty() const
{
    return m_dirty;
}

bool ProjectObject::IsDirtyRecursive() const
{
    if (m_dirty) return true;

    foreach (ProjectObject* child, m_children)
    {
        if (child->IsDirtyRecursive()) return true;
    }

    return false;
}

void ProjectObject::ClearDirtyRecursive()
{
    m_propertyUpdateTimer->stop();
    m_dirty = false;
    foreach (ProjectObject* child, m_children)
    {
        child->ClearDirtyRecursive();
    }
}

void ProjectObject::Accept(ProjectObjectVisitor& visitor)
{
    if (visitor.VisitRecursive)
    {
        foreach (ProjectObject* child, m_children)
        {
            child->Accept(visitor);
        }
    }

    AcceptVisitor(visitor);
}

void ProjectObject::AcceptVisitor(ProjectObjectVisitor& visitor)
{
    visitor.VisitProjectObject(this);
}

void ProjectObject::NotifyPropertyUpdated()
{
    if (signalsBlocked()) return;
    m_dirty = true;
    m_propertyUpdateTimer->start(1000);
}

void ProjectObject::NotifyPropertyUpdatedRedrawNeeded()
{
    if (signalsBlocked()) return;
    m_propertyUpdatedRedrawNeededTimer->start(1000);
}

void ProjectObject::addChild(ProjectObject* child)
{
    m_children.append(child);
    connect(child, &ProjectObject::destroyed, this, &ProjectObject::removeDestroyedChild);
    connect(child, &ProjectObject::PropertyUpdated, this, &ProjectObject::PropertyUpdated);
}

void ProjectObject::removeDestroyedChild()
{
    m_children.removeAll(static_cast<ProjectObject*>(sender()));
}

void ProjectObject::removeChild(ProjectObject* child)
{
    m_children.removeAll(child);
    disconnect(child, &ProjectObject::PropertyUpdated, this, &ProjectObject::PropertyUpdated);
    disconnect(child, &ProjectObject::destroyed, this, &ProjectObject::removeDestroyedChild);
}
