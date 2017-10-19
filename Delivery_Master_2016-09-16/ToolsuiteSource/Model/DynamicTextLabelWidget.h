#ifndef DYNAMICTEXTLABELWIDGET_H
#define DYNAMICTEXTLABELWIDGET_H

#include "UIWidgetBase.h"
#include "ILabelHasFont.h"
#include "Font.h"
#include "LabelDefinition.h"
#include "LayoutDefinition.h"

#include <QWidget>

class DynamicTextLabelWidget : public UIWidgetBase, public ILabelHasFont
{
    Q_OBJECT

public:
    static const QString TypeName;

    explicit DynamicTextLabelWidget(LabelDefinition& labelDefinition, LayoutDefinition& layoutDefinition, ProjectObject *parent = 0);
    explicit DynamicTextLabelWidget(LabelDefinition& labelDefinition, quint32 id, LayoutDefinition& layoutDefinition, ProjectObject *parent = 0);
    ~DynamicTextLabelWidget();

    void SetRightAligned(bool rightAligned);
    bool GetRightAligned() const;

public:
    //Belongs to the ILabelFont interface
    void SetFontID(quint32 id);
    quint32 GetFontID() const;

private:
    typedef enum
    {
        RightAlignedColumn,
        FontIDColumn,
        SinkColumn,
        DynamicTextLabelWidgetColumnCount
    } dynamicTextLabelWidgetColumns_t;

    LabelDefinition& m_labelDefinition;
    LayoutDefinition& m_layoutDefinition;
    bool m_rightAligned;
    quint32 m_fontId;

    // UIWidgetBase interface
public:
    GraphicsItemBase* GetGraphicsItem(bool drawChromaKeyColor);
    QString ShortDescription();
    QString LongDescription();
    QString GetUIWidgetTypeName() const;

protected:
    UIWidgetBase* CloneThis(quint32 widgetId);
    QVariant GetWidgetPropertyData(int column) const;
    QString GetWidgetPropertyHeader(int column) const;
    void SetWidgetProperty(int column, QVariant value);
    int GetWidgetPropertyCount() const;
    UIWidgetBase::DataType_t GetWidgetPropertyDataType(int column) const;
    QString WidgetPropertyToolTip(int column) const;

};

#endif // DYNAMICTEXTLABELWIDGET_H
