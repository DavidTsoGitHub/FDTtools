#ifndef PROJECTPROPERTIESWIDGET_H
#define PROJECTPROPERTIESWIDGET_H

#include "ProjectObjectPropertiesWidget.h"

class Project;

class QLineEdit;
class QString;
class QWidget;

class ProjectPropertiesWidget : public ProjectObjectPropertiesWidget
{
    Q_OBJECT

public:
    explicit ProjectPropertiesWidget(Project* project, QWidget* parent = 0);
    ~ProjectPropertiesWidget();

    void SetProject(Project* project);

private:
    QLineEdit* m_projectNameEditor;

private slots:
    void slot_projectNameChanged(const QString& newName);

    // ProjectObjectPropertiesWidget interface
protected:
    void beforePopulateSignals();
};

#endif // PROJECTPROPERTIESWIDGET_H
