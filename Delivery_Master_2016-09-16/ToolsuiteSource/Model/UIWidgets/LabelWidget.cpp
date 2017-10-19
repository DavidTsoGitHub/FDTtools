#include "LabelWidget.h"
#include "Label.h"
#include "ColorPair.h"

const QString LabelWidget::TypeName = "Label";

LabelWidget::LabelWidget(LabelDefinition& labelDefinition, LayoutDefinition &layoutDefinition, ProjectObject* parent) :
    UIWidgetBase(UIWidget_Type_Label, 0, parent),
    m_labelID(UNSET_LABEL),
    m_labelDefinition(labelDefinition),
    m_layoutDefinition(layoutDefinition)
{
    SetUserDefinedName(LabelWidget::TypeName);
}

LabelWidget::LabelWidget(LabelDefinition& labelDefinition, quint32 uiWidgetID, LayoutDefinition &layoutDefinition, ProjectObject* parent) :
    UIWidgetBase(UIWidget_Type_Label, uiWidgetID, parent),
    m_labelID(UNSET_LABEL),
    m_labelDefinition(labelDefinition),
    m_layoutDefinition(layoutDefinition)
{
    SetUserDefinedName(LabelWidget::TypeName);
}

quint32 LabelWidget::GetLabelID() const
{
    return m_labelID;
}

void LabelWidget::SetLabelID(quint32 id)
{
    m_labelID = id;
}

UIWidgetBase* LabelWidget::CloneThis(quint32 uiWidgetID)
{
    return new LabelWidget(m_labelDefinition, uiWidgetID, m_layoutDefinition);
}

QString LabelWidget::GetUIWidgetTypeName() const
{
    return LabelWidget::TypeName;
}

QString LabelWidget::LongDescription()
{
    return "This widget displays a static Label. Labels are added in the Label Editor. " \
           "The label can be written on multiple lines and an new line is represented " \
           "by '\\n' in the label text.";
}

GraphicsItemBase* LabelWidget::GetGraphicsItem(bool drawChromaKeyColor)
{
    QColor bgColor;

    if (GetPrimaryColorPair()->GetBGIsTransparent())
    {
        bgColor = m_layoutDefinition.GetStartChromaKeyColor();
    }
    else
    {
        bgColor = GetPrimaryColorPair()->GetBGColor();
        drawChromaKeyColor = true;
    }

    return new LabelWidgetGraphicsItem(GetX(), GetY(), GetWidth(), GetHeight(), GetPrimaryColorPair()->GetFGColor(), bgColor, m_labelDefinition.GetResource(GetLabelID()), drawChromaKeyColor);
}

QVariant LabelWidget::GetWidgetPropertyData(int column) const
{
    switch (column)
    {
        case LabelIDColumn:
            return GetLabelID();
        default:
            return "";
    }
}

QString LabelWidget::GetWidgetPropertyHeader(int column) const
{
    switch (column)
    {
        case LabelIDColumn:
            return "Label";
        default:
            return "";
    }
}

void LabelWidget::SetWidgetProperty(int column, QVariant value)
{
    switch (column)
    {
        case LabelIDColumn:
            if (value.canConvert<quint32>())
            {
                SetLabelID(value.toUInt());
            }
            return;
        default:
            return;
    }
}

UIWidgetBase::DataType_t LabelWidget::GetWidgetPropertyDataType(int column) const
{
    switch (column)
    {
        case LabelIDColumn:
            return LabelsComboBox;
        default:
            return Not_Available;
    }
}

QString LabelWidget::WidgetPropertyToolTip(int column) const
{
    switch (column)
    {
        case LabelIDColumn:
            return "The label to be displayed";
        default:
            return "INVALID";
    }
}

int LabelWidget::GetWidgetPropertyCount() const
{
    return LabelWidgetColumnCount;
}

bool LabelWidget::objectSelfValidation(Project* project, QStringList *errorList)
{
    bool validation = true;
    validation &= UIWidgetBase::objectSelfValidation(project, errorList);

    if(m_labelID == UNSET_LABEL)
    {
        //Font wasn't set as a widget property
        errorList->append("Widget '" + GetUIWidgetUniqueName() + "' has no associated label");
        validation = false;
    }

    if (m_labelDefinition.GetLabel(m_labelID) == &Label::UndefinedLabel)
    {
        //Font may have been set, but later removed from the LabelEditor
        errorList->append("Widget '" + GetUIWidgetUniqueName() + "' was mapped to labelID " + QString::number(m_labelID) + ", but this labelID does no longer match any entry in the LabelEditor.");
        validation = false;
    }

    return validation;
}
