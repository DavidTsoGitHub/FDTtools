#include "Layout.h"

#include "MenuDefinition.h"
#include "GCLSignalSink.h"
#include "ProjectObjectVisitor.h"
#include "TransitionClause.h"
#include "TransitionCondition.h"
#include "ColorPair.h"

Layout::Layout(QString name, ProjectObject* parent) : ProjectObject("Layout", parent),
    m_id(UndefinedLayoutID),
    m_name(name),
    m_menuDefinition(NULL),
    m_menuArea(NULL),
    m_menuLayerIndex(0),
    m_isStartupScreen(false),
    m_isCheckMessageLayout(false),
    m_backgroundColor(Qt::black)
{
    m_componentName = "LayoutManager_" + name.replace(" ", "");
    setObjectName(m_name);
    addGCLSignalSource(new GCLSignalSource("UpButtonClicked", m_componentName, "void"));
    addGCLSignalSource(new GCLSignalSource("DownButtonClicked", m_componentName, "void"));
    addGCLSignalSource(new GCLSignalSource("SetButtonClicked", m_componentName, "void"));
    addGCLSignalSource(new GCLSignalSource("ClearButtonClicked", m_componentName, "void"));
}

Layout::~Layout()
{

}

QString Layout::Name() const
{
    return m_name;
}

void Layout::SetName(const QString& name)
{
    QString localName = name;
    m_name = name;
    m_componentName = "LayoutManager_" + localName.replace(" ", "");

    setObjectName(name);
    updateAllConnectedSinks();
    NotifyPropertyUpdated();
}

quint32 Layout::GetID() const
{
    return m_id;
}

void Layout::SetID(quint32 value)
{
    m_id = value;
}

const QList<DisplayArea *> Layout::DisplayAreas() const
{
    return m_displayAreas;
}

MenuDefinition* Layout::GetMenu() const
{
    return m_menuDefinition;
}

void Layout::AddDisplayArea(DisplayArea* area)
{
    m_displayAreas.insert(0, area);
    area->SetParent(this);
    NotifyPropertyUpdated();
}

void Layout::AppendDisplayArea(DisplayArea* area)
{
    m_displayAreas.append(area);
    area->SetParent(this);
    NotifyPropertyUpdated();
}

void Layout::RemoveDisplayArea(DisplayArea* area)
{
    m_displayAreas.removeAll(area);
    area->SetParent(NULL);

    if (area != m_menuArea)
    {
        delete area;
    }

    NotifyPropertyUpdated();
}

void Layout::MoveDisplayAreaUp(DisplayArea* area)
{
    qint32 currentIndex = m_displayAreas.indexOf(area);

    if (currentIndex < 1)
    {
        return;
    }

    qint32 newIndex = currentIndex - 1;
    m_displayAreas.move(currentIndex, newIndex);
    NotifyPropertyUpdated();
}

void Layout::MoveDisplayAreaDown(DisplayArea* area)
{
    qint32 currentIndex = m_displayAreas.indexOf(area);

    qint32 newIndex = currentIndex + 1;
    if (currentIndex < 0 || newIndex >= m_displayAreas.count())
    {
        return;
    }

    m_displayAreas.move(currentIndex, newIndex);
    NotifyPropertyUpdated();
}

void Layout::GetAllSignalSinks(QList<GCLSignalSink*>& sinks) const
{
    // All sinks belonging to the widgets in the layout (and menu if available)
    QList<UIWidgetBase*> displayWidgets;
    GetUIWidgets(displayWidgets);
    foreach (UIWidgetBase* widget, displayWidgets)
    {
        sinks.append(widget->GetSignalSinks());
    }

    if (m_menuDefinition != NULL)
    {
        // All sinks belonging to the menu manager
        sinks.append(m_menuDefinition->GetSignalSinks());
    }
}

DisplayArea *Layout::GetAreaFromIndex(int index) const
{
    if((index >= 0 ) && (index < m_displayAreas.count()))
    {
        return m_displayAreas.at(index);
    }

    return NULL;
}

void Layout::GetAllSignalSources(QList<GCLSignalSource*>& sources) const
{
    // All sources belonging to the widgets in the display (and menu if available)
    QList<UIWidgetBase*> displayWidgets;
    GetUIWidgets(displayWidgets);
    foreach (UIWidgetBase* widget, displayWidgets)
    {
        sources.append(widget->GetSignalSources());
    }

    if (m_menuDefinition != NULL)
    {
        // All sources belonging to the menu manager
        sources.append(m_menuDefinition->GetSignalSources());
    }
}

