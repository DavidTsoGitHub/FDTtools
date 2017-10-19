#include "TransitionClause.h"

#include "TransitionCondition.h"
#include "Layout.h"

TransitionClause::TransitionClause(quint16 referenceNumber, ProjectObject *parent, QString componentName) : ProjectObject("TransitionClause", parent),
    m_firstCondition(NULL),
    m_secondCondition(NULL),
    m_referenceNumber(referenceNumber)
{
    if (componentName == "")
    {
        QString parentName = parent->objectName();
        m_gclComponentName = "Transition_" + parentName.replace(" ", "");
    }
    else
    {
        m_gclComponentName = "Transition_" + componentName;
    }

    m_gclPortName = "Transition_" + QString::number(m_referenceNumber);

    addFirstCondition();
    m_connective = LayoutTransitionTypes::NONE;
}

TransitionClause::~TransitionClause()
{
    delete m_firstCondition;
    delete m_secondCondition;
}

LayoutTransitionTypes::TransitionConnective TransitionClause::GetConnective()
{
    return m_connective;
}

void TransitionClause::SetConnective(LayoutTransitionTypes::TransitionConnective transitionConnective)
{
    m_connective = transitionConnective;
    NotifyPropertyUpdated();
    emit PropertyUpdatedRedrawNeeded();
}

TransitionCondition* TransitionClause::GetFirstCondition()
{
    return m_firstCondition;
}


TransitionCondition* TransitionClause::GetSecondCondition()
{
    return m_secondCondition;
}

void TransitionClause::AddSecondCondition()
{
    if (m_secondCondition != NULL)
    {
        RemoveSecondCondition();
    }
    QString gclPortName = m_gclPortName + "_2";
    m_secondCondition = new TransitionCondition(m_gclComponentName, gclPortName, this);
    NotifyPropertyUpdated();
}

void TransitionClause::RemoveSecondCondition()
{
    delete m_secondCondition;
    m_secondCondition = NULL;
}

void TransitionClause::addFirstCondition()
{
    QString gclPortName = m_gclPortName + "_1";

    m_firstCondition = new TransitionCondition(m_gclComponentName, gclPortName, this);
}

quint16 TransitionClause::GetReferenceNumber() {
    return m_referenceNumber;
}

QString TransitionClause::GetComponentName()
{
    return m_gclComponentName;
}

void TransitionClause::SetComponentName(QString newComponentName)
{
    m_gclComponentName = newComponentName;
}

void TransitionClause::SetSignalName(QString name)
{
    m_gclPortName = name;
}
