#ifndef FUELLEVEL_H
#define FUELLEVEL_H

#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsLineItem>
#include <QGraphicsPathItem>

class UreaLevel : public QWidget
{
    Q_OBJECT
public:
    explicit UreaLevel(QWidget *parent = 0);
    ~UreaLevel();

    void SetSegment0State(bool enabled);
    void SetSegment1State(bool enabled);
    void SetSegment2State(bool enabled);
    void SetSegment3State(bool enabled);

private:
    QGraphicsScene* m_graphicsScene;
    QGraphicsView* m_graphicsView;

    QBrush* m_segmentEnabledBrush;
    QBrush* m_segmentDisabledBrush;

    QGraphicsRectItem* m_segment0Graphics;
    QGraphicsRectItem* m_segment1Graphics;
    QGraphicsRectItem* m_segment2Graphics;
    QGraphicsRectItem* m_segment3Graphics;

    void drawSegments();
    void setSegmentState(QGraphicsRectItem* segmentGraphics, bool enabled);

signals:

public slots:
};

#endif // FUELLEVEL_H
