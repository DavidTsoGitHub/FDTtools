#include "DynamicImageUIWidgetGenerator.h"

#include "TransitionCondition.h"

DynamicImageUIWidgetGenerator::DynamicImageUIWidgetGenerator(QList<UIWidgetBase *> uiWidgetList) :
    m_uiWidgetList(uiWidgetList)
{
}


DynamicImageUIWidgetGenerator::~DynamicImageUIWidgetGenerator()
{
}


void DynamicImageUIWidgetGenerator::generateConfigurationSourceFile()
{
    QStringList fileLines;
    QStringList configLines;
    QStringList animationTriggersPropertiesLines;
    QStringList animationTriggerLines;

    fileLines.append("/********************************* Includes **********************************/");
    fileLines.append("#include \"DynamicImageWidgetCfg.h\"");
    fileLines.append("#include \"GCL_Widgets.h\"");

    if (m_uiWidgetList.count() != 0)
    {
        bool animationTriggersUsed = false;
        foreach (UIWidgetBase* widget, m_uiWidgetList)
        {
            if (widget->GetUIWidgetType() == UIWidgetBase::UIWidget_Type_DynamicImage)
            {   
                DynamicImageWidget* dynamicImageWidget = static_cast<DynamicImageWidget*>(widget);

                QString animation1TriggersPropertiesString = "NULL";
                QString animation1TriggerString = "NULL";
                quint8 animation1TriggerCount = 0;

                if ((dynamicImageWidget->GetAnimationUseTriggers(1) == true) && (dynamicImageWidget->GetAnimationTriggers(1).count() > 0))
                {
                    animationTriggersUsed = true;

                    animation1TriggersPropertiesString = dynamicImageWidget->GetUIWidgetTypeName() + "_" + QString::number(dynamicImageWidget->GetUIWidgetID()) + "_Animation1_Triggers_Properties";

                    animationTriggersPropertiesLines.append("static animation_triggers_properties_t " + animation1TriggersPropertiesString + " =\n");
                    animationTriggersPropertiesLines.append("{\n");
                    animationTriggersPropertiesLines.append("    .onLoadBehavior = " + UIWidgetBase::GetOnLoadBehaviorString(widget->GetAnimationOnLoadBehavior(1)) + ",\n");
                    animationTriggersPropertiesLines.append("    .onTriggerActiveBehavior = " + UIWidgetBase::GetOnTriggerActiveBehaviorString(widget->GetAnimationOnTriggerActiveBehavior(1)) + ",\n");
                    animationTriggersPropertiesLines.append("    .onTriggerInactiveBehavior = " + UIWidgetBase::GetOnTriggerInactiveBehaviorString(widget->GetAnimationOnTriggerInactiveBehavior(1)) + "\n");
                    animationTriggersPropertiesLines.append("};\n");
                    animationTriggersPropertiesLines.append("");

                    animation1TriggersPropertiesString = "&" + animation1TriggersPropertiesString;

                    animation1TriggerCount = dynamicImageWidget->GetAnimationTriggers(1).count();
                    animation1TriggerString = dynamicImageWidget->GetUIWidgetTypeName() + "_" + QString::number(dynamicImageWidget->GetUIWidgetID()) + "_Animation1_Triggers";
                    animationTriggerLines.append("static signal_transition_t " + animation1TriggerString + "[" + QString::number(dynamicImageWidget->GetAnimationTriggers(1).count()) +"] =");
                    animationTriggerLines.append("{");
                    QStringList triggers;
                    foreach (TransitionClause* trigger, dynamicImageWidget->GetAnimationTriggers(1))
                    {
                        QStringList triggerString;
                        TransitionCondition* firstCondition = trigger->GetFirstCondition();
                        const GCLSignalSink* firstConditionSink = firstCondition->GetSignalSinks().first();
                        triggerString.append("    { .firstConditionGCLSink = GCL_Read_" + firstConditionSink->ComponentName() + "_" + firstConditionSink->SignalName() +
                                           ", .firstConditionCompareOperator = " + getCompareString(firstCondition->GetCompareOperator()) +
                                           ", .firstConditionCompareValue = " + QString::number(firstCondition->GetConditionValue()) +
                                           ", .connective = " + getConnectiveString(trigger->GetConnective()));

                        if (trigger->GetConnective() != LayoutTransitionTypes::NONE)
                        {
                            TransitionCondition* secondCondition = trigger->GetSecondCondition();
                            const GCLSignalSink* secondConditionSink = secondCondition->GetSignalSinks().first();
                            triggerString.append(", .secondConditionGCLSink = GCL_Read_" + secondConditionSink->ComponentName() + "_" + secondConditionSink->SignalName() +
                                               ", .secondConditionCompareOperator = " + getCompareString(secondCondition->GetCompareOperator()) +
                                               ", .secondConditionCompareValue = " + QString::number(secondCondition->GetConditionValue()) + " }");
                        }
                        else
                        {
                            triggerString.append(", .secondConditionGCLSink = NULL" \
                                               ", .secondConditionCompareOperator = EQUALS_TO" \
                                               ", .secondConditionCompareValue = 0 }");
                        }

                        if (trigger != dynamicImageWidget->GetAnimationTriggers(1).last())
                        {
                            triggerString.append(",");
                        }

                        triggers.append(triggerString.join(""));
                    }
                    animationTriggerLines.append(triggers.join(",\n"));
                    animationTriggerLines.append("};");
                }

                QString animation2TriggersPropertiesString = "NULL";
                QString animation2TriggerString = "NULL";
                quint8 animation2TriggerCount = 0;
                if ((dynamicImageWidget->GetAnimationUseTriggers(2) == true) && (dynamicImageWidget->GetAnimationTriggers(2).count() > 0))
                {
                    animationTriggersUsed = true;

                    animation2TriggersPropertiesString = dynamicImageWidget->GetUIWidgetTypeName() + "_" + QString::number(dynamicImageWidget->GetUIWidgetID()) + "_Animation2_Triggers_Properties";

                    animationTriggersPropertiesLines.append("static animation_triggers_properties_t " + animation2TriggersPropertiesString + " =\n");
                    animationTriggersPropertiesLines.append("{");
                    animationTriggersPropertiesLines.append("    .onLoadBehavior = " + UIWidgetBase::GetOnLoadBehaviorString(widget->GetAnimationOnLoadBehavior(2)) + ",\n");
                    animationTriggersPropertiesLines.append("    .onTriggerActiveBehavior = " + UIWidgetBase::GetOnTriggerActiveBehaviorString(widget->GetAnimationOnTriggerActiveBehavior(2)) + ",\n");
                    animationTriggersPropertiesLines.append("    .onTriggerInactiveBehavior = " + UIWidgetBase::GetOnTriggerInactiveBehaviorString(widget->GetAnimationOnTriggerInactiveBehavior(2)) + "\n");
                    animationTriggersPropertiesLines.append("};");
                    animationTriggersPropertiesLines.append("");

                    animation2TriggersPropertiesString = "&" + animation2TriggersPropertiesString;

                    animation2TriggerCount = dynamicImageWidget->GetAnimationTriggers(2).count();
                    animation2TriggerString = dynamicImageWidget->GetUIWidgetTypeName() + "_" + QString::number(dynamicImageWidget->GetUIWidgetID()) + "_Animation2_Triggers";
                    animationTriggerLines.append("static signal_transition_t " + animation2TriggerString + "[" + QString::number(dynamicImageWidget->GetAnimationTriggers(2).count()) +"] =");
                    animationTriggerLines.append("{");
                    foreach (TransitionClause* trigger, dynamicImageWidget->GetAnimationTriggers(2))
                    {
                        QStringList triggerString;
                        TransitionCondition* firstCondition = trigger->GetFirstCondition();
                        const GCLSignalSink* firstConditionSink = firstCondition->GetSignalSinks().first();
                        triggerString.append("    { .firstConditionGCLSink = GCL_Read_" + firstConditionSink->ComponentName() + "_" + firstConditionSink->SignalName() +
                                           ", .firstConditionCompareOperator = " + getCompareString(firstCondition->GetCompareOperator()) +
                                           ", .firstConditionCompareValue = " + QString::number(firstCondition->GetConditionValue()) +
                                           ", .connective = " + getConnectiveString(trigger->GetConnective()));

                        if (trigger->GetConnective() != LayoutTransitionTypes::NONE)
                        {
                            TransitionCondition* secondCondition = trigger->GetSecondCondition();
                            const GCLSignalSink* secondConditionSink = secondCondition->GetSignalSinks().first();
                            triggerString.append(", .secondConditionGCLSink = GCL_Read_" + secondConditionSink->ComponentName() + "_" + secondConditionSink->SignalName() +
                                               ", .secondConditionCompareOperator = " + getCompareString(secondCondition->GetCompareOperator()) +
                                               ", .secondConditionCompareValue = " + QString::number(secondCondition->GetConditionValue()) + " }");
                        }
                        else
                        {
                            triggerString.append(", .secondConditionGCLSink = NULL" \
                                               ", .secondConditionCompareOperator = EQUALS_TO" \
                                               ", .secondConditionCompareValue = 0 }");
                        }

                        if (trigger != dynamicImageWidget->GetAnimationTriggers(2).last())
                        {
                            triggerString.append(",");
                        }

                        animationTriggerLines.append(triggerString.join(""));
                    }
                    animationTriggerLines.append("};");
                }

                const GCLSignalSink* imageIDSink = dynamicImageWidget->GetSignalSink("ResourceIDToDisplay");

                configLines.append("    {\n" \
                                   "        .widgetID = " + QString::number(dynamicImageWidget->GetUIWidgetID()) + ",\n" \
                                   "        .currentImageID = 0U,\n" \
                                   "        .readImageID = GCL_Read_" + dynamicImageWidget->GetUIWidgetUniqueName() + "_" + imageIDSink->SignalName() + ",\n" \
                                   "        .animation1TriggersCount = " + QString::number(animation1TriggerCount) + "u,\n" \
                                   "        .animation1TriggersProperties = " + animation1TriggersPropertiesString + ",\n" \
                                   "        .animation1Triggers = " + animation1TriggerString + ",\n" \
                                   "        .animation2TriggersCount = " + QString::number(animation2TriggerCount) + "u,\n" \
                                   "        .animation2TriggersProperties = " + animation2TriggersPropertiesString + ",\n" \
                                   "        .animation2Triggers = " + animation2TriggerString + "\n" \
                                   "    }");
            }
        }

        if (animationTriggersUsed == true)
        {
            fileLines.append("#include \"GCL_TransitionManager.h\"");
        }
    }
    else
    {
        configLines.append("    {\n" \
                           "        .widgetID = 999U, \n" \
                           "        .currentImageID = 0u,\n" \
                           "        .readImageID = NULL,\n" \
                           "        .animation1TriggersCount = 0u,\n" \
                           "        .animation1TriggersProperties = NULL,\n" \
                           "        .animation1Triggers = NULL,\n" \
                           "        .animation2TriggersCount = 0u,\n" \
                           "        .animation2TriggersProperties = NULL,\n" \
                           "        .animation2Triggers = NULL\n" \
                           "    }");
    }

    fileLines.append("");
    fileLines.append("/**************************** Constants and Types ****************************/");
    if (m_uiWidgetList.count() != 0)
    {
        fileLines.append("#define DYNAMIC_IMAGE_CONFIG_COUNT    " + QString::number(configLines.count()) + "U");
    }
    else
    {
        fileLines.append("#define DYNAMIC_IMAGE_CONFIG_COUNT    0U");
    }
    fileLines.append("");
    fileLines.append("/********************************* Variables *********************************/");
    fileLines.append("const uint32 DynamicImageConfigCount = DYNAMIC_IMAGE_CONFIG_COUNT;");
    fileLines.append("");
    fileLines.append(animationTriggersPropertiesLines.join(""));
    fileLines.append(animationTriggerLines.join("\n"));
    fileLines.append("");
    fileLines.append("static DynamicImageCfg_t dynamicImageConfigs[DYNAMIC_IMAGE_CONFIG_COUNT] =");
    fileLines.append("{");
    fileLines.append(configLines.join(",\n"));
    fileLines.append("};");
    fileLines.append("DynamicImageCfg_t* const DynamicImageConfigs = dynamicImageConfigs;");
    fileLines.append("");
    fileLines.append("/***************************** Exported Functions ****************************/");
    fileLines.append("");

    AddGeneratedFile(QString(PlatformPaths::CreateUIWidgetPath("DynamicImage/DynamicImageWidgetCfg.c")), fileLines.join("\n"));
}


