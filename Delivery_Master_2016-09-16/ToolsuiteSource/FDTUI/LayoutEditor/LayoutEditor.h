#ifndef LAYOUTEDITOR_H
#define LAYOUTEDITOR_H

class GCLSignalSink;
class Layout;
class LayoutsTreeModel;
class LayoutPreviewWidget;
class Project;
class ProjectObject;
class QModelIndex;
class QWidget;
template <typename T> class QList;

namespace Ui
{
class LayoutEditor;
}

class LayoutEditor : public QWidget
{
    Q_OBJECT

public:
    explicit LayoutEditor(QWidget* parent = 0);
    ~LayoutEditor();

public slots:
    void SetProject(Project* project);
    void ClearProject();
    void slot_addNewCheckMessageLayout();
    void JustSelected();
    void LayoutChoosenInProjectBrowser(void);
    void LayoutNotChoosenInProjectBrowser(void);

signals:
    void signal_LayoutEditorItemSelected(ProjectObject* selectedItem);
    void signal_updateButtonStates();
    void signal_modified();
    void signal_WidgetToBeRemoved(GCLSignalSink* sink, quint32 widgetID);
    void signal_updateWidgetOrder(QList<quint32> widgetIDs);

    void signal_layoutAdded(quint32 layoutID);
    void signal_layoutRemoved(quint32 layoutID);

private slots:
    void slot_moveWidgetButton_clicked();
    void slot_layoutView_Clicked(const QModelIndex &clicked);
    void slot_layoutView_CurrentChanged(const QModelIndex& current);
    void slot_areasView_Clicked(const QModelIndex &clicked);
    void slot_areasView_CurrentChanged(const QModelIndex& current);
    void slot_widgetsView_Clicked(const QModelIndex &clicked);
    void slot_widgetsView_CurrentChanged(const QModelIndex& current);

private:
    void setupLayoutModel();
    void setupListSignals();
    void setupButtonSignals();
    void setLayoutButtonsDisabled();
    void setAreaButtonsDisabled();
    void setWidgetButtonsDisabled();
    void populate();
    void setButtonStates();
    void setLayoutsModel(LayoutsTreeModel* model);
    void setAreasModel(LayoutsTreeModel* model);
    void setWidgetsModel(LayoutsTreeModel* model);

    LayoutPreviewWidget* m_layoutPreviewWidget;
    LayoutsTreeModel* m_layoutsTreeModel;
    Project* m_project;
    Ui::LayoutEditor* m_ui;
    Layout* m_layout;
    ProjectObject* m_currentSelectedObject;
    bool m_layoutChoosenInProjectBrowser;
};

#endif // LAYOUTEDITOR_H
