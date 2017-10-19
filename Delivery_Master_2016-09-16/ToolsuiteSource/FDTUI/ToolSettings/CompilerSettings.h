#ifndef COMPILERSETTINGS_H
#define COMPILERSETTINGS_H

#include "CompilerBase.h"

class KeyValueTableWidget;
class ToolSettings;

template <class Key, class T>class QMap;
class QWidget;

namespace Ui {
class CompilerSettings;
}

class CompilerSettings : public QWidget
{
    Q_OBJECT

public:
    explicit CompilerSettings(ToolSettings& toolSettings, QWidget* parent = 0);
    ~CompilerSettings(void);

    void SetUnsavedChanges(bool unsavedChanges);
    bool HasUnsavedChanges(void);

    void SaveChanges();

private slots:
    void slot_addCompilerComboBox_currentIndexChanged(QString compilerType);
    void slot_compilersList_currentTextChanged(QString currentItemText);
    void slot_saveCompilerButton_clicked();
    void slot_removeCompilerButton_clicked();
    void slot_importButton_clicked();
    void slot_exportButton_clicked();
    void slot_enableSave();

private:
    Ui::CompilerSettings *m_ui;
    ToolSettings& m_settings;
    KeyValueTableWidget* m_environmentVariablesTable;
    CompilerBase* m_currentCompiler;
    QMap<int, CompilerBase::CompilerAttribute> m_compilerAttributeMapping;
    bool m_unsavedChanges;

    void connectUISignals();
    void populateFields();
    bool compilerExistsInList(QString compilerName);
    void setCompilerData(CompilerBase* compiler);
    void populateCurrentCompilerWithFieldValues();
    void setCompilerFieldsEnabled(bool enabled);
    void refreshCompilerList();
};

#endif // COMPILERSETTINGS_H
