#include "LayoutDefinition.h"

#include "DisplayArea.h"
#include "MenuDefinition.h"
#include "Layout.h"
#include "ProjectObjectVisitor.h"
#include "ColorPair.h"

#include <QString>
#include <QList>

LayoutDefinition::LayoutDefinition(ProjectObject* parent) : ProjectObject(QString("Layouts"), parent),
    m_highestID(0),
    m_cropTop(0),
    m_cropBottom(0),
    m_cropLeft(0),
    m_cropRight(0),
    m_mainLayout(0),
    m_startupEnabled(false),
    m_startupScreenDisplayTime(0),
    m_startChromaKeyColor(QColor(0xFF, 0x00, 0xFF)),
    m_endChromaKeyColor(QColor(0xFF, 0x00, 0xFF))
{
    addGCLSignalSink(new GCLSignalSink("SystemPowerMode", "LayoutManager", "PowerMode"));
    addGCLSignalSink(new GCLSignalSink("CurrentLanguage", "LayoutManager", "uint8"));
    addInputToCommandMapping(InputUp);
    addInputToCommandMapping(InputDown);
    addInputToCommandMapping(InputLeft);
    addInputToCommandMapping(InputRight);
    addInputToCommandMapping(InputSet);
    addInputToCommandMapping(InputClear);

    m_tasks.append(new Task("DisplayManager_Init", "DisplayManager_Run", 5, 50));
    m_tasks.append(new Task("ContentsManager_Init", "ContentsManager_Run", 30, 50));
    m_tasks.append(new Task("LayoutManager_Init", "LayoutManager_Run", 15, 50));

    appendCLUTGetIndex(0x00000000); // Add black to the CLUT
    appendCLUTGetIndex(0x00FFFFFF); // Add white to the CLUT

    Project* project = static_cast<Project*>(parent);
    if (project != NULL)
    {
        connect(&project->GetAnimationDefinition(), &AnimationDefinition::signal_AnimationRemoved, this, &LayoutDefinition::animationRemoved);
    }
}

LayoutDefinition::~LayoutDefinition()
{
}

quint32 LayoutDefinition::GetHighestID() const
{
    return m_highestID;
}

void LayoutDefinition::SetHighestID(quint32 value)
{
    if(value > m_highestID)
    {
        m_highestID = value;
    }
}

const QList<Layout *> LayoutDefinition::Layouts() const
{
    return m_layouts;
}

void LayoutDefinition::moveLayoutToHighestPriority(Layout* layout)
{
    qint32 currentIndex = m_layouts.indexOf(layout);

    if (currentIndex < 1)
    {
        return;
    }

    qint32 newIndex = 0;
    m_layouts.move(currentIndex, newIndex);
    emit signal_LayoutPriorityUpdated();
}

void LayoutDefinition::moveLayoutToLowestPriority(Layout* layout)
{
    qint32 currentIndex = m_layouts.indexOf(layout);

    if (currentIndex >= m_layouts.count() - 1)
    {
        return;
    }

    qint32 newIndex = m_layouts.count() - 1;
    m_layouts.move(currentIndex, newIndex);
    emit signal_LayoutPriorityUpdated();
}

void LayoutDefinition::MoveLayoutUp(Layout* layout)
{
    qint32 currentIndex = m_layouts.indexOf(layout);

    if (currentIndex < 1)
    {
        return;
    }

    qint32 newIndex = currentIndex - 1;
    m_layouts.move(currentIndex, newIndex);
    updateButtonTransitionLayoutMappings(currentIndex, newIndex);
    emit signal_LayoutPriorityUpdated();
    NotifyPropertyUpdated();
}

void LayoutDefinition::MoveLayoutDown(Layout* layout)
{
    qint32 currentIndex = m_layouts.indexOf(layout);
    qint32 newIndex = currentIndex + 1;
    if (currentIndex < 0 || newIndex >= m_layouts.count())
    {
        return;
    }

    m_layouts.move(currentIndex, newIndex);
    updateButtonTransitionLayoutMappings(currentIndex, newIndex);
    emit signal_LayoutPriorityUpdated();
    NotifyPropertyUpdated();
}

