#ifndef DIAGEDITOR_H
#define DIAGEDITOR_H

class DIDToCALMappingModel;
class DTCLabelMappingModel;
class Project;
class QItemSelection;
class QModelIndex;
class QWidget;

namespace Ui
{
class DiagEditor;
}

class DiagEditor : public QWidget
{
    Q_OBJECT

public:
    explicit DiagEditor(QWidget *parent = 0);
    ~DiagEditor();

public slots:
    void SetProject(Project* project);
    void ClearProject();

private slots:
    void slot_removeDTCLabelMappingButtonClicked();
    void slot_dtcLabelMappingSelectionChanged(const QItemSelection&, const QItemSelection&);
    void slot_removeDIDMappingButtonClicked();
    void slot_didMappingSelectionChanged(const QItemSelection&, const QItemSelection&);
    void slot_dtcAdded(const QModelIndex &index);
    void slot_didMappingAdded(const QModelIndex &index);

private:
    Ui::DiagEditor *m_ui;
    DTCLabelMappingModel* m_dtcLabelMappingModel;
    DIDToCALMappingModel* m_didMappingModel;
    Project* m_project;

    void slot_projectUpdated();
};

#endif // DIAGEDITOR_H
