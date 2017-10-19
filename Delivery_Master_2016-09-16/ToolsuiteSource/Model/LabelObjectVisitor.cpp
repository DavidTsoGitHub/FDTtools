#include "LabelObjectVisitor.h"

LabelObjectVisitor::LabelObjectVisitor(bool visitRecursive) :
    ProjectObjectVisitor(visitRecursive)
{
}

void LabelObjectVisitor::VisitProjectObject(ProjectObject* object)
{
    if (ILabelHasFont* label = dynamic_cast<ILabelHasFont*>(object))
    {
        m_foundObjects.append(qMakePair(object, label));
    }
}

const QList<QPair<ProjectObject*, ILabelHasFont*> > LabelObjectVisitor::GetFoundObjectsList()
{
    return m_foundObjects;
}
