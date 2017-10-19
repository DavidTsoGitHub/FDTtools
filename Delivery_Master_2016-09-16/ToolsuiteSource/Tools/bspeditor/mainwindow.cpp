#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "limits"
#include "memory"

#include <QAbstractMessageHandler>
#include <QCloseEvent>
#include <QtConcurrent/QtConcurrent>
#include <QDomDocument>
#include <QFileDialog>
#include <QMessageBox>
#include <QSettings>
#include <QTextStream>

#include "bspcustomdelegate.h"
#include "bsprules.h"
#include "bspschemarules.h"
#include "bsptreefilter.h"
#include "propertiesmodel.h"
#include "settingsdialog.h"
#include "treenavigatormodel.h"

class XmlMessageHandler : public QAbstractMessageHandler
{
public:
    explicit XmlMessageHandler(QObject *parent) : QAbstractMessageHandler(parent) {}

    QString description() const
    {
        return m_description;
    }

protected:
    virtual void handleMessage(QtMsgType type, const QString &description,
                               const QUrl &identifier, const QSourceLocation &sourceLocation)
    {
        Q_UNUSED(type);
        Q_UNUSED(identifier);
        Q_UNUSED(sourceLocation);
        m_description = description;
    }

private:
    QString m_description;
};

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    m_ui(new Ui::MainWindow)
{
    init();
}

MainWindow::~MainWindow()
{
    delete m_ui;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (maybeSave())
    {
        // Remember window pos/size
        QSettings settings;
        settings.setValue("windowPos", pos());
        settings.setValue("windowSize", size());

        // Proceed and exit
        event->accept();
        return;
    }

    event->ignore();
}

void MainWindow::newFile()
{
    if(isWindowModified())
        if(!maybeSave())
            return;

    QFile file(":/xml/new_spec_template.xml");
    if (!file.open(QFile::ReadOnly))
        return;

    m_spec.setContent(&file);
    file.close();

    m_treeModel->populate(m_spec);
    m_ui->treeView->expand(m_treeModel->index(0, 0, QModelIndex()));
    m_propertiesModel->clearNodes();

    statusBar()->showMessage(tr("New spec created"), 2000);
    setCurrentFile("");
    validate();
}

void MainWindow::open()
{
    if(isWindowModified())
        if(!maybeSave())
            return;

    QString defaultPath = "";
    if(m_openRecentActions[0]->isVisible())
        defaultPath = QFileInfo(m_openRecentActions[0]->data().toString()).canonicalPath();

    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Open BSP spec"),
                                                    defaultPath,
                                                    tr("BSP spec (*.xml)"));
    if(fileName.isEmpty())
        return;

    loadFile(fileName);
}

void MainWindow::openRecent()
{
    QAction *action = qobject_cast<QAction*>(sender());
    if (action)
        loadFile(action->data().toString());
}

bool MainWindow::save()
{
    if (m_isUntitled)
        return saveAs();

    return saveFile(m_currentFileName);
}

bool MainWindow::saveAs()
{
    QString fileName = QFileDialog::getSaveFileName(this,
                                                    tr("Save BSP spec as"),
                                                    m_currentFileName);
    if (fileName.isEmpty())
        return false;

    return saveFile(fileName);
}

void MainWindow::showSettings()
{
    m_settingsDialog->exec();
    loadSchema();
    validate();
}

void MainWindow::about()
{
   QMessageBox::about(this, tr("About ") + QCoreApplication::applicationName(),
                      tr("BSP Editor\n\n") + \
                         tr("Application Version") + ": " + QCoreApplication::applicationVersion() + \
                         "\n" + tr("Build Date") + ": " + QString(APP_BUILD_DATE) + \
                         "\n" + tr("Build") + ": " + QString(APP_BUILD) + \
                         "\n\nCopyright © 2015-2016 Swedspot AB");
}

void MainWindow::specWasModified()
{
    setWindowModified(true);
    validate();
    m_ui->tableView->resizeColumnsToContents();
}

