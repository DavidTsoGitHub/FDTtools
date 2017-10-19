#ifndef PROJECTOBJECT_H
#define PROJECTOBJECT_H

#include <QObject>

class ProjectObjectVisitor;
class ProjectObject;

template <typename T> class QList;
typedef QList<ProjectObject*> ProjectObjectList;
class QTimer;

class ProjectObject : public QObject
{
    Q_OBJECT
public:

    explicit ProjectObject(const QString& objectName, ProjectObject* parent = 0);
    virtual ~ProjectObject();

    /* Project Object Hierarchy */
    ProjectObject* GetParent() const;
    void SetParent(ProjectObject* parent);
    const ProjectObjectList& GetChildren() const;
    const ProjectObjectList GetEnabledChildren() const;


    /* Properties */
    bool GetEnabled() const;
    void SetEnabled(bool enabled);

    bool IsDirty() const;
    bool IsDirtyRecursive() const;
    void ClearDirtyRecursive();

    void Accept(ProjectObjectVisitor& visitor);

    void EnableSignals();
    void NotifyPropertyUpdated();
    void NotifyPropertyUpdatedRedrawNeeded();

signals:
    void PropertyUpdated();
    void PropertyUpdatedRedrawNeeded();

protected:
    virtual void AcceptVisitor(ProjectObjectVisitor& visitor);

private:
    ProjectObject* m_parent;
    ProjectObjectList m_children;
    bool m_enabled;
    bool m_dirty;

    QTimer* m_propertyUpdateTimer;
    QTimer* m_propertyUpdatedRedrawNeededTimer;

    void addChild(ProjectObject* child);
    void removeChild(ProjectObject* child);
    void removeDestroyedChild();
};

#endif // PROJECTOBJECT_H
