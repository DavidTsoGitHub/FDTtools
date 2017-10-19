#ifndef ICON_H
#define ICON_H

#include "ProjectObject.h"
#include <QString>

class ProjectObjectVisitor;

class Icon : public ProjectObject
{
public:
    explicit Icon();

    quint32 GetID(void) const;
    void SetID(const quint32 id);

    const QString GetName(void) const;
    void SetName(const QString& name);

    const QString GetPath(void) const;   
    void SetPath(const QString& path);

    const QString GetDescription(void) const;
    void SetDescription(const QString& path);

    const QString GetFileName() const;

private:
    quint32 m_id;
    QString m_name;
    QString m_filePath;
    QString m_description;

    // ProjectObject interface
protected:
    void AcceptVisitor(ProjectObjectVisitor& visitor);
};

#endif // ICON_H
