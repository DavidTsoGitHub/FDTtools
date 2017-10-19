#ifndef SELFVALIDATIONVISITOR_H
#define SELFVALIDATIONVISITOR_H

#include "ProjectObjectVisitor.h"

class SelfValidationVisitor : public ProjectObjectVisitor
{
public:
    explicit SelfValidationVisitor(Project* project, bool visitRecursive = true);
    const QStringList GetErrors() const;
    bool GetStatus() const;

private:
    Project* m_project;
    QStringList m_errors;
    bool m_validationStatus;

    //ProjectObjectVisitor interface
public:
    void VisitProjectObject(ProjectObject* object);
};

#endif // SELFVALIDATIONVISITOR_H
