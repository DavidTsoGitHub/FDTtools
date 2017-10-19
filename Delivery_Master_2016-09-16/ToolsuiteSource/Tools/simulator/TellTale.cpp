#include "TellTale.h"

#include <QImage>

TellTale::TellTale(QString resourcePath, QColor activeColor, QWidget *parent) : QLabel(parent)
{
    QImage image = QImage(resourcePath);
    image = image.convertToFormat(QImage::Format_MonoLSB, Qt::MonoOnly|Qt::ThresholdDither|Qt::AvoidDither);

    image.setColor(0, QColor("transparent").rgba());
    image.setColor(1, activeColor.rgba());
    m_activePixmap = QPixmap::fromImage(image);

    image.setColor(1, QColor(40,40,40).rgba());
    m_inactivePixmap = QPixmap::fromImage(image);

    setPixmap(m_inactivePixmap);
}

void TellTale::SetTellTaleActive(bool active)
{
    if (active)
    {
        setPixmap(m_activePixmap);
    }
    else
    {
        setPixmap(m_inactivePixmap);
    }
}

