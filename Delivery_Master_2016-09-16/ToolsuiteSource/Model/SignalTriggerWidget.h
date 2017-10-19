#ifndef SIGNALTRIGGERWIDGET_H
#define SIGNALTRIGGERWIDGET_H

#include "UIWidgetBase.h"

class SignalTriggerWidget : public UIWidgetBase
{
public:
    static const QString TypeName;

    explicit SignalTriggerWidget(ProjectObject* parent = 0);
    explicit SignalTriggerWidget(quint32 widgetID, ProjectObject* parent = 0);
    ~SignalTriggerWidget();

private:
    typedef enum
    {
        SignalSourceColumn,
        triggerSignalWidgetColumnCount
    } triggerSignalWidgetColumns_t;

    // UIWidgetBase interface
public:
    QString GetUIWidgetTypeName() const;
    GraphicsItemBase* GetGraphicsItem(bool drawChromaKeyColor);

protected:
    UIWidgetBase *CloneThis(quint32 widgetId);
    QVariant GetWidgetPropertyData(int column) const;
    QString GetWidgetPropertyHeader(int column) const;
    void SetWidgetProperty(int column, QVariant value);
    int GetWidgetPropertyCount() const;
    DataType_t GetWidgetPropertyDataType(int column) const;
    QString WidgetPropertyToolTip(int column) const;
    QString LongDescription();

};

#endif // SIGNALTRIGGERWIDGET_H
