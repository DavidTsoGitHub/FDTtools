#include "DynamicIconWidget.h"
#include "DynamicIconWidgetGraphicsItem.h"
#include "GCLSignalSinkMetaType.h"

const QString DynamicIconWidget::TypeName = "DynamicIcon";

DynamicIconWidget::DynamicIconWidget(ProjectObject* parent) :
    UIWidgetBase(UIWidget_Type_DynamicIcon, 0, parent)
{
    SetUserDefinedName(DynamicIconWidget::TypeName);
}

DynamicIconWidget::DynamicIconWidget(quint32 widgetID, ProjectObject* parent) :
    UIWidgetBase(UIWidget_Type_DynamicIcon, widgetID, parent)
{
    SetUserDefinedName(DynamicIconWidget::TypeName);
    if (widgetID > 0)
    {
        AddSignalSink("ResourceIDToDisplay", "uint32", false, true);
    }
}

DynamicIconWidget::~DynamicIconWidget()
{
}

QString DynamicIconWidget::GetUIWidgetTypeName() const
{
    return DynamicIconWidget::TypeName;
}

GraphicsItemBase* DynamicIconWidget::GetGraphicsItem(bool drawChromaKeyColor)
{
    Q_UNUSED(drawChromaKeyColor)
    return new DynamicIconWidgetGraphicsItem(GetX(), GetY(), GetWidth(), GetHeight(), true);
}

QString DynamicIconWidget::LongDescription()
{
    return "This widget can be used to display different icons depending on an input value." \
           "The input value is an uint32 which represents the icon ID which can be found in the Icons Manager.";
}

UIWidgetBase* DynamicIconWidget::CloneThis(quint32 widgetId)
{
    return new DynamicIconWidget(widgetId);
}

QVariant DynamicIconWidget::GetWidgetPropertyData(int column) const
{
    switch (column)
    {
        case IconIDSinkColumn:
        {
            GCLSignalSink* currentValueSink = GetSignalSink("ResourceIDToDisplay");
            GCLSignalSinkMetaType gclSignalSinkMetaType(currentValueSink);
            QVariant variant;
            variant.setValue(gclSignalSinkMetaType);
            return variant;
        }
        default:
            return "";
    }
}

QString DynamicIconWidget::GetWidgetPropertyHeader(int column) const
{
    switch (column)
    {
        case IconIDSinkColumn:
            return "IconID to display";
        default:
            return "INVALID";
    }
}

void DynamicIconWidget::SetWidgetProperty(int column, QVariant value)
{
    // We don't use this interface to do signal mapping, which is currently the only widget specific data field
    Q_UNUSED(column);
    Q_UNUSED(value);
}

int DynamicIconWidget::GetWidgetPropertyCount() const
{
    return dynamicIconWidgetColumnCount;
}

UIWidgetBase::DataType_t DynamicIconWidget::GetWidgetPropertyDataType(int column) const
{
    switch (column)
    {
        case IconIDSinkColumn:
            return SignalSink;
        default:
            return Not_Available;
    }
}

QString DynamicIconWidget::WidgetPropertyToolTip(int column) const
{
    switch (column)
    {
        case IconIDSinkColumn:
            return "The sink which will receive the current IconID to display";
        default:
            return "INVALID";
    }
}
