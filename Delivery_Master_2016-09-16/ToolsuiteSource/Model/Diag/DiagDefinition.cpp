#include "DiagDefinition.h"
#include "ProjectObjectVisitor.h"

DiagDefinition::DiagDefinition(ProjectObject* parent) : ProjectObject(QString("Diagnosis"), parent)
{}

DiagDefinition::~DiagDefinition()
{
    while (!m_DTCLabelMappings.isEmpty())
    {
        delete m_DTCLabelMappings.takeFirst();
    }

    while (!m_DIDToCALMappings.isEmpty())
    {
        delete m_DIDToCALMappings.takeFirst();
    }
}

const QList<DTCLabelMapping*>& DiagDefinition::GetDTCLabelMappings() const
{
    return m_DTCLabelMappings;
}

void DiagDefinition::AddDTCLabelMapping(quint32 dtc, quint32 labelId)
{
    DTCLabelMapping* mapping = new DTCLabelMapping(dtc, labelId);
    m_DTCLabelMappings.append(mapping);
    mapping->SetParent(this);
    NotifyPropertyUpdated();
}

void DiagDefinition::RemoveDTCLabelMappingByIndex(quint32 index)
{
    delete m_DTCLabelMappings.takeAt(index);
    NotifyPropertyUpdated();
}

const QList<DIDToCALMapping*>& DiagDefinition::GetDIDToCALMappings() const
{
    return m_DIDToCALMappings;
}

void DiagDefinition::AddDIDToCALMapping(quint16 did, const QString& calParameter)
{
    DIDToCALMapping* mapping = new DIDToCALMapping(did, calParameter);
    m_DIDToCALMappings.append(mapping);
    mapping->SetParent(this);
    NotifyPropertyUpdated();
}

void DiagDefinition::RemoveDIDToCALMappingByIndex(quint32 index)
{
    delete m_DIDToCALMappings.takeAt(index);
    NotifyPropertyUpdated();
}

void DiagDefinition::ClearMappingForCALParameter(const QString& calParameter)
{
    foreach (DIDToCALMapping* mapping, m_DIDToCALMappings)
    {
        if (mapping->GetCALParameter().compare(calParameter) == 0)
        {
            mapping->SetCALParameter("");
        }
    }
    NotifyPropertyUpdated();
}

void DiagDefinition::AcceptVisitor(ProjectObjectVisitor& visitor)
{
    visitor.VisitDiagDefinition(this);
}


DTCLabelMapping::DTCLabelMapping(quint32 dtc, quint32 labelId) : ProjectObject(QString::number(dtc)),
    m_dtc(dtc),
    m_labelID(labelId)
{
}

quint32 DTCLabelMapping::GetDTC() const
{
    return m_dtc;
}

void DTCLabelMapping::SetDTC(const quint32& dtc)
{
    m_dtc = dtc;
    NotifyPropertyUpdated();
}

quint32 DTCLabelMapping::GetLabelID() const
{
    return m_labelID;
}

void DTCLabelMapping::SetLabelID(const quint32& labelID)
{
    m_labelID = labelID;
    NotifyPropertyUpdated();
}

DIDToCALMapping::DIDToCALMapping(quint16 did, const QString &calParameter) : ProjectObject(QString::number(did)),
    m_did(did),
    m_calParameter(calParameter)
{
}

quint16 DIDToCALMapping::GetDID() const
{
    return m_did;
}

void DIDToCALMapping::SetDID(const quint16& did)
{
    m_did = did;
    NotifyPropertyUpdated();
}

QString DIDToCALMapping::GetCALParameter() const
{
    return m_calParameter;
}

void DIDToCALMapping::SetCALParameter(const QString& calParameter)
{
    m_calParameter = calParameter;
    NotifyPropertyUpdated();
}
