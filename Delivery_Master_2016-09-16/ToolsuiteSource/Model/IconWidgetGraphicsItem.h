#ifndef ICONCONTAINERGRAPHICSITEM_H
#define ICONCONTAINERGRAPHICSITEM_H

#include <GraphicsItemBase.h>
class Project;
class ColorPair;

class IconWidgetGraphicsItem : public GraphicsItemBase
{
public:
    IconWidgetGraphicsItem(Project* project, quint16 x, quint16 y, quint16 width, quint16 height,
                           ColorPair *colorPair, const QString& resourcePath, bool drawChromaKeyColor);

    const QString GetResourcePath(void);
    void SetResourcePath(QString resourcePath);

    // QGraphicsItem interface
public:
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget);

private:
    QString m_resourcePath;
    Project* m_project;
    ColorPair* m_colorPair;
};

#endif // ICONCONTAINERGRAPHICSITEM_H
