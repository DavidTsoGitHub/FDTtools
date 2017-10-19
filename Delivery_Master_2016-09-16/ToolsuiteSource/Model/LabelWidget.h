#ifndef LABELWIDGET_H
#define LABELWIDGET_H

#define UNSET_LABEL 0

#include "UIWidgetBase.h"
#include "LabelWidgetGraphicsItem.h"
#include "LabelDefinition.h"
#include "LayoutDefinition.h"

class Project;

class LabelWidget : public UIWidgetBase
{

public:
    static const QString TypeName;

    typedef enum
    {
        LabelIDColumn,
        LabelWidgetColumnCount
    } labelWidgetColumnIndex_t;

    explicit LabelWidget(LabelDefinition& labelDefinition, LayoutDefinition& layoutDefinition, ProjectObject* parent = 0);
    LabelWidget(LabelDefinition& labelDefinition, quint32 uiWidgetID, LayoutDefinition& layoutDefinition, ProjectObject* parent = 0);

    const QString GetResourceName() const;
    quint32 GetLabelID() const;
    void SetLabelID(quint32 id);

private:
    quint32 m_labelID;
    QString m_description;
    LabelWidgetGraphicsItem* m_labelWidgetGraphicsItem;
    LabelDefinition& m_labelDefinition;
    LayoutDefinition& m_layoutDefinition;

    // UIWidgetBase interface
public:
    QString GetUIWidgetTypeName() const;
    QString LongDescription();

protected:
    UIWidgetBase* CloneThis(quint32 uiWidgetID);
    GraphicsItemBase* GetGraphicsItem(bool drawChromaKeyColor);
    QVariant GetWidgetPropertyData(int column) const;
    QString GetWidgetPropertyHeader(int column) const;
    void SetWidgetProperty(int column, QVariant value);
    DataType_t GetWidgetPropertyDataType(int column) const;
    QString WidgetPropertyToolTip(int column) const;
    int GetWidgetPropertyCount() const;


    // ISelfValidation interface
public:
    bool objectSelfValidation(Project* project, QStringList *errorList);
};


#endif // LABELWIDGET_H

