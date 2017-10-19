#include "SignalTriggerWidget.h"
#include "SignalTriggerWidgetGraphicsItem.h"
#include "GCLSignalSourceMetaType.h"

const QString SignalTriggerWidget::TypeName = "SignalTrigger";

SignalTriggerWidget::SignalTriggerWidget(ProjectObject* parent) : UIWidgetBase(UIWidget_Type_SignalTrigger, 0, parent)
{
    SetUserDefinedName(SignalTriggerWidget::TypeName);
}

SignalTriggerWidget::SignalTriggerWidget(quint32 widgetID, ProjectObject* parent) :
    UIWidgetBase(UIWidget_Type_SignalTrigger, widgetID, parent)
{
    SetUserDefinedName(SignalTriggerWidget::TypeName);
    if (widgetID)
    {
        AddSignalSource("SignalTriggerSource", "void");
    }
}

SignalTriggerWidget::~SignalTriggerWidget()
{
}

QString SignalTriggerWidget::GetUIWidgetTypeName() const
{
    return SignalTriggerWidget::TypeName;
}

GraphicsItemBase *SignalTriggerWidget::GetGraphicsItem(bool drawChromaKeyColor)
{
    return new SignalTriggerWidgetGraphicsItem(GetX(), GetY(), GetWidth(), GetHeight(), drawChromaKeyColor);
}

UIWidgetBase *SignalTriggerWidget::CloneThis(quint32 widgetId)
{
    return new SignalTriggerWidget(widgetId);
}

QVariant SignalTriggerWidget::GetWidgetPropertyData(int column) const
{
    switch (column)
    {
        case SignalSourceColumn:
        {
            GCLSignalSource* triggerSignalSource = GetSignalSource("SignalTriggerSource");
            GCLSignalSourceMetaType gclSignalSourceMetaType(triggerSignalSource);
            QVariant variant;
            variant.setValue(gclSignalSourceMetaType);
            return variant;
        }
        default:
            return "";
    }
}

QString SignalTriggerWidget::GetWidgetPropertyHeader(int column) const
{
    switch (column)
    {
        case SignalSourceColumn:
            return "SignalTriggerSource";
        default:
            return "INVALID";
    }
}

void SignalTriggerWidget::SetWidgetProperty(int column, QVariant value)
{
    Q_UNUSED(column);
    Q_UNUSED(value);
}

int SignalTriggerWidget::GetWidgetPropertyCount() const
{
    return triggerSignalWidgetColumnCount;
}

UIWidgetBase::DataType_t SignalTriggerWidget::GetWidgetPropertyDataType(int column) const
{
    switch (column)
    {
        case SignalSourceColumn:
        default:
            return Not_Available;
    }
}

QString SignalTriggerWidget::WidgetPropertyToolTip(int column) const
{
    switch (column)
    {
        case SignalSourceColumn:
            return "Trigger signal to sink";
        default:
            return "INVALID";
    }
}

QString SignalTriggerWidget::LongDescription()
{
    return "This widget can be used to trigger a signal to raise an event in a function block. " \
           "It outputs a void to signal that a trigger has occurred.";
}

