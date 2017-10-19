#ifndef ICONWIDGET_H
#define ICONWIDGET_H

#include "UIWidgetBase.h"
#include "IconWidgetGraphicsItem.h"
#include "IconDefinition.h"
#include "Icon.h"

class IconWidget : public UIWidgetBase
{
public:
    static const QString TypeName;

    explicit IconWidget(IconDefinition& iconDefinition, ProjectObject* parent = 0);
    IconWidget(IconDefinition& iconDefinition, quint32 uiWidgetID, ProjectObject* parent = 0);

    //const Icon* GetIconByID(quint32 id) const;
    void SetIconID(quint32 id);
    quint32 GetIconID() const;
    const QString GetFullPath(void) const;

private:
    typedef enum
    {
        IconIDColumn,
        IconWidgetColumnCount
    } iconWidgetColumnIndex_t;

    quint32 m_iconID;
    IconWidgetGraphicsItem* m_iconWidgetGraphicsItem;
    IconDefinition& m_iconDefinition;

    // UIWidgetBase interface
public:
    GraphicsItemBase* GetGraphicsItem(bool drawChromaKeyColor);
    QString GetUIWidgetTypeName() const;

protected:
    UIWidgetBase* CloneThis(quint32 widgetID);
    QVariant GetWidgetPropertyData(int column) const;
    QString GetWidgetPropertyHeader(int column) const;
    void SetWidgetProperty(int column, QVariant value);
    int GetWidgetPropertyCount() const;
    DataType_t GetWidgetPropertyDataType(int column) const;
    QString WidgetPropertyToolTip(int column) const;
    QString LongDescription();

    // ISelfValidation interface
public:
    bool objectSelfValidation(Project* project, QStringList *errorList);

};

#endif // ICONWIDGET_H
