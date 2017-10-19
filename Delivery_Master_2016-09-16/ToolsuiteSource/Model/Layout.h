#ifndef LAYOUT_H
#define LAYOUT_H

#include "CheckMessage.h"
#include "DisplayArea.h"
#include "IGCLSignalSource.h"
#include "ProjectObject.h"

#include <QList>
#include <QAbstractItemModel>


class MenuDefinition;
class GCLSignalSink;
class TransitionClause;
class UIWidgetBase;

const quint32 UndefinedLayoutID = std::numeric_limits<quint32>::max();

class Layout: public ProjectObject, public IGCLSignalSource, public ISelfValidation
{
    typedef enum
    {
        NAME_ROW,
        COLUMN_COUNT
    } LayoutProperties;

public:
    typedef enum
    {
        LAYOUT_UP = 0,
        LAYOUT_DOWN = 1,
        LAYOUT_LEFT = 2,
        LAYOUT_RIGHT = 3,
        LAYOUT_SET = 4,
        LAYOUT_CLEAR = 5
    } LayoutButton;

    explicit Layout(QString name, ProjectObject* parent = 0);
    virtual ~Layout();

    QString Name() const;
    void SetName(const QString& name);

    quint32 GetID(void) const;
    void SetID(quint32 value);

    const QList<DisplayArea *> DisplayAreas() const;

    void AddDisplayArea(DisplayArea* area);
    void AppendDisplayArea(DisplayArea* area);
    void RemoveDisplayArea(DisplayArea* area);
    void MoveDisplayAreaUp(DisplayArea* area);
    void MoveDisplayAreaDown(DisplayArea* area);

    MenuDefinition* GetMenu(void) const;
    void SetMenu(MenuDefinition* menuDefinition, DisplayArea* area);
    void RemoveMenu(bool removeMenuRelatedItemsInLayout);
    bool HasMenu(void) const;
    DisplayArea* GetMenuArea(void) const;
    quint8 GetMenuAreaIndex(void) const;
    bool IsMenuArea(const DisplayArea* area) const;
    void SetMenuLayerIndex(quint8 layerIndex);
    quint8 GetMenuLayerIndex(void);

    bool IsStartupScreen(void) const;
    void SetStartupScreen(bool isStartupLayout);

    bool IsCheckMessageScreen(void) const;
    void SetCheckMessageScreen(bool isCheckMessageLayout);

    bool IsCheckMessagesEnabled(void) const;
    void AddCheckMessage(CheckMessage* checkMessage);
    void RemoveCheckMessage(CheckMessage* checkMessage);

    void SetBackgroundColorComponent(ColorComponent colorComponent, quint8 value);
    QColor GetBackgroundColor() const;

    void AddButtonTransition(LayoutButton button, quint8 layoutID, bool notifyPropertyUpdated = true);
    void RemoveButtonTransition(LayoutButton button);
    QMap<LayoutButton, quint8> GetButtonTransitions(void);
    qint8 GetButtonTransition(LayoutButton button);

    QList<TransitionClause*> GetSignalTransitions(void);
    void AddSignalTransition(TransitionClause* transition);
    void RemoveTransition(TransitionClause* transition);

    void GetUIWidgets(QList<UIWidgetBase*>& widgets) const;

    void GetAllSignalSources(QList<GCLSignalSource*>& sources) const;
    void GetAllSignalSinks(QList<GCLSignalSink*>& sinks) const;

    DisplayArea* GetAreaFromIndex(int index) const;

private:
    quint32 m_id;
    QString m_name;
    QList<DisplayArea*> m_displayAreas;
    QList<GCLSignalSink*> m_signalSinks;
    MenuDefinition* m_menuDefinition;
    DisplayArea* m_menuArea;
    quint8 m_menuLayerIndex;
    bool m_isStartupScreen;
    quint8 m_startupScreenDisplayTime;
    bool m_isCheckMessageLayout;
    QList<CheckMessage*> m_checkMessages;
    QColor m_backgroundColor;
    QString m_componentName;
    QList<TransitionClause*> m_signalTransitions;
    QMap<LayoutButton, quint8> m_buttonTransitions;
    QList<GCLSignalSource*> m_signalSources;
    QList<GCLSignalSource*> m_emptySignalSources;

    void addGCLSignalSource(GCLSignalSource* source);
    void addGCLSignalSink(GCLSignalSink *sink);
    void updateAllConnectedSinks();

    // ProjectObject interface
protected:
    void AcceptVisitor(ProjectObjectVisitor& visitor);

    // IGCLSignalSource interface
public:
    const QString GetComponentName() const;
    const QList<GCLSignalSource *> &GetSignalSources() const;
    GCLSignalSource *GetSignalSource(QString signalName) const;

    // ISelfValidation interface
public:
    bool objectSelfValidation(Project* project, QStringList *errorList);
};

#endif // LAYOUT_H
