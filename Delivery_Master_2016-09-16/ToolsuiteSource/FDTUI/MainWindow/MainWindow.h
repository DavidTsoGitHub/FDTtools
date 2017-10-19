#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QProcess>


class AnimationEditor;
class CheckMessageEditor;
class CompilerBase;
class DiagEditor;
class FunctionBlock;
class IconManager;
class ImageManager;
class IssueListSyntaxHighlighter;
class LabelEditor;
class LayoutEditor;
class MenuEditor;
class PlatformBuilder;
class PlatformGenerator;
class Project;
class ProjectBrowser;
class ProjectController;
class ProjectObject;
class PropertiesWidgetContainer;
class SelectDisplayAreaDialog;
class SettingsHandler;
class SignalMappingCanvas;
class SimulatorRunner;
class SinksOverview;
class SourcesOverview;
class UIToolboxWidget;
class UIWidgetsModel;

class QFile;
class QFileInfo;
class QFileSystemModel;
class QFileSystemWatcher;
class QMainWindow;
class QPlainTextEdit;
class QSortFilterProxyModel;
class QString;
class QStringList;
class QTextStream;
class QWidget;

namespace Ui
{
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = 0);
    ~MainWindow(void);

    void OpenProject(const QFileInfo& projectFile);
    void SetFunctionLibraryPath(QString);
    void RestoreWindowStateAndGeometry();
signals:
    void signal_ProjectFileSelected(const QFileInfo& projectFile);

public slots:
    void slot_ShowCaughtError(QStringList errorMessages);

private:
    Ui::MainWindow* ui;

    PropertiesWidgetContainer* m_propertiesWidgetContainer;
    PlatformGenerator* m_platformGenerator;
    PlatformBuilder* m_platformBuilder;
    SimulatorRunner* m_simulatorRunner;

    QFileSystemModel* m_functionLibraryModel;
    QSortFilterProxyModel* m_functionLibraryProxyModel;
    FunctionBlock* m_selectedFunctionBlock;

    Project* m_currentProject;

    ProjectBrowser* m_projectBrowser;
    QString m_selectedFunctionLibraryPath;

    SignalMappingCanvas* m_signalMappingCanvas;
    SinksOverview* m_sinksOverview;
    SourcesOverview* m_sourcesOverview;
    LayoutEditor* m_layoutEditor;
    LabelEditor* m_labelEditor;
    IconManager* m_iconManager;
    ImageManager* m_imageManager;
    MenuEditor* m_menuEditor;
    AnimationEditor* m_animationEditor;
    CheckMessageEditor* m_checkMessageEditor;
    DiagEditor* m_diagEditor;
    UIWidgetsModel* m_uiWidgetsModel;

    ProjectController* m_projectController;

    QString m_lastProjectRoot;
    SettingsHandler* m_settingsHandler;

    bool m_runSimulator;
    bool m_buildAllowed;
    bool m_buildPending;
    bool m_platformGenerationNeeded;

    QProcess m_helpProcess;

    SelectDisplayAreaDialog* m_addMenuDialog;
    IssueListSyntaxHighlighter* m_issueListSyntaxHighlighter;

    QFileSystemWatcher* functionBlockFileSystemWatcher;

    CompilerBase* m_compiler;

    QFile* m_generatorLogFile;
    QTextStream* m_generatorLogStream;
    QFile* m_builderLogFile;
    QTextStream* m_builderLogStream;
    QFile* m_issuesLogFile;
    QTextStream* m_issuesLogStream;
    QFile* m_simulatorLogFile;
    QTextStream* m_simulatorLogStream;

    void setupFunctionBlockLibrary(void);
    void setupSignals(void);

    void updateFunctionBlockFileSystemWatcher();

    QString getRelativePath(QFileInfo, QFileInfo);

    void setDisplayEditorWindowsEnabled(bool enabled);
    void setCommonEditorWindowsEnabled(bool enabled);

    bool setupCompiler(void);
    bool currentTargetBinaryExist(void);
    const QDateTime getLastBuildTime(void);
    bool saveNeeded(void);
    void enableGenerateBuildRun(void);
    void disableGenerateBuildRun(void);

    void setSimulatorAsActive();
    void setTargetAsActive();
    void setupDisplayEnvironment(bool hasDisplay);
    void slot_projectBrowserChanged(ProjectObject *projectObject);

    void saveWindowStateAndGeometry();

    void logPlainToolOutput(QPlainTextEdit* plainTextEdit, QTextStream* logStream, QString msg);
    void logHtmlToolOutput(QPlainTextEdit* plainTextEdit, QTextStream* logStream, QString msg);
    void clearToolOutput(QPlainTextEdit* plainTextEdit, QFile* logFile);
    void setupLogging(QTextStream** logStream, QFile** logFile, QString filePath);

private slots:
    void slot_actionNewProject_triggered();
    void slot_OpenProjectFileSelector();
    void slot_actionQuit_triggered();
    void slot_actionSettings_triggered();
    void slot_actionGenerate_triggered();
    void slot_actionBuild_triggered();
    void slot_actionQuickBuild_triggered();
    void slot_actionRun_triggered();
    void slot_actionAbortBuild_triggered();

    void slot_actionCreateFunctionBlock_triggered(void);
    void slot_generatePlatform_starting(QString program, QStringList arguments);
    void slot_generatePlatform_finished(int exitCode);
    void slot_generatePlatform_error(QString error);
    void slot_noGeneratorFound(void);
    void slot_buildPlatform_starting(QString program, QStringList arguments);
    void slot_buildPlatform_finished(int exitCode);
    void slot_buildPlatform_error(QString error);
    void slot_noBuilderFound(void);
    void slot_simulation_starting(QString program, QStringList arguments);
    void slot_noSimulatorFound(void);
    void slot_projectClosed(void);
    void slot_projectLoaded(Project* project);
    void slot_noValidFunctionLibrary(void);

    void slot_addFunctionBlockButton_clicked(void);
    void slot_editFunctionBlockButton_clicked(void);
    void slot_functionLibraryTreeSelectionModel_currentChanged(QModelIndex, QModelIndex);
    void slot_actionOpenProjectProperties_triggered(void);
	void slot_schedulerAnalysis_triggered();

    void build_triggered(bool isQuickBuild);
	
    // QWidget interface
protected:
    void closeEvent(QCloseEvent*);
};

#endif // MAINWINDOW_H
