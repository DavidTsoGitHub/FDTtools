#ifndef ICONDEFINITION_H
#define ICONDEFINITION_H

#include "UIWidgetDefinitionBase.h"

class Icon;
class ProjectObjectVisitor;

template <typename T> class QList;
class QString;

class IconDefinition : public UIWidgetDefinitionBase
{
public:
    explicit IconDefinition(ProjectObject* parent = 0);
    ~IconDefinition();

    quint32 GetNextAvailableIconId() const;

    const QList<Icon*>& Icons(void) const;
    Icon* GetIconByID(quint32 id);
    void AddIcon(Icon* icon);
    void RemoveIconAtIndex(int index);

    const QString GetIconFolderPath() const;
    void SetIconFolderPath(QString iconFolderPath);

private:
    QList<Icon*> m_icons;

    Icon* m_undefinedIcon;
    QString m_iconFolderPath;

    // UIWidgetDefinitionBase interface
public:
    QString GetResource(quint32 id);
    QString GetResourceName(quint32 id);

    // ProjectObject interface
protected:
    void AcceptVisitor(ProjectObjectVisitor& visitor);
};

#endif // ICONDEFINITION_H
