#include "CurveWidget.h"

#include "CurveAxis.h"
#include "CurveBezierSegment.h"
#include "CurveControlMarker.h"
#include "CurveSegment.h"

#include <QEasingCurve>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsSimpleTextItem>

CurveWidget::CurveWidget(QWidget *parent) : QGraphicsView(parent)
{
    m_scene = new QGraphicsScene(this);
    m_scene->setItemIndexMethod(QGraphicsScene::NoIndex);
    setScene(m_scene);
    setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
    setTransformationAnchor(AnchorUnderMouse);
    setMinimumSize(300, 300);
}

void CurveWidget::itemMoving(int id)
{
    Q_UNUSED(id);
}

void CurveWidget::itemMoved(int id)
{
    Q_UNUSED(id);
}

void CurveWidget::AddControlMarker(QPointF pos, int id)
{
    AddControlMarker(pos, Qt::darkGray, Qt::gray, id);
}

void CurveWidget::AddControlMarker(QPointF pos, QColor color1, QColor color2, int id)
{
    CurveControlMarker* marker = new CurveControlMarker(this, color1, color2);
    m_scene->addItem(marker);
    marker->setPos(pos);
    marker->setData(DATA_ID, id);
}

void CurveWidget::SetControlMarkerPos(QPointF pos, int id)
{
    auto *item = qgraphicsitem_cast<CurveControlMarker*>(findItem(id));
    if(item != NULL)
        item->setPos(pos);
}

QPointF CurveWidget::GetControlMarkerPos(int id)
{
    auto *item = qgraphicsitem_cast<CurveControlMarker*>(findItem(id));
    if(item != NULL)
        return item->pos();

    return QPointF();
}

void CurveWidget::SetControlMarkerValidRect(QRectF rect, int id)
{
    auto *item = qgraphicsitem_cast<CurveControlMarker*>(findItem(id));
    if(item != NULL)
        item->SetValidRect(rect);
}

void CurveWidget::AddAxis(QPointF p1, QPointF p2, int id)
{
   CurveAxis *axis = new CurveAxis(p1, p2, this);
   m_scene->addItem(axis);
   axis->setPos(p1);
   axis->setData(DATA_ID, id);
}

void CurveWidget::SetAxisEndPoints(QPointF p1, QPointF p2, int id)
{
    auto *axis = qgraphicsitem_cast<CurveAxis*>(findItem(id));
    if(axis != NULL)
        axis->SetEndpoints(p1, p2);
}

void CurveWidget::SetAxisTicks(qreal major, qreal minor, int id)
{
    auto *axis = qgraphicsitem_cast<CurveAxis*>(findItem(id));
    if(axis != NULL)
    {
        axis->SetMajorTickInterval(major);
        axis->SetMinorTickInterval(minor);
    }
}

void CurveWidget::AddLabel(QPointF pos, QString str, Placement pl, QColor color, int id)
{
    QGraphicsSimpleTextItem *label = new QGraphicsSimpleTextItem(str);
    label->setBrush(color);
    label->setData(DATA_ID, id);
    label->setData(DATA_PLACEMENT, static_cast<int>(pl));
    m_scene->addItem(label);
    SetLabelPos(pos, id);
}

void CurveWidget::SetLabelPos(QPointF pos, int id)
{
    auto *label = qgraphicsitem_cast<QGraphicsSimpleTextItem*>(findItem(id));
    if(label == NULL)
        return;

    auto pl = static_cast<Placement>(label->data(DATA_PLACEMENT).toInt());

    const auto halfW = label->boundingRect().width()/2.0;
    const auto halfH = label->boundingRect().height()/2.0;
    QPointF adjust;
    if(pl == Placement::Above)
        adjust = QPointF(-halfW, -2.0*halfH);
    else if(pl == Placement::Below)
        adjust = QPointF(-halfW, 0.0);
    else if(pl == Placement::Left)
        adjust = QPointF(-2.0*halfW, -halfH);
    else if(pl == Placement::Right)
        adjust = QPointF(0.0, -halfH);

    label->setPos(pos + adjust);
}

