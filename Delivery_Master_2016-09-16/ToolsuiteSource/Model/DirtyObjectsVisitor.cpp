#include "DirtyObjectsVisitor.h"

DirtyObjectsVisitor::DirtyObjectsVisitor(bool visitRecursive) : ProjectObjectVisitor(visitRecursive)
{
}

const QList<const ProjectObject*> DirtyObjectsVisitor::GetDirtyObjects()
{
    return m_dirtyObjects;
}

void DirtyObjectsVisitor::VisitProjectObject(ProjectObject* object)
{
    if (object->IsDirty()) m_dirtyObjects.append(object);
}
