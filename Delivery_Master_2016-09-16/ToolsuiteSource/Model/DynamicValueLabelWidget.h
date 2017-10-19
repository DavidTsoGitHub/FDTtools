#ifndef DYNAMICVALUELABELWIDGET_H
#define DYNAMICVALUELABELWIDGET_H

#include "UIWidgetBase.h"
#include "ILabelHasFont.h"
#include "Font.h"
#include "LabelDefinition.h"
#include "LayoutDefinition.h"

#include <QWidget>

class DynamicValueLabelWidget : public UIWidgetBase, public ILabelHasFont
{
    Q_OBJECT

public:
    static const QString TypeName;

    explicit DynamicValueLabelWidget(LabelDefinition& labelDefinition, LayoutDefinition& layoutDefinition, ProjectObject* parent = 0);
    explicit DynamicValueLabelWidget(LabelDefinition& labelDefinition, quint32 id, LayoutDefinition& layoutDefinition, ProjectObject* parent = 0);
    ~DynamicValueLabelWidget();

    void SetScaling(qint8 scaling);
    qint8 GetScaling(void) const;

    void SetNbrOfDecimals(quint8 nbrOfDecimals);
    quint8 GetNbrOfDecimals(void) const;

    void SetNbrOfDigits(quint8 nbrOfDigits);
    quint8 GetNbrOfDigits(void) const;

public:
    //Belongs to the ILabelFont interface
    void SetFontID(quint32 id);
    quint32 GetFontID() const;

private:
    typedef enum
    {
        FontIDColumn,
        ScalingColumn,
        DecimalsColumn,
        DigitsColumn,
        SinkColumn,
        DynamicValueLabelWidgetColumnCount
    } dynamicValueLabelWidgetColumns_t;

    LabelDefinition& m_labelDefinition;
    LayoutDefinition& m_layoutDefinition;
    quint32 m_fontId;
    qint8 m_scaling;
    quint8 m_nbrOfDecimals;
    quint8 m_nbrOfDigits;

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

#endif // DYNAMICVALUELABELWIDGET_H
