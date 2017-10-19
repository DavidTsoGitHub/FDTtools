#include "CurveControlMarker.h"

#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QStyleOptionGraphicsItem>

#include "CurveWidget.h"

CurveControlMarker::CurveControlMarker(CurveWidget *curveWidget, QColor color1, QColor color2)
    : m_curveWidget(curveWidget), m_validRect(QRectF()), m_color1(color1), m_color2(color2)
{
    setFlag(ItemIsMovable);
    setFlag(ItemSendsGeometryChanges);
    setCacheMode(DeviceCoordinateCache);
    setZValue(2);
}

void CurveControlMarker::SetValidRect(QRectF rect)
{
    m_validRect = rect;
}

QRectF CurveControlMarker::boundingRect() const
{
    const qreal extra = 3;
    return QRectF( -5 - extra, -5 - extra, 10 + extra, 10 + extra);
}

void CurveControlMarker::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget);

    QRadialGradient gradient(-1, -1, 5);
    if (option->state & QStyle::State_Sunken) {
        gradient.setCenter(1, 1);
        gradient.setFocalPoint(1, 1);
        gradient.setColorAt(1, m_color1.light(120));
        gradient.setColorAt(0, m_color2.light(120));
    } else {
        gradient.setColorAt(1, m_color1);
        gradient.setColorAt(0, m_color2);
    }
    painter->setBrush(gradient);

    painter->setPen(QPen(m_color1, 0));
    painter->drawEllipse(-5, -5, 10, 10);
}

int CurveControlMarker::type() const
{
    return UserType + CurveWidget::GRAPHICS_ITEM_ID_CONTROL_MARKER;
}

void CurveControlMarker::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    update();
    QGraphicsItem::mousePressEvent(event);
}

void CurveControlMarker::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    update();
    QGraphicsItem::mouseReleaseEvent(event);
    m_curveWidget->itemMoved(data(CurveWidget::DATA_ID).toInt());
}

QVariant CurveControlMarker::itemChange(GraphicsItemChange change, const QVariant &value)
{
    switch (change) {

    case ItemPositionChange:
    {
        if(m_validRect.isNull())
            break;

        QPointF newPos = value.toPointF();
        if (!m_validRect.contains(newPos)) {
            newPos.setX(qMin(m_validRect.right(), qMax(newPos.x(), m_validRect.left())));
            newPos.setY(qMin(m_validRect.bottom(), qMax(newPos.y(), m_validRect.top())));
            return newPos;
        }
        break;
    }
    case ItemPositionHasChanged:
    {
        m_curveWidget->itemMoving(data(CurveWidget::DATA_ID).toInt());
        break;
    }
    default:
        break;
    };

    return QGraphicsItem::itemChange(change, value);
}
