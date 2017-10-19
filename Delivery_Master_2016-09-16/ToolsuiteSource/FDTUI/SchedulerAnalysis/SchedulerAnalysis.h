#ifndef SCHEDULERANALYSIS_H
#define SCHEDULERANALYSIS_H

class Project;
class SchedulerTaskGraph;
class SchedulerTaskModel;

class QDialog;
class QGraphicsScene;
class QModelIndex;
class QWidget;

namespace Ui {
class SchedulerAnalysis;
}

class SchedulerAnalysis : public QDialog
{
    Q_OBJECT

public:
    explicit SchedulerAnalysis(const Project& project, QWidget *parent = 0);
    ~SchedulerAnalysis();

private slots:
    void slot_currentTimeChanged(int newValue);
    void slot_timeLimitChanged(int newValue);
    void timeClicked(int newTime);

private:
    Ui::SchedulerAnalysis *m_ui;
    const Project& m_project;
    SchedulerTaskModel* m_taskModel;
    SchedulerTaskGraph* m_taskGraph;

    void setupData();
    void updateCurrentTimeTable();
    void modelDataChanged(const QModelIndex&, const QModelIndex&);
};

#endif // SCHEDULERANALYSIS_H