void MainWindow::loadSchema()
{
    QString filePath = m_settingsDialog->useDefaultSchema() ?
                ":/xml/default.xsd" :
                m_settingsDialog->customSchema();

    QFile file(filePath);
    if(!file.open(QIODevice::ReadOnly))
    {
        setStatusMessage("Can't open schema file " + filePath, true);
        return;
    }

    if(!m_schema.load(file.readAll()))
    {
        setStatusMessage("Can't load schema. Check that " + filePath +
                         " is a valid W3C XML Schema 1.0 file. Error: " +
                         m_xmlMsgHandler->description(), true);
        return;
    }

    m_validator.setSchema(m_schema);
    file.reset();
    m_schemaAsXml.setContent(file.readAll());
    file.close();

    setStatusMessage("Schema loaded.", false);
}

void MainWindow::setStatusMessage(const QString &msg, bool error)
{
    QString color = error ? QColor(Qt::red).lighter(160).name() : QColor(Qt::white).name();
    const QString styleSheet = QString("QTextEdit {background: %1}").arg(color);
    m_ui->textEditStatus->setStyleSheet(styleSheet);
    m_ui->textEditStatus->setText(msg);
}

void MainWindow::validate()
{
    if(!m_schema.isValid())
        return;

    // Async XML Schema validation
    // QXmlSchemaValidator and QDomDocument are both reentrant
    QFuture<bool> future = QtConcurrent::run([&](){
        return m_validator.validate(m_spec.toByteArray());
    });
    // Update status message when finished
    m_validationWatcher.setFuture(future);
}

void MainWindow::refreshTree()
{
    QStringList list;
    auto indices = m_treeModel->getPersistentModelIndices();
    foreach (QModelIndex index, indices) {
        if (m_ui->treeView->isExpanded(index)) {
            list << index.data(Qt::DisplayRole).toString();
        }
    }

    m_treeModel->populate(m_spec);

    foreach (QString item, list) {
        QModelIndexList items = m_treeModel->match(m_treeModel->index(0, 0),
            Qt::DisplayRole, QVariant::fromValue(item), 1, Qt::MatchRecursive);
        if(items.isEmpty())
            continue;

        m_ui->treeView->setExpanded(items.first(), true);
    }
}

void MainWindow::refreshTable(QDomNode node)
{
    m_propertiesModel->clearNodes();
    m_propertiesModel->addElement(node.toElement());
    m_ui->tableView->resizeColumnsToContents();
}

void MainWindow::init()
{
    m_ui->setupUi(this);
    m_settingsDialog = new SettingsDialog(this);
    m_isUntitled = true;

    // Resize according to settings
    QSettings settings;
    QPoint pos = settings.value("windowPos", QPoint(200, 200)).toPoint();
    QSize size = settings.value("windowSize", QSize(640, 480)).toSize();
    move(pos);
    resize(size);

    // Splitter ratio
    m_ui->splitter->setStretchFactor(1, 3);

    // Xml validation setup
    m_xmlMsgHandler = new XmlMessageHandler(this);
    m_schema.setMessageHandler(m_xmlMsgHandler);
    m_validator.setMessageHandler(m_xmlMsgHandler);
    connect(&m_validationWatcher, &QFutureWatcher<bool>::finished, [&]()
    {
        bool ok = m_validationWatcher.future().result();
        setStatusMessage(ok ? "Ok" : m_xmlMsgHandler->description(), !ok);
    });

    loadSchema();

    // Properties table
    m_propertiesModel = new PropertiesModel(m_schemaAsXml);
    connect(m_propertiesModel, &PropertiesModel::dataChanged, this, &MainWindow::specWasModified);
    m_ui->tableView->setModel(m_propertiesModel);
    m_ui->tableView->setSortingEnabled(true);
    m_ui->tableView->setAlternatingRowColors(true);
    m_ui->tableView->verticalHeader()->hide();
    m_ui->tableView->setContextMenuPolicy(Qt::CustomContextMenu);

    // Navigation tree
    m_treeModel = new TreeNavigatorModel(std::unique_ptr<TreeNavigatorFilter>(new BSPTreeFilter()));
    m_ui->treeView->setModel(m_treeModel);
    m_ui->treeView->setAlternatingRowColors(true);
    m_ui->treeView->header()->hide();
    m_ui->treeView->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(m_ui->treeView, &QTreeView::clicked, [&](const QModelIndex &index){
        refreshTable(m_treeModel->node(index));
    });

    createActions();
    createMenus();
    createStatusBar();

    // Automatically open recent file
    if(m_openRecentActions[0]->isVisible() && m_settingsDialog->autoOpenRecent())
        loadFile(m_openRecentActions[0]->data().toString());
    // Create new file
    else
        newFile();

    // Install custom delegate when spec is loaded
    m_bspCustomDelegate = new BspCustomDelegate(m_schemaAsXml, m_spec, this);
    m_ui->tableView->setItemDelegate(m_bspCustomDelegate);
}

