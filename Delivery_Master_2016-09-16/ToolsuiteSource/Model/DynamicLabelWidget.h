#ifndef DYNAMICLABELWIDGET_H
#define DYNAMICLABELWIDGET_H
#include "UIWidgetBase.h"
#include "GraphicsItemBase.h"
#include "LayoutDefinition.h"

#include <QWidget>

class DynamicLabelWidget : public UIWidgetBase
{
    Q_OBJECT
public:
    static const QString TypeName;

    explicit DynamicLabelWidget(LayoutDefinition& layoutDefinition, ProjectObject* parent= 0);
    explicit DynamicLabelWidget(quint32 widgetID, LayoutDefinition& layoutDefinition, ProjectObject* parent = 0);
    ~DynamicLabelWidget();

    GraphicsItemBase* GetGraphicsItem(bool drawChromaKeyColor);
    TextAlignment_t GetTextAlignment() const;
    void SetTextAlignment(const TextAlignment_t& textAlignment);
    QString GetTextAlignmentEnumString(void);

private:

    typedef enum
    {
        LabelIDSinkColumn,
        TextAlignmentColumn,
        dynamicLabelWidgetColumnCount
    } dynamicLabelWidgetColumns_t;

    LayoutDefinition& m_layoutDefinition;
    TextAlignment_t m_textAlignment;

    // UIWidgetBase interface
public:
    QString GetUIWidgetTypeName() const;
    QVariant GetWidgetPropertyData(int index) const;
    QString GetWidgetPropertyHeader(int index) const;
    int GetWidgetPropertyCount() const;
    void SetWidgetProperty(int index, QVariant value);
    DataType_t GetWidgetPropertyDataType(int index) const;
    QString WidgetPropertyToolTip(int index) const;
    QString LongDescription();

protected:
    UIWidgetBase *CloneThis(quint32 widgetId);

    // ISelfValidation interface
public:
    bool objectSelfValidation(Project* project, QStringList *errorList);

};

#endif // DYNAMICLABELWIDGET_H
