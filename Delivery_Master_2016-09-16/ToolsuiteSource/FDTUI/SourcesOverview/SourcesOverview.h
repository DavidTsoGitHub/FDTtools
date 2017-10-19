#ifndef _SIGNALOVERVIEW_H_
#define _SIGNALOVERVIEW_H_

class Project;
class SourcesOverviewModel;
class SourcesOverviewProxyModel;

class QWidget;

namespace Ui
{
class SourcesOverview;
}

class SourcesOverview : public QWidget
{
    Q_OBJECT

public:
    explicit SourcesOverview(QWidget* parent = 0);
    ~SourcesOverview();

public slots:
    void SetProject(Project* project);
    void ClearProject();
    void JustSelected();
    void JustDeselected();

private:
    typedef enum
    {
        SignalSourceColumn
    } sourcesOverviewCellType;

    Ui::SourcesOverview* m_ui;
    Project* m_project;

    bool m_showCAN = true;
    bool m_showHAL = true;
    bool m_showFunctionBlocks = true;
    bool m_showDisplay = true;
    bool m_showAnimations = true;
    bool m_showUnmappedSources = true;
    bool m_showMappedSources = true;

    bool m_tabShowing = true;
    bool m_populateTableNeeded = true;

    SourcesOverviewModel* m_sourceModel;
    SourcesOverviewProxyModel* m_proxyModel;

    void populate();
    void setupSignals();
};
#endif //_SIGNALOVERVIEW_H_