void CurveWidget::SetLabelText(QString text, int id)
{
    auto *label = qgraphicsitem_cast<QGraphicsSimpleTextItem*>(findItem(id));
    if(label != NULL)
        label->setText(text);
}

void CurveWidget::AddCurveSegment(QPointF p1, QPointF p2, const QEasingCurve &curve, int id)
{
    CurveSegment *segment = new CurveSegment(this);
    segment->SetEndpoints(p1, p2);
    segment->SetEasingCurve(curve);
    segment->setData(DATA_ID, id);
    m_scene->addItem(segment);
}

void CurveWidget::SetCurveSegmentEndPoints(QPointF p1, QPointF p2, int id)
{
    auto *item = qgraphicsitem_cast<CurveSegment*>(findItem(id));
    if(item != NULL)
        item->SetEndpoints(p1, p2);
}

void CurveWidget::SetCurveSegmentEasingCurve(const QEasingCurve &curve, int id)
{
    auto *item = qgraphicsitem_cast<CurveSegment*>(findItem(id));
    if(item != NULL)
        item->SetEasingCurve(curve);
}

void CurveWidget::AddBezier(int id)
{
    CurveBezierSegment *bezier = new CurveBezierSegment(this);
    bezier->setData(DATA_ID, id);
    m_scene->addItem(bezier);
}

void CurveWidget::SetBezierP0(QPointF p, int id)
{
    auto *bezier = qgraphicsitem_cast<CurveBezierSegment*>(findItem(id));
    if(bezier != NULL)
        bezier->SetP0(p);
}

void CurveWidget::SetBezierP1(QPointF p, int id)
{
    auto *bezier = qgraphicsitem_cast<CurveBezierSegment*>(findItem(id));
    if(bezier != NULL)
        bezier->SetP1(p);
}

void CurveWidget::SetBezierC0(QPointF p, int id)
{
    auto *bezier = qgraphicsitem_cast<CurveBezierSegment*>(findItem(id));
    if(bezier != NULL)
        bezier->SetC0(p);
}

void CurveWidget::SetBezierC1(QPointF p, int id)
{
    auto *bezier = qgraphicsitem_cast<CurveBezierSegment*>(findItem(id));
    if(bezier != NULL)
        bezier->SetC1(p);
}

void CurveWidget::AddLine(int id)
{
    auto *line = m_scene->addLine(QLineF(), QPen(Qt::black));
    line->setData(DATA_ID, id);
}

void CurveWidget::SetLineP0(QPointF p, int id)
{
    auto *item = qgraphicsitem_cast<QGraphicsLineItem*>(findItem(id));
    if(item != NULL)
        item->setLine(QLineF(p, item->line().p2()));
}

void CurveWidget::SetLineP1(QPointF p, int id)
{
    auto *item = qgraphicsitem_cast<QGraphicsLineItem*>(findItem(id));
    if(item != NULL)
        item->setLine(QLineF(item->line().p1(), p));
}

void CurveWidget::AddGrid(QRectF rect, qreal spacing)
{
    for(int y = rect.top() - 0.5; y < rect.bottom(); y += spacing)
        m_scene->addLine(rect.left(), y, rect.right(), y, QPen(Qt::lightGray));

    for(int x = rect.left() - 0.5; x < rect.right(); x += spacing)
        m_scene->addLine(x, rect.top(), x, rect.bottom(), QPen(Qt::lightGray));
}

void CurveWidget::AddCross(QPointF pos, qreal size)
{
    QPen pen(Qt::black, 2);
    m_scene->addLine(pos.x() - size / 2, pos.y(), pos.x() + size /2, pos.y(), pen);
    m_scene->addLine(pos.x(), pos.y() - size / 2, pos.x(), pos.y() + size / 2, pen);
}

void CurveWidget::SetVisible(bool visible, int id)
{
    auto *item = findItem(id);
    if(item != NULL)
        item->setVisible(visible);
}

QGraphicsItem *CurveWidget::findItem(int id)
{
    foreach(auto *item, m_scene->items())        
        if(item->data(DATA_ID).toInt() == id)
            return item;

    return NULL;
}
