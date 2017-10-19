#ifndef COLORPAIR_H
#define COLORPAIR_H

#include "ProjectObject.h"
class QColor;

enum class ColorComponent
{
    RED,
    GREEN,
    BLUE
};

class ColorPair : public ProjectObject
{
public:


    explicit ColorPair(QColor fgColor, QColor bgColor, bool backgroundIsTransparent, ProjectObject* parent = 0);
    explicit ColorPair(ProjectObject* parent = 0);

    void SetFGColorComponent(ColorComponent colorComponent, quint8 value);
    QColor GetFGColor() const;

    void SetBGColorComponent(ColorComponent colorComponent, quint8 value);
    QColor GetBGColor() const;

    void SetBGIsTransparent(bool transparent);
    bool GetBGIsTransparent(void);

private:
    QColor m_foregroundColor;
    QColor m_backgroundColor;
    bool m_transparentBackground;
};

#endif // COLORPAIR_H
