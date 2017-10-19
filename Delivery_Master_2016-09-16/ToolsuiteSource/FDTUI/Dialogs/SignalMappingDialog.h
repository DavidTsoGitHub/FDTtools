#ifndef SIGNALMAPPINGDIALOG_H
#define SIGNALMAPPINGDIALOG_H

class GCLSignalSink;
class GCLSignalSource;
class Project;
class QDialog;
template <typename T> class QList;
template <class Key, class T> class QMultiMap;
class QString;
class QWidget;

namespace Ui
{
class SignalMappingDialog;
}

class SignalMappingDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SignalMappingDialog(Project* project, GCLSignalSink* signalToMap, QWidget *parent = 0);
    ~SignalMappingDialog(void);

private slots:
    void slot_componentNameList_currentTextChanged(QString componentName);
    void slot_signalNameList_currentTextChanged(QString signalName);
    void slot_okButton_clicked(bool checked);
    void slot_cancelButton_clicked(bool);
    void slot_removeMappingButton_clicked(bool);

private:
    Ui::SignalMappingDialog *m_ui;
    bool m_preselectCurrentlyMappedSignal;
    Project* m_project;
    GCLSignalSource* m_selectedSource;
    GCLSignalSink* m_signalToMap;
    QList<GCLSignalSource*> m_validSources;
    QMultiMap<QString, GCLSignalSource*> m_sourcesByComponentName;
    bool m_hasMapping;

    void populateComponentNameList(void);
    void connectUISignals(void);
    void setSelectedSource(GCLSignalSource *source);
};

#endif // SIGNALMAPPINGDIALOG_H
