#include "BlockBarWidget.h"
#include "BlockBarWidgetGraphicsItem.h"
#include "GCLSignalSinkMetaType.h"
#include "ColorPair.h"

const QString BlockBarWidget::TypeName = "BlockBar";

BlockBarWidget::BlockBarWidget(LayoutDefinition& layoutDefinition, ProjectObject* parent) : UIWidgetBase(UIWidget_Type_BlockBar, 0, parent),
    m_layoutDefinition(layoutDefinition),
    m_spacing(3),
    m_numberOfBlocks(8),
    m_min(0),
    m_max(0)
{
    SetWidth(128);
    SetHeight(18);
    SetUserDefinedName(BlockBarWidget::TypeName);
}

BlockBarWidget::BlockBarWidget(quint32 uiWidgetID, LayoutDefinition& layoutDefinition, ProjectObject* parent) : UIWidgetBase(UIWidget_Type_BlockBar, uiWidgetID, parent),
    m_layoutDefinition(layoutDefinition),
    m_spacing(3),
    m_numberOfBlocks(8),
    m_min(0),
    m_max(0)
{
    SetWidth(128);
    SetHeight(18);
    SetUserDefinedName(BlockBarWidget::TypeName);

    if (uiWidgetID > 0)
    {
        AddSignalSink("BlockBarValue", "DynamicWidgetValue", false, true);
    }
}


quint32 BlockBarWidget::GetMin() const
{
    return m_min;
}

void BlockBarWidget::SetMin(const quint32& min)
{
    m_min = min;
}

quint32 BlockBarWidget::GetMax() const
{
    return m_max;
}

void BlockBarWidget::SetMax(const quint32& max)
{
    m_max = max;
}

quint8 BlockBarWidget::GetNumberOfBlocks() const
{
    return m_numberOfBlocks;
}

void BlockBarWidget::SetNumberOfBlocks(const quint8& numberOfBlocks)
{
    m_numberOfBlocks = numberOfBlocks;
}

quint16 BlockBarWidget::GetSpacing() const
{
    return m_spacing;
}

void BlockBarWidget::SetSpacing(const quint16& spacing)
{
    m_spacing = spacing;
}

UIWidgetBase* BlockBarWidget::CloneThis(quint32 widgetID)
{
    return new BlockBarWidget(widgetID, m_layoutDefinition);
}

QString BlockBarWidget::GetUIWidgetTypeName() const
{
    return BlockBarWidget::TypeName;
}

GraphicsItemBase* BlockBarWidget::GetGraphicsItem(bool drawChromaKeyColor)
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


    BlockBarWidgetGraphicsItem* graphicsItem = new BlockBarWidgetGraphicsItem(GetX(), GetY(), GetWidth(), GetHeight(), GetPrimaryColorPair()->GetFGColor(), bgColor, m_spacing, m_numberOfBlocks, drawChromaKeyColor);
    return graphicsItem;
}

QString BlockBarWidget::LongDescription()
{
    return "This widget can be used to show values and levels in the form of a block bar. " \
           "It is possible to configure the amount of blocks used as well as max and min values. " \
           "It takes a DynamicValue as input.";
}

QVariant BlockBarWidget::GetWidgetPropertyData(int column) const
{
    switch (column)
    {
        case NumberOfBlocksColumn:
            return GetNumberOfBlocks();
        case MinColumn:
            return GetMin();
        case MaxColumn:
            return GetMax();
        case SpacingColumn:
            return GetSpacing();
        case SinkColumn:
        {
            GCLSignalSink* signalSink = GetSignalSinks().first();
            GCLSignalSinkMetaType gclSignalSinkMetaType(signalSink);
            QVariant variant;
            variant.setValue(gclSignalSinkMetaType);
            return variant;
        }
        default:
            return "INVALID";
    }
}

QString BlockBarWidget::GetWidgetPropertyHeader(int column) const
{
    switch (column)
    {
        case NumberOfBlocksColumn:
            return "Blocks";
        case MinColumn:
            return "Min";
        case MaxColumn:
            return "Max";
        case SpacingColumn:
            return "Spacing";
        case SinkColumn:
            return "Signal Sink";
        default:
            return "INVALID";
    }
}

void BlockBarWidget::SetWidgetProperty(int column, QVariant value)
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
        default:
            return;
    }
}

UIWidgetBase::DataType_t BlockBarWidget::GetWidgetPropertyDataType(int column) const
{
    switch (column)
    {
        case NumberOfBlocksColumn:
            return SpinBox_Uint8;
        case MinColumn:
            return SpinBox_Uint32;
        case MaxColumn:
            return SpinBox_Uint32;
        case SpacingColumn:
            return SpinBox_Uint16;
        case SinkColumn:
            return SignalSink;
        default:
            return Not_Available;
    }
}

QString BlockBarWidget::WidgetPropertyToolTip(int column) const
{
    switch (column)
    {
        case NumberOfBlocksColumn:
            return "The number of blocks to display in the block bar";
        case MinColumn:
            return "The minimum value for the block bar input";
        case MaxColumn:
            return "The maximum value for the block bar input";
        case SpacingColumn:
            return "The spacing between the blocks";
        case SinkColumn:
            return "The GCL signal mapping for this widget";
        default:
            return "INVALID";
    }
}

int BlockBarWidget::GetWidgetPropertyCount() const
{
    return BlockBarWidgetColumnCount;
}


