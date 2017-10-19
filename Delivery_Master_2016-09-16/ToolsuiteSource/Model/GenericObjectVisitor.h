#ifndef GENERICOBJECTVISITOR_H
#define GENERICOBJECTVISITOR_H

#include <QList>

#include "ProjectObjectVisitor.h"

class GenericObjectVisitor : public ProjectObjectVisitor
{
public:
    explicit GenericObjectVisitor(bool visitRecursive = true, bool populateAllProjectObjectsList = false);

    const QList<ProjectObject*> GetAllProjectObjects() const;
private:
    QList<ProjectObject*> m_allProjectObjects;

    bool m_populateAllProjectObjectsList;

    // ProjectObjectVisitor interface
public:
    void VisitProjectObject(ProjectObject* projectObject);
};

#endif // GENERICOBJECTVISITOR_H
