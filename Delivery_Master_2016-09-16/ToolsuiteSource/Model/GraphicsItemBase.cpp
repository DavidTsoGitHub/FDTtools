#include "GraphicsItemBase.h"
#include "Logger.h"

GraphicsItemBase::GraphicsItemBase() :
    m_highLight(false),
    m_width(0),
    m_height(0),
    m_fgColor(Qt::white),
    m_bgColor(Qt::black),
    m_drawChromaKeyColor(true)
{
    setX(0);
    setY(0);

    m_highLightPen.setColor(QColor(255,0,0));
    m_highLightPen.setWidth(1);
    m_highLightPen.setStyle(Qt::DashLine);

    m_boxBorderPen.setColor(QColor(0xaa, 0xaa, 0xaa));
    m_boxBorderPen.setWidth(1);
    m_boxBorderPen.setStyle(Qt::DotLine);
}

GraphicsItemBase::GraphicsItemBase(quint16 x, quint16 y, quint16 width, quint16 height, QColor fgColor, QColor bgColor, bool drawChromaKeyColor) :
    m_highLight(false),
    m_width(width),
    m_height(height),
    m_fgColor(fgColor),
    m_bgColor(bgColor),
    m_drawChromaKeyColor(drawChromaKeyColor)
{
    setX(x);
    setY(y);

    m_highLightPen.setColor(QColor(255,0,0));
//    m_highLightPen.setWidth(1);
    m_highLightPen.setStyle(Qt::DashLine);

    m_boxBorderPen.setColor(QColor(0xbb, 0xbb, 0xbb));
    m_boxBorderPen.setWidth(1);
    m_boxBorderPen.setStyle(Qt::DotLine);
}

void GraphicsItemBase::SetHighLight(bool highLight)
{
    m_highLight = highLight;
}

QRectF GraphicsItemBase::boundingRect() const
{
    return QRectF(0, 0, GetWidth(), GetHeight());
}

void GraphicsItemBase::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    painter->drawRect(0, 0, GetWidth(), GetHeight());
}

void GraphicsItemBase::drawBorder(QPainter* painter)
{
    if (m_highLight)
    {
        painter->setPen(m_highLightPen);
    }
    else
    {
        painter->setPen(m_boxBorderPen);
    }
    painter->drawRect(0, 0, GetWidth(), GetHeight());
}

void GraphicsItemBase::drawDynamicContentMarker(QPainter* painter)
{
    painter->fillRect(0, 0, GetWidth(), GetHeight(), QBrush(Qt::black, Qt::BDiagPattern));
    painter->fillRect(0, 0, GetWidth(), GetHeight(), QBrush(Qt::white, Qt::FDiagPattern));
    drawBorder(painter);
}

void GraphicsItemBase::drawAnimationPreviewRects(QPainter *painter, const QList<QRect> &rects)
{
    QColor firstLast = m_fgColor;
    QColor intermediate = m_fgColor.light();
    intermediate.setAlpha(64);

    for(int i = 0; i < rects.size(); ++i)
    {
        QRect rect = rects.at(i);
        painter->setPen(i > 0 && i < rects.size() - 1 ? intermediate : firstLast);
        painter->drawRect(rect.x(), rect.y(), rect.width(), rect.height());
    }
}

quint16 GraphicsItemBase::GetWidth() const
{
    return m_width;
}

void GraphicsItemBase::SetWidth(const quint16& width)
{
    m_width = width;
}

quint16 GraphicsItemBase::GetHeight() const
{
    return m_height;
}

void GraphicsItemBase::SetHeight(const quint16& height)
{
    m_height = height;
}

void GraphicsItemBase::SetFGColor(QColor color)
{
    m_fgColor = color;
}

QColor GraphicsItemBase::GetFGColor() const
{
    return m_fgColor;
}

void GraphicsItemBase::SetBGColor(QColor color)
{
    m_bgColor = color;
}

QColor GraphicsItemBase::GetBGColor() const
{
    return m_bgColor;
}

void GraphicsItemBase::SetX(const quint16& x)
{
    setX(x);
}

void GraphicsItemBase::SetY(const quint16& y)
{
    setY(y);
}
