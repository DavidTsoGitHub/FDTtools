#include "RootMenuItemContainer.h"

#include "MenuItem.h"
#include "ProjectObjectVisitor.h"
#include "UIWidgetBase.h"

RootMenuItemContainer::RootMenuItemContainer(ProjectObject* parent) : ProjectObject(QString("Menu Root"), parent),
    m_wrapped(false),
    m_startupMenuItem(NULL),
    m_topLevelNavigationEnabled(true)
{
}

bool RootMenuItemContainer::IsWrapped() const
{
    return m_wrapped;
}

void RootMenuItemContainer::SetWrapped(bool wrapped)
{
    m_wrapped = wrapped;
    NotifyPropertyUpdated();
}

const MenuItem* RootMenuItemContainer::GetStartupMenuItem() const
{
    return m_startupMenuItem;
}

void RootMenuItemContainer::SetStartupMenuItem(const MenuItem* startScreenMenuItem)
{
    m_startupMenuItem = startScreenMenuItem;
    NotifyPropertyUpdated();
}

void RootMenuItemContainer::GetUIWidgets(QList<UIWidgetBase*>& widgets) const
{
    getUIWidgetsFromMenuItems(widgets, m_menuItems);
}

void RootMenuItemContainer::getUIWidgetsFromMenuItems(QList<UIWidgetBase*>& widgets, const QList<MenuItem*>& menuItems) const
{
    foreach (MenuItem* menuItem, menuItems)
    {
        widgets.append(menuItem->UIWidgets());

        getUIWidgetsFromMenuItems(widgets, menuItem->SubMenuItems());
    }
}

const QList<MenuItem*>& RootMenuItemContainer::MenuItems() const
{
    return m_menuItems;
}

void RootMenuItemContainer::AddMenuItem(MenuItem* menuItem)
{
    if (menuItem == NULL) return;

    m_menuItems.append(menuItem);
    menuItem->SetParent(this);
    NotifyPropertyUpdated();
}

void RootMenuItemContainer::RemoveMenuItem(MenuItem* menuItem)
{
    if (menuItem == NULL) return;

    m_menuItems.removeAll(menuItem);
    delete menuItem;

    NotifyPropertyUpdated();
}

void RootMenuItemContainer::MoveMenuItemUp(MenuItem* menuItem)
{
    qint32 currentIndex = m_menuItems.indexOf(menuItem);
    if (currentIndex < 1)
    {
        return;
    }

    qint32 newIndex = currentIndex - 1;
    m_menuItems.move(currentIndex, newIndex);
    NotifyPropertyUpdated();
}

void RootMenuItemContainer::MoveMenuItemDown(MenuItem* menuItem)
{
    qint32 currentIndex = m_menuItems.indexOf(menuItem);
    qint32 newIndex = currentIndex + 1;
    if (currentIndex < 0 || newIndex >= m_menuItems.size())
    {
        return;
    }

    m_menuItems.move(currentIndex, newIndex);
    NotifyPropertyUpdated();
}

void RootMenuItemContainer::AcceptVisitor(ProjectObjectVisitor& visitor)
{
    visitor.VisitRootMenuItemContainer(this);
}
