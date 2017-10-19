#ifndef ROOTMENUITEMCONTAINER_H
#define ROOTMENUITEMCONTAINER_H

#include "ProjectObject.h"

class MenuItem;
class UIWidgetBase;

class RootMenuItemContainer : public ProjectObject
{
public:
    explicit RootMenuItemContainer(ProjectObject* parent = 0);

    bool IsWrapped(void) const;
    void SetWrapped(bool wrapped);

    const MenuItem* GetStartupMenuItem() const;
    void SetStartupMenuItem(const MenuItem* startScreenMenuItem);

    void GetUIWidgets(QList<UIWidgetBase*>& widgets) const;

    bool GetTopLevelNavigationEnabled() const;
    void SetTopLevelNavigationEnabled(bool topLevelNavigationEnabled);

    void AddMenuItem(MenuItem* menuItem);
    void RemoveMenuItem(MenuItem* menuItem);
    void MoveMenuItemUp(MenuItem* menuItem);
    void MoveMenuItemDown(MenuItem* menuItem);

    const QList<MenuItem*>& MenuItems() const;

private:
    void getUIWidgetsFromMenuItems(QList<UIWidgetBase*>& widgets, const QList<MenuItem*>& menuItems) const;

    QList<MenuItem*> m_menuItems;
    bool m_wrapped;
    const MenuItem* m_startupMenuItem;
    bool m_topLevelNavigationEnabled;

    // ProjectObject interface
protected:
    void AcceptVisitor(ProjectObjectVisitor& visitor);
};

#endif // ROOTMENUITEMCONTAINER_H
