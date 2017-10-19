#include "SelfValidationVisitor.h"
#include "ISelfValidation.h"

SelfValidationVisitor::SelfValidationVisitor(Project* project, bool visitRecursive) :
    ProjectObjectVisitor(visitRecursive),
    m_project(project),
    m_validationStatus(true)
{
}

const QStringList SelfValidationVisitor::GetErrors() const
{
    return m_errors;
}

bool SelfValidationVisitor::GetStatus() const
{
    return m_validationStatus;
}

void SelfValidationVisitor::VisitProjectObject(ProjectObject *object)
{
    if (ISelfValidation* selfObject = dynamic_cast<ISelfValidation*>(object))
    {
        m_validationStatus &= selfObject->objectSelfValidation(m_project, &m_errors);
    }
}
