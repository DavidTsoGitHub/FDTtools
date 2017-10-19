#include "MainWindow.h"
#include "ui_MainWindow.h"

#include "AnimationEditor.h"
#include "BSPDisplay.h"
#include "bspmemory.h"
#include "CheckMessageEditor.h"
#include "CompilerBase.h"
#include "CreateFunctionBlockWizard.h"
#include "CreateProjectWizard.h"
#include "DiagEditor.h"
#include "DirtyObjectsVisitor.h"
#include "Exceptions/CannotOpenFileException.h"
#include "FunctionBlock.h"
#include "FunctionDefinition.h"
#include "FunctionDefinitionDeserializer.h"
#include "Helpers/PlatformBuilder.h"
#include "Helpers/PlatformGenerator.h"
#include "Helpers/ProjectController.h"
#include "Helpers/SettingsHandler.h"
#include "Helpers/SimulatorRunner.h"
#include "IconManager.h"
#include "ImageManager.h"
#include "IssueListSyntaxHighlighter.h"
#include "LabelEditor.h"
#include "LayoutEditor.h"
#include "LayoutDefinition.h"
#include "MenuEditor.h"
#include "Project.h"
#include "ProjectBrowser.h"
#include "ProjectDeserializer.h"
#include "ProjectObject.h"
#include "ProjectPropertiesDialog.h"
#include "PropertiesWidgetContainer.h"
#include "SchedulerAnalysis.h"
#include "SelectDisplayAreaDialog.h"
#include "SignalMappingCanvas.h"
#include "SinksOverview.h"
#include "SourcesOverview.h"
#include "ToolSettings.h"
#include "ToolSettingsDialog.h"
#include "UIWidgetBase.h"
#include "UIWidgetsModel.h"
#include "UIWidgetsRepository.h"

#include <QAction>
#include <QDebug>
#include <QDesktopServices>
#include <QDialog>
#include <QDir>
#include <QDirIterator>
#include <QFile>
#include <QFileDialog>
#include <QFileInfo>
#include <QFileSystemModel>
#include <QFileSystemWatcher>
#include <QtGlobal>
#include <QMainWindow>
#include <QMessageBox>
#include <QSortFilterProxyModel>
#include <QStandardItemModel>
#include <QStandardPaths>
#include <QString>
#include <QStringList>
#include <QTextStream>
#include <QTreeView>
#include <QWidget>

#if defined(Q_OS_WIN32)
    #include <windows.h>
#endif

