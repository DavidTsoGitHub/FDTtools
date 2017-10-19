#ifndef LABELMAPPINGS_H
#define LABELMAPPINGS_H

#include <Label.h>
#include <QString>
#include "ProjectObject.h"

class LabelMappings : public ProjectObject
{
public:
    explicit LabelMappings(ProjectObject* parent = 0);
    ~LabelMappings();

    QString GetDescriptiveName() const;
    void SetDescriptiveName(const QString& descriptiveName);

    QString GetLabelMappingsName() const;
    void SetLabelMappingsName(const QString& labelMappingsName);

    QString GetDescription() const;
    void SetDescription(const QString& description);

    QList<Label*> GetMappedLabels(void);
    void SetMappedLabels(QList<Label*> mappedLabels);

    void AddMappedLabel(Label* label);
    void RemoveMappedLabel(Label* label);

private:
    QString m_descriptiveName;
    QString m_labelMappingsName;
    QString m_description;
    QList<Label*> m_mappedLabels;
};

#endif // LABELMAPPINGS_H
