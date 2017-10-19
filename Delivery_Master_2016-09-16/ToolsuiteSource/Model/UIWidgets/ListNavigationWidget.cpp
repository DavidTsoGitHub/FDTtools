#include "ListNavigationWidget.h"

#include "ListNavigationWidgetGraphicsItem.h"
#include "LayoutDefinition.h"
#include "ColorPair.h"

const QString ListNavigationWidget::TypeName = "ListNavigation";

ListNavigationWidget::ListNavigationWidget(LayoutDefinition &layoutDefinition, ProjectObject* parent) :
    UIWidgetBase(UIWidget_Type_ListNavigation, 0, parent),
    m_displayItemCount(4),
    m_layoutDefinition(layoutDefinition)
{
    SetUserDefinedName(ListNavigationWidget::TypeName);
    SetUseSecondaryColorPair(true);
}

ListNavigationWidget::ListNavigationWidget(quint32 uiWidgetID, LayoutDefinition& layoutDefinition, ProjectObject* parent) :
    UIWidgetBase(UIWidget_Type_ListNavigation, uiWidgetID, parent),
    m_displayItemCount(4),
    m_layoutDefinition(layoutDefinition)
{
    SetUserDefinedName(ListNavigationWidget::TypeName);
    SetUseSecondaryColorPair(true);
}

quint32 ListNavigationWidget::GetDisplayItemCount() const
{
    return m_displayItemCount;
}

void ListNavigationWidget::SetDisplayItemcount(quint32 count)
{
    m_displayItemCount = count;
}

UIWidgetBase* ListNavigationWidget::CloneThis(quint32 uiWidgetID)
{
    return new ListNavigationWidget(uiWidgetID, m_layoutDefinition);
}

QString ListNavigationWidget::GetUIWidgetTypeName() const
{
    return ListNavigationWidget::TypeName;
}

QString ListNavigationWidget::GetSecondaryColorPairForegroundHeader()
{
    return "Selected Item Foreground Color";
}

QString ListNavigationWidget::GetSecondaryColorPairBackgroundHeader()
{
    return "Selected Item Background Color";
}

GraphicsItemBase* ListNavigationWidget::GetGraphicsItem(bool drawChromaKeyColor)
{
    QColor bgColor;

    if (GetPrimaryColorPair()->GetBGIsTransparent())
    {
        bgColor = m_layoutDefinition.GetStartChromaKeyColor();
    }
    else
    {
        bgColor = GetPrimaryColorPair()->GetBGColor();
        drawChromaKeyColor = true;
    }

    return new ListNavigationWidgetGraphicsItem(GetX(), GetY(), GetWidth(), GetHeight(), GetPrimaryColorPair()->GetFGColor(), bgColor, drawChromaKeyColor);
}

QVariant ListNavigationWidget::GetWidgetPropertyData(int column) const
{
    switch (column)
    {
        case DisplayItemCountColumn:
            return GetDisplayItemCount();
        default:
            return "";
    }
}

QString ListNavigationWidget::GetWidgetPropertyHeader(int column) const
{
    switch (column)
    {
        case DisplayItemCountColumn:
            return "Display Item Count";
        default:
            return "";
    }
}

void ListNavigationWidget::SetWidgetProperty(int column, QVariant value)
{
    switch (column)
    {
        case DisplayItemCountColumn:
            if (value.canConvert<quint32>())
            {
                SetDisplayItemcount(value.toUInt());
            }
            return;
        default:
            return;
    }
}

UIWidgetBase::DataType_t ListNavigationWidget::GetWidgetPropertyDataType(int column) const
{
    switch (column)
    {
        case DisplayItemCountColumn:
            return SpinBox_Uint8_Min1;
        default:
            return Not_Available;
    }
}

QString ListNavigationWidget::WidgetPropertyToolTip(int column) const
{
    switch (column)
    {
        case DisplayItemCountColumn:
            return "The number of items to display in the list";
        default:
            return "INVALID";
    }
}

int ListNavigationWidget::GetWidgetPropertyCount() const
{
    return ListNavigationWidgetColumnCount;
}

QString ListNavigationWidget::LongDescription()
{
    return "<p>This widget can be used to navigate the menu. The number of menu items that are displayed can be configured.</p>" \
           "<p><b>Notice:</b><br/>" \
           "- The widget can only be added to the menu layout (hence, a menu have to been added to the project before this widget can be used)</b><br/>" \
           "- Only one widget off this type can be added to a project</p>";
}
