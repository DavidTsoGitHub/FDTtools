#ifndef CURVEWIDGET_H
#define CURVEWIDGET_H

class QEasingCurve;
class QGraphicsView;
class QGraphicsScene;

class CurveWidget : public QGraphicsView
{
    Q_OBJECT
public:
    explicit CurveWidget(QWidget *parent = 0);

    static const int GRAPHICS_ITEM_ID_CONTROL_MARKER = 1;
    static const int GRAPHICS_ITEM_ID_AXIS = 2;
    static const int GRAPHICS_ITEM_ID_SEGMENT = 3;
    static const int GRAPHICS_ITEM_ID_BEZIER_SEGMENT = 4;

    static const int DATA_ID = 0;
    static const int DATA_PLACEMENT = 1;

    virtual void itemMoving(int id);
    virtual void itemMoved(int id);

protected:
    // Control marker
    void AddControlMarker(QPointF pos, int id);
    void AddControlMarker(QPointF pos, QColor color1, QColor color2, int id);
    void SetControlMarkerPos(QPointF pos, int id);
    QPointF GetControlMarkerPos(int id);
    void SetControlMarkerValidRect(QRectF rect, int id);

    // Axis
    void AddAxis(QPointF p1, QPointF p2, int id);
    void SetAxisEndPoints(QPointF p1, QPointF p2, int id);
    void SetAxisTicks(qreal major, qreal minor, int id);

    // Label
    enum class Placement {Above, Below, Left, Right};
    void AddLabel(QPointF pos, QString text, Placement pl, QColor color, int id);
    void SetLabelPos(QPointF pos, int id);
    void SetLabelText(QString text, int id);

    // QEasingCurve-based curve segment
    void AddCurveSegment(QPointF p1, QPointF p2, const QEasingCurve&, int id);
    void SetCurveSegmentEndPoints(QPointF p1, QPointF p2, int id);
    void SetCurveSegmentEasingCurve(const QEasingCurve& curve, int id);

    // Bezier curve segment
    void AddBezier(int id);
    void SetBezierP0(QPointF p, int id);
    void SetBezierP1(QPointF p, int id);
    void SetBezierC0(QPointF p, int id);
    void SetBezierC1(QPointF p, int id);

    // Line segment
    void AddLine(int id);
    void SetLineP0(QPointF p, int id);
    void SetLineP1(QPointF p, int id);

    // Grid
    void AddGrid(QRectF rect, qreal spacing);

    // Cross
    void AddCross(QPointF pos, qreal size);

    // Visibility
    void SetVisible(bool visible, int id);

private:
    QGraphicsScene *m_scene;

    QGraphicsItem *findItem(int id);
};

#endif // CURVEWIDGET_H