void Layout::GetUIWidgets(QList<UIWidgetBase*>& widgets) const
{
    foreach (DisplayArea* area, m_displayAreas)
    {
        widgets.append(area->UIWidgets());
    }

    if (HasMenu())
    {
        GetMenu()->MenuRoot().GetUIWidgets(widgets);
    }
}

void Layout::SetMenu(MenuDefinition* menuDefinition, DisplayArea* area)
{
    if (HasMenu())
    {
        return;
    }

    m_menuArea = area;
    m_menuDefinition = menuDefinition;
    m_menuDefinition->SetParent(m_menuArea);
    NotifyPropertyUpdated();
}

bool Layout::HasMenu() const
{
    return m_menuArea != NULL;
}

void Layout::RemoveMenu(bool removeMenuRelatedItemsInLayout)
{
    if (removeMenuRelatedItemsInLayout)
    {
        QList<UIWidgetBase*> uiWidgets;
        GetUIWidgets(uiWidgets);
        foreach (UIWidgetBase* widget, uiWidgets)
        {
            if (widget->GetUIWidgetType() == UIWidgetBase::UIWidget_Type_ListNavigation)
            {
                DisplayArea* area = (DisplayArea*)widget->GetParent();
                area->RemoveUIWidget(widget);
            }
        }
    }

    m_menuArea = NULL;
    m_menuDefinition = NULL;
    NotifyPropertyUpdated();
}

DisplayArea* Layout::GetMenuArea(void) const
{
    return m_menuArea;
}

quint8 Layout::GetMenuAreaIndex() const
{
    if (m_menuArea == NULL) return 255;

    for (int i = 0; i < m_displayAreas.count(); ++i)
    {
        if (m_menuArea == m_displayAreas.at(i))
        {
            return m_displayAreas.count() - 1 - i;
        }
    }

    return 255;
}

bool Layout::IsMenuArea(const DisplayArea* area) const
{
    return m_menuArea == area;
}

void Layout::SetMenuLayerIndex(quint8 layerIndex)
{
    m_menuLayerIndex = layerIndex;
}

quint8 Layout::GetMenuLayerIndex(void)
{
    return m_menuLayerIndex;
}

bool Layout::IsStartupScreen() const
{
    return m_isStartupScreen;
}

void Layout::SetStartupScreen(bool isStartupScreen)
{
    m_isStartupScreen = isStartupScreen;

    if (isStartupScreen)
    {
        foreach (GCLSignalSource* source, m_signalSources)
        {
            source->DisconnectAllSinks();
        }
    }

    NotifyPropertyUpdated();
}

bool Layout::IsCheckMessageScreen() const
{
    return m_isCheckMessageLayout;
}

void Layout::SetCheckMessageScreen(bool isCheckMessageLayout)
{
    m_isCheckMessageLayout = isCheckMessageLayout;
    NotifyPropertyUpdated();
}

bool Layout::IsCheckMessagesEnabled() const
{
    return m_checkMessages.count() > 0;
}

void Layout::AddCheckMessage(CheckMessage *checkMessage)
{
    if (!m_checkMessages.contains(checkMessage))
    {
        m_checkMessages.append(checkMessage);
        NotifyPropertyUpdated();
        emit PropertyUpdatedRedrawNeeded();
    }
}

void Layout::RemoveCheckMessage(CheckMessage *checkMessage)
{
    m_checkMessages.removeAll(checkMessage);
    NotifyPropertyUpdated();
    emit PropertyUpdatedRedrawNeeded();
}

void Layout::SetBackgroundColorComponent(ColorComponent colorComponent, quint8 value)
{
    if (colorComponent == ColorComponent::RED)
    {
        m_backgroundColor.setRed(value);
    }
    else if (colorComponent == ColorComponent::GREEN)
    {
        m_backgroundColor.setGreen(value);
    }
    else if (colorComponent == ColorComponent::BLUE)
    {
        m_backgroundColor.setBlue(value);
    }

    NotifyPropertyUpdated();
}

QColor Layout::GetBackgroundColor() const
{
    return m_backgroundColor;
}

