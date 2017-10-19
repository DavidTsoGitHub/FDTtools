#include "OutputBlockGraphicItem.h"

#include "GCLSignalSink.h"
#include "SignalMappingCanvasConstants.h"

#include <QGraphicsItem>
#include <QGraphicsView>
#include <QHash>
#include <QList>
#include <QString>
#include <QPainter>
#include <QRectF>
#include <QStyleOptionGraphicsItem>
#include <QTextItem>
#include <QWidget>

OutputBlockGraphicItem::OutputBlockGraphicItem(QString componentName, QList<const GCLSignalSink*> sinks, qreal x, qreal y) :
    m_componentName(componentName)
{
    foreach (const GCLSignalSink* sink, sinks)
    {
        m_sinksByName.insert(sink->SignalName(), sink);
    }

    calculateHeight();
    setX(x);
    setY(y);
    calculateEndpointCoordinates();
}

OutputBlockGraphicItem::~OutputBlockGraphicItem()
{}

void OutputBlockGraphicItem::GetEndpointCoordinates(QHash<const GCLSignalSink*, QPointF>& endpointCoordinates)
{
    QHash<const GCLSignalSink*, QPointF>::iterator i;
    for (i = m_sinkEndpointCoordinates.begin(); i != m_sinkEndpointCoordinates.end(); ++i)
    {
        endpointCoordinates.insert(i.key(), i.value());
    }
}

QRectF OutputBlockGraphicItem::boundingRect() const
{
    return QRectF(0, 0, IOBLOCK_WIDTH, m_height);
}

void OutputBlockGraphicItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    drawBorder(painter);
    drawComponentName(painter);
    drawSignals(painter);

#ifdef QT_DEBUG
    // Draw box information in bottom right corner for debugging
    painter->setFont(SignalMappingCanvasConstants::SmallDebugFont);
    QString information(QString::number(IOBLOCK_WIDTH) + "x" + QString::number(this->m_height) + " " + QString::number(this->x()) + "," + QString::number(this->y()));
    painter->drawText((IOBLOCK_WIDTH-painter->fontMetrics().width(information))/2, m_height-5, information);
#endif
}

void OutputBlockGraphicItem::calculateHeight()
{
    m_height = IOBLOCK_TITLE_HEIGHT + (IOBLOCK_SECTION_GAP/2) + (m_sinksByName.count() * HEIGHT_PER_CONNECTOR);
}

void OutputBlockGraphicItem::calculateEndpointCoordinates()
{
    qreal y = IOBLOCK_TITLE_HEIGHT + IOBLOCK_SECTION_GAP;
    foreach (const GCLSignalSink* sink, m_sinksByName.values())
    {
        QPointF coords = mapToScene(0, y);
        m_sinkEndpointCoordinates.insert(sink, coords);

        y += HEIGHT_PER_CONNECTOR;
    }
}

void OutputBlockGraphicItem::drawBorder(QPainter* painter)
{
    painter->drawRoundedRect(1, 1, IOBLOCK_WIDTH-2, m_height-2, 5, 5);
    painter->drawLine(1, IOBLOCK_TITLE_HEIGHT, IOBLOCK_WIDTH-1, IOBLOCK_TITLE_HEIGHT);
}

void OutputBlockGraphicItem::drawComponentName(QPainter* painter)
{
    painter->setFont(SignalMappingCanvasConstants::IOBlockTitleFont);
    int nameWidth = painter->fontMetrics().width(m_componentName);
    qreal nameX = (nameWidth < IOBLOCK_WIDTH) ? (IOBLOCK_WIDTH-nameWidth)/2 : 5;
    painter->drawText(QRectF(nameX, 8, IOBLOCK_WIDTH-(nameX*2), 20), m_componentName);
}

void OutputBlockGraphicItem::drawSignals(QPainter* painter)
{
    painter->setFont(SignalMappingCanvasConstants::SignalNameFont);
    foreach (const GCLSignalSink* sink, m_sinksByName)
    {
        QPointF coords = mapFromScene(m_sinkEndpointCoordinates.value(sink));
        painter->drawText(5, coords.y()+3, sink->SignalName());
    }
}
