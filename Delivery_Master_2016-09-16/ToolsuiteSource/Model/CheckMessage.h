#ifndef CHECKMESSAGE
#define CHECKMESSAGE

#include "CheckMessageItem.h"
#include "IGCLSignalSink.h"
#include "ISelfValidation.h"
#include <QMap>
#include <QString>

class CheckMessage : public ProjectObject, public IGCLSignalSink, public ISelfValidation
{
public:

    enum class ClearCondition_e { Trigger, Timeout, Button };

    explicit CheckMessage(ProjectObject* parent = 0);
    CheckMessage(QString name, quint32 prio, QList<CheckMessageItem*> items, ProjectObject* parent = 0);

    const QString GetConditionString(void) const;

    QString GetName() const;
    void SetName(const QString &name);

    quint32 GetPrio() const;
    void SetPrio(const quint32 &prio);

    bool GetReoccurring() const;
    void SetReoccurring(bool reoccurring);

    ClearCondition_e GetClearCondition() const;
    void SetClearCondition(const ClearCondition_e &clearCondition);
    QString GetClearConditionString(const ClearCondition_e enumValue);
    ClearCondition_e GetClearConditionEnum(QString string);
    QStringList GetClearConditionStrings();


    quint32 GetTimeout() const;
    void SetTimeout(const quint32 &timeout);

    GCLSignalSink* GetTrigger() const;
    void SetTrigger(GCLSignalSink* trigger);

    QList<CheckMessageItem*> Items() const;
    void AddItem(CheckMessageItem* item);
    void RemoveItem(CheckMessageItem* item);
    void RemoveItem(quint32 index);
    void RemoveItemByWidgetID(quint32 associatedWidgetID);
    void UpdateItemResourceID(quint32 index, quint32 resourceID);
    void UpdateItemOrder(QList<quint32> associatedWidgetIDs);

    bool IsAllowedInLayout(quint32 layoutID);
    void SetAllowedInLayout(quint32 layoutID, bool allowed);
    QList<quint32> GetAllowedLayouts(void);

private:
    QString m_name;
    quint32 m_prio;
    bool m_reoccurring;
    ClearCondition_e m_clearCondition;
    quint32 m_timeout;
    GCLSignalSink* m_trigger;
    QList<CheckMessageItem*> m_items;
    QMap<quint32, bool> m_allowedInLayout;
    QList<GCLSignalSink*> m_signalSinks;

    QMap<ClearCondition_e, QString> m_clearConditionMap;

    CheckMessageItem* getItemWithWidgetID(quint32 widgetID);


    // ProjectObject interface
protected:
    void AcceptVisitor(ProjectObjectVisitor &visitor);

    // IGCLSignalSink interface
public:
    const QList<GCLSignalSink *> &GetSignalSinks() const;
    GCLSignalSink *GetSignalSink(QString signalName) const;

    // ISelfValidation interface
public:
    bool objectSelfValidation(Project* project, QStringList* errorList);
};

#endif // CHECKMESSAGE

