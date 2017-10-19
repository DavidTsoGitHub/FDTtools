#ifndef MENUDEFINITION_H
#define MENUDEFINITION_H

#include "ProjectObject.h"
#include "IGCLSignalSink.h"
#include "IGCLSignalSource.h"
#include "InputToCommandMapping.h"
#include "IRunnable.h"
#include "RootMenuItemContainer.h"

class DisplayArea;
class MenuItem;

template <typename T> class QList;
class QString;

class MenuDefinition : public ProjectObject, public IGCLSignalSink, public IGCLSignalSource, public IRunnable
{
public:
    explicit MenuDefinition(DisplayArea* parent = 0);
    ~MenuDefinition(void);

    static MenuDefinition* GetEmptyMenuDefinition(void);
    void ModifyInputToCommandMapping(MenuInput input, MenuCommandID newCommand);
    QString GetSignalNameFromCommandID(MenuCommandID command) const;
    MenuCommandID GetCommandForInput(MenuInput input);

    const QList<InputToCommandMapping*>& InputToCommandMappings(void) const;
    RootMenuItemContainer& MenuRoot(void);
    RootMenuItemContainer* MenuRootPointer();
    void setParent(DisplayArea* parent);
    DisplayArea* parent(void);

private:
    void addGCLSignalSink(GCLSignalSink* sink);
    void addGCLSignalSource(GCLSignalSource* source);
    void addInputToCommandMapping(MenuInput input, MenuCommandID command);

    QList<InputToCommandMapping*> m_inputToCommandMappings;
    QList<GCLSignalSink*> m_signalSinks;
    QList<GCLSignalSource*> m_signalSources;
    QList<const Task*> m_tasks;
    RootMenuItemContainer m_menuRoot;

    QString getSignalNameFromMenuInput(MenuInput input);

    // IGCLSignalSink interface
public:
    const QList<GCLSignalSink*>& GetSignalSinks() const;
    GCLSignalSink* GetSignalSink(QString signalName) const;


    // IGCLSignalSource interface
public:
    const QList<GCLSignalSource*>& GetSignalSources() const;
    GCLSignalSource* GetSignalSource(QString signalName) const;
    const QString GetComponentName() const;

    // IRunnable interface
public:
    const QList<const Task*>& Tasks() const;

    // ProjectObject interface
protected:
    void AcceptVisitor(ProjectObjectVisitor& visitor);
};

#endif // MENUDEFINITION_H
