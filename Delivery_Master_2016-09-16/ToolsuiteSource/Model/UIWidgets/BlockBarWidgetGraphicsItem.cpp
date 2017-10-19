#include "BlockBarWidgetGraphicsItem.h"

BlockBarWidgetGraphicsItem::BlockBarWidgetGraphicsItem(quint16 x, quint16 y, quint16 width, quint16 height,
                                                       QColor fgColor, QColor bgColor, quint16 spacing, quint8 numberOfBlocks, bool drawChromaKeyColor)
    : GraphicsItemBase(x, y, width, height, fgColor, bgColor, drawChromaKeyColor),
    m_spacing(spacing),
    m_numberOfBlocks(numberOfBlocks < 1 ? 1 : numberOfBlocks)
{}

void BlockBarWidgetGraphicsItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    drawBorder(painter);
    if(m_drawChromaKeyColor == false)
    {
        painter->setBackgroundMode(Qt::TransparentMode);
    }
    else
    {
        painter->setBackgroundMode(Qt::OpaqueMode);
        painter->fillRect(0, 0, GetWidth(), GetHeight(), QBrush(m_bgColor));
    }

    painter->setPen(m_fgColor);

    int widthPerBlock = GetWidth() / m_numberOfBlocks;
    widthPerBlock = widthPerBlock - m_spacing < 2 ? 1 : widthPerBlock - m_spacing;
    int startXPos = 0;

    for (int i = 0; i < m_numberOfBlocks; ++i)
    {
        if (i > 0)
        {
            startXPos += widthPerBlock + m_spacing;
        }

        painter->drawRect(startXPos, 2, widthPerBlock, GetHeight() - 4);
    }
}
