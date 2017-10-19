#include "DynamicLabelWidget.h"

#include "ColorPair.h"
#include "GCLSignalSinkMetaType.h"
#include "LabelWidgetGraphicsItem.h"
#include "Project.h"

const QString DynamicLabelWidget::TypeName ="DynamicLabel";


DynamicLabelWidget::DynamicLabelWidget(LayoutDefinition &layoutDefinition, ProjectObject* parent) :
    UIWidgetBase(UIWIdget_Type_DynamicLabel,0,parent),
    m_layoutDefinition(layoutDefinition)
{
    SetUserDefinedName(DynamicLabelWidget::TypeName);
}

DynamicLabelWidget::DynamicLabelWidget(quint32 widgetID, LayoutDefinition& layoutDefinition, ProjectObject* parent) :
    UIWidgetBase(UIWIdget_Type_DynamicLabel,widgetID,parent),
    m_layoutDefinition(layoutDefinition)
{
    SetUserDefinedName(DynamicLabelWidget::TypeName);
    if (widgetID > 0)
    {
		AddSignalSink("ResourceIDToDisplay","uint32", false, true);
    }
}

DynamicLabelWidget::~DynamicLabelWidget()
{

}

GraphicsItemBase* DynamicLabelWidget::GetGraphicsItem(bool drawChromaKeyColor)
{
    QColor bgColor;

    if (GetPrimaryColorPair()->GetBGIsTransparent())
    {
        bgColor = m_layoutDefinition.GetStartChromaKeyColor();
    }
    else
    {
        bgColor = GetPrimaryColorPair()->GetBGColor();
        drawChromaKeyColor = true;
    }
    return new LabelWidgetGraphicsItem(GetX(), GetY(), GetWidth(), GetHeight(), GetPrimaryColorPair()->GetFGColor(), bgColor, GetUserDefinedName(), drawChromaKeyColor);
}

UIWidgetBase::TextAlignment_t DynamicLabelWidget::GetTextAlignment() const
{
    return m_textAlignment;
}

void DynamicLabelWidget::SetTextAlignment(const TextAlignment_t& textAlignment)
{
    m_textAlignment = textAlignment;
}

QString DynamicLabelWidget::GetTextAlignmentEnumString()
{
    switch (m_textAlignment)
    {
        case CenterAligned:
            return "TEXT_CENTER_ALIGNED";
            break;
        case RightAligned:
            return "TEXT_RIGHT_ALIGNED";
            break;
        case LeftAligned:
        default:
            return "TEXT_LEFT_ALIGNED";
            break;
    }
}

QString DynamicLabelWidget::GetUIWidgetTypeName() const
{
    return DynamicLabelWidget::TypeName;
}

UIWidgetBase *DynamicLabelWidget::CloneThis(quint32 widgetId)
{
    return new DynamicLabelWidget(widgetId, m_layoutDefinition);
}

QVariant DynamicLabelWidget::GetWidgetPropertyData(int column) const
{
    switch(column)
    {
        case TextAlignmentColumn:
        {
            return static_cast<quint8>(m_textAlignment);
        }

        case LabelIDSinkColumn:
        {
            GCLSignalSink* currentValueSink = GetSignalSink("ResourceIDToDisplay");
            GCLSignalSinkMetaType gclSignalSinkMetaType(currentValueSink);
            QVariant variant;
            variant.setValue(gclSignalSinkMetaType);
            return variant;
        }

        default:
        {
            return "";
        }
    }
}

QString DynamicLabelWidget::GetWidgetPropertyHeader(int column) const
{
    switch (column)
    {
        case TextAlignmentColumn:
            return "Text Alignment";
        case LabelIDSinkColumn:
            return "Label ID to Display";
        default:
            return "Invalid";
    }
}

void DynamicLabelWidget::SetWidgetProperty(int column, QVariant value)
{
    switch (column)
    {
        case TextAlignmentColumn:
            if (value.canConvert<quint8>())
            {
                SetTextAlignment(static_cast<TextAlignment_t>(value.toUInt()));
            }
            return;
        default:
            return;
    }
}

int DynamicLabelWidget::GetWidgetPropertyCount() const
{
    return dynamicLabelWidgetColumnCount;
}

UIWidgetBase::DataType_t DynamicLabelWidget::GetWidgetPropertyDataType(int column) const
{
    switch (column)
    {
        case TextAlignmentColumn:
            return TextAlignmentComboBox;
        case LabelIDSinkColumn:
            return SignalSink;
        default:
            return Not_Available;
    }
}

QString DynamicLabelWidget::WidgetPropertyToolTip(int column) const
{
    switch (column)
    {
        case TextAlignmentColumn:
            return "The text alginment in the containing area";
        case LabelIDSinkColumn:
            return "The sink which will receive the current LabelID to display";
        default:
            return "Not_Available";
    }
}

QString DynamicLabelWidget::LongDescription()
{
    return "This widget can be used to display different labels depending on an input value. " \
           "Labels are added in the Label Editor. The label can be written on multiple lines " \
           "and an new line is represented by '\\n' in the label text." \
           "It takes an uint32 as input value which represents the Label ID found in the Label Manager.";
}


bool DynamicLabelWidget::objectSelfValidation(Project* project, QStringList* errorList)
{
    bool validation = true;
    validation &= UIWidgetBase::objectSelfValidation(project, errorList);

    if (GetTextAlignment() == 0)
    {
        //no alignment set
        errorList->append("Unset text alignment in DynamicLabelWidget '" + GetUserDefinedName() + "', widget ID: " + QString::number(GetUIWidgetID()));
        validation = false;
    }

    return validation;
}
