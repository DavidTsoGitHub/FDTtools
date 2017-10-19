#ifndef DISPLAYAREA_H
#define DISPLAYAREA_H

#include "UIWidgetContainerBase.h"
#include "UIWidgetBase.h"
#include "ISelfValidation.h"

class ColorPair;
enum class ColorComponent;
class Project;

class DisplayArea : public UIWidgetContainerBase, public ISelfValidation
{
    Q_OBJECT

    typedef enum
    {
        NameProperty,
        XProperty,
        YProperty,
        WidthProperty,
        HeightProperty,
        BorderProperty,
        FGColorRedProperty,
        FGColorGreenProperty,
        FGColorBlueProperty,
        BGColorRedProperty,
        BGColorGreenProperty,
        BGColorBlueProperty
    } DisplayAreaProperties_e;

public:
    explicit DisplayArea(ProjectObject* parent = 0);
    ~DisplayArea();

    void AddUIWidget(UIWidgetBase* widgetToAdd);
    void AppendUIWidget(UIWidgetBase* widgetToAdd);
    void RemoveUIWidget(UIWidgetBase* widgetToRemove);
    const QList<UIWidgetBase*> UIWidgets(void) const;
    void MoveWidgetUp(UIWidgetBase* widget);
    void MoveWidgetDown(UIWidgetBase* widget);

    QString GetName() const;
    void SetName(const QString& name);

    qint32 GetX() const;
    void SetX(qint32 x);

    qint32 GetY() const;
    void SetY(qint32 y);

    quint16 GetWidth() const;
    void SetWidth(quint16 width);

    quint16 GetHeight() const;
    void SetHeight(quint16 height);

    quint8 GetBorder() const;
    void SetBorder(quint8 border);

    ColorPair* GetColorPair();
    void SetColorPair(ColorPair* colorPair);

    bool IsCheckMessageArea() const;
    void SetCheckMessageArea(bool isCheckMessageArea);

    bool NeedsLayer() const;

    UIWidgetBase* GetUIWidgetFromIndex(int index) const;

signals:
    void WidgetAdded(UIWidgetBase* addedWidget);

private:
    QString m_name;
    qint32 m_x;
    qint32 m_y;
    quint16 m_width;
    quint16 m_height;
    quint8 m_border;
    ColorPair* m_colorPair;
    bool m_isCheckMessageArea;

    QList<UIWidgetBase*> m_uiWidgets;

    // ProjectObject interface
protected:
    void AcceptVisitor(ProjectObjectVisitor& visitor);

    // ISelfValidation interface
public:
    bool objectSelfValidation(Project* project, QStringList *errorList);
};


#endif // DISPLAYAREA_H