MainWindow::MainWindow(QWidget* parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_propertiesWidgetContainer(new PropertiesWidgetContainer(this)),
    m_platformGenerator(new PlatformGenerator(this)),
    m_platformBuilder(new PlatformBuilder(this)),
    m_simulatorRunner(new SimulatorRunner(this)),
    m_functionLibraryModel(NULL),
    m_functionLibraryProxyModel(NULL),
    m_currentProject(NULL),
    m_projectBrowser(new ProjectBrowser()),
    m_signalMappingCanvas(new SignalMappingCanvas()),
    m_sinksOverview(new SinksOverview()),
    m_sourcesOverview(new SourcesOverview()),
    m_layoutEditor(NULL),
    m_labelEditor(NULL),
    m_iconManager(NULL),
    m_imageManager(NULL),
    m_menuEditor(NULL),
    m_animationEditor(NULL),
    m_checkMessageEditor(NULL),
    m_diagEditor(new DiagEditor()),
    m_uiWidgetsModel(NULL),
    m_projectController(new ProjectController(this)),
    m_settingsHandler(new SettingsHandler(this)),
    m_runSimulator(false),
    m_buildAllowed(false),
    m_buildPending(false),
    m_platformGenerationNeeded(true),
    m_addMenuDialog(NULL),
    m_issueListSyntaxHighlighter(NULL),
    functionBlockFileSystemWatcher(new QFileSystemWatcher(this)),
    m_compiler(NULL),
    m_generatorLogFile(NULL),
    m_generatorLogStream(NULL),
    m_builderLogFile(NULL),
    m_builderLogStream(NULL),
    m_issuesLogFile(NULL),
    m_issuesLogStream(NULL),
    m_simulatorLogFile(NULL),
    m_simulatorLogStream(NULL)
{
    QCoreApplication::setOrganizationName("SwedSpot AB");
    QCoreApplication::setOrganizationDomain("swedspot.com");
    //QCoreApplication::setApplicationName("FDT");

    ui->setupUi(this);
    this->setStyleSheet("QMainWindow::separator { "
                        "background: rgb(200, 200, 200);"
                        "width: 4px;"
                        "height: 4px;"
                        "}");

    // Additional UI setup
    ui->propertiesDock->setWindowTitle("Properties");
    ui->propertiesDock->setWidget(m_propertiesWidgetContainer);
    ui->projectBrowserContainer->layout()->addWidget(m_projectBrowser);
    ui->SignalMappingLayout->addWidget(m_signalMappingCanvas);
    ui->SinksOverviewTab->layout()->addWidget(m_sinksOverview);
    ui->SourcesOverviewTab->layout()->addWidget(m_sourcesOverview);
    ui->DiagLayout->addWidget(m_diagEditor);

    setDisplayEditorWindowsEnabled(false);
    setCommonEditorWindowsEnabled(false);

    ui->menuWindows->addAction(ui->projectBrowserDock->toggleViewAction());
    ui->menuWindows->addAction(ui->functionLibraryDock->toggleViewAction());
    ui->menuWindows->addAction(ui->propertiesDock->toggleViewAction());
    ui->menuWindows->addAction(ui->toolOutputDock->toggleViewAction());

    Q_INIT_RESOURCE(ToolIcons);

    m_issueListSyntaxHighlighter = new IssueListSyntaxHighlighter(ui->validationOutput->document());

    m_settingsHandler->LoadToolSettings();
    if (m_settingsHandler->ValidatePaths(m_settingsHandler->GetToolSettings()) == false)
    {
        QMessageBox::warning(this, "Invalid/missing tool path", "One or more tool paths is invalid/missing, please update the tool settings in the next dialog (that opens after pressing OK)!");
        slot_actionSettings_triggered();
        if (m_settingsHandler->ValidatePaths(m_settingsHandler->GetToolSettings()) == false)
        {
            QMessageBox::critical(this, "Invalid paths", "Tool setting failed validation, please review the tool settings!", QMessageBox::Ok);
            QApplication::quit();
        }
    }
    setupFunctionBlockLibrary();
    MainWindow::tabifyDockWidget(ui->projectBrowserDock, ui->functionLibraryDock);
    ui->projectBrowserDock->raise();

    setupSignals();
    setupLogging(&m_generatorLogStream, &m_generatorLogFile, QString("./generator_session_log.txt"));
    setupLogging(&m_builderLogStream, &m_builderLogFile, QString("./builder_session_log.txt"));
    setupLogging(&m_issuesLogStream, &m_issuesLogFile, QString("./issues_session_log.txt"));
    setupLogging(&m_simulatorLogStream, &m_simulatorLogFile, QString("./simulator_session_log.txt"));
}

MainWindow::~MainWindow()
{
    delete m_functionLibraryModel;
    delete m_functionLibraryProxyModel;
    delete m_menuEditor;
    delete m_diagEditor;
    delete m_issueListSyntaxHighlighter;

    saveWindowStateAndGeometry();

    delete ui;
}

void MainWindow::OpenProject(const QFileInfo& projectFile)
{
    m_projectController->OpenProject(projectFile);
}

void MainWindow::SetFunctionLibraryPath(QString path)
{
    m_settingsHandler->GetToolSettings().FunctionLibraryPath = path;
}

//* PRIVATE HELPERS *//

void MainWindow::setupDisplayEnvironment(bool hasDisplay)
{
    if (hasDisplay)
    {
        if (m_layoutEditor == NULL)
        {
            m_layoutEditor = new LayoutEditor();
            m_labelEditor = new LabelEditor();
            m_iconManager = new IconManager();
            m_imageManager = new ImageManager();
            m_menuEditor = new MenuEditor();
            m_animationEditor = new AnimationEditor();
            m_checkMessageEditor = new CheckMessageEditor();
        }

        ui->LayoutEditorTab->layout()->addWidget(m_layoutEditor);
        ui->LabelEditorLayout->addWidget(m_labelEditor);
        ui->IconManagerTab->layout()->addWidget(m_iconManager);
        ui->ImageManagerTab->layout()->addWidget(m_imageManager);
        ui->MenuDesignerTab->layout()->addWidget(m_menuEditor);
        ui->AnimationEditorTab->layout()->addWidget(m_animationEditor);
        ui->CheckMessageTab->layout()->addWidget(m_checkMessageEditor);

        // Setup signals
        connect(m_projectBrowser, &ProjectBrowser::CurrentProjectObjectChanged, m_menuEditor, &MenuEditor::SetCurrentProjectObject);

        /* Layout Editor */
        connect(m_layoutEditor, &LayoutEditor::signal_LayoutEditorItemSelected, m_propertiesWidgetContainer, &PropertiesWidgetContainer::SetCurrentProjectObject);
        connect(m_layoutEditor, &LayoutEditor::signal_modified, m_projectBrowser, &ProjectBrowser::ResetModel);
        connect(m_layoutEditor, &LayoutEditor::signal_WidgetToBeRemoved, m_checkMessageEditor, &CheckMessageEditor::slot_RemoveWidgetSink);
        connect(m_layoutEditor, &LayoutEditor::signal_updateWidgetOrder, [=] (QList<quint32> widgetIDs)
        {
            m_currentProject->GetCheckMessagesDefinition().UpdateCheckMessageItemOrder(widgetIDs);
        });
        connect(m_layoutEditor, &LayoutEditor::signal_layoutAdded, m_checkMessageEditor, &CheckMessageEditor::slot_layoutAdded);
        connect(m_layoutEditor, &LayoutEditor::signal_layoutRemoved, m_checkMessageEditor, &CheckMessageEditor::slot_layoutRemoved);

        /* Label Editor */
        connect(m_labelEditor, &LabelEditor::signal_modified, m_projectBrowser, &ProjectBrowser::ResetModel);

        /* Menu Editor */
        connect(m_menuEditor, &MenuEditor::signal_MenuEditorItemSelected, m_propertiesWidgetContainer, &PropertiesWidgetContainer::SetCurrentProjectObject);
        connect(m_menuEditor, &MenuEditor::signal_modified, m_projectBrowser, &ProjectBrowser::ResetModel);

        /* Animation Editor */
        connect(m_animationEditor, &AnimationEditor::signal_AnimationEditorItemSelected, m_propertiesWidgetContainer, &PropertiesWidgetContainer::SetCurrentProjectObject);
        connect(m_animationEditor, &AnimationEditor::signal_modified, m_projectBrowser, &ProjectBrowser::ResetModel);

        /* Check Message Editor */
        connect(m_checkMessageEditor, &CheckMessageEditor::signal_CheckMessageEditorItemSelected, m_propertiesWidgetContainer, &PropertiesWidgetContainer::SetCurrentProjectObject);
        connect(m_checkMessageEditor, &CheckMessageEditor::signal_modified, m_projectBrowser, &ProjectBrowser::ResetModel);

        /* Icon Manager */
        connect(m_iconManager, &IconManager::signal_iconSelectionChanged, m_propertiesWidgetContainer, &PropertiesWidgetContainer::SetCurrentProjectObject);
        connect(m_iconManager, &IconManager::signal_modified, m_projectBrowser, &ProjectBrowser::ResetModel);

        /* Image Manager */
        connect(m_imageManager, &ImageManager::signal_imageSelectionChanged, m_propertiesWidgetContainer, &PropertiesWidgetContainer::SetCurrentProjectObject);
        connect(m_imageManager, &ImageManager::signal_modified, m_projectBrowser, &ProjectBrowser::ResetModel);
        connect(m_imageManager, &ImageManager::signal_modified, m_animationEditor, &AnimationEditor::RefreshAvailableImages);
        connect(m_imageManager, &ImageManager::signal_ImageDataChanged, m_animationEditor, &AnimationEditor::RefreshAvailableImages);
    }
    else
    {
        if (m_layoutEditor != NULL)
        {
            ui->LayoutEditorTab->layout()->removeWidget(m_layoutEditor);
            delete m_layoutEditor;
            m_layoutEditor = NULL;
        }

        if (m_labelEditor != NULL)
        {
            ui->LabelEditorLayout->removeWidget(m_labelEditor);
            delete m_labelEditor;
            m_labelEditor = NULL;
        }

        if (m_iconManager != NULL)
        {
            ui->IconManagerTab->layout()->removeWidget(m_iconManager);
            delete m_iconManager;
            m_iconManager = NULL;
        }

        if (m_imageManager != NULL)
        {
            ui->ImageManagerTab->layout()->removeWidget(m_imageManager);
            delete m_imageManager;
            m_imageManager = NULL;

        }

        if (m_menuEditor != NULL)
        {
            ui->MenuDesignerTab->layout()->removeWidget(m_menuEditor);
            delete m_menuEditor;
            m_menuEditor = NULL;
        }

        if (m_animationEditor != NULL)
        {
            ui->MenuDesignerTab->layout()->removeWidget(m_animationEditor);
            delete m_animationEditor;
            m_animationEditor = NULL;
        }

        if (m_checkMessageEditor != NULL)
        {
            ui->CheckMessageTab->layout()->removeWidget(m_checkMessageEditor);
            delete m_checkMessageEditor;
            m_checkMessageEditor = NULL;
        }

    }
}

void MainWindow::setupSignals()
{
    /* Menu items*/
    connect(ui->actionNewProject, &QAction::triggered, this, &MainWindow::slot_actionNewProject_triggered);
    connect(ui->actionOpenProject, &QAction::triggered, this, &MainWindow::slot_OpenProjectFileSelector);
    connect(ui->actionReloadProject, &QAction::triggered, m_projectController, &ProjectController::ReloadProject);
    connect(ui->actionCloseProject, &QAction::triggered, m_projectController, &ProjectController::CloseProject);
    connect(ui->actionSaveProject, &QAction::triggered, m_projectController, &ProjectController::SaveProject);
    connect(ui->actionQuit, &QAction::triggered, this, &MainWindow::slot_actionQuit_triggered);
    connect(ui->actionSettings, &QAction::triggered, this, &MainWindow::slot_actionSettings_triggered);
    connect(ui->actionOpenProjectProperties, &QAction::triggered, this, &MainWindow::slot_actionOpenProjectProperties_triggered);
    connect(ui->actionGenerate, &QAction::triggered, this, &MainWindow::slot_actionGenerate_triggered);
    connect(ui->actionBuild, &QAction::triggered, this, &MainWindow::slot_actionBuild_triggered);
    connect(ui->actionQuickBuild, &QAction::triggered, this, &MainWindow::slot_actionQuickBuild_triggered);
    connect(ui->actionRun, &QAction::triggered, this, &MainWindow::slot_actionRun_triggered);
    connect(ui->actionSimulationActive, &QAction::triggered, this, &MainWindow::setSimulatorAsActive);
    connect(ui->actionTargetActive, &QAction::triggered, this, &MainWindow::setTargetAsActive);
    connect(ui->actionCreateFunctionBlock, &QAction::triggered, this, &MainWindow::slot_actionCreateFunctionBlock_triggered);
    connect(ui->actionSchedulerAnalysis, &QAction::triggered, this, &MainWindow::slot_schedulerAnalysis_triggered);
    connect(ui->actionAbout_FDT, &QAction::triggered, [=] ()
    {
        QMessageBox::about(this, tr("About"),
                           "Function Definition Tool\n\n" + tr("Application Version") + ": " + QCoreApplication::applicationVersion() + \
                           "\n" + tr("Build Date") + ": " + QString(APP_BUILD_DATE) + \
                           "\n" + tr("Build") + ": " + QString(APP_BUILD) + \
                           "\n\nCopyright © 2015-2016 Swedspot AB");
    });
    connect(ui->actionHelp, &QAction::triggered, [=] ()
    {
        ui->actionHelp->setEnabled(false);

        QDir path = QApplication::applicationDirPath();
        m_helpProcess.start("hh.exe " + path.absoluteFilePath("Tutorial.chm"));
        connect(&m_helpProcess, static_cast<void(QProcess::*)(int, QProcess::ExitStatus)>(&QProcess::finished), [=] (int value, QProcess::ExitStatus exStat)
        {
            Q_UNUSED(value)
            Q_UNUSED(exStat)
            ui->actionHelp->setEnabled(true);
        });

    });

    /* Main Window signals */
    connect(this, &MainWindow::signal_ProjectFileSelected, m_projectController, &ProjectController::OpenProject);
    connect(ui->abortButton, &QAbstractButton::pressed, this, &MainWindow::slot_actionAbortBuild_triggered);

    /* Project Controller Signals */
    connect(m_projectController, &ProjectController::ProjectSaved, [=]
    {
        setWindowTitle( QCoreApplication::applicationName() + " "  + QCoreApplication::applicationVersion() + " - " + m_currentProject->GetProjectFile().absoluteFilePath());
        ui->actionSaveProject->setEnabled(false);
    });
    connect(m_projectController, &ProjectController::ProjectLoaded, [=](Project* project)
    {
        slot_projectLoaded(project);
        m_projectBrowser->SetProject(project);
        m_signalMappingCanvas->ProjectLoaded(project);
        m_propertiesWidgetContainer->SetProject(project);
        m_sinksOverview->SetProject(project);
        m_sourcesOverview->SetProject(project);
        m_diagEditor->SetProject(project);

        if (project->GetBSPSpecification().HasDisplay())
        {
            m_layoutEditor->SetProject(project);
            m_imageManager->SetProject(project);
            m_labelEditor->SetProject(project);
            m_iconManager->SetProject(project);
            m_menuEditor->SetProject(project);
            m_animationEditor->SetProject(project);
            m_checkMessageEditor->slot_SetProject(project);
        }

        m_platformGenerationNeeded = true;
    });
    connect(m_projectController, &ProjectController::ProjectClosed, [=]
    {
        //if(m_currentProject != NULL)
        //{
            m_projectBrowser->ClearProject();
            m_signalMappingCanvas->ProjectUnloaded();
            m_propertiesWidgetContainer->ClearProject();
            m_sinksOverview->ClearProject();
            m_sourcesOverview->ClearProject();
            m_layoutEditor->ClearProject();
            m_labelEditor->ClearProject();
            m_menuEditor->ClearProject();
            m_animationEditor->ClearProject();
            m_checkMessageEditor->slot_ClearProject();
            m_iconManager->ClearProject();
            m_imageManager->ClearProject();
            m_diagEditor->ClearProject();
        //}

       if (m_layoutEditor != NULL) // Indicates that display components have been loaded
       {
           delete m_layoutEditor;
           m_layoutEditor = NULL;
           delete m_labelEditor;
           m_labelEditor = NULL;
           delete m_iconManager;
           m_iconManager = NULL;
           delete m_imageManager;
           m_imageManager = NULL;
           delete m_menuEditor;
           m_menuEditor = NULL;
           delete m_animationEditor;
           m_animationEditor = NULL;
           delete m_checkMessageEditor;
           m_checkMessageEditor = NULL;
       }

       slot_projectClosed();
    });

    connect(m_projectController, &ProjectController::ProjectFunctionLibraryPathError, [=] (const QString& message)
    {
        QMessageBox::warning(this, "Error in FDT Settings, project cannot be loaded.\n", message);
    });

    connect(m_projectController, &ProjectController::ProjectLoadError, [=] (const QFileInfo& fileLoaded, const QString& message)
    {
        QMessageBox::warning(this, "Error while loading project", fileLoaded.filePath() + "\n" + message);
    });

    connect(m_projectController, &ProjectController::ProjectSaveError, [=] (const QString& message)
    {
        QMessageBox::warning(this, "Error while saving project", message);
    });

    /* Function Library */
    connect(ui->addFunctionBlockButton, &QPushButton::clicked, this, &MainWindow::slot_addFunctionBlockButton_clicked);
    connect(ui->editFunctionBlockButton, &QPushButton::clicked, this, &MainWindow::slot_editFunctionBlockButton_clicked);

    /* Project Browser */
    connect(m_projectBrowser, &ProjectBrowser::CurrentProjectObjectChanged, m_propertiesWidgetContainer, &PropertiesWidgetContainer::SetCurrentProjectObject);
    connect(m_projectBrowser, &ProjectBrowser::CurrentProjectObjectChanged, m_signalMappingCanvas, &SignalMappingCanvas::SetCurrentProjectObject);
    connect(m_signalMappingCanvas, &SignalMappingCanvas::signal_selectedProjectObject, m_propertiesWidgetContainer, &PropertiesWidgetContainer::SetCurrentProjectObject);
    connect(m_projectBrowser, &ProjectBrowser::CurrentProjectObjectChanged, this, &MainWindow::slot_projectBrowserChanged);
    connect(m_projectBrowser, &ProjectBrowser::LayoutChoosenInProjectBrowser, [=] () { m_layoutEditor->LayoutChoosenInProjectBrowser(); });

    /* Main tab changed */
    connect(ui->mainTabWidget, &QTabWidget::currentChanged, [=] {
        m_projectBrowser->ResetModel();
        //m_propertiesWidgetContainer->ClearCurrentProjectObject();
        //m_propertiesWidgetContainer->SetCurrentProjectObject(static_cast<ProjectObject*>(m_projectBrowser->currentIndex().internalPointer()));

        if (m_currentProject == NULL)
        {
            return;
        }

        /* Notify interested tabs that they have been selected */
        //SinksOverview
        if((ui->mainTabWidget->currentWidget()->findChild<SinksOverview*>() == m_sinksOverview) && (m_sinksOverview != NULL))
        {
            m_propertiesWidgetContainer->ClearCurrentProjectObject();
            m_propertiesWidgetContainer->SetCurrentProjectObject(static_cast<ProjectObject*>(m_projectBrowser->currentIndex().internalPointer()));
            m_sinksOverview->blockSignals(false);
            m_sinksOverview->JustSelected();
        }
        else if (m_sinksOverview != NULL)
        {
            m_sinksOverview->blockSignals(true);
            m_sinksOverview->JustDeselected();
        }

        //SourcesOverview
        if((ui->mainTabWidget->currentWidget()->findChild<SourcesOverview*>() == m_sourcesOverview) && (m_sourcesOverview != NULL))
        {
            m_propertiesWidgetContainer->ClearCurrentProjectObject();
            m_propertiesWidgetContainer->SetCurrentProjectObject(static_cast<ProjectObject*>(m_projectBrowser->currentIndex().internalPointer()));
            m_sourcesOverview->blockSignals(false);
            m_sourcesOverview->JustSelected();
        }
        else if (m_sourcesOverview != NULL)
        {
            m_sourcesOverview->blockSignals(true);
            m_sourcesOverview->JustDeselected();
        }

        //SignalMappingCanvas
        if((ui->mainTabWidget->currentWidget()->findChild<SignalMappingCanvas*>() == m_signalMappingCanvas) && (m_signalMappingCanvas != NULL))
        {
            m_propertiesWidgetContainer->ClearCurrentProjectObject();
            m_propertiesWidgetContainer->SetCurrentProjectObject(static_cast<ProjectObject*>(m_projectBrowser->currentIndex().internalPointer()));
            m_signalMappingCanvas->blockSignals(false);
            m_signalMappingCanvas->JustSelected();
        }
        else if (m_signalMappingCanvas != NULL)
        {
            m_signalMappingCanvas->blockSignals(true);
        }

        //LayoutEditor
        if((ui->mainTabWidget->currentWidget()->findChild<LayoutEditor*>() == m_layoutEditor) && (m_layoutEditor != NULL))
        {
            // Don't clear current project object, layout editor does not work like the other tabs. (Need to handle selection of "Layout" in ProjectBrowser)
            m_layoutEditor->blockSignals(false);
            m_layoutEditor->JustSelected();
        }
        else if (m_layoutEditor != NULL)
        {
            m_layoutEditor->blockSignals(true);
        }

        //LabelEditor
        if((ui->mainTabWidget->currentWidget()->findChild<LabelEditor*>() == m_labelEditor) && (m_labelEditor != NULL))
        {
            m_propertiesWidgetContainer->ClearCurrentProjectObject();
            m_propertiesWidgetContainer->SetCurrentProjectObject(static_cast<ProjectObject*>(m_projectBrowser->currentIndex().internalPointer()));
            m_labelEditor->JustSelected();
            m_labelEditor->blockSignals(false);
        }
        else if (m_labelEditor != NULL)
        {
            m_labelEditor->JustDeselected();
            m_labelEditor->blockSignals(true);
        }

        //IconManager
        if((ui->mainTabWidget->currentWidget()->findChild<IconManager*>() == m_iconManager) && (m_iconManager != NULL))
        {
            m_propertiesWidgetContainer->ClearCurrentProjectObject();
            m_propertiesWidgetContainer->SetCurrentProjectObject(static_cast<ProjectObject*>(m_projectBrowser->currentIndex().internalPointer()));
            m_iconManager->blockSignals(false);
            m_iconManager->JustSelected();
        }
        else if (m_iconManager != NULL)
        {
            m_iconManager->blockSignals(true);
        }

        //ImageManager
        if((ui->mainTabWidget->currentWidget()->findChild<ImageManager*>() == m_imageManager) && (m_imageManager != NULL))
        {
            m_propertiesWidgetContainer->ClearCurrentProjectObject();
            m_propertiesWidgetContainer->SetCurrentProjectObject(static_cast<ProjectObject*>(m_projectBrowser->currentIndex().internalPointer()));
            m_imageManager->blockSignals(false);
            m_imageManager->JustSelected();
        }
        else if (m_imageManager != NULL)
        {
            m_imageManager->blockSignals(true);
        }

        //MenuEditor
        if((ui->mainTabWidget->currentWidget()->findChild<MenuEditor*>() == m_menuEditor) && (m_menuEditor != NULL))
        {
            m_propertiesWidgetContainer->ClearCurrentProjectObject();
            m_propertiesWidgetContainer->SetCurrentProjectObject(static_cast<ProjectObject*>(m_projectBrowser->currentIndex().internalPointer()));
            m_menuEditor->blockSignals(false);
            m_menuEditor->JustSelected();
        }
        else if (m_menuEditor != NULL)
        {
            m_menuEditor->blockSignals(true);
        }

        //AnimationEditor
        if((ui->mainTabWidget->currentWidget()->findChild<AnimationEditor*>() == m_animationEditor) && (m_animationEditor != NULL))
        {
            m_propertiesWidgetContainer->ClearCurrentProjectObject();
            m_propertiesWidgetContainer->SetCurrentProjectObject(static_cast<ProjectObject*>(m_projectBrowser->currentIndex().internalPointer()));
            m_animationEditor->blockSignals(false);
            m_animationEditor->JustSelected();
        }
        else if (m_animationEditor != NULL)
        {
            m_animationEditor->blockSignals(true);
        }

        //CheckMessageEditor
        if((ui->mainTabWidget->currentWidget()->findChild<CheckMessageEditor*>() == m_checkMessageEditor) && (m_checkMessageEditor != NULL))
        {
            m_propertiesWidgetContainer->ClearCurrentProjectObject();
            m_propertiesWidgetContainer->SetCurrentProjectObject(static_cast<ProjectObject*>(m_projectBrowser->currentIndex().internalPointer()));
            m_checkMessageEditor->blockSignals(false);
            m_checkMessageEditor->JustSelected();
        }
        else if (m_checkMessageEditor != NULL)
        {
            m_checkMessageEditor->blockSignals(true);
        }

        //DiagEditor
        if((ui->mainTabWidget->currentWidget()->findChild<DiagEditor*>() == m_diagEditor) && (m_diagEditor != NULL))
        {
            m_propertiesWidgetContainer->ClearCurrentProjectObject();
            m_propertiesWidgetContainer->SetCurrentProjectObject(static_cast<ProjectObject*>(m_projectBrowser->currentIndex().internalPointer()));
            m_diagEditor->blockSignals(false);
        }
        else if (m_diagEditor != NULL)
        {
            m_diagEditor->blockSignals(true);
        }
    });

    /* Helper signals */
    connect(m_settingsHandler, &SettingsHandler::signal_SettingsLoaded, [=] { setupFunctionBlockLibrary(); });
    connect(m_settingsHandler, &SettingsHandler::signal_SettingsSaved, [=] { setupFunctionBlockLibrary(); });
    connect(m_settingsHandler, &SettingsHandler::signal_NoValidFunctionLibraryFound, this, &MainWindow::slot_noValidFunctionLibrary);
    connect(m_settingsHandler, &SettingsHandler::signal_UnavailablePathFound, [=] (const QString& message) { QMessageBox::warning(this,"Paths unavailable!",  message); });

    connect(m_platformGenerator, &PlatformGenerator::signal_NoGeneratorFound, this, &MainWindow::slot_noGeneratorFound);
    connect(m_platformGenerator, &PlatformGenerator::signal_PlatformGeneratorStarting, this, &MainWindow::slot_generatePlatform_starting);
    connect(m_platformGenerator, &PlatformGenerator::signal_PlatformGenertorFinished, this, &MainWindow::slot_generatePlatform_finished);
    connect(m_platformGenerator, &PlatformGenerator::signal_ReadyReadStandardOutput, [=] (const QString& output) { logPlainToolOutput(ui->generateToolOutput, m_generatorLogStream, output); });
    connect(m_platformGenerator, &PlatformGenerator::signal_ReadyReadStandardError, [=] (const QString& error) { logPlainToolOutput(ui->validationOutput, m_issuesLogStream, error); });
    connect(m_platformGenerator, &PlatformGenerator::signal_Error, this, &MainWindow::slot_generatePlatform_error);

    connect(m_platformBuilder, &PlatformBuilder::signal_NoBuilderFound, this, &MainWindow::slot_noBuilderFound);
    connect(m_platformBuilder, &PlatformBuilder::signal_PlatformBuildStarting, this, &MainWindow::slot_buildPlatform_starting);
    connect(m_platformBuilder, &PlatformBuilder::signal_PlatformBuildFinished, this, &MainWindow::slot_buildPlatform_finished);
    connect(m_platformBuilder, &PlatformBuilder::signal_ReadyReadStandardOutput, [=] (const QString& output) { logPlainToolOutput(ui->buildToolOutput, m_builderLogStream, output); });
    connect(m_platformBuilder, &PlatformBuilder::signal_ReadyReadStandardError, [=] (const QString& error) { logPlainToolOutput(ui->validationOutput, m_issuesLogStream, error); });
    connect(m_platformBuilder, &PlatformBuilder::signal_Error, this, &MainWindow::slot_buildPlatform_error);

    connect(m_simulatorRunner, &SimulatorRunner::signal_NoSimulatorFound, this, &MainWindow::slot_noSimulatorFound);
    connect(m_simulatorRunner, &SimulatorRunner::signal_SimulatorStarting, this, &MainWindow::slot_simulation_starting);
    connect(m_simulatorRunner, &SimulatorRunner::signal_SimulatorFinished, [=] (int exitCode)
    {
        if((quint32)exitCode == 0xC0000135L)
        {
            logPlainToolOutput(ui->simulationOutput, m_simulatorLogStream, "Vector CAN driver missing.\n");
        }
        else
        {
            logPlainToolOutput(ui->simulationOutput, m_simulatorLogStream, "PCIC exited with exit code \"" + QString::number((quint32)exitCode) + "\". If this happened during startup make sure the process isn't already running.\n");
        }
    });
    connect(m_simulatorRunner, &SimulatorRunner::signal_ReadyReadStandardOutput, [=] (const QString& output) { logPlainToolOutput(ui->simulationOutput, m_simulatorLogStream, output); });
    connect(m_simulatorRunner, &SimulatorRunner::signal_ReadyReadStandardError, [=] (const QString& error) { logPlainToolOutput(ui->simulationOutput, m_simulatorLogStream, error); });

    connect(m_propertiesWidgetContainer, &PropertiesWidgetContainer::LayoutNotChoosenAnymore, [=] () { m_layoutEditor->LayoutNotChoosenInProjectBrowser(); });
}

void MainWindow::setDisplayEditorWindowsEnabled(bool enabled)
{
    ui->mainTabWidget->setCurrentIndex(0);
    ui->mainTabWidget->setTabEnabled(ui->mainTabWidget->indexOf(ui->LayoutEditorTab), enabled);
    ui->mainTabWidget->setTabEnabled(ui->mainTabWidget->indexOf(ui->LabelEditorTab), enabled);
    ui->mainTabWidget->setTabEnabled(ui->mainTabWidget->indexOf(ui->IconManagerTab), enabled);
    ui->mainTabWidget->setTabEnabled(ui->mainTabWidget->indexOf(ui->MenuDesignerTab), enabled);
    ui->mainTabWidget->setTabEnabled(ui->mainTabWidget->indexOf(ui->CheckMessageTab), enabled);
    if (m_currentProject != NULL)
    {
        if (m_currentProject->GetBSPSpecification().GetDisplay()->GetRenderingType() == BSPDisplay::HW)
        {
            ui->mainTabWidget->setTabEnabled(ui->mainTabWidget->indexOf(ui->ImageManagerTab), enabled);
            ui->mainTabWidget->setTabEnabled(ui->mainTabWidget->indexOf(ui->AnimationEditorTab), enabled);
        }
    }
    else
    {
        ui->mainTabWidget->setTabEnabled(ui->mainTabWidget->indexOf(ui->ImageManagerTab), false);
        ui->mainTabWidget->setTabEnabled(ui->mainTabWidget->indexOf(ui->AnimationEditorTab), false);
    }
}

void MainWindow::setCommonEditorWindowsEnabled(bool enabled)
{
    ui->SignalLayoutTab->setTabEnabled(ui->SignalLayoutTab->indexOf(ui->SinksOverviewTab), enabled);
    ui->SignalLayoutTab->setTabEnabled(ui->SignalLayoutTab->indexOf(ui->SourcesOverviewTab), enabled);
    ui->mainTabWidget->setTabEnabled(ui->mainTabWidget->indexOf(ui->DiagnosticsTab), enabled);
}

void MainWindow::setupFunctionBlockLibrary()
{
    if (m_functionLibraryModel != NULL)
    {
        delete m_functionLibraryModel;
    }

    m_functionLibraryModel = new QFileSystemModel(this);
    m_functionLibraryModel->setReadOnly(true);
    m_functionLibraryModel->setNameFilters(QStringList("*.fdf"));
    m_functionLibraryModel->setNameFilterDisables(false);
    QModelIndex root = m_functionLibraryModel->setRootPath(m_settingsHandler->GetToolSettings().FunctionLibraryPath);

    updateFunctionBlockFileSystemWatcher();

    connect(functionBlockFileSystemWatcher, &QFileSystemWatcher::fileChanged, [=] ()
    {
        functionBlockFileSystemWatcher->blockSignals(true);
        QMessageBox::warning(this, "File change in FunctionLibrary path detected", "One or more file(s) have been changed/modifed/deleted in the FunctionLibrary, it is highly recommended to reload the project!");
        updateFunctionBlockFileSystemWatcher();
        functionBlockFileSystemWatcher->blockSignals(false);
    });

    connect(functionBlockFileSystemWatcher, &QFileSystemWatcher::directoryChanged, [=] ()
    {
        functionBlockFileSystemWatcher->blockSignals(true);
        QMessageBox::warning(this, "Directory change in FunctionLibrary path detected", "One or more directory(s) have been changed/modifed/deleted in the FunctionLibrary, it is highly recommended to reload the project!");
        updateFunctionBlockFileSystemWatcher();
        functionBlockFileSystemWatcher->blockSignals(false);
    });

    if (m_functionLibraryProxyModel != NULL)
    {
        delete m_functionLibraryProxyModel;
    }
    m_functionLibraryProxyModel = new QSortFilterProxyModel(this);
    m_functionLibraryProxyModel->setSourceModel(m_functionLibraryModel);
    QModelIndex proxyRoot = m_functionLibraryProxyModel->mapFromSource(root);


    ui->functionLibraryTree->setModel(m_functionLibraryProxyModel);
    ui->functionLibraryTree->setRootIndex(proxyRoot);

    ui->functionLibraryTree->hideColumn(1);
    ui->functionLibraryTree->hideColumn(2);
    ui->functionLibraryTree->hideColumn(3);

    QItemSelectionModel* selectionModel = ui->functionLibraryTree->selectionModel();
    connect(selectionModel, &QItemSelectionModel::currentChanged, this, &MainWindow::slot_functionLibraryTreeSelectionModel_currentChanged);
}

void MainWindow::updateFunctionBlockFileSystemWatcher()
{
    if (functionBlockFileSystemWatcher->files().count() > 0)
    {
        functionBlockFileSystemWatcher->removePaths(functionBlockFileSystemWatcher->files());
    }

    QStringList pathsToWatch;
    QStringList nameFilters = QStringList() << "*.fdf" << "*.h" << "*.c";

    QDirIterator fileIterator(m_settingsHandler->GetToolSettings().FunctionLibraryPath, nameFilters, QDir::Files, QDirIterator::Subdirectories);
    while (fileIterator.hasNext())
    {
        pathsToWatch << fileIterator.next();
    }

    QDirIterator dirIterator(m_settingsHandler->GetToolSettings().FunctionLibraryPath, QDir::Dirs | QDir::NoDotAndDotDot | QDir::NoSymLinks, QDirIterator::Subdirectories);
    while (dirIterator.hasNext())
    {
        pathsToWatch << dirIterator.next();
    }

    pathsToWatch << m_settingsHandler->GetToolSettings().FunctionLibraryPath;
    functionBlockFileSystemWatcher->addPaths(pathsToWatch);
}

QString MainWindow::getRelativePath(QFileInfo root, QFileInfo path)
{
    return path.absoluteFilePath().replace(root.absoluteFilePath().append("/"), "");
}

bool MainWindow::setupCompiler(void)
{
    if (m_settingsHandler->GetToolSettings().Compilers.count() == 0)
    {
        QMessageBox::information(this, "No compiler configured", "Please setup a compiler in the tool settings before attempting to generate and build!");
        return false;
    }

    TargetSettings& targetSettings = m_currentProject->GetTargetSettings();

    QString compilerName;

    switch (targetSettings.GetActiveTarget())
    {
        case TargetSettings::TargetHW:
            compilerName = targetSettings.GetTargetCompilerName();
            break;
        case TargetSettings::Simulator:
        default:
            compilerName = targetSettings.GetSimulatorCompilerName();
            break;
    }

    if (compilerName.isEmpty())
    {
        QMessageBox::warning(this, "No compiler defined!", "No compiler has been specified for the selected target configuration. Please select a compiler in the Project Settings!");
        return false;
    }

    m_compiler = m_settingsHandler->GetToolSettings().GetCompiler(compilerName);

    if (m_compiler == NULL)
    {
        QMessageBox::information(this, "Compiler not found!", "The configured compiler (" + compilerName + ") is not found. Please select a compiler for the current target in Project Properties and try again");
        return false;
    }

    QList<CompilerBase::CompilerAttribute> compilerAttributes = m_compiler->GetCompilerAttributes();
    QString externalMemoryAddress = "None";
    foreach (CompilerBase::CompilerAttribute attribute, compilerAttributes)
    {
        switch (attribute.Type)
        {
            case CompilerBase::BSPPropertyExternalMemoryAddress:
                externalMemoryAddress = m_compiler->GetCustomAttribute(attribute.Name);
                break;
            default:
                // Do Nothing
                break;
        }
    }

    if ((m_currentProject->GetBSPSpecification().GetMemory() != NULL) && (externalMemoryAddress != "None"))
    {
        quint32 regExternalMemoryAddress = externalMemoryAddress.toULong();
        quint32 bspExternalMemoryAddress = m_currentProject->GetBSPSpecification().GetMemory()->GetExternalMemoryAddress();

        if (regExternalMemoryAddress != bspExternalMemoryAddress)
        {
            m_compiler->SetCustomAttribute("ExternalMemoryAddress", QString::number(bspExternalMemoryAddress));
            ToolSettings& toolSettings = m_settingsHandler->GetToolSettings();
            toolSettings.UpdateCompiler(m_compiler->GetName(), m_compiler->Serialize());
            m_settingsHandler->SaveToolSettings(toolSettings);
        }
    }

    return true;
}

bool MainWindow::currentTargetBinaryExist(void)
{
    bool fileExist = false;
    QString filePath;

    switch (m_currentProject->GetTargetSettings().GetActiveTarget())
    {
        case TargetSettings::TargetHW:
            filePath = m_currentProject->GetOutputFolder().absoluteFilePath("Build/FreescaleMPC5606S.elf");
            break;
        case TargetSettings::Simulator:
        default:
            filePath = m_currentProject->GetOutputFolder().absoluteFilePath("Build/PCIC.exe");
            break;
    }

    QFile targetBinary(filePath);
    if (targetBinary.exists())
    {
        fileExist = true;
    }

    return fileExist;
}

const QDateTime MainWindow::getLastBuildTime(void)
{
    if (!currentTargetBinaryExist()) return QDateTime::fromString("yyyy-mm-dd", "1970-01-01");

    QString filePath;

    switch (m_currentProject->GetTargetSettings().GetActiveTarget())
    {
        case TargetSettings::TargetHW:
            filePath = m_currentProject->GetOutputFolder().absoluteFilePath("Build/FreescaleMPC5606S.elf");
            break;
        case TargetSettings::Simulator:
        default:
            filePath = m_currentProject->GetOutputFolder().absoluteFilePath("Build/PCIC.exe");
            break;
    }

    QFileInfo targetBinary(filePath);
    targetBinary.refresh();
    return targetBinary.lastModified();
}

bool MainWindow::saveNeeded(void)
{
    bool saveNeeded = false;
    if(m_currentProject->GetProjectName() != "")
    {

        DirtyObjectsVisitor dirtFinder;
        m_currentProject->Accept(dirtFinder);
        if (dirtFinder.GetDirtyObjects().size() > 0)
        {
            saveNeeded = true;
        }
    }

    return saveNeeded;
}

void MainWindow::enableGenerateBuildRun()
{
    ui->actionNewProject->setEnabled(true);
    ui->actionOpenProject->setEnabled(true);
    ui->actionReloadProject->setEnabled(true);
    ui->actionCloseProject->setEnabled(true);
    ui->actionQuit->setEnabled(true);

    ui->actionGenerate->setEnabled(true);
    ui->actionBuild->setEnabled(m_buildAllowed);
    ui->actionTargetActive->setEnabled(true);
    ui->actionSimulationActive->setEnabled(true);

    switch (m_currentProject->GetTargetSettings().GetActiveTarget())
    {
        case TargetSettings::TargetHW:
            ui->actionRun->setEnabled(false);
            ui->actionQuickBuild->setEnabled(m_buildAllowed);
            break;
        case TargetSettings::Simulator:
        default:
            ui->actionRun->setEnabled(true);
            ui->actionQuickBuild->setEnabled(false);
            break;
    }
}

void MainWindow::disableGenerateBuildRun()
{
    ui->actionNewProject->setEnabled(false);
    ui->actionOpenProject->setEnabled(false);
    ui->actionReloadProject->setEnabled(false);
    ui->actionCloseProject->setEnabled(false);
    ui->actionQuit->setEnabled(false);

    ui->actionGenerate->setEnabled(false);
    ui->actionBuild->setEnabled(false);
    ui->actionQuickBuild->setEnabled(false);
    ui->actionRun->setEnabled(false);
    ui->actionTargetActive->setEnabled(false);
    ui->actionSimulationActive->setEnabled(false);
}

//* SLOTS *//

/* Menu slots - Begin */

void MainWindow::slot_actionNewProject_triggered()
{
    CreateProjectWizard wizard(this);

    QDialog::DialogCode result = (QDialog::DialogCode)wizard.exec();

    if (result == QDialog::Rejected)
    {
        return;
    }
    else
    {
        QFileInfo newProjectFile(wizard.GetProjectFilePath());

        m_projectController->OpenProject(newProjectFile);
    }
}

void MainWindow::slot_OpenProjectFileSelector()
{
    QString projectFile = QFileDialog::getOpenFileName(this, "Select project file", m_lastProjectRoot, "*.fdt");

    if (!projectFile.isEmpty())
    {
        QFileInfo file(projectFile);
        emit signal_ProjectFileSelected(file);
    }
}

void MainWindow::slot_actionQuit_triggered()
{
    m_projectController->CloseProject();

    if (m_currentProject == NULL)
    {
        this->close();
    }
}

void MainWindow::slot_actionSettings_triggered()
{
    ToolSettings newSettings(m_settingsHandler->GetToolSettings());
    ToolSettingsDialog settingsDialog(newSettings);
    QDialog::DialogCode result = (QDialog::DialogCode) settingsDialog.exec();

    if (result != QDialog::Rejected)
    {
        m_settingsHandler->SaveToolSettings(newSettings);
    }
}

void MainWindow::slot_actionOpenProjectProperties_triggered()
{
    ProjectPropertiesDialog projectPropertiesDialog(m_currentProject,
                                                    m_settingsHandler->GetToolSettings(),
                                                    this);

    projectPropertiesDialog.exec();
}

void MainWindow::slot_actionGenerate_triggered()
{
    clearToolOutput(ui->validationOutput, m_issuesLogFile);

    if (saveNeeded())
    {
        QMessageBox::StandardButton result = QMessageBox::question(this, "Save before generate?", "The project has unsaved changes, do you want to save the project and generate?", QMessageBox::Yes | QMessageBox::Abort, QMessageBox::Abort);

        if (result == QMessageBox::Yes)
        {
            m_projectController->SaveProject();
        }
        else
        {
            return;
        }
    }

//    if (m_platformGenerationNeeded)
//    {
        switch (m_currentProject->GetTargetSettings().GetActiveTarget())
        {
            case TargetSettings::Simulator:
                m_platformGenerator->GenerateSimulatorPlatform(m_settingsHandler->GetToolSettings(), m_currentProject->GetProjectFile().absoluteFilePath());
                break;
            case TargetSettings::TargetHW:
                m_platformGenerator->GenerateTargetPlatform(m_settingsHandler->GetToolSettings(), m_currentProject->GetProjectFile().absoluteFilePath());
                break;
        }
//    }
}

void MainWindow::slot_actionBuild_triggered()
{
    build_triggered(false);
}

void MainWindow::slot_actionQuickBuild_triggered()
{
    build_triggered(true);
}

void MainWindow::build_triggered(bool isQuickBuild)
{
    m_buildPending = false;

    if (!setupCompiler())
    {
        return;
    }

    if (saveNeeded())
    {
        QMessageBox::StandardButton result = QMessageBox::question(this, "Save before build?", "The project has unsaved changes, do you want to save the project and generate?", QMessageBox::Yes | QMessageBox::Abort , QMessageBox::Abort);

        if (result == QMessageBox::Yes)
        {
            m_projectController->SaveProject();
            m_buildPending = true;
            slot_actionGenerate_triggered();
        }

        return;
    }

    if (m_platformGenerationNeeded)
    {
        // TODO: (maybe) ask if we shall generate or abort?
        m_buildPending = true;
        slot_actionGenerate_triggered();

        return;
    }

    if ((getLastBuildTime() >= m_projectController->GetLastSavedTime()) && currentTargetBinaryExist())
    {
        QMessageBox::StandardButton result = QMessageBox::information(this, "Nothing to be done", "Project output already up to date. Do you want to rebuild anyway?", QMessageBox::Yes | QMessageBox::No);

        if (result == QMessageBox::No)
        {
            return;
        }
    }

    m_platformBuilder->BuildPlatform(m_compiler, m_currentProject->GetOutputFolder().absolutePath(), m_settingsHandler->GetToolSettings().BuilderFilePath, isQuickBuild);
}

void MainWindow::slot_actionRun_triggered()
{
    if (saveNeeded())
    {
        QMessageBox::StandardButton result = QMessageBox::question(this, "Save before run?", "The project has unsaved changes, do you want to save the project and generate?", QMessageBox::Yes | QMessageBox::Abort, QMessageBox::Abort);

        if (result == QMessageBox::Yes)
        {
            m_projectController->SaveProject();
            m_runSimulator = true;
            slot_actionGenerate_triggered();
        }

        return;
    }

    if (!currentTargetBinaryExist() || m_platformGenerationNeeded)
    {
        m_runSimulator = true;
        slot_actionGenerate_triggered();

        return;
    }

    m_simulatorRunner->RunSimulator(m_settingsHandler->GetToolSettings().SimulatorFilePath, m_currentProject->GetProjectRoot().absoluteFilePath(m_currentProject->GetBSPSpecificationPath()));
}

void MainWindow::slot_actionAbortBuild_triggered()
{
    emit m_platformBuilder->signal_killBuildProcess();

}

void MainWindow::slot_actionCreateFunctionBlock_triggered()
{
    CreateFunctionBlockWizard wizard(m_settingsHandler->GetToolSettings().FunctionLibraryPath);
    wizard.exec();
}

void MainWindow::setSimulatorAsActive()
{
    disableGenerateBuildRun();

    if (m_currentProject->GetTargetSettings().GetActiveTarget() != TargetSettings::Simulator)
    {
        m_buildAllowed = false;
        m_platformGenerationNeeded = true;
    }

    m_currentProject->GetTargetSettings().SetActiveTarget(TargetSettings::Simulator);
    m_currentProject->GetTargetSettings().SaveSettings(m_currentProject);

    enableGenerateBuildRun();
    ui->actionSimulationActive->setChecked(true);
    ui->actionTargetActive->setChecked(false);
    ui->actionRun->setEnabled(true);
}

void MainWindow::setTargetAsActive()
{
    disableGenerateBuildRun();

    if (m_currentProject->GetTargetSettings().GetActiveTarget() != TargetSettings::TargetHW)
    {
        m_buildAllowed = false;
        m_platformGenerationNeeded = true;
    }

    m_currentProject->GetTargetSettings().SetActiveTarget(TargetSettings::TargetHW);
    m_currentProject->GetTargetSettings().SaveSettings(m_currentProject);

    enableGenerateBuildRun();
    ui->actionSimulationActive->setChecked(false);
    ui->actionTargetActive->setChecked(true);
    ui->actionRun->setEnabled(false);
}

void MainWindow::slot_projectBrowserChanged(ProjectObject* projectObject)
{
    int index = 0;

    if(projectObject->objectName().compare("Diagnosis") == 0)
    {
        index = ui->mainTabWidget->indexOf(ui->DiagnosticsTab);
    }
    else if(projectObject->objectName().compare("Layouts") == 0)
    {
        index = ui->mainTabWidget->indexOf(ui->LayoutEditorTab);
    }
    else if(projectObject->objectName().compare("CheckMessages") == 0)
    {
        index = ui->mainTabWidget->indexOf(ui->CheckMessageTab);
    }
    else if(projectObject->objectName().compare("Icons") == 0)
    {
        index = ui->mainTabWidget->indexOf(ui->IconManagerTab);
    }
    else if(projectObject->objectName().compare("Images") == 0)
    {
        index = ui->mainTabWidget->indexOf(ui->ImageManagerTab);
    }
    else if(projectObject->objectName().compare("Animations") == 0)
    {
        index = ui->mainTabWidget->indexOf(ui->AnimationEditorTab);
    }
    else if(projectObject->objectName().compare("Labels") == 0)
    {
        index = ui->mainTabWidget->indexOf(ui->LabelEditorTab);
    }

    if(index != 0)
    {
        ui->mainTabWidget->setCurrentIndex(index);
    }
}

void MainWindow::saveWindowStateAndGeometry()
{
    QSettings settings;
    settings.setValue("WindowGeometry", saveGeometry());
    settings.setValue("WindowState", saveState());
}

void MainWindow::RestoreWindowStateAndGeometry()
{
    show();
    QSettings settings;
    restoreGeometry(settings.value("WindowGeometry", geometry()).toByteArray());
    restoreState(settings.value("WindowState").toByteArray());
}

void MainWindow::logPlainToolOutput(QPlainTextEdit* plainTextEdit, QTextStream* logStream, QString msg)
{
    plainTextEdit->appendPlainText(msg);
    if (logStream != NULL && logStream->device() != NULL)
    {
        (*logStream) << QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss.zzz") << " " << msg << endl;
    }
}

void MainWindow::logHtmlToolOutput(QPlainTextEdit* plainTextEdit, QTextStream* logStream, QString msg)
{
    plainTextEdit->appendHtml(msg);
    if (logStream != NULL && logStream->device() != NULL)
    {
        (*logStream) << QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss.zzz") << " " << msg << endl;
    }
}

void MainWindow::clearToolOutput(QPlainTextEdit* plainTextEdit, QFile* logFile)
{
    plainTextEdit->clear();

    if(logFile != NULL && logFile->exists())
    {
        if (logFile->isOpen())
        {
            logFile->close();
        }

        logFile->remove();
        logFile->open(QFile::WriteOnly);
    }
}

void MainWindow::setupLogging(QTextStream** logStream, QFile** logFile, QString filePath)
{
    *logFile = new QFile(filePath);

    if (!(*logFile)->open(QFile::WriteOnly))
    {
        delete *logFile;
        *logFile = NULL;
    }

    if ((*logFile) != NULL && (*logFile)->isOpen())
    {
        *logStream = new QTextStream(*logFile);
    }
}

void MainWindow::slot_schedulerAnalysis_triggered()
{
    if (m_currentProject == NULL)
    {
        qWarning("%s m_currentProject == NULL", __FUNCTION__);
        return;
    }

    SchedulerAnalysis analysis(*m_currentProject);
    analysis.exec();
}

/* Menu slots - End */

/* Generate/Build/Run slots - Begin */

void MainWindow::slot_generatePlatform_starting(QString program, QStringList arguments)
{
    m_simulatorRunner->KillSimulatorProcesses();

    clearToolOutput(ui->generateToolOutput, m_generatorLogFile);
    logPlainToolOutput(ui->generateToolOutput, m_generatorLogStream, program + " " + arguments.join(" ") + "\n");
    ui->toolOutputTabs->setCurrentWidget(ui->generateToolOutputTab);

    disableGenerateBuildRun();
}

void MainWindow::slot_generatePlatform_finished(int exitCode)
{
    if (exitCode != 0)
    {
        logHtmlToolOutput(ui->generateToolOutput, m_generatorLogStream, "<font color=\"#FF0000\">FAILED!</font>");
        logHtmlToolOutput(ui->validationOutput, m_issuesLogStream, "<font color=\"#FF0000\">FAILED!</font>");

        QMessageBox::warning(this, "Platform generator failed!", "The platform generator process failed with exit code: " + QString::number(exitCode) + "\nSee the Issues tab for more information");
        ui->toolOutputTabs->setCurrentWidget(ui->validationOutputTab);

        m_buildAllowed = false;
        m_platformGenerationNeeded = true;
    }
    else
    {
        logHtmlToolOutput(ui->generateToolOutput, m_generatorLogStream, "<font color=\"#33AA33\">DONE!</font>");

        m_buildAllowed = true;
        m_platformGenerationNeeded = false;
    }

    if ((m_buildAllowed && m_runSimulator) || (m_buildAllowed && m_buildPending))
    {
        slot_actionBuild_triggered();
    }

    enableGenerateBuildRun();
}

void MainWindow::slot_generatePlatform_error(QString error)
{
    QMessageBox::warning(this, "Platform generation failed!", "The platform generator failed to execute: " + error);
    enableGenerateBuildRun();
}

void MainWindow::slot_noGeneratorFound()
{
    QString filepath = QFileDialog::getOpenFileName(this, "Provide path to platformgenerator.exe", QDir::currentPath(), "platformgenerator.exe");
    if (filepath.isEmpty()) return;
    m_settingsHandler->GetToolSettings().PlatformGeneratorFilePath = filepath;
    m_settingsHandler->SaveToolSettings(m_settingsHandler->GetToolSettings());
}

void MainWindow::slot_noBuilderFound()
{
    QString filepath = QFileDialog::getOpenFileName(this, "Provide path to builder.exe", QDir::currentPath(), "builder.exe");
    if (filepath.isEmpty()) return;
    m_settingsHandler->GetToolSettings().BuilderFilePath = filepath;
    m_settingsHandler->SaveToolSettings(m_settingsHandler->GetToolSettings());
}

void MainWindow::slot_noSimulatorFound()
{
    QString filepath = QFileDialog::getOpenFileName(this, "Provide path to simulator.exe", QDir::currentPath(), "simulator.exe");
    if (filepath.isEmpty()) return;
    m_settingsHandler->GetToolSettings().SimulatorFilePath = filepath;
    m_settingsHandler->SaveToolSettings(m_settingsHandler->GetToolSettings());
}

void MainWindow::slot_buildPlatform_starting(QString program, QStringList arguments)
{
    ui->abortButton->setEnabled(true);

    m_simulatorRunner->KillSimulatorProcesses();

    clearToolOutput(ui->buildToolOutput, m_builderLogFile);
    logPlainToolOutput(ui->buildToolOutput, m_builderLogStream, program + " " + arguments.join(" ") + "\n");
    ui->toolOutputTabs->setCurrentWidget(ui->buildToolOutputTab);

    disableGenerateBuildRun();
}

void MainWindow::slot_buildPlatform_finished(int exitCode)
{
    ui->abortButton->setEnabled(false);

    QStringList outputErrors;
    if ((m_compiler != NULL) && (m_compiler->ValidateBuildOutput(ui->buildToolOutput->toPlainText(), outputErrors) != true))
    {
        logPlainToolOutput(ui->validationOutput, m_issuesLogStream, outputErrors.join("/n"));
        exitCode = 0xFF;
    }

    if (exitCode != 0)
    {
        logHtmlToolOutput(ui->buildToolOutput, m_builderLogStream, "<font color=\"#FF0000\">FAILED!</font>");
        logHtmlToolOutput(ui->validationOutput, m_issuesLogStream, "<font color=\"#FF0000\">FAILED!</font>");

        ui->toolOutputTabs->setCurrentWidget(ui->validationOutputTab);
    }
    else
    {
        logHtmlToolOutput(ui->buildToolOutput, m_builderLogStream, "<font color=\"#33AA33\">DONE!</font>");

        if (m_runSimulator)
        {
            m_runSimulator = false;
            m_simulatorRunner->RunSimulator(m_settingsHandler->GetToolSettings().SimulatorFilePath, m_currentProject->GetProjectRoot().absoluteFilePath(m_currentProject->GetBSPSpecificationPath()));
        }
    }

    m_compiler = NULL;
    enableGenerateBuildRun();
}

void MainWindow::slot_buildPlatform_error(QString error)
{
    ui->abortButton->setEnabled(false);

    QMessageBox::warning(this, "Platform build failed!", "The platform compiler failed to execute: " + error);
    enableGenerateBuildRun();
}

void MainWindow::slot_simulation_starting(QString program, QStringList arguments)
{
    clearToolOutput(ui->simulationOutput, m_simulatorLogFile);
    logPlainToolOutput(ui->simulationOutput, m_simulatorLogStream, program + " " + arguments.join(" ") + "\n");
    ui->toolOutputTabs->setCurrentWidget(ui->simulationOutputTab);
}

/* Generate and builed auxiliary slots - End */

/* Helper slots - Begin */
void MainWindow::slot_projectClosed()
{
    setWindowTitle( QCoreApplication::applicationName() + " "  + QCoreApplication::applicationVersion() + " - No project loaded");
    m_currentProject = NULL;

    UIWidgetsRepository::ClearUIWidgets();

    ui->generateToolOutput->clear();
    ui->buildToolOutput->clear();
    ui->validationOutput->clear();
    ui->simulationOutput->clear();

    setDisplayEditorWindowsEnabled(false);
    setCommonEditorWindowsEnabled(false);

    ui->menuProject->setEnabled(false);

    ui->actionSchedulerAnalysis->setEnabled(false);
    ui->addFunctionBlockButton->setEnabled(false);

    if (m_uiWidgetsModel != NULL)
    {
        delete m_uiWidgetsModel;
        m_uiWidgetsModel = NULL;
    }
    UIWidgetBase::ResetWidgetIDCounter();
    Animation::ResetAnimationCount();

    disableGenerateBuildRun();

    ui->actionNewProject->setEnabled(true);
    ui->actionOpenProject->setEnabled(true);
    ui->actionQuit->setEnabled(true);
}

void MainWindow::slot_projectLoaded(Project* project)
{
    m_currentProject = project;
    m_lastProjectRoot = m_currentProject->GetProjectRoot().absolutePath();
    setWindowTitle( QCoreApplication::applicationName() + " "  + QCoreApplication::applicationVersion() + " - " + project->GetProjectFile().absoluteFilePath());
    m_currentProject->blockSignals(true);

    setupDisplayEnvironment(m_currentProject->GetBSPSpecification().HasDisplay());

    setDisplayEditorWindowsEnabled(m_currentProject->GetBSPSpecification().HasDisplay());
    setCommonEditorWindowsEnabled(true);

    ui->actionSaveProject->setEnabled(false);
    ui->actionCloseProject->setEnabled(true);
    ui->actionReloadProject->setEnabled(true);
    ui->actionSchedulerAnalysis->setEnabled(true);

    ui->menuProject->setEnabled(true);

    switch (m_currentProject->GetTargetSettings().GetActiveTarget())
    {
        case TargetSettings::Simulator:
            setSimulatorAsActive();
            break;
        case TargetSettings::TargetHW:
            setTargetAsActive();
            break;
        default:
            setSimulatorAsActive();
    }

    enableGenerateBuildRun();
    QItemSelectionModel* selectionModel = ui->functionLibraryTree->selectionModel();
    slot_functionLibraryTreeSelectionModel_currentChanged(selectionModel->currentIndex(), QModelIndex());

    connect(m_currentProject, &Project::PropertyUpdated, [=]
    {
        if (!windowTitle().endsWith("*"))
        {
            setWindowTitle(windowTitle() + " *");
        }
        ui->actionSaveProject->setEnabled(true);
        m_platformGenerationNeeded = true;
    });

    m_simulatorRunner->SetOutputPath(m_currentProject->GetOutputFolder().absolutePath());
    m_currentProject->blockSignals(false);
}

void MainWindow::slot_noValidFunctionLibrary()
{
    QString newFunctionLibraryPath = "";
    while (newFunctionLibraryPath.isEmpty() || !QDir(newFunctionLibraryPath).exists())
    {
        newFunctionLibraryPath = QFileDialog::getExistingDirectory(this, "Set Function Library Path");
    }

    if (!newFunctionLibraryPath.isEmpty() && QDir(newFunctionLibraryPath).exists())
    {
            QSettings settings;
            m_settingsHandler->SaveFunctionLibraryPath(settings, newFunctionLibraryPath);
    }

    m_settingsHandler->LoadToolSettings();
}

/* Helper slots - End */

/* Other slots - Begin */
void MainWindow::slot_addFunctionBlockButton_clicked()
{
    if (ui->functionLibraryTree->selectionModel()->selectedIndexes().count() == 0 &&
        m_selectedFunctionBlock == NULL)
    {
        return;
    }

    m_currentProject->AddFunctionBlock(m_selectedFunctionBlock);
    ui->addFunctionBlockButton->setEnabled(false);
}

void MainWindow::slot_editFunctionBlockButton_clicked()
{
    if (ui->functionLibraryTree->selectionModel()->selectedIndexes().count() == 0)
    {
        return;
    }

    QFileInfo selectedFdfFile(m_selectedFunctionLibraryPath);

    QStringList nameFilters = QStringList() << "*.fdf" << "*.h" << "*.c";
    QStringList files;
    QDirIterator it(selectedFdfFile.absolutePath(), nameFilters, QDir::Files, QDirIterator::Subdirectories);
    while (it.hasNext())
    {
        files << it.next();
    }

    QMessageBox::warning(this, "Warning, project reload needed", "The project MUST be reloaded if any changes are made to a function block while FDT is running!");

    foreach (QString file, files)
    {
        QDesktopServices::openUrl(QUrl::fromLocalFile(file));
    }

    return;
}

void MainWindow::slot_functionLibraryTreeSelectionModel_currentChanged(QModelIndex current, QModelIndex previous)
{
    Q_UNUSED(previous);

    const QModelIndex selectedSourceIndex = m_functionLibraryProxyModel->mapToSource(current);
    m_selectedFunctionLibraryPath = this->m_functionLibraryModel->filePath(selectedSourceIndex);
    QFileInfo selectedFile(m_selectedFunctionLibraryPath);

    if (m_currentProject != NULL)
    {
        bool isBlockNotInProject = true;

        FunctionBlock* block = new FunctionBlock();
        QDir functionLibraryDir(m_settingsHandler->GetToolSettings().FunctionLibraryPath);
        block->SetPathInFunctionLibrary(getRelativePath(QFileInfo(m_settingsHandler->GetToolSettings().FunctionLibraryPath), selectedFile));
        try
        {
            block->Initialize(functionLibraryDir);
            m_selectedFunctionBlock = block;
            isBlockNotInProject &= !(m_currentProject->IsFunctionBlock(block->GetComponentName()));
        }
        catch (CannotOpenFileException& e)
        {
            isBlockNotInProject = false;
            delete block;
            block = NULL;
        }

        bool isValidSelectionItem = m_selectedFunctionLibraryPath.endsWith(".fdf");
        ui->addFunctionBlockButton->setEnabled(isValidSelectionItem && isBlockNotInProject);
        ui->editFunctionBlockButton->setEnabled(isValidSelectionItem);
    }
    else
    {
        bool isValidSelectionItem = m_selectedFunctionLibraryPath.endsWith(".fdf");
        ui->addFunctionBlockButton->setEnabled(false);
        ui->editFunctionBlockButton->setEnabled(isValidSelectionItem);
    }
}

void MainWindow::slot_ShowCaughtError(QStringList errorMessages)
{
    QMessageBox::warning(NULL, "Error", errorMessages.join('\n'), QMessageBox::Ok);
}

/* Other slots - End */

void MainWindow::closeEvent(QCloseEvent* closeEvent)
{
    if (m_currentProject != NULL)
    {
        m_projectController->CloseProject();
    }

    if (m_currentProject == NULL)
    {
        closeEvent->accept();
    }
    else
    {
        closeEvent->ignore();
    }
}

