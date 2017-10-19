#ifndef LABELOBJECTVISITOR_H
#define LABELOBJECTVISITOR_H

#include "ProjectObjectVisitor.h"
#include "ILabelHasFont.h"

class ProjectObject;

class LabelObjectVisitor : public ProjectObjectVisitor
{
public:
    explicit LabelObjectVisitor(bool visitRecursive = true);
    const QList<QPair<ProjectObject*, ILabelHasFont*> > GetFoundObjectsList();

    //ProjectObjectVisitor interface
public:
    void VisitProjectObject(ProjectObject* object);

private:
    QList<UIWidgetBase::uiWidgetType_t> m_widgetTypesToCheck;
    QList<QPair<ProjectObject*, ILabelHasFont*> > m_foundObjects;
};

#endif // LABELOBJECTVISITOR_H
