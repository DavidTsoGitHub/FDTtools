#include "ColumnLevelWidgetGraphicsItem.h"

ColumnLevelWidgetGraphicsItem::ColumnLevelWidgetGraphicsItem(quint16 x, quint16 y, quint16 width, quint16 height,
                                                             QColor fgColor, QColor bgColor, quint16 spacing, quint8 numberOfBlocks,
                                                             quint8 minBlockHeight, quint8 maxBlockHeight, bool drawChromaKeyColor)
    : GraphicsItemBase(x, y, width, height, fgColor, bgColor, drawChromaKeyColor),
    m_spacing(spacing),
    m_numberOfBlocks(numberOfBlocks < 1 ? 1 : numberOfBlocks),
    m_minBlockHeight(minBlockHeight),
    m_maxBlockHeight(maxBlockHeight)
{}

void ColumnLevelWidgetGraphicsItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    drawBorder(painter);

    int heightDiff = 0;
    int widthPerBlock = GetWidth() / m_numberOfBlocks;
    widthPerBlock = widthPerBlock - m_spacing < 2 ? 1 : widthPerBlock - m_spacing;
    if (m_numberOfBlocks > 1)
    {
        heightDiff = (m_maxBlockHeight - m_minBlockHeight) / (m_numberOfBlocks - 1);
    }

    if(m_drawChromaKeyColor == true)
    {
        painter->setBackgroundMode(Qt::OpaqueMode);
        painter->fillRect(0, 0, GetWidth(), GetHeight(), QBrush(m_bgColor));
        painter->setBackground(QBrush(m_bgColor));
    }
    else
    {
        painter->setBackgroundMode(Qt::TransparentMode);
    }
    painter->setPen(m_fgColor);

    int startXPos = 0;
    int currentBlockHeight = m_minBlockHeight;

    for (int i = 0; i < m_numberOfBlocks; ++i)
    {
        if (i > 0)
        {
            startXPos += widthPerBlock + m_spacing;
            currentBlockHeight += heightDiff;
        }

        painter->drawRect(startXPos, GetHeight() - currentBlockHeight, widthPerBlock, currentBlockHeight);
    }
}
