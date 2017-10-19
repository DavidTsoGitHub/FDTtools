#include "ColorPair.h"

#include <QColor>

ColorPair::ColorPair(QColor fgColor, QColor bgColor, bool backgroundIsTransparent, ProjectObject* parent) : ProjectObject("ColorPair", parent),
    m_foregroundColor(fgColor),
    m_backgroundColor(bgColor),
    m_transparentBackground(backgroundIsTransparent)
{
}

ColorPair::ColorPair(ProjectObject* parent) : ProjectObject("ColorPair", parent),
    m_foregroundColor(Qt::white),
    m_backgroundColor(Qt::black),
    m_transparentBackground(false)
{
}

void ColorPair::SetFGColorComponent(ColorComponent colorComponent, quint8 value)
{
    if (colorComponent == ColorComponent::RED)
    {
        m_foregroundColor.setRed(value);
    }
    else if (colorComponent == ColorComponent::GREEN)
    {
        m_foregroundColor.setGreen(value);
    }
    else if (colorComponent == ColorComponent::BLUE)
    {
        m_foregroundColor.setBlue(value);
    }

    NotifyPropertyUpdated();
}

QColor ColorPair::GetFGColor() const
{
    return m_foregroundColor;
}

void ColorPair::SetBGIsTransparent(bool transparent)
{
    if (m_transparentBackground != transparent)
    {
        m_transparentBackground = transparent;
        NotifyPropertyUpdated();
    }
}

bool ColorPair::GetBGIsTransparent()
{
    return m_transparentBackground;
}

void ColorPair::SetBGColorComponent(ColorComponent colorComponent, quint8 value)
{
    if (colorComponent == ColorComponent::RED)
    {
        m_backgroundColor.setRed(value);
    }
    else if (colorComponent == ColorComponent::GREEN)
    {
        m_backgroundColor.setGreen(value);
    }
    else if (colorComponent == ColorComponent::BLUE)
    {
        m_backgroundColor.setBlue(value);
    }

    NotifyPropertyUpdated();
}

QColor ColorPair::GetBGColor() const
{
    return m_backgroundColor;
}
