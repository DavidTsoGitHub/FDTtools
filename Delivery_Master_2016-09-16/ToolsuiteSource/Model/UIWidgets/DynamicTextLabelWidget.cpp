#include "DynamicTextLabelWidget.h"
#include "GCLSignalSinkMetaType.h"
#include "LabelWidgetGraphicsItem.h"
#include "ColorPair.h"

#include "Logger.h"
#include "Project.h"

const QString DynamicTextLabelWidget::TypeName = "DynamicTextLabel";

DynamicTextLabelWidget::DynamicTextLabelWidget(LabelDefinition& labelDefinition, LayoutDefinition &layoutDefinition, ProjectObject* parent) :
    UIWidgetBase(UIWidget_Type_DynamicText, 0, parent),
    m_labelDefinition(labelDefinition),
    m_layoutDefinition(layoutDefinition)
{
    m_rightAligned = false;
    m_fontId = 0;
    SetUserDefinedName(DynamicTextLabelWidget::TypeName);
}

DynamicTextLabelWidget::DynamicTextLabelWidget(LabelDefinition& labelDefinition, quint32 uiWidgetID, LayoutDefinition &layoutDefinition, ProjectObject* parent) :
    UIWidgetBase(UIWidget_Type_DynamicText, uiWidgetID, parent),
    m_labelDefinition(labelDefinition),
    m_layoutDefinition(layoutDefinition)
{
    m_rightAligned = false;
    m_fontId = 0;
    SetUserDefinedName(DynamicTextLabelWidget::TypeName);

    if (uiWidgetID > 0)
    {
        AddSignalSink("StringToPrint", "wchar*", false, true);
    }
}

DynamicTextLabelWidget::~DynamicTextLabelWidget()
{}

void DynamicTextLabelWidget::SetRightAligned(bool rightAligned)
{
    m_rightAligned = rightAligned;
}

bool DynamicTextLabelWidget::GetRightAligned() const
{
    return m_rightAligned;
}

void DynamicTextLabelWidget::SetFontID(quint32 id)
{
    if (m_labelDefinition.GetFontByID(id) == NULL)
    {
        m_fontId = UndefinedFontID;
        return;
    }

    m_fontId = id;
}

quint32 DynamicTextLabelWidget::GetFontID() const
{
    return m_fontId;
}

GraphicsItemBase* DynamicTextLabelWidget::GetGraphicsItem(bool drawChromaKeyColor)
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

QString DynamicTextLabelWidget::ShortDescription()
{
    return "A Dynamic Text Widget";
}

QString DynamicTextLabelWidget::LongDescription()
{
    return "This widget can be used to display a string from a function block. " \
           "It takes a wchar* as input.";
}

QString DynamicTextLabelWidget::GetUIWidgetTypeName() const
{
    return DynamicTextLabelWidget::TypeName;
}

UIWidgetBase* DynamicTextLabelWidget::CloneThis(quint32 widgetID)
{
    return new DynamicTextLabelWidget(m_labelDefinition, widgetID, m_layoutDefinition);
}

QVariant DynamicTextLabelWidget::GetWidgetPropertyData(int column) const
{
    switch (column)
    {
        case RightAlignedColumn:
            return GetRightAligned();
        case FontIDColumn:
            return GetFontID();
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

QString DynamicTextLabelWidget::GetWidgetPropertyHeader(int column) const
{
    switch (column)
    {
        case RightAlignedColumn:
            return "Right Aligned";
        case FontIDColumn:
            return "Font";
        case SinkColumn:
            return "Data Sink";
        default:
            return "";
    }
}

void DynamicTextLabelWidget::SetWidgetProperty(int column, QVariant value)
{
    switch (column)
    {
        case RightAlignedColumn:
            if (value.canConvert<bool>())
            {
                SetRightAligned(value.toBool());
            }
            break;
        case FontIDColumn:
            if (value.canConvert<uint>())
            {
                SetFontID(value.toUInt());
            }
            break;
        case SinkColumn: // Should not be set using this method
        default:
            break;
    }
}

UIWidgetBase::DataType_t DynamicTextLabelWidget::GetWidgetPropertyDataType(int column) const
{
    switch (column)
    {
        case RightAlignedColumn:
            return CheckBox;
        case FontIDColumn:
            return FontComboBox;
        case SinkColumn:
            return SignalSink;
        default:
            return Not_Available;
    }
}

int DynamicTextLabelWidget::GetWidgetPropertyCount() const
{
    return DynamicTextLabelWidgetColumnCount;
}

QString DynamicTextLabelWidget::WidgetPropertyToolTip(int column) const
{
    switch (column)
    {
        case RightAlignedColumn:
            return "Aligns the dynamic text to the right in the box";
        case FontIDColumn:
            return "The font to be used in the widget";
        case SinkColumn:
            return "The signal mapping for this widget";
        default:
            return "INVALID";
    }
}
