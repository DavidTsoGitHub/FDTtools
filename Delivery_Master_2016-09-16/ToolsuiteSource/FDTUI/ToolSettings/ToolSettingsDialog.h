#ifndef TOOLSETTINGSDIALOG_H
#define TOOLSETTINGSDIALOG_H

class CompilerSettings;
class ToolSettings;

class QDialog;
class QPushButton;
class QWidget;

namespace Ui
{
    class ToolSettingsDialog;
}

class ToolSettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ToolSettingsDialog(ToolSettings& settings, QWidget *parent = 0);
    ~ToolSettingsDialog(void);

private slots:
    void slot_functionLibraryBrowseButton_clicked(bool checked);
    void slot_platformGeneratorBrowseButton_clicked(bool checked);
    void slot_simulatorBrowseButton_clicked(bool checked);
    void slot_builderBrowseButton_clicked(bool checked);    
    void slot_buttonBoxOk_clicked(bool);

private:
    Ui::ToolSettingsDialog *m_ui;
    ToolSettings& m_settings;
    QPushButton* m_okButton;
    CompilerSettings* m_compilerSettings;

    void populateFields(void);
    void connectUISignals(void);
};

#endif // TOOLSETTINGSDIALOG_H
