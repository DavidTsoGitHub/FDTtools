#include "DynamicValueLabelWidget.h"
#include "GCLSignalSinkMetaType.h"
#include "ColorPair.h"

#include <LabelWidgetGraphicsItem.h>
#include "Logger.h"
#include "Project.h"

const QString DynamicValueLabelWidget::TypeName = "DynamicValueLabel";

DynamicValueLabelWidget::DynamicValueLabelWidget(LabelDefinition& labelDefinition, LayoutDefinition &layoutDefinition, ProjectObject* parent) :
    UIWidgetBase(UIWidget_Type_DynamicValue, 0, parent),
    m_labelDefinition(labelDefinition),
    m_layoutDefinition(layoutDefinition),
    m_fontId(UndefinedFontID),
    m_scaling(0),
    m_nbrOfDecimals(0),
    m_nbrOfDigits(0)
{
    SetUserDefinedName(DynamicValueLabelWidget::TypeName);
}

DynamicValueLabelWidget::DynamicValueLabelWidget(LabelDefinition& labelDefinition, quint32 uiWidgetID, LayoutDefinition& layoutDefinition, ProjectObject* parent) :
    UIWidgetBase(UIWidget_Type_DynamicValue, uiWidgetID, parent),
    m_labelDefinition(labelDefinition),
    m_layoutDefinition(layoutDefinition),
    m_fontId(UndefinedFontID),
    m_scaling(0),
    m_nbrOfDecimals(0),
    m_nbrOfDigits(0)
{
    SetUserDefinedName(DynamicValueLabelWidget::TypeName);

    if (uiWidgetID > 0)
    {
        AddSignalSink("ValueToDisplay", "DynamicWidgetValue", false, true);
    }
}

DynamicValueLabelWidget::~DynamicValueLabelWidget()
{}

void DynamicValueLabelWidget::SetFontID(quint32 id)
{
    if (m_labelDefinition.GetFontByID(id) == NULL)
    {
        m_fontId = UndefinedFontID;
        return;
    }

    m_fontId = id;
}

quint32 DynamicValueLabelWidget::GetFontID() const
{
    return m_fontId;
}

void DynamicValueLabelWidget::SetScaling(qint8 scaling)
{
    m_scaling = scaling;
}

qint8 DynamicValueLabelWidget::GetScaling() const
{
    return m_scaling;
}

void DynamicValueLabelWidget::SetNbrOfDecimals(quint8 nbrOfDecimals)
{
    m_nbrOfDecimals = nbrOfDecimals;
}

quint8 DynamicValueLabelWidget::GetNbrOfDecimals() const
{
    return m_nbrOfDecimals;
}

void DynamicValueLabelWidget::SetNbrOfDigits(quint8 nbrOfDigits)
{
    m_nbrOfDigits = nbrOfDigits;
}

quint8 DynamicValueLabelWidget::GetNbrOfDigits() const
{
    return m_nbrOfDigits;
}

GraphicsItemBase* DynamicValueLabelWidget::GetGraphicsItem(bool drawChromaKeyColor)
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

QString DynamicValueLabelWidget::ShortDescription()
{
    return "A Dynamic Value Widget";
}

QString DynamicValueLabelWidget::LongDescription()
{
    return "This widget can be used to display a dynamic value depeding on the input value. " \
           "It takes a DynamicWidgetValue as an input which represents value that will be displayed.";
}

QString DynamicValueLabelWidget::GetUIWidgetTypeName() const
{
    return DynamicValueLabelWidget::TypeName;
}

UIWidgetBase* DynamicValueLabelWidget::CloneThis(quint32 widgetID)
{
    return new DynamicValueLabelWidget(m_labelDefinition, widgetID, m_layoutDefinition);
}

QVariant DynamicValueLabelWidget::GetWidgetPropertyData(int column) const
{
    switch (column)
    {
        case FontIDColumn:
            return GetFontID();
        case ScalingColumn:
            return GetScaling();
        case DecimalsColumn:
            return GetNbrOfDecimals();
        case DigitsColumn:
            return GetNbrOfDigits();
        case SinkColumn:
        {
            GCLSignalSink* signalSink = GetSignalSinks().first();
            GCLSignalSinkMetaType gclSignalSinkMetaType(signalSink);
            QVariant variant;
            variant.setValue(gclSignalSinkMetaType);
            return variant;
        }
        default:
            return "";
    }
}

QString DynamicValueLabelWidget::GetWidgetPropertyHeader(int column) const
{
    switch (column)
    {
        case FontIDColumn:
            return "Font";
        case ScalingColumn:
            return "Multiplier (10^n)";
        case DecimalsColumn:
            return "Number of Decimals";
        case DigitsColumn:
            return "Maximum Number of Digits";
        case SinkColumn:
            return "Data Sink";
        default:
            return "";
    }
}

void DynamicValueLabelWidget::SetWidgetProperty(int column, QVariant value)
{
    switch (column)
    {
        case FontIDColumn:
            if (value.canConvert<uint>())
            {
                SetFontID(value.toUInt());
            }
            break;
        case ScalingColumn:
            if (value.canConvert<int>())
            {
                SetScaling(value.toInt());
            }
            break;
        case DecimalsColumn:
            if (value.canConvert<uint>())
            {
                SetNbrOfDecimals(value.toUInt());
            }
            break;
        case DigitsColumn:
            if (value.canConvert<uint>())
            {
                SetNbrOfDigits(value.toUInt());
            }
            break;
        case SinkColumn: // Should not be set using this method
        default:
            break;
    }
}

UIWidgetBase::DataType_t DynamicValueLabelWidget::GetWidgetPropertyDataType(int column) const
{
    switch (column)
    {
        case FontIDColumn:
            return FontComboBox;
        case ScalingColumn:
            return SpinBox_Int8;
        case DecimalsColumn:
            return SpinBox_DynamicValueType;
        case DigitsColumn:
            return SpinBox_DynamicValueType;
        case SinkColumn:
            return SignalSink;
        default:
            return Not_Available;
    }
}

int DynamicValueLabelWidget::GetWidgetPropertyCount() const
{
    return DynamicValueLabelWidgetColumnCount;
}

QString DynamicValueLabelWidget::WidgetPropertyToolTip(int column) const
{
    switch (column)
    {
        case FontIDColumn:
            return "The font to be used in the widget";
        case ScalingColumn:
            return "E = value^N";
        case DecimalsColumn:
            return "Number of decimals to display";
        case DigitsColumn:
            return "Max number of digits (including decimal point) to display";
        case SinkColumn:
            return "The signal mapping for this widget";
        default:
            return "INVALID";
    }
}
