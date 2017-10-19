#include "CheckMessage.h"
#include "ProjectObjectVisitor.h"

CheckMessage::CheckMessage(ProjectObject* parent) : ProjectObject("Check Message", parent),
    m_name("NewCheckMessage"),
    m_prio(0xFFFFFFFF),
    m_reoccurring(false),
    m_clearCondition(ClearCondition_e::Button),
    m_timeout(0),
    m_trigger(NULL)
{
    m_clearConditionMap.insert(ClearCondition_e::Button, "Button");
    m_clearConditionMap.insert(ClearCondition_e::Trigger, "Trigger");
    m_clearConditionMap.insert(ClearCondition_e::Timeout, "Timeout");
}

CheckMessage::CheckMessage(QString name, quint32 prio, QList<CheckMessageItem *> items, ProjectObject* parent) : ProjectObject("Check Message", parent),
    m_prio(prio),
    m_reoccurring(false),
    m_clearCondition(ClearCondition_e::Button),
    m_timeout(0)
{
    SetName(name);
    m_clearConditionMap.insert(ClearCondition_e::Button, "Button");
    m_clearConditionMap.insert(ClearCondition_e::Trigger, "Trigger");
    m_clearConditionMap.insert(ClearCondition_e::Timeout, "Timeout");


    m_items.append(items);
    SetTrigger(new GCLSignalSink(m_name + "Trigger", "CheckMessageManager", "uint8"));
}

const QString CheckMessage::GetConditionString(void) const
{
    if(m_clearCondition == ClearCondition_e::Trigger)
    {
        return "CHECKMESSAGE_CLEAR_BY_TRIGGER";
    }
    else if(m_clearCondition == ClearCondition_e::Timeout)
    {
        return "CHECKMESSAGE_CLEAR_BY_TIMEOUT";
    }
    else
    {
        return "CHECKMESSAGE_CLEAR_BY_BUTTON";
    }
}

QString CheckMessage::GetName() const
{
    return m_name;
}


void CheckMessage::SetName(const QString &name)
{
    m_name = "";
    for(int i = 0; i < name.length();++i)
    {
        QString chr = name.at(i);
        if(i == 0 && chr.contains(QRegExp("[a-zA-Z_]")))
        {
            m_name.append(chr);

        }
        else if(i > 0 && chr.contains(QRegExp("[a-zA-Z0-9_]")))
        {
            m_name.append(chr);
        }
        else
        {
            m_name.append("_");
        }
    }

    NotifyPropertyUpdated();
}



quint32 CheckMessage::GetPrio() const
{
    return m_prio;
}

void CheckMessage::SetPrio(const quint32 &prio)
{
    m_prio = prio;
    NotifyPropertyUpdated();
}

bool CheckMessage::GetReoccurring() const
{
    return m_reoccurring;
}

void CheckMessage::SetReoccurring(bool reoccurring)
{
    m_reoccurring = reoccurring;
    NotifyPropertyUpdated();
}

CheckMessage::ClearCondition_e CheckMessage::GetClearCondition() const
{
    return m_clearCondition;
}

void CheckMessage::SetClearCondition(const ClearCondition_e &clearCondition)
{
    m_clearCondition = clearCondition;
    NotifyPropertyUpdated();
}

QString CheckMessage::GetClearConditionString(const CheckMessage::ClearCondition_e enumValue)
{
    return m_clearConditionMap.value(enumValue);
}

CheckMessage::ClearCondition_e CheckMessage::GetClearConditionEnum(QString string)
{
    QMap<ClearCondition_e,QString>::Iterator iter = m_clearConditionMap.begin();
    while(iter != m_clearConditionMap.end())
    {
        if(iter.value() == string )
        {
            return iter.key();
        }
        iter++;
    }
    return ClearCondition_e::Trigger;
}

QStringList CheckMessage::GetClearConditionStrings()
{
    return QStringList(m_clearConditionMap.values());
}

quint32 CheckMessage::GetTimeout() const
{
    return m_timeout;
}

void CheckMessage::SetTimeout(const quint32 &timeout)
{
    m_timeout = timeout;
    NotifyPropertyUpdated();
}

GCLSignalSink *CheckMessage::GetTrigger() const
{
    return m_trigger;
}

void CheckMessage::SetTrigger(GCLSignalSink* trigger)
{
    m_trigger = trigger;
    m_signalSinks.clear();
    m_signalSinks.append(m_trigger);
    NotifyPropertyUpdated();
}

QList<CheckMessageItem *> CheckMessage::Items() const
{
    return m_items;
}

void CheckMessage::AddItem(CheckMessageItem* item)
{
    m_items.append(item);
}

void CheckMessage::RemoveItem(CheckMessageItem* item)
{
    m_items.removeOne(item);
}

void CheckMessage::RemoveItem(quint32 index)
{
    int signed_index = static_cast<int>(index);
    if (signed_index < m_items.count())
        m_items.removeAt(signed_index);
}

