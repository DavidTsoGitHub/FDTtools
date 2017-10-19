#ifndef DYNAMICICONWIDGET_H
#define DYNAMICICONWIDGET_H

#include "UIWidgetBase.h"

class DynamicIconWidget : public UIWidgetBase
{
public:
    static const QString TypeName;

    explicit DynamicIconWidget(ProjectObject* parent = 0);
    explicit DynamicIconWidget(quint32 widgetID, ProjectObject* parent = 0);
    ~DynamicIconWidget();

private:
    typedef enum
    {
        IconIDSinkColumn,
        dynamicIconWidgetColumnCount
    } dynamicIconWidgetColumns_t;

    // UIWidgetBase interface
public:
    QString GetUIWidgetTypeName() const;
    GraphicsItemBase* GetGraphicsItem(bool drawChromaKeyColor);
    QString LongDescription();

protected:
    UIWidgetBase* CloneThis(quint32 widgetId);
    QVariant GetWidgetPropertyData(int column) const;
    QString GetWidgetPropertyHeader(int column) const;
    void SetWidgetProperty(int column, QVariant value);
    int GetWidgetPropertyCount() const;
    DataType_t GetWidgetPropertyDataType(int column) const;
    QString WidgetPropertyToolTip(int column) const;  

};

#endif // DYNAMICICONWIDGET_H
