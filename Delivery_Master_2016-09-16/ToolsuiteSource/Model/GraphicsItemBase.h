#ifndef GRAPHICSITEMBASE_H
#define GRAPHICSITEMBASE_H

#include <QColor>
#include <QGraphicsItem>
#include <QList>
#include <QPainter>
#include <QRect>

class GraphicsItemBase : public QGraphicsItem
{
public:
    GraphicsItemBase(void);
    GraphicsItemBase(quint16 x, quint16 y, quint16 width, quint16 height, QColor fgColor, QColor bgColor, bool drawChromaKeyColor);

    virtual void SetHighLight(bool highLight);

    void SetX(const quint16& x);
    void SetY(const quint16& y);

    quint16 GetWidth() const;
    void SetWidth(const quint16& width);

    quint16 GetHeight() const;
    void SetHeight(const quint16& height);

    void SetFGColor(QColor color);
    QColor GetFGColor() const;
    void SetBGColor(QColor color);
    QColor GetBGColor() const;

    // QGraphicsItem interface
public:
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget);
    QRectF boundingRect(void) const;
protected:
    bool m_highLight;
    QPen m_highLightPen;
    QPen m_boxBorderPen;

    quint16 m_width;
    quint16 m_height;

    QColor m_fgColor;
    QColor m_bgColor;

    bool m_drawChromaKeyColor;

    void drawBorder(QPainter* painter);
    void drawDynamicContentMarker(QPainter* painter);
    void drawAnimationPreviewRects(QPainter* painter, const QList<QRect>& rects);
};

#endif // GRAPHICSITEMBASE_H