void LayoutDefinition::updateButtonTransitionLayoutMappings(qint32 currentIndex, qint32 newIndex)
{

    blockSignals(true);

    foreach(Layout* layout, m_layouts)
    {
        QMap<Layout::LayoutButton, quint8> map = layout->GetButtonTransitions();
        QMap<Layout::LayoutButton, quint8>::iterator iterator;
        for (iterator = map.begin(); iterator != map.end(); ++iterator)
        {
            if (iterator.value() == currentIndex)
            {
                layout->AddButtonTransition(iterator.key(), newIndex, false);
            }
            else if (iterator.value() == newIndex)
            {
                layout->AddButtonTransition(iterator.key(), currentIndex, false);
            }
        }
    }

    blockSignals(false);
}

quint32 LayoutDefinition::getNextLayoutID()
{
    ++m_highestID;
    return m_highestID;
}

void LayoutDefinition::animationRemoved(quint32 animationID)
{
    bool propertyUpdated = false;
    foreach (Layout* layout, m_layouts)
    {
        QList<UIWidgetBase*> widgets;
        layout->GetUIWidgets(widgets);
        foreach (UIWidgetBase* widget, widgets)
        {
            if (widget->GetAnimationID(1) == animationID)
            {
                widget->SetAnimationID(1, 0);
                widget->SetAnimationUseTriggers(1, false);
                propertyUpdated = true;
            }

            if (widget->GetAnimationID(2) == animationID)
            {
                widget->SetAnimationID(2, 0);
                widget->SetAnimationUseTriggers(2, false);
                propertyUpdated = true;
            }
        }
    }

    if (propertyUpdated)
    {
        NotifyPropertyUpdated();
    }
}

void LayoutDefinition::addGCLSignalSink(GCLSignalSink* sink)
{
    m_signalSinks.append(sink);
    sink->SetParent(this);
}

quint16 LayoutDefinition::CropLeft() const
{
    return m_cropLeft;
}

void LayoutDefinition::SetCropLeft(quint16 value)
{
    m_cropLeft = value;
    NotifyPropertyUpdated();
}

quint16 LayoutDefinition::CropRight() const
{
    return m_cropRight;
}

void LayoutDefinition::SetCropRight(quint16 value)
{
    m_cropRight = value;
    NotifyPropertyUpdated();
}

quint16 LayoutDefinition::CropTop() const
{
    return m_cropTop;
}

void LayoutDefinition::SetCropTop(quint16 value)
{
    m_cropTop = value;
    NotifyPropertyUpdated();
}

quint16 LayoutDefinition::CropBottom() const
{
    return m_cropBottom;
}

void LayoutDefinition::SetCropBottom(quint16 value)
{
    m_cropBottom = value;
    NotifyPropertyUpdated();
}

Layout* LayoutDefinition::GetMainLayout() const
{
    return m_mainLayout;
}

void LayoutDefinition::SetMainLayout(Layout* layout)
{
    m_mainLayout = layout;
    NotifyPropertyUpdated();
}

bool LayoutDefinition::StartupEnabled() const
{
    return m_startupEnabled;
}

void LayoutDefinition::SetStartupEnabled(bool enabled)
{
    m_startupEnabled = enabled;
    NotifyPropertyUpdated();
}

quint8 LayoutDefinition::StartupScreenDisplayTime() const
{
    return m_startupScreenDisplayTime;
}

void LayoutDefinition::SetStartupScreenDisplayTime(quint8 value)
{
    m_startupScreenDisplayTime = value;
    if (m_startupScreenDisplayTime == 0)
    {
        SetStartupEnabled(false);
    }
    NotifyPropertyUpdated();
}

void LayoutDefinition::AddLayout(Layout *layout)
{
    if (layout->GetID() == UndefinedLayoutID)
    {
        layout->SetID(getNextLayoutID());
    }
    else
    {
        SetHighestID(layout->GetID());
    }

    if(!m_layouts.isEmpty() && m_layouts.last()->IsCheckMessageScreen())
    {
        m_layouts.insert(m_layouts.count() - 1, layout);
    }
    else
    {
        m_layouts.append(layout);
    }

    if (layout->IsCheckMessageScreen())
    {
        m_tasks.append(new Task("CheckMessageManager_Init", "CheckMessageManager_Run", 8, 75));
    }

    layout->SetParent(this);
    NotifyPropertyUpdated();
}

