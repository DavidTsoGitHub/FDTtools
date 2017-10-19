#ifndef PROJECTOBJECTPROPERTIESWIDGET_H
#define PROJECTOBJECTPROPERTIESWIDGET_H

#include "Project.h"

class ProjectObject;
class QWidget;
class QString;
class QTreeWidgetItem;

namespace Ui
{
class ProjectObjectPropertiesWidget;
}

class ProjectObjectPropertiesWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ProjectObjectPropertiesWidget(Project* project, QWidget *parent = 0);
    ~ProjectObjectPropertiesWidget();
    virtual void SetProjectObject(ProjectObject* projectObject);
    QTreeWidgetItem* AddContainer(QString text, QTreeWidgetItem* parent = 0);
    QTreeWidgetItem* AddEditableData(QString header, QWidget* editorWidget, QTreeWidgetItem* parent = 0);
    void AddTreeWidgetItem(QTreeWidgetItem *headerItem, QWidget *editorItem, int column = 0);

protected:
    QTreeWidgetItem* AddDisplayData(QString header, QString data, QTreeWidgetItem* parent = 0);

    virtual void beforePopulateUIHook();
    virtual void beforePopulateSignals();
    virtual void beforePopulateSignalSinks();
    virtual void afterPopulateSignalSinks();
    virtual void beforePopulateSignalSources();
    virtual void afterPopulateSignalSources();
    virtual void afterPopulateSignals();
    virtual void afterPopulateUIHook();

    Project* getProject() const;
    void populate();

private slots:
    void slot_collapseAllButton_pressed();
    void slot_expandAllButton_pressed();
    void slot_showContextMenu(QPoint pos);

private:
    Ui::ProjectObjectPropertiesWidget *m_ui;
    Project* m_project;
    ProjectObject* m_projectObject;

    QTreeWidgetItem* m_inputSignalsRoot;
    QTreeWidgetItem* m_outputSignalsRoot;

    bool m_usesFrameByFrameAnimation;

    QFont m_headerFont;
    QFont m_normalFont;

    void clearPropertiesTree();
    void populateSignalSinks();
    void populateSignalSources();
    void addInputSignal(QTreeWidgetItem* parent, GCLSignalSink* sink);
    void addOutputSignal(QTreeWidgetItem* parent, const GCLSignalSource* source);
    void setExpandedRecursive(QTreeWidgetItem* item, bool expanded);
};

#endif // PROJECTOBJECTPROPERTIESWIDGET_H
