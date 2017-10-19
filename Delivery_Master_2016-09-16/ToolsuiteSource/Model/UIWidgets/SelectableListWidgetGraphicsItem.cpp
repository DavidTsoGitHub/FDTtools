#include "SelectableListWidgetGraphicsItem.h"

SelectableListWidgetGraphicsItem::SelectableListWidgetGraphicsItem(quint16 x, quint16 y, quint16 width, quint16 height, QColor fgColor, QColor bgColor, bool drawChromaKeyColor) :
    GraphicsItemBase(x, y, width, height, fgColor, bgColor, drawChromaKeyColor)
{}

void SelectableListWidgetGraphicsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    drawBorder(painter);

    quint16 width = GetWidth();
    quint16 height = GetHeight();
    QFontMetrics fm(painter->font());

    if(m_drawChromaKeyColor == false)
    {
        painter->setBackgroundMode(Qt::TransparentMode);
    }
    else
    {
        painter->setBackgroundMode(Qt::OpaqueMode);
        //painter->fillRect(0, 0, GetWidth(), GetHeight(), QBrush(m_bgColor));
        painter->setBackground(QBrush(m_bgColor));
    }

    painter->setPen(m_fgColor);
    painter->drawText(2, height/2+fm.ascent()/2, "SelectableList");

    QPainterPath upArrow;
    upArrow.moveTo(width-5, 0);
    upArrow.lineTo(width-10, 5);
    upArrow.lineTo(width, 5);
    upArrow.lineTo(width-5, 0);
    painter->fillPath(upArrow, QBrush(m_fgColor));

    QPainterPath downArrow;
    downArrow.moveTo(width-5, height);
    downArrow.lineTo(width-10, height-5);
    downArrow.lineTo(width, height-5);
    downArrow.lineTo(width-5, height);
    painter->fillPath(downArrow, QBrush(m_fgColor));
}