void LayoutDefinition::RemoveLayout(Layout *layout)
{
    quint8 layoutID = m_layouts.indexOf(layout);
    m_layouts.removeAll(layout);
    removeLayoutFromButtonTransitions(layoutID);
    layout->SetParent(NULL);
    foreach (GCLSignalSource* source, layout->GetSignalSources())
    {
        source->DisconnectAllSinks();
    }

    QList<GCLSignalSink*> sinks = QList<GCLSignalSink*>();
    layout->GetAllSignalSinks(sinks);
    foreach(GCLSignalSink* sink, sinks)
    {
        sink->DisconnectFromSource();
    }

    if (layout == m_mainLayout)
    {
        m_mainLayout = NULL;
    }

    if (layout->IsStartupScreen())
    {
        SetStartupEnabled(false);
    }

    if (layout->IsCheckMessageScreen())
    {
        foreach (const Task* task, m_tasks)
        {
            if (task->GetInitFunctionPrototype() == "CheckMessageManager_Init")
            {
                m_tasks.removeAll(task);
            }
        }
    }

    layout = NULL;
    delete layout;

    emit signal_LayoutPriorityUpdated();
    NotifyPropertyUpdated();
}

Layout* LayoutDefinition::GetStartupLayout() const
{
    foreach (Layout* layout, m_layouts)
    {
        if (layout->IsStartupScreen())
        {
            return layout;
        }
    }

    return NULL;
}

void LayoutDefinition::SetStartupLayout(Layout *startupLayout)
{
    moveLayoutToHighestPriority(startupLayout);
    startupLayout->SetStartupScreen(true);
    SetStartupEnabled(true);
}

bool LayoutDefinition::HasStartupLayout()
{
    foreach (Layout* layout, m_layouts)
    {
        if (layout->IsStartupScreen())
        {
            return true;
        }
    }

    return false;
}

void LayoutDefinition::SetCheckMessageLayout(Layout* checkmessageLayout)
{
    moveLayoutToLowestPriority(checkmessageLayout);
}

Layout *LayoutDefinition::GetCheckMessageLayout() const
{
    foreach (Layout* layout, m_layouts)
    {
        if (layout->IsCheckMessageScreen())
        {
            return layout;
        }
    }

    return NULL;
}

bool LayoutDefinition::HasCheckMessageLayout()
{
    return (GetCheckMessageLayout() != NULL);
}

QList<Layout *> LayoutDefinition::GetValidLayoutsForCheckMessages() const
{
    QList<Layout*> usableLayouts = QList<Layout*>();

    foreach(Layout* layout, m_layouts)
    {
        if(layout->IsStartupScreen() || layout->IsCheckMessageScreen())
        {
            continue;
        }
        else
        {
            usableLayouts.append(layout);
        }
    }
    return usableLayouts;
}

QList<quint32> LayoutDefinition::GetValidLayoutIDsForCheckMessages()
{
    QList<quint32> layoutIds = QList<quint32>();

    foreach(Layout* layout, m_layouts)
    {
        if(layout->IsStartupScreen() || layout->IsCheckMessageScreen())
        {
            continue;
        }
        else
        {
            layoutIds.append(layout->GetID());
        }
    }
    return layoutIds;
}

bool LayoutDefinition::HasMenu() const
{
    foreach (Layout* layout, m_layouts)
    {
        if (layout->HasMenu())
        {
            return true;
        }
    }

    return false;
}

Layout *LayoutDefinition::GetMenuLayout()
{
    foreach (Layout* layout, m_layouts)
    {
        if (layout->HasMenu())
        {
            return layout;
        }
    }
    return NULL;
}

DisplayArea *LayoutDefinition::GetMenuArea() const
{
    foreach (Layout* layout, m_layouts)
    {
        if (layout->HasMenu())
        {
            return layout->GetMenuArea();
        }
    }

    return NULL;
}