void MainWindow::createActions()
{
    m_newAction = new QAction(QIcon(":/images/new.png"), tr("&New"), this);
    m_newAction->setShortcuts(QKeySequence::New);
    m_newAction->setStatusTip(tr("Create a new BSP spec"));
    connect(m_newAction, &QAction::triggered, this, &MainWindow::newFile);

    m_openAction = new QAction(QIcon(":/images/open.png"), tr("&Open..."), this);
    m_openAction->setShortcuts(QKeySequence::Open);
    m_openAction->setStatusTip(tr("Open an existing BSP spec"));
    connect(m_openAction, &QAction::triggered, this, &MainWindow::open);

    m_saveAction = new QAction(QIcon(":/images/save.png"), tr("&Save"), this);
    m_saveAction->setShortcuts(QKeySequence::Save);
    m_saveAction->setStatusTip(tr("Save the BSP spec to disk"));
    connect(m_saveAction, &QAction::triggered, this, &MainWindow::save);

    m_saveAsAction = new QAction(tr("Save &As..."), this);
    m_saveAsAction->setShortcuts(QKeySequence::SaveAs);
    m_saveAsAction->setStatusTip(tr("Save the BSP spec under a new name"));
    connect(m_saveAsAction, &QAction::triggered, this, &MainWindow::saveAs);

    m_settingsAction = new QAction(tr("Settings..."), this);
    m_settingsAction->setShortcuts(QKeySequence::Preferences);
    m_settingsAction->setStatusTip(tr("Open settings dialog"));
    connect(m_settingsAction, &QAction::triggered, this, &MainWindow::showSettings);

    for (int i = 0; i < MaxRecent; ++i) {
        m_openRecentActions[i] = new QAction(this);
        m_openRecentActions[i]->setVisible(false);
        connect(m_openRecentActions[i], &QAction::triggered, this, &MainWindow::openRecent);
    }

    m_exitAction = new QAction(tr("E&xit"), this);
    m_exitAction->setShortcuts(QKeySequence::Quit);
    m_exitAction->setStatusTip(tr("Exit the application"));
    connect(m_exitAction, &QAction::triggered, qApp, &QApplication::closeAllWindows);

    m_aboutAction = new QAction(tr("&About"), this);
    m_aboutAction->setStatusTip(tr("Show the application's About box"));
    connect(m_aboutAction, &QAction::triggered, this, &MainWindow::about);
}

