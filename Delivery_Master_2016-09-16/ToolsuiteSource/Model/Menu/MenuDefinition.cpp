#include "MenuDefinition.h"

#include "DisplayArea.h"
#include "Logger.h"
#include "MenuItem.h"
#include "ProjectObjectVisitor.h"

#include <QList>
#include <QString>

MenuDefinition::MenuDefinition(DisplayArea* parent) : ProjectObject(QString("Menu"), parent),
    m_menuRoot(this)
{
    m_tasks.append(new Task("MenuController_Init", "MenuController_Run", 55, 50));
}

MenuDefinition::~MenuDefinition()
{
    while (!m_signalSinks.isEmpty())
    {
        delete m_signalSinks.takeFirst();
    }

    while (!m_signalSources.isEmpty())
    {
        delete m_signalSources.takeFirst();
    }
}

MenuDefinition* MenuDefinition::GetEmptyMenuDefinition()
{
    MenuDefinition* menuDefinition = new MenuDefinition();
    menuDefinition->addInputToCommandMapping(InputUp, CommandNone);
    menuDefinition->addInputToCommandMapping(InputDown, CommandNone);
    menuDefinition->addInputToCommandMapping(InputLeft, CommandNone);
    menuDefinition->addInputToCommandMapping(InputRight, CommandNone);
    menuDefinition->addInputToCommandMapping(InputSet, CommandNone);
    menuDefinition->addInputToCommandMapping(InputClear, CommandNone);

    menuDefinition->addGCLSignalSink(new GCLSignalSink("VehicleSpeed", "MenuController", "uint16"));

    //Display information
    menuDefinition->addGCLSignalSource(new GCLSignalSource("CurrentItemImage","MenuController","uint32"));
    menuDefinition->addGCLSignalSource(new GCLSignalSource("PreviousItemImage","MenuController","uint32"));
    menuDefinition->addGCLSignalSource(new GCLSignalSource("NextItemImage","MenuController","uint32"));
    menuDefinition->addGCLSignalSource(new GCLSignalSource("PrevPrevItemImage","MenuController","uint32"));
    menuDefinition->addGCLSignalSource(new GCLSignalSource("NextNextItemImage","MenuController","uint32"));

    menuDefinition->addGCLSignalSource(new GCLSignalSource("CurrentItemLabel","MenuController","uint32"));
    menuDefinition->addGCLSignalSource(new GCLSignalSource("PreviousItemLabel","MenuController","uint32"));
    menuDefinition->addGCLSignalSource(new GCLSignalSource("NextItemLabel","MenuController","uint32"));
    menuDefinition->addGCLSignalSource(new GCLSignalSource("PrevPrevItemLabel","MenuController","uint32"));
    menuDefinition->addGCLSignalSource(new GCLSignalSource("NextNextItemLabel","MenuController","uint32"));

    menuDefinition->addGCLSignalSource(new GCLSignalSource("NextItemEvent","MenuController","uint32"));
    menuDefinition->addGCLSignalSource(new GCLSignalSource("PreviousItemEvent","MenuController","uint32"));
    menuDefinition->addGCLSignalSource(new GCLSignalSource("EnterItemEvent","MenuController","uint32"));
    menuDefinition->addGCLSignalSource(new GCLSignalSource("LeaveItemEvent","MenuController","uint32"));

    return menuDefinition;
}

void MenuDefinition::addGCLSignalSink(GCLSignalSink* sink)
{
    m_signalSinks.append(sink);
    sink->SetParent(this);
}

void MenuDefinition::addGCLSignalSource(GCLSignalSource* source)
{
    m_signalSources.append(source);
    source->SetParent(this);
}

void MenuDefinition::addInputToCommandMapping(MenuInput input, MenuCommandID command)
{
    QString signalName = getSignalNameFromMenuInput(input);
    GCLSignalSink* sink = new GCLSignalSink(signalName, "MenuController", "uint8", true, false);
    InputToCommandMapping* newMapping = new InputToCommandMapping();
    newMapping->CommandID = command;
    newMapping->Input = input;
    newMapping->GCLSignal = sink;

    m_inputToCommandMappings.append(newMapping);
    addGCLSignalSink(sink);
}

QString MenuDefinition::GetSignalNameFromCommandID(MenuCommandID command) const
{
    foreach (InputToCommandMapping* mapping, m_inputToCommandMappings)
    {
        if (mapping->CommandID == command)
        {
            return mapping->GCLSignal->SignalName();
        }
    }

    return QString();
}

MenuCommandID MenuDefinition::GetCommandForInput(MenuInput input)
{
    foreach (InputToCommandMapping* mapping, m_inputToCommandMappings)
    {
        if (mapping->Input == input)
        {
            return mapping->CommandID;
        }
    }

    return CommandNone;
}

void MenuDefinition::ModifyInputToCommandMapping(MenuInput input, MenuCommandID newCommand)
{
    foreach (InputToCommandMapping* mapping, m_inputToCommandMappings)
    {
        if (mapping->Input == input)
        {
            mapping->CommandID = newCommand;
            NotifyPropertyUpdated();
            return;
        }
    }
}

const QList<InputToCommandMapping*>& MenuDefinition::InputToCommandMappings() const
{
    return m_inputToCommandMappings;
}

RootMenuItemContainer& MenuDefinition::MenuRoot()
{
    return m_menuRoot;
}

RootMenuItemContainer* MenuDefinition::MenuRootPointer()
{
    return &m_menuRoot;
}

const QList<GCLSignalSink*>& MenuDefinition::GetSignalSinks() const
{
    return m_signalSinks;
}

GCLSignalSink* MenuDefinition::GetSignalSink(QString signalName) const
{
    foreach (GCLSignalSink* sink, m_signalSinks)
    {
        if (sink->SignalName() == signalName)
        {
            return sink;
        }
    }
    return NULL;
}

const QList<GCLSignalSource*>& MenuDefinition::GetSignalSources() const
{
    return m_signalSources;
}

const QString MenuDefinition::GetComponentName() const
{
    return "MenuController";
}

GCLSignalSource *MenuDefinition::GetSignalSource(QString signalName) const
{
    foreach (GCLSignalSource* source, m_signalSources)
    {
        if (source->SignalName() == signalName)
        {
            return source;
        }
    }
    return NULL;
}

QString MenuDefinition::getSignalNameFromMenuInput(MenuInput input)
{
    switch (input)
    {
        case InputUp:
            return QString("Up");
        case InputDown:
            return QString("Down");
        case InputLeft:
            return QString("Left");
        case InputRight:
            return QString("Right");
        case InputSet:
            return QString("Set");
        case InputClear:
            return QString("Clear");
        default:
            return QString("Invalid");
    }
}


const QList<const Task*>& MenuDefinition::Tasks() const
{
    return m_tasks;
}


void MenuDefinition::AcceptVisitor(ProjectObjectVisitor& visitor)
{
    visitor.VisitMenuDefinition(this);
}
