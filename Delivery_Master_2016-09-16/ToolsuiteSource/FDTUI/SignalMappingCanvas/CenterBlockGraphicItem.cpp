#include "CenterBlockGraphicItem.h"

#include "IGCLSignalSink.h"
#include "IGCLSignalSource.h"
#include "GCLSignalSink.h"
#include "GCLSignalSource.h"
#include "ProjectObject.h"
#include "SignalMappingCanvasConstants.h"

#include <QHash>
#include <QGraphicsItem>
#include <QPainter>
#include <QRectF>
#include <QStyleOptionGraphicsItem>
#include <QWidget>

CenterBlockGraphicItem::CenterBlockGraphicItem(ProjectObject* centerObject, qreal x, qreal y) :
    m_centerObject(centerObject)
{
    m_centerObjectAsSink = dynamic_cast<IGCLSignalSink*>(centerObject);
    m_centerObjectAsSource = dynamic_cast<IGCLSignalSource*>(centerObject);

    calculateHeight();

    setX(x);
    setY(y);
    calculateEndpointCoordinates();
    setFlag(QGraphicsItem::ItemIsSelectable, true);
}

CenterBlockGraphicItem::~CenterBlockGraphicItem()
{}

void CenterBlockGraphicItem::GetSourceEndpointCoordinates(QHash<const GCLSignalSource*, QPointF>& endpointCoordinates)
{
    QHash<const GCLSignalSource*, QPointF>::iterator i;
    for (i = m_sourceEndpointCoordinates.begin(); i != m_sourceEndpointCoordinates.end(); ++i)
    {
        endpointCoordinates.insert(i.key(), i.value());
    }
}

void CenterBlockGraphicItem::GetSinkEndpointCoordinates(QHash<const GCLSignalSink*, QPointF>& endpointCoordinates)
{
    QHash<const GCLSignalSink*, QPointF>::iterator i;
    for (i = m_sinkEndpointCoordinates.begin(); i != m_sinkEndpointCoordinates.end(); ++i)
    {
        endpointCoordinates.insert(i.key(), i.value());
    }
}

const QList<GCLSignalSink*> CenterBlockGraphicItem::GetSignalSinks() const
{
    if (m_centerObjectAsSink == NULL) return QList<GCLSignalSink*>();

    return m_centerObjectAsSink->GetSignalSinks();
}

const QList<GCLSignalSource*> CenterBlockGraphicItem::GetSignalSources() const
{
    if (m_centerObjectAsSource== NULL) return QList<GCLSignalSource*>();

    return m_centerObjectAsSource->GetSignalSources();
}

QRectF CenterBlockGraphicItem::boundingRect() const
{
    return QRectF(0, 0, CENTERBLOCK_WIDTH, m_height);
}

void CenterBlockGraphicItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    drawBorder(painter);
    drawComponentName(painter);
    drawInputs(painter);
    drawOutputs(painter);

#ifdef QT_DEBUG
    // Draw box information in bottom right corner for debugging
    painter->setFont(SignalMappingCanvasConstants::SmallDebugFont);
    QString information(QString::number(CENTERBLOCK_WIDTH) + "x" + QString::number(this->m_height) + " " + QString::number(this->x()) + "," + QString::number(this->y()));
    painter->drawText((CENTERBLOCK_WIDTH-painter->fontMetrics().width(information))/2, m_height-5, information);
#endif
}

/*!
 * \brief Calculate height for the center block
 * Determine the required height for the block by checking the number of signals we need to visualize.
 * We will alternate output and input signals to avoid text collision
 */
void CenterBlockGraphicItem::calculateHeight()
{
    int sinkCount = GetSignalSinks().size();
    int sourceCount = GetSignalSources().size();
    int countToConsider = qMax(sinkCount, sourceCount);

    m_height = CENTERBLOCK_TITLE_HEIGHT + countToConsider * HEIGHT_PER_CONNECTOR;

    m_height += (sinkCount == sourceCount ? HEIGHT_PER_CONNECTOR : HEIGHT_PER_CONNECTOR/2);
}

void CenterBlockGraphicItem::calculateEndpointCoordinates()
{
    qreal inputY = CENTERBLOCK_TITLE_HEIGHT + CENTERBLOCK_SECTION_GAP;
    qreal outputY = CENTERBLOCK_TITLE_HEIGHT + CENTERBLOCK_SECTION_GAP;

    const QList<GCLSignalSink*>& sinks = GetSignalSinks();
    const QList<GCLSignalSource*>& sources = GetSignalSources();

    if (sources.count() > sinks.count())
    {
        inputY += HEIGHT_PER_CONNECTOR/2;
    }
    else
    {
        outputY += HEIGHT_PER_CONNECTOR/2;
    }

    foreach (const GCLSignalSource* source, sources)
    {
        QPointF coords = mapToScene(CENTERBLOCK_WIDTH, outputY);
        m_sourceEndpointCoordinates.insert(source, coords);
        outputY += HEIGHT_PER_CONNECTOR;
    }

    foreach (const GCLSignalSink* sink, sinks)
    {
        QPointF coords = mapToScene(0, inputY);
        m_sinkEndpointCoordinates.insert(sink, coords);
        inputY += HEIGHT_PER_CONNECTOR;
    }
}

void CenterBlockGraphicItem::drawBorder(QPainter* painter)
{
    QPen oldPen(painter->pen());
    QPen newPen(painter->pen());
    newPen.setColor(QColor("blue"));
    newPen.setWidth(2);

    if (isSelected())
    {
        painter->setPen(newPen);
    }

    painter->drawRoundedRect(1, 1, CENTERBLOCK_WIDTH-2, m_height-2, 5, 5);
    painter->drawLine(1, CENTERBLOCK_SECTION_GAP, CENTERBLOCK_WIDTH, CENTERBLOCK_SECTION_GAP);
    painter->setPen(oldPen);
}

void CenterBlockGraphicItem::drawComponentName(QPainter* painter)
{
    painter->setFont(SignalMappingCanvasConstants::CenterBlockTitleFont);
    int nameWidth = painter->fontMetrics().width(m_centerObject->objectName());
    qreal nameX = (nameWidth < CENTERBLOCK_WIDTH) ? (CENTERBLOCK_WIDTH-nameWidth)/2 : 5;
    painter->drawText(QRectF(nameX, 6, CENTERBLOCK_WIDTH-(nameX*2), 22), m_centerObject->objectName());
}

void CenterBlockGraphicItem::drawInputs(QPainter* painter)
{
    painter->setFont(SignalMappingCanvasConstants::SignalNameFont);
    foreach (const GCLSignalSink* sink, GetSignalSinks())
    {
        QPointF coords = mapFromScene(m_sinkEndpointCoordinates.value(sink));
        painter->drawText(5, coords.y()+3, sink->SignalName());
    }
}

void CenterBlockGraphicItem::drawOutputs(QPainter* painter)
{
    foreach (const GCLSignalSource* source, GetSignalSources())
    {
        qreal x = CENTERBLOCK_WIDTH - painter->fontMetrics().width(source->SignalName()) - 5;
        QPointF coords = mapFromScene(m_sourceEndpointCoordinates.value(source));
        painter->drawText(x, coords.y()+3, source->SignalName());
    }
}
