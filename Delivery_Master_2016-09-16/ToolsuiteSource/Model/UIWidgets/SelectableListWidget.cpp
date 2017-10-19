#include "SelectableListWidget.h"

#include "GCLSignalSinkMetaType.h"
#include "GCLSignalSourceMetaType.h"
#include "LayoutDefinition.h"
#include "SelectableListWidgetGraphicsItem.h"
#include "ColorPair.h"

const QString SelectableListWidget::TypeName = "SelectableList";

SelectableListWidget::SelectableListWidget(LayoutDefinition &layoutDefinition, ProjectObject* parent) : UIWidgetBase(UIWidget_Type_SelectableList, 0, parent),
    m_layoutDefinition(layoutDefinition),
    m_selectable(false),
    m_showNumbering(true),
    m_displayItemCount(1u),
    m_textAlignment(LeftAligned)
{
    SetUserDefinedName(SelectableListWidget::TypeName);
    SetUseSecondaryColorPair(true);
}

SelectableListWidget::SelectableListWidget(quint32 widgetID, LayoutDefinition &layoutDefinition, ProjectObject *parent) :
    UIWidgetBase(UIWidget_Type_SelectableList, widgetID, parent),
    m_layoutDefinition(layoutDefinition),
    m_selectable(false),
    m_showNumbering(true),
    m_displayItemCount(1u),
    m_textAlignment(LeftAligned)
{
    SetUserDefinedName(SelectableListWidget::TypeName);
    if (widgetID > 0)
    {
        AddSignalSink("CurrentValue", "uint8", false, true);
        AddSignalSink("LabelToValueMappings", "LabelMappings*", false, true);
        AddSignalSource("SelectedValue", "uint8");
    }

    SetCanHandleFocus(true);
    SetUseSecondaryColorPair(true);
}

SelectableListWidget::~SelectableListWidget()
{
}

UIWidgetBase* SelectableListWidget::CloneThis(quint32 widgetID)
{
    return new SelectableListWidget(widgetID, m_layoutDefinition);
}

QString SelectableListWidget::GetUIWidgetTypeName() const
{
    return SelectableListWidget::TypeName;
}

GraphicsItemBase* SelectableListWidget::GetGraphicsItem(bool drawChromaKeyColor)
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
    return new SelectableListWidgetGraphicsItem(GetX(), GetY(), GetWidth(), GetHeight(), GetPrimaryColorPair()->GetFGColor(), bgColor, drawChromaKeyColor );
}

QString SelectableListWidget::GetSecondaryColorPairForegroundHeader()
{
    return "Selected Item Foreground Color";
}

QString SelectableListWidget::GetSecondaryColorPairBackgroundHeader()
{
    return "Selected Item Background Color";
}

QVariant SelectableListWidget::GetWidgetPropertyData(int column) const
{
    switch (column)
    {
        case SelectableColumn:
        {
            return GetSelectable();
        }
        case ShowNumberingColumn:
        {
            return GetShowNumbering();
        }
        case DisplayItemCountColumn:
        {
            return GetDisplayItemCount();
        }
        case TextAlignmentColumn:
        {
            return static_cast<quint8>(m_textAlignment);
        }
        case CurrentValueSinkColumn:
        {
            GCLSignalSink* currentValueSink = GetSignalSink("CurrentValue");
            GCLSignalSinkMetaType gclSignalSinkMetaType(currentValueSink);
            QVariant variant;
            variant.setValue(gclSignalSinkMetaType);
            return variant;
        }
        case LabelMappingsSinkColumn:
        {
            GCLSignalSink* labelMappingsSink = GetSignalSink("LabelToValueMappings");
            GCLSignalSinkMetaType gclSignalSinkMetaType(labelMappingsSink);
            QVariant variant;
            variant.setValue(gclSignalSinkMetaType);
            return variant;
        }
        case SelectedValueSourceColumn:
        {
            GCLSignalSource* selectedValueSource = GetSignalSource("SelectedValue");
            GCLSignalSourceMetaType gclSignalSourceMetaType(selectedValueSource);
            QVariant variant;
            variant.setValue(gclSignalSourceMetaType);
            return variant;
        }
        default:
            return "";
    }
}

