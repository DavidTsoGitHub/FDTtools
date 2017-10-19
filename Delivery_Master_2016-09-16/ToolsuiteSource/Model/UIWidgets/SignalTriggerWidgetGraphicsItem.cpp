#include "SignalTriggerWidgetGraphicsItem.h"

SignalTriggerWidgetGraphicsItem::SignalTriggerWidgetGraphicsItem(quint16 x, quint16 y, quint16 width, quint16 height, bool drawChromaKeyColor) :
    GraphicsItemBase(x, y, width, height, Qt::green, Qt::black, drawChromaKeyColor)
{

}

void SignalTriggerWidgetGraphicsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    drawBorder(painter);

    quint16 height = GetHeight();
    QFontMetrics fm(painter->font());

    if(m_drawChromaKeyColor == true)
    {
        painter->setBackgroundMode(Qt::OpaqueMode);
        painter->setBackground(QBrush(m_bgColor));
    }
    else
    {
        painter->setBackgroundMode(Qt::TransparentMode);
    }

    painter->setPen(m_fgColor);
    painter->drawText(2, height/2+fm.ascent()/2, "SignalTrigger");
}

