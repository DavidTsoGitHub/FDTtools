#ifndef TRANSITIONCONDITION_H
#define TRANSITIONCONDITION_H

#include "IGCLSignalSink.h"
#include "LayoutTransitionTypes.h"

class GCLSignalSink;

class TransitionCondition : public ProjectObject, public IGCLSignalSink
{
public:
    TransitionCondition(QString gclComponentName, QString gclPortName, ProjectObject* parent);
    ~TransitionCondition();

    LayoutTransitionTypes::TransitionCompare GetCompareOperator(void);
    void SetCompareOperator(LayoutTransitionTypes::TransitionCompare compareOperator);

    quint32 GetConditionValue(void);
    void SetConditionValue(quint32 conditionValue);

    void SetSignalSink(GCLSignalSink* sink);

private:
    LayoutTransitionTypes::TransitionCompare m_compareOperator;
    quint32 m_conditionValue;
    QList<GCLSignalSink*> m_signalSinks;

    // IGCLSignalSink interface
public:
    const QList<GCLSignalSink *> &GetSignalSinks() const;
    GCLSignalSink *GetSignalSink(QString signalName) const;
};

#endif // TRANSITIONCONDITION_H