void MainWindow::createMenus()
{
    // File menu
    m_fileMenu = menuBar()->addMenu(tr("&File"));
    m_fileMenu->addAction(m_newAction);
    m_fileMenu->addAction(m_openAction);
    m_fileMenu->addAction(m_saveAction);
    m_fileMenu->addAction(m_saveAsAction);
    m_fileMenu->addSeparator();

    m_fileMenu->addSeparator();
    for (int i = 0; i < MaxRecent; ++i)
        m_fileMenu->addAction(m_openRecentActions[i]);
    m_fileMenu->addSeparator();
    updateRecentActions();

    m_fileMenu->addAction(m_settingsAction);
    m_fileMenu->addSeparator();
    m_fileMenu->addAction(m_exitAction);

    // Help menu
    menuBar()->addSeparator();
    m_helpMenu = menuBar()->addMenu(tr("&Help"));
    m_helpMenu->addAction(m_aboutAction);

    // Context menu for Tree Navigator
    connect(m_ui->treeView, &QWidget::customContextMenuRequested, [&](const QPoint& pos)
    {
        QMenu menu;
        QDomNode node = m_treeModel->node(m_ui->treeView->currentIndex());

        // Actions for appending child elements
        auto allowedToBeAdded = BspRules::elementsAllowedToBeAppended(node);
        foreach(auto toAdd, allowedToBeAdded)
        {
            auto action = menu.addAction(tr("Add ") + BspRules::nodeName(toAdd));
            connect(action, &QAction::triggered, [toAdd, &node, this]()
            {
                auto newElement = BspSchemaRules::createElementUsingSchema(m_spec, m_schemaAsXml, toAdd);
                node.appendChild(newElement);
                refreshTree();
                refreshTable(node);

                // Select new row when editing in table
                if(BspRules::howToEditElement(node) == BspRules::ElementEditHint::ChildrenInTable)
                {
                    m_ui->tableView->scrollToBottom();
                    auto lastRowIndex = m_propertiesModel->index(m_propertiesModel->rowCount() -1, 0);
                    m_ui->tableView->selectionModel()->setCurrentIndex(lastRowIndex, QItemSelectionModel::SelectCurrent);
                    m_ui->tableView->edit(lastRowIndex);
                }

                statusBar()->showMessage(tr("New element created"), 2000);
                specWasModified();
            });

            // Disable action if no more elements allowed according to schema
            if(!node.toElement().isNull())
            {
                auto numExisting = node.toElement().elementsByTagName(toAdd).size();
                if(numExisting >= BspSchemaRules::numElementsAllowedUsingSchema(m_schemaAsXml, toAdd))
                    action->setEnabled(false);
            }
        }

        // Delete element action
        bool allowedToBeRemoved = BspRules::elementAllowedToBeRemoved(node);
        if(allowedToBeRemoved)
        {
            if(allowedToBeAdded.size() > 0)
                menu.addSeparator();

            auto action = menu.addAction("Delete " + BspRules::nodeName(node));
            connect(action, &QAction::triggered, [&node, this]()
            {
                node.parentNode().removeChild(node);
                refreshTree();
                refreshTable(QDomNode());
                statusBar()->showMessage(BspRules::nodeName(node) + " deleted", 2000);
                specWasModified();
            });
        }

        if(menu.actions().size() > 0)
            menu.exec(m_ui->treeView->mapToGlobal(pos));
    });

    // Context menu for Properties table
    connect(m_ui->tableView, &QWidget::customContextMenuRequested, [&](const QPoint& pos)
    {
        QMenu menu;
        auto currentIndex = m_ui->tableView->currentIndex();
        auto currentRow = currentIndex.row();
        auto node = m_propertiesModel->rowNode(currentIndex);

        // Insert row action
        bool allowedToInsert = !node.isNull();
        if(allowedToInsert)
        {
            auto insertAction = menu.addAction("Insert row");
            connect(insertAction, &QAction::triggered, [&node, currentRow, this]()
            {
                auto firstElement = m_propertiesModel->rowNode(m_propertiesModel->index(0, 0));
                auto newElement = BspSchemaRules::createElementUsingSchema(m_spec, m_schemaAsXml, firstElement.nodeName());
                node.parentNode().insertBefore(newElement, node);
                refreshTree();
                refreshTable(node.parentNode());
                auto newRowIndex = m_propertiesModel->index(currentRow, 0);
                m_ui->tableView->scrollTo(newRowIndex);
                m_ui->tableView->selectionModel()->setCurrentIndex(newRowIndex, QItemSelectionModel::SelectCurrent);
                m_ui->tableView->edit(newRowIndex);
                statusBar()->showMessage(tr("New element created"), 2000);
                specWasModified();
            });

            menu.addSeparator();
        }

        // Delete row action
        bool allowedToBeRemoved = BspRules::elementAllowedToBeRemoved(node);
        if(allowedToBeRemoved)
        {
            auto action = menu.addAction("Delete row");
            connect(action, &QAction::triggered, [&node, this]()
            {
                auto parent = node.parentNode();
                parent.removeChild(node);
                refreshTree();
                refreshTable(parent);
                statusBar()->showMessage(tr("Row deleted"), 2000);
                specWasModified();
            });
        }

        if(menu.actions().size() > 0)
            menu.exec(m_ui->tableView->mapToGlobal(pos));
    });
}

