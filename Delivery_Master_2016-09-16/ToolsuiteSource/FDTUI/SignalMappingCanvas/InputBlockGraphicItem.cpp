#include "InputBlockGraphicItem.h"

#include "GCLSignalSink.h"
#include "GCLSignalSource.h"
#include "Project.h"
#include "SignalMappingCanvasConstants.h"

#include <QHash>
#include <QGraphicsItem>
#include <QString>
#include <QPainter>
#include <QRectF>
#include <QStyleOptionGraphicsItem>
#include <QTextItem>
#include <QWidget>

InputBlockGraphicItem::InputBlockGraphicItem(QString componentName, QList<const GCLSignalSource*> sources, qreal x, qreal y) :
    m_componentName(componentName)
{
    foreach (const GCLSignalSource* source, sources)
    {
        m_sourcesByName.insert(source->SignalName(), source);
    }

    calculateHeight();
    setX(x);
    setY(y);
    calculateEndpointCoordinates();
}

InputBlockGraphicItem::~InputBlockGraphicItem()
{}

void InputBlockGraphicItem::GetEndpointCoordinates(QHash<const GCLSignalSource*, QPointF>& endpointCoordinates)
{
    QHash<const GCLSignalSource*, QPointF>::iterator i;
    for (i = m_sourceEndpointCoordinates.begin(); i != m_sourceEndpointCoordinates.end(); ++i)
    {
        endpointCoordinates.insert(i.key(), i.value());
    }
}

QRectF InputBlockGraphicItem::boundingRect() const
{
    return QRectF(0, 0, IOBLOCK_WIDTH, m_height);
}

void InputBlockGraphicItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
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

void InputBlockGraphicItem::calculateHeight()
{
    m_height = IOBLOCK_TITLE_HEIGHT + (IOBLOCK_SECTION_GAP/2) + (m_sourcesByName.count() * HEIGHT_PER_CONNECTOR);
}

void InputBlockGraphicItem::calculateEndpointCoordinates()
{
    qreal y = IOBLOCK_TITLE_HEIGHT + IOBLOCK_SECTION_GAP;
    foreach (const GCLSignalSource* source, m_sourcesByName.values())
    {
        QPointF coords = mapToScene(IOBLOCK_WIDTH, y);
        m_sourceEndpointCoordinates.insert(source, coords);

        y += HEIGHT_PER_CONNECTOR;
    }
}

void InputBlockGraphicItem::drawBorder(QPainter* painter)
{
    painter->drawRoundedRect(1, 1, IOBLOCK_WIDTH-2, m_height-2, 5, 5);
    painter->drawLine(1, IOBLOCK_TITLE_HEIGHT, IOBLOCK_WIDTH-1, IOBLOCK_TITLE_HEIGHT);
}

void InputBlockGraphicItem::drawComponentName(QPainter* painter)
{
    painter->setFont(SignalMappingCanvasConstants::IOBlockTitleFont);
    int nameWidth = painter->fontMetrics().width(m_componentName);
    qreal nameX = (nameWidth < IOBLOCK_WIDTH) ? (IOBLOCK_WIDTH-nameWidth)/2 : 5;
    painter->drawText(QRectF(nameX, 8, IOBLOCK_WIDTH-(nameX*2), 20), m_componentName);
}

void InputBlockGraphicItem::drawSignals(QPainter* painter)
{
    painter->setFont(SignalMappingCanvasConstants::SignalNameFont);
    foreach (const GCLSignalSource* source, m_sourcesByName)
    {
        qreal x = IOBLOCK_WIDTH - painter->fontMetrics().width(source->SignalName()) - 5;
        QPointF coords = mapFromScene(m_sourceEndpointCoordinates.value(source));
        painter->drawText(x, coords.y()+3, source->SignalName());
    }
}
