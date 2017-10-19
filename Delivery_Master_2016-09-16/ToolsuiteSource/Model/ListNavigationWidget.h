#ifndef LISTNAVIGATIONWIDGET_H
#define LISTNAVIGATIONWIDGET_H

#include "UIWidgetBase.h"

class ListNavigationWidgetGraphicsItem;
class LayoutDefinition;

class ListNavigationWidget : public UIWidgetBase
{

public:
    static const QString TypeName;

    typedef enum
    {
        DisplayItemCountColumn,
        ListNavigationWidgetColumnCount
    } listNavigationWidgetColumnIndex_t;

    explicit ListNavigationWidget(LayoutDefinition& layoutDefinition, ProjectObject* parent = 0);
    explicit ListNavigationWidget(quint32 uiWidgetID, LayoutDefinition& layoutDefinition, ProjectObject* parent = 0);


    quint32 GetDisplayItemCount() const;
    void SetDisplayItemcount(quint32 count);

private:
    quint32 m_displayItemCount;
    ListNavigationWidgetGraphicsItem* m_listNavigationWidgetGraphicsItem;
    LayoutDefinition& m_layoutDefinition;

    // UIWidgetBase interface
public:
    QString GetUIWidgetTypeName() const;
    QString GetSecondaryColorPairForegroundHeader();
    QString GetSecondaryColorPairBackgroundHeader();

protected:
    UIWidgetBase* CloneThis(quint32 uiWidgetID);
    GraphicsItemBase* GetGraphicsItem(bool drawChromaKeyColor);
    QVariant GetWidgetPropertyData(int column) const;
    QString GetWidgetPropertyHeader(int column) const;
    void SetWidgetProperty(int column, QVariant value);
    DataType_t GetWidgetPropertyDataType(int column) const;
    QString WidgetPropertyToolTip(int column) const;
    int GetWidgetPropertyCount() const;
    QString LongDescription();
};

#endif // LISTNAVIGATIONWIDGET_H