void MainWindow::createStatusBar()
{
    statusBar()->showMessage(tr("Ready"));
}

bool MainWindow::maybeSave()
{
    if (isWindowModified()) {
        QMessageBox::StandardButton ret;
        ret = QMessageBox::warning(this, QCoreApplication::applicationName(),
                     tr("The BSP spec has been modified.\n"
                        "Do you want to save your changes?"),
                     QMessageBox::Save | QMessageBox::Discard
                     | QMessageBox::Cancel);
        if (ret == QMessageBox::Save)
            return save();
        else if (ret == QMessageBox::Cancel)
            return false;
    }
    return true;
}

void MainWindow::loadFile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly)) {
        QMessageBox::warning(this, QCoreApplication::applicationName(),
                             tr("Cannot read file %1:\n%2.")
                             .arg(fileName)
                             .arg(file.errorString()));
        return;
    }

    QApplication::setOverrideCursor(Qt::WaitCursor);
    m_spec.setContent(&file);
    file.close();

    m_treeModel->populate(m_spec);
    m_ui->treeView->expand(m_treeModel->index(0, 0, QModelIndex()));
    m_propertiesModel->clearNodes();

    // Update recent file list in settings
    QSettings settings;
    QStringList files = settings.value("recentFileList").toStringList();
    files.removeAll(fileName);

    if(QFileInfo(fileName).exists())
        files.prepend(fileName);
    while (files.size() > MaxRecent)
        files.removeLast();

    settings.setValue("recentFileList", files);
    updateRecentActions();

    setCurrentFile(fileName);
    QApplication::restoreOverrideCursor();
    statusBar()->showMessage(tr("File loaded"), 2000);

    // Run validation
    validate();
}

bool MainWindow::saveFile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QFile::WriteOnly)) {
        QMessageBox::warning(this, QCoreApplication::applicationName(),
                             tr("Can't write file %1:\n%2.")
                             .arg(fileName)
                             .arg(file.errorString()));
        return false;
    }

    QTextStream out(&file);
    QApplication::setOverrideCursor(Qt::WaitCursor);
    m_spec.save(out, 2);
    file.close();
    QApplication::restoreOverrideCursor();

    setCurrentFile(fileName);
    statusBar()->showMessage(tr("Spec saved"), 2000);
    return true;
}

void MainWindow::setCurrentFile(const QString &fileName)
{
    m_isUntitled = fileName.isEmpty();
    if (m_isUntitled) {
        m_currentFileName = tr("new_spec.xml");
    } else {
        m_currentFileName = QFileInfo(fileName).canonicalFilePath();
    }

    setWindowTitle(m_currentFileName + "[*] - " + QCoreApplication::applicationName());
    setWindowModified(false);
}

void MainWindow::updateRecentActions()
{
    QSettings settings;
    QStringList files = settings.value("recentFileList").toStringList();
    QStringList::Iterator it = files.begin();
    while(it != files.end())
        it = QFileInfo(*it).exists() ? it + 1 : files.erase(it);

    int numRecentFiles = qMin(files.size(), (int)MaxRecent);
    for (int i = 0; i < numRecentFiles; ++i) {
        m_openRecentActions[i]->setText(QFileInfo(files[i]).fileName());
        m_openRecentActions[i]->setData(files[i]);
        m_openRecentActions[i]->setStatusTip("Open " + files[i]);
        m_openRecentActions[i]->setVisible(true);
    }
    for (int j = numRecentFiles; j < MaxRecent; ++j)
        m_openRecentActions[j]->setVisible(false);
}
