#include "ColumnLevelWidget.h"
#include "ColumnLevelWidgetGraphicsItem.h"
#include "GCLSignalSinkMetaType.h"
#include "GCLSignalSourceMetaType.h"
#include "ColorPair.h"

const QString ColumnLevelWidget::TypeName = "ColumnLevel";

ColumnLevelWidget::ColumnLevelWidget(LayoutDefinition& layoutDefinition, ProjectObject* parent) : UIWidgetBase(UIWidget_Type_ColumnLevel, 0, parent),
    m_layoutDefinition(layoutDefinition),
    m_spacing(3),
    m_numberOfBlocks(5),
    m_min(1),
    m_max(5),
    m_minBlockHeight(12),
    m_maxBlockHeight(42),
    m_sinkDataType("uint8"),
    m_sourceDataType("uint8")
{
    SetWidth(82);
    SetHeight(46);
    SetUserDefinedName(ColumnLevelWidget::TypeName);
    SetUseSecondaryColorPair(true);
}

ColumnLevelWidget::ColumnLevelWidget(quint32 uiWidgetID, LayoutDefinition &layoutDefinition, ProjectObject* parent) : UIWidgetBase(UIWidget_Type_ColumnLevel, uiWidgetID, parent),
    m_layoutDefinition(layoutDefinition),
    m_spacing(3),
    m_numberOfBlocks(5),
    m_min(1),
    m_max(5),
    m_minBlockHeight(12),
    m_maxBlockHeight(42),
    m_sinkDataType("uint8"),
    m_sourceDataType("uint8")
{
    SetWidth(82);
    SetHeight(46);
    SetUserDefinedName(ColumnLevelWidget::TypeName);
    SetUseSecondaryColorPair(true);
    SetCanHandleFocus(true);

    if (uiWidgetID > 0)
    {
        AddSignalSink("CurrentValue", m_sinkDataType, false, true);
        AddSignalSource("SelectedValue", m_sourceDataType);
    }
}

quint8 ColumnLevelWidget::GetMin() const
{
    return m_min;
}

void ColumnLevelWidget::SetMin(const quint8& min)
{
    m_min = min;
}

quint8 ColumnLevelWidget::GetMax() const
{
    return m_max;
}

void ColumnLevelWidget::SetMax(const quint8& max)
{
    m_max = max;
}

quint8 ColumnLevelWidget::GetMinBlockHeight() const
{
    return m_minBlockHeight;
}

void ColumnLevelWidget::SetMinBlockHeight(const quint8& minBlockHeight)
{
    m_minBlockHeight = minBlockHeight;
}

quint8 ColumnLevelWidget::GetMaxBlockHeight() const
{
    return m_maxBlockHeight;
}

void ColumnLevelWidget::SetMaxBlockHeight(const quint8& maxBlockHeight)
{
    m_maxBlockHeight = maxBlockHeight;
}

quint8 ColumnLevelWidget::GetNumberOfBlocks() const
{
    return m_numberOfBlocks;
}

void ColumnLevelWidget::SetNumberOfBlocks(const quint8& numberOfBlocks)
{
    m_numberOfBlocks = numberOfBlocks;
}

quint16 ColumnLevelWidget::GetSpacing() const
{
    return m_spacing;
}

void ColumnLevelWidget::SetSpacing(const quint16& spacing)
{
    m_spacing = spacing;
}

UIWidgetBase* ColumnLevelWidget::CloneThis(quint32 widgetID)
{
    return new ColumnLevelWidget(widgetID, m_layoutDefinition);
}

QString ColumnLevelWidget::GetUIWidgetTypeName() const
{
    return ColumnLevelWidget::TypeName;
}

GraphicsItemBase* ColumnLevelWidget::GetGraphicsItem(bool drawChromaKeyColor)
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
    return new ColumnLevelWidgetGraphicsItem(GetX(), GetY(), GetWidth(), GetHeight(), GetPrimaryColorPair()->GetFGColor(), bgColor, m_spacing, m_numberOfBlocks, m_minBlockHeight, m_maxBlockHeight, drawChromaKeyColor);
}

QString ColumnLevelWidget::GetSecondaryColorPairForegroundHeader()
{
    return "Selected Item Foreground Color";
}

QString ColumnLevelWidget::GetSecondaryColorPairBackgroundHeader()
{
    return "Selected Item Background Color";
}

