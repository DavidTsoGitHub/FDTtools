#include "DisplayArea.h"

#include "ProjectObjectVisitor.h"
#include "CheckMessagesDefinition.h"
#include "ColorPair.h"

DisplayArea::DisplayArea(ProjectObject* parent) : UIWidgetContainerBase(QString("Area"), parent),
    m_x(0),
    m_y(0),
    m_width(240),
    m_height(320),
    m_border(0),
    m_colorPair(new ColorPair(Qt::white, Qt::black, true, this)),
    m_isCheckMessageArea(false)
{
}

DisplayArea::~DisplayArea()
{
}

void DisplayArea::AddUIWidget(UIWidgetBase* widgetToAdd)
{
    m_uiWidgets.insert(0, widgetToAdd);
    widgetToAdd->SetParent(this);
    emit WidgetAdded(widgetToAdd);
}

void DisplayArea::AppendUIWidget(UIWidgetBase* widgetToAdd)
{
    m_uiWidgets.append(widgetToAdd);
    widgetToAdd->SetParent(this);
    emit WidgetAdded(widgetToAdd);
}

void DisplayArea::RemoveUIWidget(UIWidgetBase* widgetToRemove)
{
    m_uiWidgets.removeAll(widgetToRemove);
    widgetToRemove->SetParent(NULL);
    NotifyPropertyUpdated();
}

const QList<UIWidgetBase*> DisplayArea::UIWidgets() const
{
    return m_uiWidgets;
}

void DisplayArea::MoveWidgetUp(UIWidgetBase* widget)
{
    qint32 currentIndex = m_uiWidgets.indexOf(widget);

    if (currentIndex < 1)
    {
        return;
    }

    qint32 newIndex = currentIndex - 1;
    m_uiWidgets.move(currentIndex, newIndex);
    NotifyPropertyUpdated();
}

void DisplayArea::MoveWidgetDown(UIWidgetBase* widget)
{
    qint32 currentIndex = m_uiWidgets.indexOf(widget);

    qint32 newIndex = currentIndex + 1;
    if (currentIndex < 0 || newIndex >= m_uiWidgets.count())
    {
        return;
    }

    m_uiWidgets.move(currentIndex, newIndex);
    NotifyPropertyUpdated();
}

QString DisplayArea::GetName() const
{
    return m_name;
}

void DisplayArea::SetName(const QString& name)
{
    setObjectName(name);
    m_name = name;
    NotifyPropertyUpdated();
}

qint32 DisplayArea::GetX() const
{
    return m_x;
}

void DisplayArea::SetX(qint32 x)
{
    m_x = x;
    NotifyPropertyUpdated();
}

qint32 DisplayArea::GetY() const
{
    return m_y;
}

void DisplayArea::SetY(qint32 y)
{
    m_y = y;
    NotifyPropertyUpdated();
}

quint16 DisplayArea::GetWidth() const
{
    return m_width;
}

void DisplayArea::SetWidth(quint16 width)
{
    m_width = width;
    NotifyPropertyUpdated();
}

quint16 DisplayArea::GetHeight() const
{
    return m_height;
}

void DisplayArea::SetHeight(quint16 height)
{
    m_height = height;
    NotifyPropertyUpdated();
}

quint8 DisplayArea::GetBorder() const
{
    return m_border;
}

void DisplayArea::SetBorder(quint8 border)
{
    m_border = border;
    NotifyPropertyUpdated();
}

ColorPair* DisplayArea::GetColorPair()
{
    return m_colorPair;
}

void DisplayArea::SetColorPair(ColorPair* colorPair)
{
    m_colorPair = colorPair;
}

bool DisplayArea::IsCheckMessageArea() const
{
    return m_isCheckMessageArea;
}

void DisplayArea::SetCheckMessageArea(bool isCheckMessageArea)
{
    m_isCheckMessageArea = isCheckMessageArea;
}

bool DisplayArea::NeedsLayer() const
{
    foreach (UIWidgetBase* widget, m_uiWidgets)
    {
        if (widget->GetNbrOfLayersNeeded() < 1)
        {
            return true;
        }
    }

    return false;
}

UIWidgetBase *DisplayArea::GetUIWidgetFromIndex(int index) const
{
    if((index >= 0) && (index < m_uiWidgets.count()))
    {
         return m_uiWidgets.at(index);
    }

    return NULL;
}


void DisplayArea::AcceptVisitor(ProjectObjectVisitor& visitor)
{
    visitor.VisitDisplayArea(this);
}

bool DisplayArea::objectSelfValidation(Project* project, QStringList *errorList)
{
    Q_UNUSED(project);

    bool validation = true;

    if (GetWidth() < 4 || GetHeight() < 4)
    {
        //this causes platform errors if "boxed" is set
        //and is worthless if too small to render anything
        errorList->append("Area size error in '" + this->GetName() + "': height or width below 4");
        validation &= false;
    }

    return validation;
}