QString SelectableListWidget::GetWidgetPropertyHeader(int column) const
{
    switch (column)
    {
        case SelectableColumn:
            return "Selectable";
        case ShowNumberingColumn:
            return "Show Numbering";
        case DisplayItemCountColumn:
            return "Display Item Count";
        case TextAlignmentColumn:
            return "Text Alignment";
        case CurrentValueSinkColumn:
            return "CurrentValue";
        case LabelMappingsSinkColumn:
            return "LabelToValueMappings";
        case SelectedValueSourceColumn:
            return "SelectedValue";
        default:
            return "INVALID";
    }
}

void SelectableListWidget::SetWidgetProperty(int column, QVariant value)
{
    switch(column)
    {
        case SelectableColumn:
        {
            if (value.canConvert<bool>())
            {
                m_selectable = value.toBool();
            }
            return;
        }
        case ShowNumberingColumn:
        {
            if (value.canConvert<bool>())
            {
                m_showNumbering = value.toBool();
            }
            return;
        }
        case DisplayItemCountColumn:
        {
            if (value.canConvert<int>())
            {
                m_displayItemCount = value.toInt();
            }
            return;
        }
        case TextAlignmentColumn:
        {
            if (value.canConvert<quint8>())
            {
                SetTextAlignment(static_cast<TextAlignment_t>(value.toUInt()));
            }
            break;
        }
    }
}

int SelectableListWidget::GetWidgetPropertyCount() const
{
    return selectableListWidgetColumnCount;
}

UIWidgetBase::DataType_t SelectableListWidget::GetWidgetPropertyDataType(int column) const
{
    switch (column)
    {
        case SelectableColumn:
            return CheckBox;
        case ShowNumberingColumn:
            return CheckBox;
        case DisplayItemCountColumn:
            return SpinBox_Uint8_Min1;
        case TextAlignmentColumn:
            return TextAlignmentComboBox;
        case CurrentValueSinkColumn:
            return SignalSink;
        case LabelMappingsSinkColumn:
            return SignalSink;
        case SelectedValueSourceColumn:
        default:
            return Not_Available;
    }
}

QString SelectableListWidget::WidgetPropertyToolTip(int column) const
{
    switch (column)
    {
        case SelectableColumn:
            return "Defines if the item should be highlighted when selected";
        case ShowNumberingColumn:
            return "Defines if a numbering label and up/down arrows is to be displayed";
        case DisplayItemCountColumn:
            return "Defines the number of items to display";
        case CurrentValueSinkColumn:
            return "The sink which will receive the current value to be selected in the list widget";
        case LabelMappingsSinkColumn:
            return "The sink which will receive a list of value/label mappings";
        case SelectedValueSourceColumn:
            return "When a value is selected this source will output the selected value";
        default:
            return "";
    }
}

QString SelectableListWidget::LongDescription()
{
    return "This widget can be used as and input to the cluster to make settings based on a number of labels, for example" \
           " to set language. It can also be used to display information in form of a scrollable list.";
}

bool SelectableListWidget::GetSelectable() const
{
    return m_selectable;
}

void SelectableListWidget::SetSelectable(bool selectable)
{
    m_selectable = selectable;
    NotifyPropertyUpdated();
}

bool SelectableListWidget::GetShowNumbering(void) const
{
    return m_showNumbering;
}

void SelectableListWidget::SetShowNumbering(bool showNumbering)
{
    m_showNumbering = showNumbering;
    NotifyPropertyUpdated();
}

quint8 SelectableListWidget::GetDisplayItemCount(void) const
{
    return m_displayItemCount;
}

void SelectableListWidget::SetDisplayItemCount(quint8 count)
{
    m_displayItemCount = count;
    NotifyPropertyUpdated();
}

UIWidgetBase::TextAlignment_t SelectableListWidget::GetTextAlignment() const
{
    return m_textAlignment;
}

QString SelectableListWidget::GetTextAlignmentEnumString(void) const
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

void SelectableListWidget::SetTextAlignment(const TextAlignment_t& textAlignment)
{
    m_textAlignment = textAlignment;
    NotifyPropertyUpdated();
}