void CheckMessage::RemoveItemByWidgetID(quint32 associatedWidgetID)
{
    foreach (CheckMessageItem* item, m_items)
    {
        if (item->AssociatedWidgetID == associatedWidgetID)
        {
            m_items.removeOne(item);
        }
    }
}

void CheckMessage::UpdateItemResourceID(quint32 index, quint32 resourceID)
{
    m_items.at(index)->ResourceID = resourceID;
    NotifyPropertyUpdated();
}

void CheckMessage::UpdateItemOrder(QList<quint32> associatedWidgetIDs)
{
    QList<CheckMessageItem*> newItemOrder;

    foreach (quint32 widgetID, associatedWidgetIDs)
    {
        newItemOrder.append(getItemWithWidgetID(widgetID));
    }

    m_items.clear();
    m_items.append(newItemOrder);
}

void CheckMessage::SetAllowedInLayout(quint32 layoutID, bool allowed)
{
    if(!m_allowedInLayout.contains(layoutID))
    {
        m_allowedInLayout.insert(layoutID,allowed);
    }
    else
    {
        m_allowedInLayout.remove(layoutID);
        m_allowedInLayout.insert(layoutID, allowed);
    }
    NotifyPropertyUpdated();
}

QList<quint32> CheckMessage::GetAllowedLayouts()
{

    QList<quint32> allowed;
    foreach (quint32 key, m_allowedInLayout.keys()) {
        if(m_allowedInLayout.value(key) == true)
        {
            allowed.append(key);
        }
    }
    return allowed;
}

bool CheckMessage::IsAllowedInLayout(quint32 layoutID)
{
    if(m_allowedInLayout.contains(layoutID))
    {
        return m_allowedInLayout.value(layoutID);
    }
    else
    {
        //If layout not defined it is not allowed! :P
        return false;
    }
}

void CheckMessage::AcceptVisitor(ProjectObjectVisitor &visitor)
{
    visitor.VisitCheckMessage(this);
}


const QList<GCLSignalSink*> &CheckMessage::GetSignalSinks() const
{
    return m_signalSinks;
}

GCLSignalSink* CheckMessage::GetSignalSink(QString signalName) const
{
    Q_UNUSED(signalName);
    return m_trigger;
}

CheckMessageItem* CheckMessage::getItemWithWidgetID(quint32 widgetID)
{
    foreach (CheckMessageItem* item, m_items)
    {
        if (item->AssociatedWidgetID == widgetID)
        {
            return item;
        }
    }

    return NULL;
}

bool CheckMessage::objectSelfValidation(Project* project, QStringList* errorList)
{
    bool valid = true;

    foreach (CheckMessageItem* item, m_items)
    {
        QList<UIWidgetBase*> widgets;
        project->GetLayoutDefinition().GetCheckMessageLayout()->GetUIWidgets(widgets);
        foreach (UIWidgetBase* widget, widgets)
        {
            if (widget->GetUIWidgetID() != item->AssociatedWidgetID)
            {
                continue;
            }

            if (widget->GetUIWidgetType() == UIWidgetBase::UIWidget_Type_DynamicImage)
            {
                if (project->GetImageDefinition().GetImageByID(item->ResourceID) == &Image::UndefinedImage)
                {
                    errorList->append("CheckMessage: '" + GetName() + "' -> Item with index: '" + QString::number(item->Index) + "' (" + widget->GetUserDefinedName() +
                                      ") was mapped to imageID '" + QString::number(item->ResourceID) + "', but this imageID does no longer match an entry in the Image Manager.");
                    valid = false;
                }
            }

            if (widget->GetUIWidgetType() == UIWidgetBase::UIWidget_Type_DynamicIcon)
            {
                if (project->GetIconDefinition().GetIconByID(item->ResourceID)->GetID() == std::numeric_limits<quint32>::max())
                {
                    errorList->append("CheckMessage: '" + GetName() + "' -> Item with index: '" + QString::number(item->Index) + "' (" + widget->GetUserDefinedName() +
                                      ") was mapped to iconID '" + QString::number(item->ResourceID) + "', but this iconID does no longer match an entry in the Icon Manager.");
                    valid = false;
                }
            }

            if (widget->GetUIWidgetType() == UIWidgetBase::UIWIdget_Type_DynamicLabel)
            {
                if(project->GetLabelDefinition().GetLabel(item->ResourceID) == &Label::UndefinedLabel)
                {
                    if (project->GetIconDefinition().GetIconByID(item->ResourceID)->GetID() == std::numeric_limits<quint32>::max())
                    {
                        errorList->append("CheckMessage: '" + GetName() + "' -> Item with index: '" + QString::number(item->Index) + "' (" + widget->GetUserDefinedName() +
                                          ") was mapped to labelID '" + QString::number(item->ResourceID) + "', but this labelID does no longer match an entry in the Label Editor.");
                        valid = false;
                    }
                }
            }
        }

        if (item->ResourceID == 0)
        {
            errorList->append("CheckMessage: '" + GetName() + "' -> Item with index: '" + QString::number(item->Index) + "' has no assosicated resourceID");
            valid = false;
        }
    }

    return valid;
}
