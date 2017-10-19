#ifndef DIAGDEFINITION
#define DIAGDEFINITION

#include "ProjectObject.h"

class DTCLabelMapping;
class DIDToCALMapping;

template <typename T> class QList;

class DiagDefinition : public ProjectObject
{
public:

    explicit DiagDefinition(ProjectObject* parent = 0);
    ~DiagDefinition();

    const QList<DTCLabelMapping*>& GetDTCLabelMappings() const;
    void AddDTCLabelMapping(quint32 dtc, quint32 labelId);
    void RemoveDTCLabelMappingByIndex(quint32 index);

    const QList<DIDToCALMapping*>& GetDIDToCALMappings() const;
    void AddDIDToCALMapping(quint16 did, const QString& calParameter);
    void RemoveDIDToCALMappingByIndex(quint32 index);
    void ClearMappingForCALParameter(const QString& calParameter);

private:
    QList<DTCLabelMapping*> m_DTCLabelMappings;
    QList<DIDToCALMapping*> m_DIDToCALMappings;

    // ProjectObject interface
protected:
    void AcceptVisitor(ProjectObjectVisitor& visitor);
};

class DTCLabelMapping : public ProjectObject
{
public:
    DTCLabelMapping(quint32 dtc, quint32 labelId);

    quint32 GetDTC() const;
    void SetDTC(const quint32& dtc);

    quint32 GetLabelID() const;
    void SetLabelID(const quint32& labelID);

private:
    quint32 m_dtc;
    quint32 m_labelID;
};

class DIDToCALMapping : public ProjectObject
{
public:
    DIDToCALMapping(quint16 did, const QString& calParameter);


    quint16 GetDID() const;
    void SetDID(const quint16& did);

    QString GetCALParameter() const;
    void SetCALParameter(const QString& calParameter);

private:
    quint16 m_did;
    QString m_calParameter;

};


#endif // DIAGDEFINITION

