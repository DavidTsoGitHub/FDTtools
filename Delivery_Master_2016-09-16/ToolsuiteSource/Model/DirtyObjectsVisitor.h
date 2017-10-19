#ifndef DIRTYOBJECTSVISITOR_H
#define DIRTYOBJECTSVISITOR_H

#include "ProjectObjectVisitor.h"

class DirtyObjectsVisitor : public ProjectObjectVisitor
{
public:
    explicit DirtyObjectsVisitor(bool visitRecursive = true);

    const QList<const ProjectObject*> GetDirtyObjects();

private:
    QList<const ProjectObject*> m_dirtyObjects;

    // ProjectObjectVisitor interface
public:
    void VisitProjectObject(ProjectObject* object);
};

#endif // DIRTYOBJECTSVISITOR_H
