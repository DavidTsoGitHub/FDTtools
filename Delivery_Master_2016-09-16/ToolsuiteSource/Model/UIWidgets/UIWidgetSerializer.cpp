#include "UIWidgetSerializer.h"

#include "TransitionCondition.h"
#include "UIWidgetBase.h"
#include "RegEx/RegExPatterns.h"

#include <QXmlStreamWriter>

void UIWidgetSerializer::Serialize(QXmlStreamWriter& xmlWriter, UIWidgetBase* uiWidget)
{
    quint32 numberOfProperties = uiWidget->GetTotalPropertyCount();

    xmlWriter.writeAttribute("Type", uiWidget->GetUIWidgetTypeName());
    xmlWriter.writeAttribute("WidgetID", QString::number(uiWidget->GetUIWidgetID()));

    for (quint32 i = 0; i < numberOfProperties; ++i)
    {
        QString fieldName = uiWidget->GetPropertyHeader(i).replace(RegExPatterns::NotAcceptedCodeCharacters(), "");
        xmlWriter.writeAttribute(fieldName, uiWidget->GetPropertyData(i).toString());
    }

    if (uiWidget->GetAnimationTriggers(1).count() > 0)
    {
        xmlWriter.writeStartElement("Animation1_Properties");
        xmlWriter.writeAttribute("BehaviorOnLoad", QString::number(uiWidget->GetAnimationOnLoadBehavior(1)));
        xmlWriter.writeAttribute("BehaviorOnTriggerActive", QString::number(uiWidget->GetAnimationOnTriggerActiveBehavior(1)));
        xmlWriter.writeAttribute("BehaviorOnTriggerInactive", QString::number(uiWidget->GetAnimationOnTriggerInactiveBehavior(1)));
        xmlWriter.writeStartElement("Animation1_Triggers");

        foreach(TransitionClause* transition, uiWidget->GetAnimationTriggers(1))
        {
            xmlWriter.writeStartElement("Trigger");

            TransitionCondition* firstCondition = transition->GetFirstCondition();
            xmlWriter.writeStartElement("FirstCondition");
            xmlWriter.writeAttribute("Value", QString::number(firstCondition->GetConditionValue()));
            xmlWriter.writeAttribute("CompareCondition", QString::number(firstCondition->GetCompareOperator()));
            xmlWriter.writeEndElement(); // FirstCondition

            xmlWriter.writeStartElement("Connective");
            xmlWriter.writeAttribute("Connective", QString::number(transition->GetConnective()));
            xmlWriter.writeEndElement(); // Connective

            if (transition->GetConnective() != LayoutTransitionTypes::NONE)
            {
                TransitionCondition* secondCondition = transition->GetSecondCondition();
                xmlWriter.writeStartElement("SecondCondition");
                xmlWriter.writeAttribute("Value", QString::number(secondCondition->GetConditionValue()));
                xmlWriter.writeAttribute("CompareCondition", QString::number(secondCondition->GetCompareOperator()));
                xmlWriter.writeEndElement(); // SecondCondition
            }
            xmlWriter.writeEndElement(); // Triggers
        }
        xmlWriter.writeEndElement(); // Animation1_Triggers
        xmlWriter.writeEndElement(); // Animation1_Properties
    }

    if (uiWidget->GetAnimationTriggers(2).count() > 0)
    {
        xmlWriter.writeStartElement("Animation2_Properties");
        xmlWriter.writeAttribute("BehaviorOnLoad", QString::number(uiWidget->GetAnimationOnLoadBehavior(2)));
        xmlWriter.writeAttribute("BehaviorOnTriggerActive", QString::number(uiWidget->GetAnimationOnTriggerActiveBehavior(2)));
        xmlWriter.writeAttribute("BehaviorOnTriggerInactive", QString::number(uiWidget->GetAnimationOnTriggerInactiveBehavior(2)));
        xmlWriter.writeStartElement("Animation2_Triggers");

        foreach(TransitionClause* transition, uiWidget->GetAnimationTriggers(2))
        {
            xmlWriter.writeStartElement("Trigger");

            TransitionCondition* firstCondition = transition->GetFirstCondition();
            xmlWriter.writeStartElement("FirstCondition");
            xmlWriter.writeAttribute("Value", QString::number(firstCondition->GetConditionValue()));
            xmlWriter.writeAttribute("CompareCondition", QString::number(firstCondition->GetCompareOperator()));
            xmlWriter.writeEndElement(); // FirstCondition

            xmlWriter.writeStartElement("Connective");
            xmlWriter.writeAttribute("Connective", QString::number(transition->GetConnective()));
            xmlWriter.writeEndElement(); // Connective

            if (transition->GetConnective() != LayoutTransitionTypes::NONE)
            {
                TransitionCondition* secondCondition = transition->GetSecondCondition();
                xmlWriter.writeStartElement("SecondCondition");
                xmlWriter.writeAttribute("Value", QString::number(secondCondition->GetConditionValue()));
                xmlWriter.writeAttribute("CompareCondition", QString::number(secondCondition->GetCompareOperator()));
                xmlWriter.writeEndElement(); // SecondCondition
            }
            xmlWriter.writeEndElement(); // Triggers
        }
        xmlWriter.writeEndElement(); // Animation2_Triggers
        xmlWriter.writeEndElement(); // Animation2_Properties
    }
}
