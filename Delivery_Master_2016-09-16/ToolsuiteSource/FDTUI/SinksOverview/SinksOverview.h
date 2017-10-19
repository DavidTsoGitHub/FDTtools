#ifndef _SINKSOVERVIEW_H_
#define _SINKSOVERVIEW_H_

class Project;
class SinksOverviewModel;
class SinksOverviewProxyModel;

class QWidget;

namespace Ui
{
class SinksOverview;
}

class SinksOverview : public QWidget
{
    Q_OBJECT

public:
    explicit SinksOverview(QWidget* parent = 0);
    ~SinksOverview();

public slots:
    void SetProject(Project* project);
    void ClearProject();
    void JustSelected();
    void JustDeselected();

private:
    typedef enum
    {
        SignalSinkColumn,
        SignalSourceColumn
    } sinksOverviewCellType;

    Project* m_project;
    Ui::SinksOverview* m_ui;

    bool m_tabShowing;
    bool m_populateTableNeeded;

    SinksOverviewModel* m_sinkModel;
    SinksOverviewProxyModel* m_proxyModel;

    void populate();
    void setupSignals();
};
#endif //_SINKSOVERVIEW_H_
