#include "MenuItem.h"

#include "ProjectObjectVisitor.h"
#include "RegEx/RegExPatterns.h"
#include "UIWidgetBase.h"

quint32 MenuItem::menuItemCounter = 1;

MenuItem::MenuItem(const QString& name, ProjectObject* parent) : UIWidgetContainerBase(name, parent),
    m_name(name),
    m_id(menuItemCounter++),
    m_imageID(0),
    m_subMenuWrapped(false)
{
}

MenuItem::~MenuItem()
{}

const QString MenuItem::GetName() const
{
    return m_name;
}

void MenuItem::SetName(const QString& name)
{
    m_name = QString(name).replace(RegExPatterns::NotAcceptedCodeCharacters(), "");
    setObjectName(m_name);
    NotifyPropertyUpdated();
}

quint32 MenuItem::GetID() const
{
    return m_id;
}

void MenuItem::SetID(quint32 id)
{
    m_id = id;

    if (menuItemCounter <= id)
    {
        menuItemCounter = ++id;
    }
    NotifyPropertyUpdated();
}

quint16 MenuItem::GetIndex() const
{
    return m_index;
}

void MenuItem::SetIndex(quint16 index)
{
    m_index = index;
    NotifyPropertyUpdated();
}

quint16 MenuItem::GetLabelID() const
{
    return m_labelID;
}

void MenuItem::SetLabelID(quint16 labelID)
{
    m_labelID = labelID;
    NotifyPropertyUpdated();
}

bool MenuItem::IsSubMenuWrapped() const
{
    return m_subMenuWrapped;
}

void MenuItem::SetSubMenuWrapped(bool wrapped)
{
    m_subMenuWrapped = wrapped;
    NotifyPropertyUpdated();
}

const QList<UIWidgetBase*>& MenuItem::UIWidgets() const
{
    return m_uiWidgets;
}

const QList<UIWidgetBase*> MenuItem::UIWidgetsOfType(UIWidgetBase::uiWidgetType_t type) const
{
    QList<UIWidgetBase*> uiWidgetsOfType;
    foreach (UIWidgetBase* uiWidget, m_uiWidgets)
    {
        if (uiWidget->GetUIWidgetType() == type)
        {
            uiWidgetsOfType.append(uiWidget);
        }
    }

    return uiWidgetsOfType;
}

void MenuItem::AddUIWidget(UIWidgetBase* uiWidget)
{
    m_uiWidgets.append(uiWidget);
    uiWidget->SetParent(this);
    NotifyPropertyUpdated();
}

void MenuItem::RemoveUIWidget(UIWidgetBase* uiWidget)
{
    m_uiWidgets.removeAll(uiWidget);
    uiWidget->SetParent(NULL);
    delete uiWidget;
    NotifyPropertyUpdated();
}

const QList<MenuItem*>& MenuItem::SubMenuItems() const
{
    return m_subMenuItems;
}

void MenuItem::AddSubMenuItem(MenuItem* subMenuItem)
{
    m_subMenuItems.append(subMenuItem);
    subMenuItem->SetParent(this);
    NotifyPropertyUpdated();
}

void MenuItem::RemoveSubMenuItem(MenuItem* subMenuItem, bool blockSignals)
{
    m_subMenuItems.removeAll(subMenuItem);
    delete subMenuItem;

    if (!blockSignals)
    {
        NotifyPropertyUpdated();
    }
}

void MenuItem::MoveSubMenuItemUp(MenuItem* subMenuItem)
{
    qint32 currentIndex = m_subMenuItems.indexOf(subMenuItem);
    if (currentIndex < 1)
    {
        return;
    }

    qint32 newIndex = currentIndex - 1;
    m_subMenuItems.move(currentIndex, newIndex);
    NotifyPropertyUpdated();
}

void MenuItem::MoveSubMenuItemDown(MenuItem* subMenuItem)
{
    qint32 currentIndex = m_subMenuItems.indexOf(subMenuItem);
    qint32 newIndex = currentIndex + 1;
    int size = m_subMenuItems.size();
    if (currentIndex < 0 || newIndex >= size)
    {
        return;
    }

    m_subMenuItems.move(currentIndex, newIndex);
    NotifyPropertyUpdated();
}

bool MenuItem::IsTopLevelMenuItem() const
{
    return (dynamic_cast<MenuItem*>(GetParent()) == NULL);
}

quint32 MenuItem::GetImageID() const
{
    return m_imageID;
}

void MenuItem::setImageID(const quint32 &imageID)
{
    m_imageID = imageID;
    NotifyPropertyUpdated();
}

void MenuItem::AcceptVisitor(ProjectObjectVisitor& visitor)
{
    visitor.VisitMenuItem(this);
}
