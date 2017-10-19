#ifndef PROPERTIESWIDGETCONTAINER_H
#define PROPERTIESWIDGETCONTAINER_H

class Project;
class ProjectObject;

class QStackedWidget;

class PropertiesWidgetContainer : public QStackedWidget
{
    Q_OBJECT
public:
    explicit PropertiesWidgetContainer(QWidget *parent = 0);

signals:
    void LayoutNotChoosenAnymore();

public slots:
    void SetProject(Project* project);
    void ClearProject();

    void SetCurrentProjectObject(ProjectObject* selectedItem);
    void ClearCurrentProjectObject();

private:
    Project* m_project;
};

#endif // PROPERTIESWIDGETCONTAINER_H
