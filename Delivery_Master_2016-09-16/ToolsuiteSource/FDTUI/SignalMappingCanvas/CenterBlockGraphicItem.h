#ifndef FUNCTIONBLOCKGRAPHICITEM_H
#define FUNCTIONBLOCKGRAPHICITEM_H

class IGCLSignalSink;
class IGCLSignalSource;
class GCLSignalSink;
class GCLSignalSource;
class ProjectObject;

template <class Key, class T> class QHash;
class QGraphicsItem;
class QPainter;
class QRectF;
class QStyleOptionGraphicsItem;
class QWidget;

class CenterBlockGraphicItem : public QGraphicsItem
{

public:
    CenterBlockGraphicItem(ProjectObject* centerBlock, qreal x, qreal y);
    ~CenterBlockGraphicItem();

    void GetSourceEndpointCoordinates(QHash<const GCLSignalSource*, QPointF>& endpointCoordinates);
    void GetSinkEndpointCoordinates(QHash<const GCLSignalSink*, QPointF>& endpointCoordinates);
    const QList<GCLSignalSink*> GetSignalSinks() const;
    const QList<GCLSignalSource*> GetSignalSources() const;

    // QGraphicsItem interface
public:
    QRectF boundingRect(void) const;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget);

private:
    ProjectObject* m_centerObject;
    IGCLSignalSink* m_centerObjectAsSink;
    IGCLSignalSource* m_centerObjectAsSource;
    QHash<const GCLSignalSource*, QPointF> m_sourceEndpointCoordinates;
    QHash<const GCLSignalSink*, QPointF> m_sinkEndpointCoordinates;
    qreal m_height;

    void calculateHeight(void);
    void calculateEndpointCoordinates(void);

    void drawBorder(QPainter* painter);
    void drawComponentName(QPainter* painter);
    void drawInputs(QPainter* painter);
    void drawOutputs(QPainter* painter);
};

#endif // FUNCTIONBLOCKGRAPHICITEM_H
