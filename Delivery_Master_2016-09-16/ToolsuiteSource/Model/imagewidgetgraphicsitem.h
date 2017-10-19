#ifndef IMAGEWIDGETGRAPHICSITEM
#define IMAGEWIDGETGRAPHICSITEM

#include <GraphicsItemBase.h>

class ImageWidgetGraphicsItem : public GraphicsItemBase
{
public:
    ImageWidgetGraphicsItem(quint16 x, quint16 y, quint16 width, quint16 height, bool drawChromaKeyColor, QColor chromaKeyColor, const QString& resourcePath,
                            const QList<QRect>& animPreviewRects = {});

    const QString GetResourcePath(void);
    void SetResourcePath(QString resourcePath);

    // QGraphicsItem interface
public:
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget);

private:
    QColor m_chromaKeyColor;
    QString m_resourcePath;
    QList<QRect> m_animPreviewRects;
};

#endif // IMAGEWIDGETGRAPHICSITEM

