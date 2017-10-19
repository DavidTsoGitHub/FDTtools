#include "LabelMappings.h"

LabelMappings::LabelMappings(ProjectObject* parent) : ProjectObject("LabelMappings", parent)
{

}

LabelMappings::~LabelMappings()
{

}

QString LabelMappings::GetDescriptiveName() const
{
    return m_descriptiveName;
}

void LabelMappings::SetDescriptiveName(const QString &descriptiveName)
{
    m_descriptiveName = descriptiveName;
    NotifyPropertyUpdated();
}

QString LabelMappings::GetLabelMappingsName() const
{
    return m_labelMappingsName;
}

void LabelMappings::SetLabelMappingsName(const QString &labelMappingsName)
{
    m_labelMappingsName = labelMappingsName;
    NotifyPropertyUpdated();
}

QString LabelMappings::GetDescription() const
{
    return m_description;
}

void LabelMappings::SetDescription(const QString &description)
{
    m_description = description;
    NotifyPropertyUpdated();
}

QList<Label*> LabelMappings::GetMappedLabels()
{
    return m_mappedLabels;
}

void LabelMappings::SetMappedLabels(QList<Label*> mappedLabels)
{
    m_mappedLabels = mappedLabels;
    NotifyPropertyUpdated();
}

void LabelMappings::AddMappedLabel(Label* label)
{
    m_mappedLabels.append(label);
    NotifyPropertyUpdated();
}

void LabelMappings::RemoveMappedLabel(Label* label)
{
    m_mappedLabels.removeOne(label);
    NotifyPropertyUpdated();
}


