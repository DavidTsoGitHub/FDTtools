#ifndef SIGNALMAPPINGWIDGET_H
#define SIGNALMAPPINGWIDGET_H

class GCLSignalSink;
class Project;
class QLabel;
class QWidget;

class SignalMappingWidget : public QWidget
{
    Q_OBJECT
public:
    SignalMappingWidget(GCLSignalSink* sink, Project* projectModel, QWidget* parent = 0);

    void RefreshCurrentMapping();

signals:
    void signal_MappingUpdated(GCLSignalSink* sink);

private:
    GCLSignalSink* m_sink;
    Project* m_project;
    QLabel* m_sourceLabel;

    void showSignalMappingDialog();

    // QWidget interface
protected:
    void mouseDoubleClickEvent(QMouseEvent* event);
};

#endif // SIGNALMAPPINGWIDGET_H