void DynamicImageUIWidgetGenerator::OnGenerateFiles()
{
    generateConfigurationSourceFile();
}

QString DynamicImageUIWidgetGenerator::getConnectiveString(LayoutTransitionTypes::TransitionConnective connective)
{
    QString connectiveString;

    switch (connective)
    {
        case LayoutTransitionTypes::NONE:
            connectiveString = "NONE";
            break;
        case LayoutTransitionTypes::AND:
            connectiveString = "AND";
            break;
        case LayoutTransitionTypes::XOR:
            connectiveString = "XOR";
            break;
        default:
            connectiveString = "NONE";
            break;
    }

    return connectiveString;
}

QString DynamicImageUIWidgetGenerator::getCompareString(LayoutTransitionTypes::TransitionCompare compareCondition)
{
    QString compareString;

    switch (compareCondition)
    {
        case LayoutTransitionTypes::EqualTo:
            compareString = "EQUALS_TO";
            break;
        case LayoutTransitionTypes::GreaterThan:
            compareString = "GREATER_THAN";
            break;
        case LayoutTransitionTypes::EqualOrGreaterThan:
            compareString = "EQUALS_OR_GREATER_THAN";
            break;
        case LayoutTransitionTypes::LessThan:
            compareString = "LESS_THAN";
            break;
        case LayoutTransitionTypes::EqualOrLessThan:
            compareString = "EQUALS_OR_LESS_THAN";
            break;
        default:
            compareString = "EQUALS_TO";
            break;
    }

    return compareString;
}
