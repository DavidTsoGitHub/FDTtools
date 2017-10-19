#include "ImageUIWidgetGenerator.h"

#include "TransitionCondition.h"

ImageUIWidgetGenerator::ImageUIWidgetGenerator(QList<UIWidgetBase *> uiWidgetList) :
    m_uiWidgetList(uiWidgetList)
{

}

ImageUIWidgetGenerator::~ImageUIWidgetGenerator()
{

}

void ImageUIWidgetGenerator::generateImageConfigurationSourceFile()
{
    QStringList fileLines;
    QStringList configLines;
    QStringList animationTriggersPropertiesLines;
    QStringList animationTriggerLines;

    fileLines.append("/********************************* Includes **********************************/");
    fileLines.append("#include \"ImageWidgetCfg.h\"");

    if (m_uiWidgetList.count() != 0)
    {
        bool animationTriggersUsed = false;
        foreach (UIWidgetBase* widget, m_uiWidgetList)
        {
            if (widget->GetUIWidgetType() == UIWidgetBase::UIWidget_Type_Image)
            {
                ImageWidget* imageWidget = static_cast<ImageWidget*>(widget);

                QString animation1TriggersPropertiesString = "NULL";
                QString animation1TriggerString = "NULL";
                quint8 animation1TriggerCount = 0;

                if ((imageWidget->GetAnimationUseTriggers(1) == true) && (imageWidget->GetAnimationTriggers(1).count() > 0))
                {
                    animationTriggersUsed = true;

                    animation1TriggersPropertiesString = imageWidget->GetUIWidgetTypeName() + "_" + QString::number(imageWidget->GetUIWidgetID()) + "_Animation1_Triggers_Properties";

                    animationTriggersPropertiesLines.append("static animation_triggers_properties_t " + animation1TriggersPropertiesString + " =\n");
                    animationTriggersPropertiesLines.append("{\n");
                    animationTriggersPropertiesLines.append("    .onLoadBehavior = " + UIWidgetBase::GetOnLoadBehaviorString(widget->GetAnimationOnLoadBehavior(1)) + ",\n");
                    animationTriggersPropertiesLines.append("    .onTriggerActiveBehavior = " + UIWidgetBase::GetOnTriggerActiveBehaviorString(widget->GetAnimationOnTriggerActiveBehavior(1)) + ",\n");
                    animationTriggersPropertiesLines.append("    .onTriggerInactiveBehavior = " + UIWidgetBase::GetOnTriggerInactiveBehaviorString(widget->GetAnimationOnTriggerInactiveBehavior(1)) + "\n");
                    animationTriggersPropertiesLines.append("};\n");
                    animationTriggersPropertiesLines.append("");

                    animation1TriggersPropertiesString = "&" + animation1TriggersPropertiesString;

                    animation1TriggerCount = imageWidget->GetAnimationTriggers(1).count();
                    animation1TriggerString = imageWidget->GetUIWidgetTypeName() + "_" + QString::number(imageWidget->GetUIWidgetID()) + "_Animation1_Triggers";
                    animationTriggerLines.append("static signal_transition_t " + animation1TriggerString + "[" + QString::number(imageWidget->GetAnimationTriggers(1).count()) +"] =");
                    animationTriggerLines.append("{");
                    foreach (TransitionClause* trigger, imageWidget->GetAnimationTriggers(1))
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

                        if (trigger != imageWidget->GetAnimationTriggers(1).last())
                        {
                            triggerString.append(",");
                        }
                        animationTriggerLines.append(triggerString.join(""));
                    }
                    animationTriggerLines.append("};");
                }

                QString animation2TriggersPropertiesString = "NULL";
                QString animation2TriggerString = "NULL";
                quint8 animation2TriggerCount = 0;
                if ((imageWidget->GetAnimationUseTriggers(2) == true) && (imageWidget->GetAnimationTriggers(2).count() > 0))
                {
                    animationTriggersUsed = true;

                    animation2TriggersPropertiesString = imageWidget->GetUIWidgetTypeName() + "_" + QString::number(imageWidget->GetUIWidgetID()) + "_Animation2_Triggers_Properties";

                    animationTriggersPropertiesLines.append("static animation_triggers_properties_t " + animation2TriggersPropertiesString + " =\n");
                    animationTriggersPropertiesLines.append("{");
                    animationTriggersPropertiesLines.append("    .onLoadBehavior = " + UIWidgetBase::GetOnLoadBehaviorString(widget->GetAnimationOnLoadBehavior(2)) + ",\n");
                    animationTriggersPropertiesLines.append("    .onTriggerActiveBehavior = " + UIWidgetBase::GetOnTriggerActiveBehaviorString(widget->GetAnimationOnTriggerActiveBehavior(2)) + ",\n");
                    animationTriggersPropertiesLines.append("    .onTriggerInactiveBehavior = " + UIWidgetBase::GetOnTriggerInactiveBehaviorString(widget->GetAnimationOnTriggerInactiveBehavior(2)) + "\n");
                    animationTriggersPropertiesLines.append("};");
                    animationTriggersPropertiesLines.append("");

                    animation2TriggersPropertiesString = "&" + animation2TriggersPropertiesString;

                    animation2TriggerCount = imageWidget->GetAnimationTriggers(2).count();
                    animation2TriggerString = imageWidget->GetUIWidgetTypeName() + "_" + QString::number(imageWidget->GetUIWidgetID()) + "_Animation2_Triggers";
                    animationTriggerLines.append("static signal_transition_t " + animation2TriggerString + "[" + QString::number(imageWidget->GetAnimationTriggers(2).count()) +"] =");
                    animationTriggerLines.append("{");
                    foreach (TransitionClause* trigger, imageWidget->GetAnimationTriggers(2))
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

                        if (trigger != imageWidget->GetAnimationTriggers(2).last())
                        {
                            triggerString.append(",");
                        }
                        animationTriggerLines.append(triggerString.join(""));
                    }
                    animationTriggerLines.append("};");
                }

                configLines.append("    {\n" \
                                   "        .widgetID = " + QString::number(imageWidget->GetUIWidgetID()) + ",\n" \
                                   "        .imageID = " + QString::number(imageWidget->GetImageID()) + ",\n" \
                                   "        .layerEnabled = FALSE,\n" \
                                   "        .animation1TriggersCount = " + QString::number(animation1TriggerCount) + ",\n" \
                                   "        .animation1TriggersProperties = " + animation1TriggersPropertiesString + ",\n" \
                                   "        .animation1Triggers = " + animation1TriggerString + ",\n" \
                                   "        .animation2TriggersCount = " + QString::number(animation2TriggerCount) + ",\n" \
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
                           "        .widgetID = 999,\n" \
                           "        .imageID = 999,\n" \
                           "        .layerEnabled = FALSE,\n" \
                           "        .animation1TriggersCount = 0,\n" \
                           "        .animation1TriggersProperties = NULL,\n" \
                           "        .animation1Triggers = NULL,\n" \
                           "        .animation2TriggersCount = 0,\n" \
                           "        .animation2TriggersProperties = NULL,\n" \
                           "        .animation2Triggers = NULL\n" \
                           "    }");
    }

    fileLines.append("");
    fileLines.append("/**************************** Constants and Types ****************************/");
    if (m_uiWidgetList.count() != 0)
    {
        fileLines.append("#define IMAGE_CONFIG_COUNT    " + QString::number(configLines.count()) + "U");
    }
    else
    {
        fileLines.append("#define IMAGE_CONFIG_COUNT    1U");
    }
    fileLines.append("");
    fileLines.append("/********************************* Variables *********************************/");
    fileLines.append("const uint32 ImageConfigCount = IMAGE_CONFIG_COUNT;");
    fileLines.append("");
    fileLines.append(animationTriggersPropertiesLines.join(""));
    fileLines.append(animationTriggerLines.join("\n"));
    fileLines.append("");
    fileLines.append("static ImageCfg_t imageConfigs[IMAGE_CONFIG_COUNT] =");
    fileLines.append("{");
    fileLines.append(configLines.join(",\n"));
    fileLines.append("};");
    fileLines.append("ImageCfg_t* ImageConfigs = imageConfigs;");
    fileLines.append("");
    fileLines.append("/***************************** Exported Functions ****************************/");
    fileLines.append("");

    AddGeneratedFile(QString(PlatformPaths::CreateUIWidgetPath("Image/ImageWidgetCfg.c")), fileLines.join("\n"));
}

void ImageUIWidgetGenerator::OnGenerateFiles()
{
    generateImageConfigurationSourceFile();
}

QString ImageUIWidgetGenerator::getConnectiveString(LayoutTransitionTypes::TransitionConnective connective)
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

QString ImageUIWidgetGenerator::getCompareString(LayoutTransitionTypes::TransitionCompare compareCondition)
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
