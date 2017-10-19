#include "LabelWidgetGraphicsItem.h"
#include "LabelWidget.h"
#include "Project.h"
#include "QTextStream"
#include "Logger.h"
#include "QFontDatabase"

LabelWidgetGraphicsItem::LabelWidgetGraphicsItem(quint16 x, quint16 y, quint16 width, quint16 height,
                                                 QColor fgColor, QColor bgColor, const QString& labelText, bool drawCromaKeyColor)
    : GraphicsItemBase(x, y, width, height, fgColor, bgColor, drawCromaKeyColor),
    m_labelText(labelText)
{}

void LabelWidgetGraphicsItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    drawBorder(painter);

    //TODO: Set painters font to the current label font before painting.

    if(m_drawChromaKeyColor == false)
    {
        painter->setBackgroundMode(Qt::TransparentMode);
    }
    else
    {
        painter->setBackgroundMode(Qt::OpaqueMode);
        //painter->fillRect(0, 0, GetWidth(), GetHeight(), QBrush(m_bgColor));
        painter->setBackground(QBrush(m_bgColor));
    }

    painter->setPen(m_fgColor);
    QRectF rect;
    rect.setX(0);
    rect.setY(0);
    rect.setWidth(GetWidth());
    rect.setHeight(GetHeight());

    // The following is needed to handle multiline labels
    QStringList list = m_labelText.split("\\n");

    if (list.count() > 1)
    {
        painter->drawText(rect, list.join('\n'));
    }
    else
    {
        painter->drawText(rect, Qt::TextSingleLine, m_labelText);
    }
}

QString LabelWidgetGraphicsItem::GetLabelText() const
{
    return m_labelText;
}

void LabelWidgetGraphicsItem::SetLabelText(QString labelText)
{
    m_labelText = labelText;
}
