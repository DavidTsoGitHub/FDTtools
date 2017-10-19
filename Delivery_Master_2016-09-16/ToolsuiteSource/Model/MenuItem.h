#ifndef MENUITEM
#define MENUITEM

#include "UIWidgetContainerBase.h"

class UIWidgetBase;

class MenuItem : public UIWidgetContainerBase
{
public:
    explicit MenuItem(const QString& name, ProjectObject* parent = 0);
    ~MenuItem(void);

    const QString GetName(void) const;
    void SetName(const QString& name);

    quint32 GetID(void) const;
    void SetID(quint32 id);

    quint16 GetIndex(void) const;
    void SetIndex(quint16 index);

    quint16 GetLabelID(void) const;
    void SetLabelID(quint16 labelID);

    bool IsSubMenuWrapped(void) const;
    void SetSubMenuWrapped(bool wrapped);

    const QList<UIWidgetBase*>& UIWidgets(void) const;
    const QList<UIWidgetBase*> UIWidgetsOfType(UIWidgetBase::uiWidgetType_t type) const;

    void AddUIWidget(UIWidgetBase* uiWidget);
    void RemoveUIWidget(UIWidgetBase* uiWidget);

    const QList<MenuItem*>& SubMenuItems(void) const;
    void AddSubMenuItem(MenuItem* subMenuItem);
    void RemoveSubMenuItem(MenuItem* subMenuItem, bool blockSignals = false);
    void MoveSubMenuItemUp(MenuItem* subMenuItem);
    void MoveSubMenuItemDown(MenuItem* subMenuItem);

    bool IsTopLevelMenuItem() const;

    quint32 GetImageID() const;
    void setImageID(const quint32 &imageID);

private:
    QString m_name;
    quint32 m_id;
    quint16 m_index;
    quint16 m_labelID;
    quint32 m_imageID;

    QList<MenuItem*> m_subMenuItems;
    bool m_subMenuWrapped;
    QList<UIWidgetBase*> m_uiWidgets;

    static quint32 menuItemCounter;

    // ProjectObject interface
protected:
    void AcceptVisitor(ProjectObjectVisitor& visitor);
};

#endif // MENUITEM