MenuDefinition *LayoutDefinition::GetMenu() const
{
    foreach (Layout* layout, m_layouts)
    {
        if (layout->HasMenu())
        {
            return layout->GetMenu();
        }
    }
    return NULL;
}

QList<quint32> LayoutDefinition::GetCLUT() const
{
    return m_clutList;
}

quint16 LayoutDefinition::appendCLUTGetIndex(quint32 value)
{
    quint16 index = 0;
    if (m_clutList.contains(value))
    {
        index = m_clutList.indexOf(value);
    }
    else
    {
        m_clutList.append(value);
        index = m_clutList.count() - 1;
    }

    return index;
}

void LayoutDefinition::removeLayoutFromButtonTransitions(quint8 layoutID)
{
    foreach (Layout* layout, m_layouts)
    {
        QMap<Layout::LayoutButton, quint8> map = layout->GetButtonTransitions();
        QMap<Layout::LayoutButton, quint8>::iterator iterator;

        for (iterator = map.begin(); iterator != map.end(); ++iterator)
        {
            if (iterator.value() == layoutID)
            {
                layout->RemoveButtonTransition(iterator.key());
            }
        }
    }
}

void LayoutDefinition::addInputToCommandMapping(ButtonInput input)
{
    QString signalName = getSignalNameFromButtonInput(input);
    GCLSignalSink* sink = new GCLSignalSink(signalName, "LayoutManager", "uint8", true, false);

    m_signalSinks.append(sink);
    sink->SetParent(this);
}

QString LayoutDefinition::getSignalNameFromButtonInput(ButtonInput input)
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

QColor LayoutDefinition::GetStartChromaKeyColor() const
{
    return m_startChromaKeyColor;
}

QColor LayoutDefinition::GetEndChromaKeyColor() const
{
    return m_endChromaKeyColor;
}

void LayoutDefinition::SetStartChromaKeyColorComponent(ColorComponent colorComponent, quint8 value)
{
    if (colorComponent == ColorComponent::RED)
    {
        m_startChromaKeyColor.setRed(value);
    }
    else if (colorComponent == ColorComponent::GREEN)
    {
        m_startChromaKeyColor.setGreen(value);
    }
    else if (colorComponent == ColorComponent::BLUE)
    {
        m_startChromaKeyColor.setBlue(value);
    }

    NotifyPropertyUpdated();
}

void LayoutDefinition::SetEndChromaKeyColorComponent(ColorComponent colorComponent, quint8 value)
{
    if (colorComponent == ColorComponent::RED)
    {
        m_endChromaKeyColor.setRed(value);
    }
    else if (colorComponent == ColorComponent::GREEN)
    {
        m_endChromaKeyColor.setGreen(value);
    }
    else if (colorComponent == ColorComponent::BLUE)
    {
        m_endChromaKeyColor.setBlue(value);
    }

    NotifyPropertyUpdated();
}

Layout* LayoutDefinition::GetLayoutFromID(quint32 id) const
{
    foreach (Layout* layout, m_layouts)
    {
        if(layout->GetID() == id)
        {
            return layout;
        }
    }
    return NULL;
}

Layout *LayoutDefinition::GetLayoutFromIndex(int index) const
{
    if((index >= 0) && (index < m_layouts.count()))
    {
        return m_layouts.at(index);
    }
    return NULL;
}

const QList<GCLSignalSink*>& LayoutDefinition::GetSignalSinks() const
{
    return m_signalSinks;
}

GCLSignalSink* LayoutDefinition::GetSignalSink(QString signalName) const
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

const QList<const Task*>& LayoutDefinition::Tasks() const
{
    return m_tasks;
}


void LayoutDefinition::AcceptVisitor(ProjectObjectVisitor& visitor)
{
    visitor.VisitLayoutDefinition(this);
}

bool LayoutDefinition::objectSelfValidation(Project* project, QStringList *errorList)
{
    Q_UNUSED(project);

    bool validation = true;

    if (m_mainLayout == NULL)
    {
        errorList->append("No main layout has been set");
        validation = false;
    }

    return validation;
}
