#include <QGridLayout>
#include <QPainterPath>
#include <QRectF>

#include "UreaLevel.h"

UreaLevel::UreaLevel(QWidget *parent) : QWidget(parent)
{
    m_graphicsScene = new QGraphicsScene(this);
    m_graphicsView = new QGraphicsView(m_graphicsScene);
    setWindowFlags(Qt::FramelessWindowHint);
    m_graphicsView->setAlignment(Qt::AlignCenter);
    m_graphicsView->setStyleSheet("QGraphicsView { border-style: none; background: transparent}");
    m_graphicsView->setRenderHint(QPainter::Antialiasing);

    m_segmentEnabledBrush = new QBrush(QColor("blue"));
    m_segmentDisabledBrush = new QBrush(QColor(40,40,40));

    drawSegments();
    setLayout(new QGridLayout());
    layout()->setContentsMargins(0, 0, 0, 0);
    layout()->addWidget(m_graphicsView);
}

UreaLevel::~UreaLevel()
{

}

void UreaLevel::SetSegment0State(bool enabled)
{
    setSegmentState(m_segment0Graphics, enabled);
}

void UreaLevel::SetSegment1State(bool enabled)
{
    setSegmentState(m_segment1Graphics, enabled);
}

void UreaLevel::SetSegment2State(bool enabled)
{
    setSegmentState(m_segment2Graphics, enabled);
}

void UreaLevel::SetSegment3State(bool enabled)
{
    setSegmentState(m_segment3Graphics, enabled);
}

void UreaLevel::setSegmentState(QGraphicsRectItem* segmentGraphics, bool enabled)
{
    if (enabled)
    {
        segmentGraphics->setBrush(*m_segmentEnabledBrush);
    }
    else
    {
        segmentGraphics->setBrush(*m_segmentDisabledBrush);
    }
}

void UreaLevel::drawSegments()
{
    m_segment0Graphics = m_graphicsScene->addRect(0, 10, 10, 20);
    m_segment1Graphics = m_graphicsScene->addRect(15, 10, 10, 20);
    m_segment2Graphics = m_graphicsScene->addRect(30, 10, 10, 20);
    m_segment3Graphics = m_graphicsScene->addRect(45, 10, 10, 20);
}
