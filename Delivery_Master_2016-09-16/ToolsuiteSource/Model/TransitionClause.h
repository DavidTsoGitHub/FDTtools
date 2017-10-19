#ifndef TRANSITIONCLAUSE_H
#define TRANSITIONCLAUSE_H

#include "LayoutTransitionTypes.h"
#include "ProjectObject.h"

class TransitionCondition;

class TransitionClause : public ProjectObject
{
public:
    TransitionClause(quint16 referenceNumber, ProjectObject* parent = 0, QString componentName = "");
    ~TransitionClause();

    LayoutTransitionTypes::TransitionConnective GetConnective();
    void SetConnective(LayoutTransitionTypes::TransitionConnective transitionConnective);

    TransitionCondition* GetFirstCondition();

    //void SetCondition(TransitionCondition& transitionCondition, LayoutTransitionTypes::TransitionConditionOrder order);

    void AddSecondCondition();
    void RemoveSecondCondition();
    TransitionCondition* GetSecondCondition();

    quint16 GetReferenceNumber();
    QString GetComponentName();
    void SetComponentName(QString newComponentName);
    void SetSignalName(QString name);

private:
    void addFirstCondition();

    TransitionCondition* m_firstCondition;
    TransitionCondition* m_secondCondition;
    LayoutTransitionTypes::TransitionConnective m_connective;

    QString m_gclComponentName;
    QString m_gclPortName;

    quint16 m_referenceNumber; //injected with unique global counter
};

#endif // TRANSITIONCLAUSE_H
