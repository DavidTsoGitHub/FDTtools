#include "IconWidgetGraphicsItem.h"
#include "Logger.h"
#include "Project.h"
#include "ColorPair.h"

#include <QDir>

IconWidgetGraphicsItem::IconWidgetGraphicsItem(Project* project, quint16 x, quint16 y, quint16 width, quint16 height,
                                               ColorPair* colorPair, const QString& resourcePath, bool drawChromaKeyColor)
    : GraphicsItemBase(x, y, width, height, colorPair->GetFGColor(), colorPair->GetBGColor(), drawChromaKeyColor),
    m_resourcePath(resourcePath),
    m_project(project),
    m_colorPair(colorPair)
{}

const QString IconWidgetGraphicsItem::GetResourcePath()
{
    if (m_resourcePath.isEmpty())
    {
        LOG_ERROR("No resource path set.");
    }

    return m_resourcePath;
}

void IconWidgetGraphicsItem::SetResourcePath(QString resourcePath)
{
    m_resourcePath = resourcePath;
}

void IconWidgetGraphicsItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    drawBorder(painter);

    QImage image;
    image.load(GetResourcePath());
    image = image.convertToFormat(QImage::Format_MonoLSB, Qt::MonoOnly|Qt::ThresholdDither|Qt::AvoidDither);

    if (m_drawChromaKeyColor && m_colorPair->GetBGIsTransparent())
    {
        image.setColorTable({m_project->GetLayoutDefinition().GetStartChromaKeyColor().rgb(), m_fgColor.rgb()});
    }
    else
    {
        if (m_colorPair->GetBGIsTransparent())
        {
            image.setColorTable({QColor("transparent").rgb(), m_fgColor.rgb()});
        }
        else
        {
            image.setColorTable({m_bgColor.rgb(), m_fgColor.rgb()});
        }
    }
    painter->drawImage( (GetWidth()/2) - (image.width()/2), (GetHeight()/2) - (image.height()/2), image);
}
