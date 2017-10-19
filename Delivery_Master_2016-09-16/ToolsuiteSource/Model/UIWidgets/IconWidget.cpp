#include "IconWidget.h"
#include "ColorPair.h"
#include "Project.h"

#include <Exceptions/ObjectNotFoundException.h>
#include <QFileInfo>
#include <limits>

const QString IconWidget::TypeName = "Icon";

IconWidget::IconWidget(IconDefinition& iconDefinition, ProjectObject* parent) :
    UIWidgetBase(UIWidget_Type_Icon, 0, parent),
    m_iconID(std::numeric_limits<quint32>::max()),
    m_iconDefinition(iconDefinition)
{
    SetUserDefinedName(IconWidget::TypeName);
}

IconWidget::IconWidget(IconDefinition& iconDefinition, quint32 uiWidgetID, ProjectObject* parent) :
    UIWidgetBase(UIWidget_Type_Icon, uiWidgetID, parent),
    m_iconID(std::numeric_limits<quint32>::max()),
    m_iconDefinition(iconDefinition)
{
    SetUserDefinedName(IconWidget::TypeName);
}

void IconWidget::SetIconID(quint32 id)
{
    m_iconID = id;
}

quint32 IconWidget::GetIconID() const
{
    return m_iconID;
}

UIWidgetBase* IconWidget::CloneThis(quint32 widgetID)
{
    return new IconWidget(m_iconDefinition, widgetID);
}

GraphicsItemBase* IconWidget::GetGraphicsItem(bool drawChromaKeyColor)
{
    Project* project = static_cast<Project*>(m_iconDefinition.GetParent());
    return new IconWidgetGraphicsItem(project, GetX(), GetY(), GetWidth(), GetHeight(), GetPrimaryColorPair(), m_iconDefinition.GetResource(m_iconID), drawChromaKeyColor);
}

QString IconWidget::GetUIWidgetTypeName() const
{
    return IconWidget::TypeName;
}

QVariant IconWidget::GetWidgetPropertyData(int column) const
{
    switch (column)
    {
        case IconIDColumn:
            return m_iconID;
        default:
            return "";
    }
}

QString IconWidget::GetWidgetPropertyHeader(int column) const
{
    switch (column)
    {
        case IconIDColumn:
            return "Icon";
        default:
            return "INVALID";
    }
}

void IconWidget::SetWidgetProperty(int column, QVariant value)
{
    switch (column)
    {
        case IconIDColumn:
            if (value.canConvert<quint32>())
            {
                SetIconID(value.toUInt());
            }
            return;
        default:
            return;
    }
}

UIWidgetBase::DataType_t IconWidget::GetWidgetPropertyDataType(int column) const
{
    switch (column)
    {
        case IconIDColumn:
            return IconsComboBox;
        default:
            return Not_Available;
    }
}

int IconWidget::GetWidgetPropertyCount() const
{
    return IconWidgetColumnCount;
}

QString IconWidget::WidgetPropertyToolTip(int column) const
{
    switch (column)
    {
        case IconIDColumn:
            return "The icon to display in this widget";
        default:
            return "INVALID";
    }
}

QString IconWidget::LongDescription()
{
    return "This widget can be used to display a static icon. " \
           "Icons must first be added in the Icons Manager before they can be used with a widget.";
}

bool IconWidget::objectSelfValidation(Project* project, QStringList *errorList)
{
    bool validation = true;
    validation &= UIWidgetBase::objectSelfValidation(project, errorList);

    if (GetIconID() == 0)
    {
        errorList->append("Icon widget '" + GetUserDefinedName() + "' ID: " + QString::number(GetUIWidgetID()) + " does not reference any icon.");
        validation = false;
    }

    return validation;
}
