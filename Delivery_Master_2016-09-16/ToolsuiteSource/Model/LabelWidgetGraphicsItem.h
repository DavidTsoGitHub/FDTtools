#ifndef LABELWIDGETGRAPHICSITEM_H
#define LABELWIDGETGRAPHICSITEM_H

#include "GraphicsItemBase.h"
#include "LabelDefinition.h"
#include "UIWidgetBase.h"

class LabelWidgetGraphicsItem : public GraphicsItemBase
{
public:
    LabelWidgetGraphicsItem(quint16 x, quint16 y, quint16 width, quint16 height, QColor fgColor, QColor bgColor, const QString& labelText, bool drawCromaKeyColor);

    QString GetLabelText() const;
    void SetLabelText(QString labelText);

    // QGraphicsItem interface
public:
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget);

private:
    QString m_labelText;
};

#endif // LABELWIDGETGRAPHICSITEM_H
