#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDomDocument>
#include <QFutureWatcher>
#include <QMainWindow>
#include <QString>
#include <QXmlSchema>
#include <QXmlSchemaValidator>

class BspCustomDelegate;
class PropertiesModel;
class SettingsDialog;
class TreeNavigatorModel;
class XmlMessageHandler;

class QAction;
class QCloseEvent;
class QMenu;
class QWidget;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent *event) override;

private slots:
    void newFile();
    void open();
    void openRecent();
    bool save();
    bool saveAs();
    void showSettings();
    void about();
    void specWasModified();
    void loadSchema();
    void setStatusMessage(const QString& msg, bool error);
    void validate();
    void refreshTree();
    void refreshTable(QDomNode node);

private:
    static const int MaxRecent = 5;

    void init();
    void createActions();
    void createMenus();
    void createStatusBar();
    bool maybeSave();
    void loadFile(const QString &fileName);
    bool saveFile(const QString &fileName);
    void setCurrentFile(const QString &fileName);
    void updateRecentActions();
    QDomElement createElementUsingSchema(const QString& tagName);
    int numElementsAllowedUsingSchema(const QString& tagName);

    Ui::MainWindow *m_ui;
    SettingsDialog *m_settingsDialog;

    QString m_currentFileName;
    bool m_isUntitled;

    QMenu *m_fileMenu;
    QMenu *m_editMenu;
    QMenu *m_helpMenu;

    QAction *m_newAction;
    QAction *m_openAction;
    QAction *m_saveAction;
    QAction *m_saveAsAction;
    QAction *m_settingsAction;
    QAction *m_exitAction;
    QAction *m_aboutAction;
    QAction *m_openRecentActions[MaxRecent];

    QDomDocument m_spec;

    TreeNavigatorModel *m_treeModel;
    BspCustomDelegate *m_bspCustomDelegate;
    PropertiesModel *m_propertiesModel;

    XmlMessageHandler *m_xmlMsgHandler;
    QXmlSchemaValidator m_validator;
    QXmlSchema m_schema;
    QDomDocument m_schemaAsXml;
    QFutureWatcher<bool> m_validationWatcher;

};

#endif // MAINWINDOW_H
