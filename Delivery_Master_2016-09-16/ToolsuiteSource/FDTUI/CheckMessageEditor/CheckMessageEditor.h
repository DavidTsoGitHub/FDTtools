#ifndef CHECKMESSAGEEDITOR_H
#define CHECKMESSAGEEDITOR_H

#include "Project.h"

class CheckMessageModel;
class QWidget;

namespace Ui
{
class CheckMessageEditor;
}

class CheckMessageEditor : public QWidget
{
    Q_OBJECT

public:
    explicit CheckMessageEditor(QWidget* parent = 0);
    ~CheckMessageEditor(void);

signals:
    void signal_modified();
    void signal_CheckMessageEditorItemSelected(ProjectObject* selectedItem);

public slots:
    void slot_SetProject(Project* project);
    void slot_ClearProject();
    void slot_RemoveWidgetSink(GCLSignalSink* sink, quint32 widgetID);

    void slot_layoutAdded(quint32 layoutID);
    void slot_layoutRemoved(quint32 layoutID);

    void slot_layoutModelChanged();

    void JustSelected();
private:
    Ui::CheckMessageEditor* m_ui;
    Project* m_project;
    CheckMessageModel* m_checkMessageModel;
    QDir m_projectRoot;
    ProjectObject* m_currentCheckMessageObject;

private slots:
    void slot_checkMessageSelectionChanged(const QItemSelection&, const QItemSelection&);
    void slot_removeSelectedCheckMessages();
    void slot_selectionChanged(const QItemSelection& selected, const QItemSelection& deselected);
    void slot_checkMessageAdded(const QModelIndex& index);
};

#endif // CHECKMESSAGEEDITOR_H
