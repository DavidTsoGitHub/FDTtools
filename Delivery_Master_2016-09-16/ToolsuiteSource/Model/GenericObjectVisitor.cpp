#include "GenericObjectVisitor.h"

GenericObjectVisitor::GenericObjectVisitor(bool visitRecursive, bool populateAllProjectObjectsList) :
    ProjectObjectVisitor(visitRecursive),
    m_populateAllProjectObjectsList(populateAllProjectObjectsList)
{
}

const QList<ProjectObject *> GenericObjectVisitor::GetAllProjectObjects() const
{
    return m_allProjectObjects;
}

void GenericObjectVisitor::VisitProjectObject(ProjectObject* projectObject)
{
    if (m_populateAllProjectObjectsList)
    {
        m_allProjectObjects.append(projectObject);
    }
}