QVariant ColumnLevelWidget::GetWidgetPropertyData(int column) const
{
    switch (column)
    {
        case NumberOfBlocksColumn:
            return GetNumberOfBlocks();
        case MinColumn:
            return GetMin();
        case MaxColumn:
            return GetMax();
        case MinBlockHeight:
            return GetMinBlockHeight();
        case MaxBlockHeight:
            return GetMaxBlockHeight();
        case SpacingColumn:
            return GetSpacing();
        case SinkColumn:
        {
            GCLSignalSink* currentValueSignalSink = GetSignalSink("CurrentValue");
            GCLSignalSinkMetaType gclSignalSinkMetaType(currentValueSignalSink);
            QVariant variant;
            variant.setValue(gclSignalSinkMetaType);
            return variant;
        }
        case SourceColumn:
        {
            GCLSignalSource* selectedValueSignalSource = GetSignalSource("SelectedValue");
            GCLSignalSourceMetaType gclSignalSourceMetaType(selectedValueSignalSource);
            QVariant variant;
            variant.setValue(gclSignalSourceMetaType);
            return variant;
        }
        default:
            return "INVALID";
    }
}

QString ColumnLevelWidget::GetWidgetPropertyHeader(int column) const
{
    switch (column)
    {
        case NumberOfBlocksColumn:
            return "Blocks";
        case MinColumn:
            return "Min";
        case MaxColumn:
            return "Max";
        case MinBlockHeight:
            return "Min block height";
        case MaxBlockHeight:
            return "Max block height";
        case SpacingColumn:
            return "Spacing";
        case SinkColumn:
            return "Current Level Sink";
        case SourceColumn:
            return "Selected Level Source";
        default:
            return "INVALID";
    }
}

void ColumnLevelWidget::SetWidgetProperty(int column, QVariant value)
{
    switch (column)
    {
        case SpacingColumn:
        {
            if (value.canConvert<int>())
            {
                SetSpacing(value.toInt());
            }
            return;
        }
        case NumberOfBlocksColumn:
        {
            if (value.canConvert<int>())
            {
                int noOfBlocks = value.toInt() < 1 ? 1 : value.toInt();
                SetNumberOfBlocks(noOfBlocks);
            }
            return;
        }
        case MinColumn:
        {
            if (value.canConvert<int>())
            {
                SetMin(value.toInt());
            }
            return;
        }
        case MaxColumn:
        {
            if (value.canConvert<int>())
            {
                SetMax(value.toInt());
            }
            return;
        }
        case MinBlockHeight:
        {
            if (value.canConvert<int>())
            {
                SetMinBlockHeight(value.toInt());
            }
            return;
        }
        case MaxBlockHeight:
        {
            if (value.canConvert<int>())
            {
                SetMaxBlockHeight(value.toInt());
            }
            return;
        }
        default:
            return;
    }
}

UIWidgetBase::DataType_t ColumnLevelWidget::GetWidgetPropertyDataType(int column) const
{
    switch (column)
    {
        case NumberOfBlocksColumn:
            return SpinBox_Uint8;
        case MinColumn:
            return SpinBox_Uint8;
        case MaxColumn:
            return SpinBox_Uint8;
        case MinBlockHeight:
            return SpinBox_Uint8;
        case MaxBlockHeight:
            return SpinBox_Uint8;
        case SpacingColumn:
            return SpinBox_Uint16;
        case SinkColumn:
            return SignalSink;
        case SourceColumn:
        default:
            return Not_Available;
    }
}

QString ColumnLevelWidget::WidgetPropertyToolTip(int column) const
{
    switch (column)
    {
        case NumberOfBlocksColumn:
            return "The number of blocks to display in the block bar";
        case MinColumn:
            return "The minimum value for the block bar input";
        case MaxColumn:
            return "The maximum value for the block bar input";
        case MinBlockHeight:
            return "The height of the the lowest block";
        case MaxBlockHeight:
            return "The height of the the heighest block";
        case SpacingColumn:
            return "The spacing between the blocks";
        case SinkColumn:
            return "The GCL signal mapping for this widget";
        case SourceColumn:
            return "When an illumination level is set/selected this source will output the selected level";
        default:
            return "INVALID";
    }
}

QString ColumnLevelWidget::LongDescription()
{
    return "This widget can be used as and input to the cluster to set levels such as illumination levels. " \
           "It takes an uint8 as an input and outputs an uint8 that can be used as input for other compontents.";
}

int ColumnLevelWidget::GetWidgetPropertyCount() const
{
    return ColumnLevelWidgetColumnCount;
}
