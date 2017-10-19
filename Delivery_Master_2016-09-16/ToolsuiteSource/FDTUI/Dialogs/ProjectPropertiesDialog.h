#ifndef PROJECTPROPERTIESDIALOG_H
#define PROJECTPROPERTIESDIALOG_H

#include "TargetSettings.h"

class CompilerBase;
class Project;
class ToolSettings;
class QComboBox;
class QDialog;
class QFileInfo;
class QListView;
class QString;
class QWidget;

namespace Ui {
class ProjectPropertiesDialog;
}

class ProjectPropertiesDialog : public QDialog
{
    Q_OBJECT

public:
    ProjectPropertiesDialog(Project* project, ToolSettings& toolSettings, QWidget *parent = 0);
    ~ProjectPropertiesDialog();

private:
    Ui::ProjectPropertiesDialog* m_ui;
    Project* m_project;
    TargetSettings m_targetSettings;

    QFileInfo m_newCANFile;

    void populateCompilerComboBox(QList<CompilerBase*>& compilers, QComboBox* combobox, QString currentCompiler);
    void populateEndianessComboBox(QComboBox* combobox, TargetSettings::TargetEndianess currentEndianess);
    void simulatorCompilerChanged(const QString& compiler);
    void targetCompilerChanged(const QString& compiler);
    void targetEndianessChanged(const QString& endianess);

    void parseNewCanSpecification();

    void okButtonClicked();
    void cancelButtonClicked();
    void updateModel();
    void cleanupCANChanges();

private slots:
    void slot_changeCanSpecificationButton_clicked();
    void slot_canCopyToClipboardButton_clicked(bool);

    // QDialog interface
public slots:
    void reject();
};

#endif // PROJECTPROPERTIESDIALOG_H