//This function will have the same effect as reassignment (QMap property)
void Layout::AddButtonTransition(Layout::LayoutButton button, quint8 layoutID, bool notifyPropertyUpdated)
{
    m_buttonTransitions.insert(button, layoutID);

    if (notifyPropertyUpdated)
        NotifyPropertyUpdated();
}

void Layout::RemoveButtonTransition(Layout::LayoutButton button)
{
    m_buttonTransitions.remove(button);
    NotifyPropertyUpdated();
}

QMap<Layout::LayoutButton, quint8> Layout::GetButtonTransitions()
{
    return m_buttonTransitions;
}

qint8 Layout::GetButtonTransition(Layout::LayoutButton button)
{
    return m_buttonTransitions.value(button, -1);
}

void Layout::AddSignalTransition(TransitionClause *transition)
{
    m_signalTransitions.append(transition);
    NotifyPropertyUpdated();
    emit PropertyUpdatedRedrawNeeded();
}

void Layout::RemoveTransition(TransitionClause *transition)
{
    m_signalTransitions.removeAll(transition);
    NotifyPropertyUpdated();
    emit PropertyUpdatedRedrawNeeded();
}

QList<TransitionClause *> Layout::GetSignalTransitions()
{
    return m_signalTransitions;
}

void Layout::AcceptVisitor(ProjectObjectVisitor& visitor)
{
    visitor.VisitLayout(this);
}

void Layout::addGCLSignalSource(GCLSignalSource* source)
{
    m_signalSources.append(source);
    source->SetParent(this);
}

void Layout::addGCLSignalSink(GCLSignalSink* sink)
{
    m_signalSinks.append(sink);
    sink->SetParent(this);
}

void Layout::updateAllConnectedSinks()
{
    QList<GCLSignalSource*> newSignalSources;

    foreach (GCLSignalSource* source, m_signalSources)
    {
        GCLSignalSource* newSource = new GCLSignalSource(source->SignalName(), m_componentName, "void");
        newSignalSources.append(newSource);

        foreach (GCLSignalSink* sink, source->GetConnectedSinks())
        {
            sink->ConnectTo(newSource);
        }
        m_signalSources.removeAll(source);
    }

    foreach (TransitionClause* transitionClause, m_signalTransitions)
    {
        GCLSignalSink* firstSink = transitionClause->GetFirstCondition()->GetSignalSinks().first();
        QString componentShortName = m_name;
        QString newComponentName = "Transition_" + componentShortName.replace(" ", "");

        if (firstSink != NULL)
        {
            GCLSignalSink* newSink = new GCLSignalSink(firstSink->SignalName(), newComponentName, "DynamicWidgetValue");
            newSink->ConnectTo(firstSink->GetSignalSource());
            firstSink->DisconnectFromSource();
            transitionClause->GetFirstCondition()->SetSignalSink(newSink);
        }

        if (transitionClause->GetSecondCondition() != NULL)
        {
            GCLSignalSink* secondSink = transitionClause->GetSecondCondition()->GetSignalSinks().first();
            GCLSignalSink* newSink = new GCLSignalSink(secondSink->SignalName(), newComponentName, "DynamicWidgetValue");
            newSink->ConnectTo(secondSink->GetSignalSource());
            secondSink->DisconnectFromSource();
            transitionClause->GetSecondCondition()->SetSignalSink(newSink);
        }

        transitionClause->SetComponentName(newComponentName);
    }

    foreach (GCLSignalSource* source, newSignalSources)
    {
        addGCLSignalSource(source);
    }
}

const QString Layout::GetComponentName() const
{
    return m_componentName;
}

const QList<GCLSignalSource *> &Layout::GetSignalSources() const
{
    if (IsStartupScreen())
    {
        return m_emptySignalSources;
    }

    return m_signalSources;
}

GCLSignalSource *Layout::GetSignalSource(QString signalName) const
{
    if (IsStartupScreen()) return NULL;

    foreach (GCLSignalSource* source, m_signalSources)
    {
        if (source->SignalName() == signalName)
        {
            return source;
        }
    }

    return NULL;
}

bool Layout::objectSelfValidation(Project* project, QStringList *errorList)
{
    Q_UNUSED(project);

    if(DisplayAreas().empty()){
        errorList->append("Layout " + m_name + " has no child areas.");
        return false;
    }

    return true;
}
